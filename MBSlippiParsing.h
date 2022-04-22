#pragma once
#include "MBSlippiTypes.h"
#include <MBUtility/MBInterfaces.h>

#include <assert.h>
#include <queue>
#include <stack>
namespace MBSlippi
{
	typedef MBUtility::MBOctetInputStream T;

	class SlippiEventParser
	{
	private:
		std::unique_ptr<T> m_InputStream;
		ParseVersion m_ParseVersion;
		std::unordered_map<EventType, size_t> m_PossibleEvents;

		std::queue<Event> m_StoredEvents = {};//fullhack
		void p_Initialize()
		{
			uint8_t NextByte;
			size_t ReadBytes = m_InputStream->Read(&NextByte, 1);
			EventType CurrentEvent = EventType(NextByte);
			if (ReadBytes == 0 || ReadBytes == -1)
			{
				return;//throw exception?
			}
			uint8_t Size;
			ReadBytes = m_InputStream->Read(&Size, 1);
			if (ReadBytes == 0 || ReadBytes == -1)
			{
				return;
			}
			std::string TotalEventData = std::string(Size, 0);
			TotalEventData[0] = Size;
			ReadBytes = m_InputStream->Read(TotalEventData.data() + 1, Size-1);
			Event_Payloads PayloadData;
			PayloadData.ParseData(m_ParseVersion, TotalEventData.data(), Size);
			for (size_t i = 0; i < PayloadData.EventSizes.size(); i++)
			{
				m_PossibleEvents[PayloadData.EventSizes[i].first] = PayloadData.EventSizes[i].second;
			}
			Event GameStartEvent = GetNextEvent();
			Event_GameStart& GameStartData = GameStartEvent.GetEventData<Event_GameStart>();
			m_ParseVersion = GameStartData.Version;
			m_StoredEvents.push(Event(std::make_unique<Event_Payloads>(std::move(PayloadData))));
			m_StoredEvents.push(std::move(GameStartEvent));
		}
	public:
		SlippiEventParser(std::unique_ptr<T> InputStream)
		{
			m_InputStream = std::move(InputStream);
			p_Initialize();
		}
		ParseVersion GetVersion()
		{
			return(m_ParseVersion);
		}
		Event GetNextEvent()
		{
			if (m_StoredEvents.size() > 0)
			{
				Event ReturnValue = std::move(m_StoredEvents.front());
				m_StoredEvents.pop();
				return(ReturnValue);
			}
			std::unique_ptr<EventData> DataToReturn;
			uint8_t NextByte;
			size_t ReadBytes = m_InputStream->Read(&NextByte, 1);
			EventType CurrentEvent = EventType(NextByte);
			if (ReadBytes == 0 || ReadBytes == -1)
			{
				//EOF
				DataToReturn = std::make_unique<Event_Null>();
			}
			else if (m_PossibleEvents.find(CurrentEvent) == m_PossibleEvents.end())
			{
				throw std::runtime_error("Invalid event encountered in event stream");
			}
			else
			{
				size_t BytesToRead = m_PossibleEvents[EventType(NextByte)];
				std::string ByteBuffer = std::string(BytesToRead, 0);
				size_t EventBytesCount = m_InputStream->Read(ByteBuffer.data(),BytesToRead);
				if (EventBytesCount != BytesToRead)
				{
					throw std::runtime_error("Insufficient data to parse event in stream");
				}
				else
				{
					if (CurrentEvent == EventType::MessageSplitter)
					{
						//då måste vi läsa in data så vi får hela eventet
						std::string NewByteBuffer;
						Event_MessageSplit CurrentSplit;
						CurrentSplit.ParseData(m_ParseVersion,ByteBuffer.data(),0);
						while (true)
						{
							NewByteBuffer += std::string((const char*) CurrentSplit.FixedSizeBlock,CurrentSplit.ActualSize);
							if (CurrentSplit.LastMessage == true)
							{
								break;
							}
							else
							{
								//vi vet att nästa är en message splitter
								uint8_t temp;
								m_InputStream->Read(&temp,1);
								std::string EventData = std::string(EventBytesCount, 0);
								size_t BytesRead = m_InputStream->Read(EventData.data(), EventBytesCount);
								if(BytesRead != EventBytesCount)
								{
									throw std::runtime_error("Insufficent data when parsing message splitter event");
								}
								CurrentSplit.ParseData(m_ParseVersion,EventData.data(),EventData.size());
							}
						}
						CurrentEvent = EventType(CurrentSplit.InternalCommand);
						ByteBuffer = std::move(NewByteBuffer);
					}
					if (CurrentEvent == EventType::GameStart)
						DataToReturn = std::make_unique<Event_GameStart>();
					else if (CurrentEvent == EventType::PreFrameUpdate)
						DataToReturn = std::make_unique<Event_PreFrameUpdate>();
					else if (CurrentEvent == EventType::PostFrameUpdate)
						DataToReturn = std::make_unique<Event_PostFrameUpdate>();
					else if (CurrentEvent == EventType::GameEnd)
						DataToReturn = std::make_unique<Event_GameEnd>();
					else if (CurrentEvent == EventType::FrameStart)
						DataToReturn = std::make_unique<Event_FrameStart>();
					else if (CurrentEvent == EventType::ItemUpdate)
						DataToReturn = std::make_unique<Event_ItemUpdate>();
					else if (CurrentEvent == EventType::FrameBockend)
						DataToReturn = std::make_unique<Event_FrameBookend>();
					else if (CurrentEvent == EventType::GeckoList)
						DataToReturn = std::make_unique<Event_GeckoList>();	
					else
					{
						//mest för debugg
						throw std::runtime_error("Unsupported event type");
					}
					size_t ParsedBytes = DataToReturn->ParseData(m_ParseVersion,ByteBuffer.data(), ByteBuffer.size());
					assert(ParsedBytes == ByteBuffer.size());
				}
			}
			//DEBUG
			//EventType Type = DataToReturn->GetType();
			//size_t EventSize = DataToReturn->Serialize(ParseVersion{ 3,6,0 }).size();
			//size_t ActualVersion = m_PossibleEvents[DataToReturn->GetType()];
			//if (Type != EventType::GeckoList && Type != EventType::Null)
			//{
			//	assert(EventSize == ActualVersion + 1);
			//}
			//
			return(Event(std::move(DataToReturn)));
		}
	};

