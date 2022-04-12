
#include "MBSlippi.h"

#include <iostream>
#include <MBParsing/MBParsing.h>
#include <filesystem>
int main(int argc, const char** argv)
{
	std::filesystem::current_path(std::filesystem::current_path().parent_path());
	MBUtility::MBFileInputStream InputStream("./Game_20200630T190553.slp");
	MBParsing::JSONObject ResultObject = MBParsing::ParseUBJSON(&InputStream, nullptr);
	std::cout << ResultObject["metadata"].ToString() << std::endl;
	std::string const& EventData = ResultObject["raw"].GetStringData();
	std::unique_ptr<MBUtility::MBOctetInputStream> RawDataStream = std::make_unique<MBUtility::MBBufferInputStream>(EventData.data(), EventData.size());
	MBSlippi::SlippiEventParser EventParser = MBSlippi::SlippiEventParser(std::move(RawDataStream));
	MBSlippi::Event CurrentEvent = MBSlippi::Event(std::make_unique<MBSlippi::Event_Null>());
	std::vector<MBSlippi::Event> AllEvents = {};
	while (true)
	{
		CurrentEvent = EventParser.GetNextEvent();
		if (CurrentEvent.GetType() == MBSlippi::EventType::Null)
		{
			break;
		}
		AllEvents.push_back(std::move(CurrentEvent));
	}
	
	std::string TotalRawData = "";
	uint32_t TotalEvents[11];
	std::memset(TotalEvents, 0, sizeof(TotalEvents));
	
	for (size_t i = 0; i < AllEvents.size(); i++)
	{
		size_t EventIndex = size_t(AllEvents[i].GetType());
		if (EventIndex == 0x10)
		{
			EventIndex -= 0x10;
		}
		else
		{
			EventIndex -= 0x34;
		}
		TotalEvents[EventIndex] += 1;
		TotalRawData += AllEvents[i].Serialize(MBSlippi::ParseVersion{ 3,6,0 });
	}
	assert(TotalRawData.size() == EventData.size());
	assert(std::memcmp(TotalRawData.data(), EventData.data(), 29) == 0);
	for (size_t i = 0; i < TotalRawData.size(); i++)
	{
		if (TotalRawData[i] != EventData[i])
		{
			size_t DebugPos = i - 29;
			uint8_t* BytePointer =(uint8_t*)  TotalRawData.data();
			std::cout << "First differing byte position: " << i<<std::endl;
			break;
		}
	}
	std::vector<std::vector<MBSlippi::Event>> TotalFrames = {};
	MBSlippi::FrameParser FrameParser;
	FrameParser.SetVersion(EventParser.GetVersion());
	for (size_t i = 0; i < AllEvents.size(); i++)
	{
		FrameParser.InsertEvent(AllEvents[i]);
		if (FrameParser.AvailableFrames() > 0)
		{
			TotalFrames.push_back(FrameParser.ExtractFrame());
		}
	}
	while (FrameParser.AvailableFrames() > 0)
	{
		TotalFrames.push_back(FrameParser.ExtractFrame());
	}
	std::cout << "Total number of frames: " << TotalFrames.size() << std::endl;
	
	MBParsing::JSONObject TestCopy = ResultObject;
	TestCopy.GetAttribute("raw") = MBParsing::JSONObject(TotalRawData);
	std::ofstream FileOutStream("TestTest.slp", std::ios::out | std::ios::binary);
	MBUtility::MBFileOutputStream OutStream(&FileOutStream);
	//direkt till filen test
	//OutStream.Write("\x7b\x55\x03\x72\x61\x77\x5b\x24\x55\x23\x6c", 11);
	//OutStream.Write(TotalRawData.data(),TotalRawData.size());
	//OutStream.Write("U\x08metadata",10);
	//MBParsing::SerialiseUBJSON(OutStream, TestCopy.GetAttribute("metadata"));
	MBParsing::SerialiseUBJSON(OutStream, TestCopy);

	MBUtility::MBFileInputStream CopiedInputStream("TestTest.slp");
	MBParsing::JSONObject CopiedResultObject = MBParsing::ParseUBJSON(&CopiedInputStream, nullptr);
	std::cout << CopiedResultObject.GetAttribute("metadata").ToString() << std::endl;
	assert(CopiedResultObject.GetAttribute("raw").GetStringData() == TotalRawData);
	//eld testet, spela varannan frame 
	std::cout << AllEvents.size() << std::endl;


	//Varannan sekund test
	MBUtility::MBFileInputStream OriginalInputStream("./Game_20200630T190553.slp");
	MBParsing::JSONObject OriginalJSONData = MBParsing::ParseUBJSON(&OriginalInputStream, nullptr);
	MBUtility::MBBufferInputStream BufferInput(OriginalJSONData.GetAttribute("raw").GetStringData().data(), OriginalJSONData.GetAttribute("raw").GetStringData().size());
	
	MBSlippi::SlippiEventParser OriginalEventParser(std::make_unique<MBUtility::MBBufferInputStream>(std::move(BufferInput)));
	std::vector<std::vector<MBSlippi::Event>> AllFrames = {};
	MBSlippi::FrameParser FrameParserToUse;
	FrameParserToUse.SetVersion(OriginalEventParser.GetVersion());
	TotalRawData = "";
	std::string LastEventData;
	while (true)
	{
		MBSlippi::Event NewEvent = OriginalEventParser.GetNextEvent();
		MBSlippi::EventType CurrentType = NewEvent.GetType();
		if(CurrentType == MBSlippi::EventType::Null)
		{
			break;
		}
		if (CurrentType == MBSlippi::EventType::GameStart || CurrentType == MBSlippi::EventType::EventPayloads || CurrentType == MBSlippi::EventType::GeckoList)
		{
			TotalRawData += NewEvent.Serialize(OriginalEventParser.GetVersion());
		}
		else if (CurrentType == MBSlippi::EventType::GameEnd)
		{
			LastEventData = NewEvent.Serialize(OriginalEventParser.GetVersion());
		}
		else
		{
			FrameParserToUse.InsertEvent(std::move(NewEvent));
			if (FrameParserToUse.AvailableFrames() > 0)
			{
				AllFrames.push_back(FrameParserToUse.ExtractFrame());
			}
		}
	}
	while (FrameParserToUse.AvailableFrames() > 0)
	{
		AllFrames.push_back(FrameParserToUse.ExtractFrame());
	}
	int FrameCounter = 0;
	size_t TotalFrameCount = 0;
	int32_t CurrentFrameIndex = -123;
	size_t FramePerChunk = 360;
	for (size_t i = 0; i < AllFrames.size(); i++)
	{
		if (FrameCounter < FramePerChunk)
		{
			for (size_t j = 0; j < AllFrames[i].size(); j++)
			{
				MBSlippi::UpdateFrameIndex(AllFrames[i][j], CurrentFrameIndex);
				TotalRawData += AllFrames[i][j].Serialize(OriginalEventParser.GetVersion());
			}
			TotalFrameCount += 1;
			CurrentFrameIndex += 1;
		}
		if(FrameCounter > FramePerChunk*2)
		{
			FrameCounter = 0;
		}
		FrameCounter++;
	}
	TotalRawData += LastEventData;
	MBParsing::JSONObject ObjectToWrite(MBParsing::JSONObjectType::Aggregate);
	ObjectToWrite["raw"] = std::move(TotalRawData);
	ObjectToWrite["metadata"] = ResultObject.GetAttribute("metadata");
	ObjectToWrite["metadata"]["lastFrame"] = int64_t(TotalFrameCount - 705);
	ObjectToWrite["metadata"]["players"]["0"]["characters"]["9"] = int64_t(TotalFrameCount);
	ObjectToWrite["metadata"]["players"]["1"]["characters"]["1"] = int64_t(TotalFrameCount);
	std::cout << ObjectToWrite["metadata"].ToString() << std::endl;
	std::ofstream FrameSkipTestFile("FrameSkipTest.slp", std::ios::out | std::ios::binary);
	MBUtility::MBFileOutputStream FrameSkipTestStream(&FrameSkipTestFile);
	MBParsing::SerialiseUBJSON(FrameSkipTestStream, ObjectToWrite); 
}