	class FrameParser
	{
	private:
		ParseVersion m_Version;
		std::deque<std::vector<Event>> m_StoredFrames = {};
		std::vector<Event> m_CurrentFrameData = {};
		bool m_Finished = false;
	public:
		void SetVersion(ParseVersion Version)
		{
			m_Version = Version;
		}
		void Reset()
		{
			ParseVersion PreviousVersion = m_Version;
			*this = FrameParser();
			m_Version = PreviousVersion;
		}
		void InsertEvent(Event EventToInsert)
		{
			if(m_Finished)
			{
				throw std::runtime_error("cant insert more frames after stream has finished");
			}
			EventType CurrentType = EventToInsert.GetType();
			if (CurrentType == EventType::EventPayloads || CurrentType == EventType::GameStart || CurrentType == EventType::GeckoList)
			{
				return;
			}
			if (CurrentType == EventType::GameEnd)
			{
				if (m_CurrentFrameData.size() > 0)
				{
					m_StoredFrames.push_back(std::move(m_CurrentFrameData));
					m_CurrentFrameData = {};
				}
				m_Finished = true;
				return;
			}
			if (m_CurrentFrameData.size() > 0 && m_CurrentFrameData.back().GetType() == EventType::PostFrameUpdate && (CurrentType == EventType::FrameStart || CurrentType == EventType::PreFrameUpdate))
			{
				m_StoredFrames.push_back(std::move(m_CurrentFrameData));
				m_CurrentFrameData = {};
			}
			m_CurrentFrameData.push_back(std::move(EventToInsert));
			if (CurrentType == EventType::FrameBockend)
			{
				m_StoredFrames.push_back(std::move(m_CurrentFrameData));
				m_CurrentFrameData = {};
			}
			
		}
		bool Finished()
		{
			return(m_Finished);
		}
		size_t AvailableFrames()
		{
			return(m_StoredFrames.size());
		}
		std::vector<Event> ExtractFrame()
		{
			if (m_StoredFrames.size() == 0)
			{
				throw std::runtime_error("No stored frames to extract");
			}
			std::vector<Event> ReturnValue = std::move(m_StoredFrames.front());
			m_StoredFrames.pop_front();
			return(ReturnValue);
		}
	};
}