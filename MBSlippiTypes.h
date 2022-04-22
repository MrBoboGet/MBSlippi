#pragma once
#include <memory>
#include <vector>
#include <string>
#include <MBParsing/MBParsing.h>


#include "MeleeID.h"
namespace MBSlippi
{

	enum class EventType
	{
		MessageSplitter = 0x10,
		EventPayloads = 0x35,
		GameStart = 0x36,
		PreFrameUpdate = 0x37,
		PostFrameUpdate = 0x38,
		GameEnd = 0x39,
		FrameStart = 0x3a,
		ItemUpdate = 0x3b,
		FrameBockend = 0x3c,
		GeckoList = 0x3D,


		Null,
	};
	struct ParseVersion
	{
		uint8_t Major  = uint8_t( - 1);
		uint8_t Minor =	 uint8_t( - 1);
		uint8_t Build =  uint8_t( - 1);
		uint8_t Unused = uint8_t( -1 );
		bool operator<(ParseVersion const& rhs) const
		{
			bool ReturnValue = false;
			if (Major < rhs.Major)
			{
				ReturnValue = true;
			}
			else if (Major == rhs.Major)
			{
				if (Minor < rhs.Minor)
				{
					ReturnValue = true;
				}
				else if (Minor == rhs.Minor)
				{
					if (Build < rhs.Build)
					{
						ReturnValue = true;
					}
				}
			}
			return(ReturnValue);
		}
		bool operator>=(ParseVersion const& rhs) const
		{
			return(!(*this < rhs));
		}
		bool operator==(ParseVersion const& rhs) const
		{
			bool ReturnValue = Major == rhs.Major && Minor == rhs.Minor && Build == rhs.Build;
			return(ReturnValue);
		}
		bool operator>(ParseVersion const& rhs) const
		{
			return(!(*this == rhs) && *this > rhs);
		}
	};
	class EventData
	{
	protected:
		EventType m_Type = EventType::Null;
	public:
		EventType GetType() const
		{
			return(m_Type);
		}
		virtual std::unique_ptr<EventData> Copy() const = 0;
		virtual size_t ParseData(ParseVersion const& Version, void const* Data, size_t DataSize) = 0;
		virtual std::string Serialize(ParseVersion const& Version) const = 0;
		virtual ~EventData()
		{
			
		}
	};
	class Event_Null : public EventData
	{
	public:
		Event_Null()
		{
			m_Type = EventType::Null;
		}
		virtual std::unique_ptr<EventData> Copy() const
		{
			return(std::make_unique<Event_Null>());
		}
		virtual size_t ParseData(ParseVersion const& Version, void const* Data, size_t DataSize)
		{
			return(0);
		}
		virtual std::string Serialize(ParseVersion const& Version) const
		{
			return("");
		}
	};
	class Event_MessageSplit : public EventData
	{
	public:
		uint8_t	FixedSizeBlock[512];
		uint16_t ActualSize = -1;
		uint8_t InternalCommand = 0;
		bool LastMessage = 0;
		Event_MessageSplit()
		{
			m_Type = EventType::MessageSplitter;
		}
		virtual std::unique_ptr<EventData> Copy() const
		{
			return(std::make_unique<Event_MessageSplit>());
		}
		virtual size_t ParseData(ParseVersion const& Version, void const* Data, size_t DataSize)
		{
			size_t ParseOffset = 0;
			std::memcpy(FixedSizeBlock, Data, 512);
			ParseOffset += 512;
			ActualSize = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			InternalCommand = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			LastMessage = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		virtual std::string Serialize(ParseVersion const& Version) const
		{
			std::string ReturnValue(1+512 + 2 + 1 + 1, 0);
			ReturnValue[0] = char(m_Type);
			size_t ParseOffset = 1;
			std::memcpy(ReturnValue.data()+1, FixedSizeBlock, 512);
			ParseOffset += 512;
			MBParsing::WriteBigEndianInteger(ReturnValue.data() + ParseOffset, ActualSize, 2);
			ParseOffset += 2;
			MBParsing::WriteBigEndianInteger(ReturnValue.data() + ParseOffset, InternalCommand, 1);
			ParseOffset += 1;
			MBParsing::WriteBigEndianInteger(ReturnValue.data() + ParseOffset, LastMessage, 1);
			return(ReturnValue);
		}
	};
	class Event_Payloads : public EventData
	{
	public:
		uint8_t Size = 0;
		std::vector<std::pair<EventType, uint16_t>> EventSizes = {};
		Event_Payloads()
		{
			m_Type = EventType::EventPayloads;
		}
		virtual size_t ParseData(ParseVersion const& , void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			Size = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			while (ParseOffset < Size)
			{
				std::pair<EventType, uint16_t> NewSize;
				NewSize.first	= EventType(MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset));
				NewSize.second	= MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
				EventSizes.push_back(NewSize);
			}
			return(ParseOffset);
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_Payloads> ReturnValue = std::make_unique<Event_Payloads>();
			ReturnValue->Size = Size;
			ReturnValue->EventSizes = EventSizes;
			return(ReturnValue);
		}
		virtual std::string Serialize(ParseVersion const& Version) const
		{
			std::string ReturnValue = std::string(2 + 3 * EventSizes.size(),0);
			size_t ParseOffset = 0;
			ReturnValue[0] = char(m_Type);
			ParseOffset += 1;
			ReturnValue[ParseOffset] = 1 + 3 * EventSizes.size();
			ParseOffset += 1;
			for (size_t i = 0; i < EventSizes.size(); i++)
			{
				ReturnValue[ParseOffset] = char(EventSizes[i].first);
				ParseOffset += 1;
				MBParsing::WriteBigEndianInteger(ReturnValue.data() + ParseOffset, EventSizes[i].second, 2);
				ParseOffset += 2;
			}
			return(ReturnValue);
		}
	};

	struct GameInfoBlock
	{
		StageID Stage;

		GameInfoBlock() {};
		GameInfoBlock(const void* Data, size_t DataSize)
		{
			size_t ParseOffset = 0;
			//lat, vill bara ha stage
			ParseOffset = 0x0e;
			Stage = StageID(MBParsing::ParseBigEndianInteger(Data, 2,0,nullptr));
		}
	};

	class Event_GameStart : public EventData
	{
	public:
		Event_GameStart()
		{
			m_Type = EventType::GameStart;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_GameStart> ReturnValue = std::make_unique<Event_GameStart>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			uint8_t const* ByteData = (uint8_t const*)Data;
			//
			Version.Major = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Version.Minor= MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Version.Build = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Version.Unused = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			std::memcpy(GameInfoBlock, ByteData + ParseOffset, 312);
			ParseOffset += 312;
			RandomSeed = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			if (Version.Major < 1)
			{
				return ParseOffset;
			}
			for (size_t i = 0; i < 4; i++)
			{
				DashBackFix[i] = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			}
			for (size_t i = 0; i < 4; i++)
			{
				ShieldDropFix[i] = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			}
			if (Version < ParseVersion{ 1,3,0,0 })
			{
				return(ParseOffset);
			}
			for (size_t i = 0; i < 4; i++)
			{
				NameTags[i] = std::string((const char*)ByteData + ParseOffset);
				ParseOffset += 16;
			}
			if (Version < ParseVersion{ 1,5,0,0 })
			{
				return ParseOffset;
			}
			IsPal = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 2,0,0 })
			{
				return ParseOffset;
			}
			IsFrozenPS = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);;
			if (Version < ParseVersion{ 3,7,0 })
			{
				return ParseOffset;
			}
			MinorScene = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			MajorScene = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,9,0 })
			{
				return ParseOffset;
			}
			for (size_t i = 0; i < 4; i++)
			{
				DisplayName[i] = std::string((const char*) ByteData + ParseOffset);
				ParseOffset += 31;
			}
			for (size_t i = 0; i < 4; i++)
			{
				ConnectCode[i] = std::string((const char*)ByteData + ParseOffset);
				ParseOffset += 10;
			}
			if (Version < ParseVersion{3, 11, 0})
			{
				return ParseOffset;
			}
			for (size_t i = 0; i < 4; i++)
			{
				SlippiUID[i] = std::string((const char*)ByteData + ParseOffset);
				ParseOffset += 29;
			}
			if (Version < ParseVersion{ 3,12,0 })
			{
				return ParseOffset;
			}
			LanguageOption = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		virtual std::string Serialize(ParseVersion const& CurrentVersion) const
		{
			std::string ReturnValue = std::string(sizeof(Event_GameStart)+1+ 31 * 4+10*4+ 4 * 29, 0);
			size_t ParseOffset = 0;
			ReturnValue[0] = char(m_Type);
			ParseOffset += 1;
			ReturnValue[ParseOffset] = Version.Major;
			ReturnValue[ParseOffset+1] = Version.Minor;
			ReturnValue[ParseOffset+2] = Version.Build;
			ReturnValue[ParseOffset+3] = Version.Unused;
			ParseOffset += 4;
			std::memcpy(ReturnValue.data() + ParseOffset, GameInfoBlock, 312);
			ParseOffset += 312;
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), RandomSeed, 4,ParseOffset,&ParseOffset);
			if (CurrentVersion < ParseVersion{ 1,0,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			for (size_t i = 0; i < 4; i++)
			{
				MBParsing::WriteBigEndianInteger(ReturnValue.data(), DashBackFix[i], 4,ParseOffset,&ParseOffset);
			}
			for (size_t i = 0; i < 4; i++)
			{
				MBParsing::WriteBigEndianInteger(ReturnValue.data(), ShieldDropFix[i], 4,ParseOffset,&ParseOffset);
			}
			if (CurrentVersion < ParseVersion{ 1,3,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			for (size_t i = 0; i < 4; i++)
			{
				std::memcpy(ReturnValue.data() + ParseOffset, NameTags[i].data(), NameTags[i].size());
				ParseOffset += 16;
			}
			if (CurrentVersion < ParseVersion{ 1,5,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			ReturnValue[ParseOffset] = char(IsPal);
			ParseOffset += 1;
			if (CurrentVersion < ParseVersion{ 2,0,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			ReturnValue[ParseOffset] = char(IsFrozenPS);
			ParseOffset += 1;
			if (CurrentVersion < ParseVersion{ 3,7,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			ReturnValue[ParseOffset] = MinorScene;
			ReturnValue[ParseOffset] = MajorScene;
			ParseOffset += 2;
			if (CurrentVersion < ParseVersion{ 3,9,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			for (size_t i = 0; i < 4; i++)
			{
				std::memcpy(ReturnValue.data() + ParseOffset, DisplayName[i].data(), DisplayName[i].size());
				ParseOffset += 31;
			}
			for (size_t i = 0; i < 4; i++)
			{
				std::memcpy(ReturnValue.data() + ParseOffset, ConnectCode[i].data(), ConnectCode[i].size());
				ParseOffset += 10;
			}
			if (CurrentVersion < ParseVersion{ 3,11,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			for (size_t i = 0; i < 4; i++)
			{
				std::memcpy(ReturnValue.data() + ParseOffset, SlippiUID[i].data(), SlippiUID[i].size());
				ParseOffset += 29;
			}
			if (CurrentVersion < ParseVersion{ 3,12,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}

			ReturnValue[ParseOffset] = LanguageOption;
			ReturnValue.resize(ParseOffset);
			return(ReturnValue);
		}
		//0.1.0
		ParseVersion Version;
		uint8_t GameInfoBlock[312];
		uint32_t RandomSeed = 0;
		//1.0.0
		uint32_t DashBackFix[4];
		uint32_t ShieldDropFix[4];
		//1.3.0
		//16 bytes total size med null terminator
		std::string NameTags[4];
		//1.5.0
		bool IsPal = false;
		//2.0.0
		bool IsFrozenPS = true;
		//3.7.0
		uint8_t MinorScene = 0x2;
		uint8_t MajorScene = 0;
		//3.9.0
		std::string DisplayName[4];
		std::string ConnectCode[4];
		//3.11.0
		std::string SlippiUID[4];
		//3.12.0
		uint8_t LanguageOption = 1;
	};

	class Event_PreFrameUpdate : public EventData
	{
	public:
		Event_PreFrameUpdate()
		{
			m_Type = EventType::PreFrameUpdate;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_PreFrameUpdate> ReturnValue = std::make_unique<Event_PreFrameUpdate>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PlayIndex = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			IsFollower = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			RandomSeed = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			ActionStateID =ActionState(MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset));
			XPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			JoystickX = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			JoystickY = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			CStickX = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			CStickY = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Trigger = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			ProcessedButtons = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PhysicalButtons = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			PhysicalLTrigger = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			PhysicalRTrigger = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 1,2,0 })
				return ParseOffset;
			XAnalog = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 1,4,0 })
				return ParseOffset;
			Percent = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(sizeof(Event_PreFrameUpdate)+1,0);
			size_t ParseOffset = 1;
			void* OutBuffer = ReturnValue.data();
			ReturnValue[0] = char(m_Type);
			MBParsing::WriteBigEndianInteger(OutBuffer, FrameNumber, 4,ParseOffset,&ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, PlayIndex, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, IsFollower, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, RandomSeed,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, uint64_t(ActionStateID),2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, XPosition,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, YPosition,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, FacingDirection,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, JoystickX,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, JoystickY,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, CStickX,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, CStickY,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, Trigger,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, ProcessedButtons, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, PhysicalButtons, 2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, PhysicalLTrigger, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, PhysicalRTrigger, 4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 1,2,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, XAnalog, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 1,4,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, Percent, 4, ParseOffset, &ParseOffset);
			ReturnValue.resize(ParseOffset);
			return(ReturnValue);
		}
		int32_t FrameNumber = -1;
		uint8_t PlayIndex = -1;
		bool IsFollower = false;
		uint32_t RandomSeed = 0;
		ActionState ActionStateID = ActionState(0);
		float XPosition = 0;
		float YPosition = 0;
		float FacingDirection = 0;
		float JoystickX = 0;
		float JoystickY = 0;
		float CStickX = 0;
		float CStickY = 0;
		float Trigger = 0;
		uint32_t ProcessedButtons = 0;
		uint16_t PhysicalButtons = 0;
		float PhysicalLTrigger = 0;
		float PhysicalRTrigger = 0;
		//1.2.0
		uint8_t XAnalog = 0;
		//1.4.0
		float Percent = 0;

	};
	class Event_PostFrameUpdate : public EventData
	{
	public:
		Event_PostFrameUpdate()
		{
			m_Type = EventType::PostFrameUpdate;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_PostFrameUpdate> ReturnValue = std::make_unique<Event_PostFrameUpdate>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PlayerIndex = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			IsFollower = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			InternalCharID = InternalCharacterID(MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset));
			ActionStateID = ActionState(MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset));
			XPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Percent = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			ShieldSize = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			LastHittingAttackID = AttackID(MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset));
			CurrentComboCount = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			LastHitBy = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			StocksRemaining = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			if (StocksRemaining != 6 || Percent != 0)
			{
				int hej = 2;
			}
			if (CurrentVersion < ParseVersion{ 0,2,0 })
				return ParseOffset;
			ActionStateFrameCounter = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,0,0 })
				return ParseOffset;
			StateBitFlags = MBParsing::ParseBigEndianInteger(Data, 5, ParseOffset, &ParseOffset);
			//StateBitFlags1 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			//StateBitFlags2 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			//StateBitFlags3 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			//StateBitFlags4 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			//StateBitFlags5 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			MiscAS = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Airborne = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			LastGroundID = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			JumpsRemaining = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			LCancelStatus = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,1,0 })
				return ParseOffset;
			HurtboxCollisionState = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,5,0 })
				return ParseOffset;
			SelfInducedAirXSpeed = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SelfInducedYSpeed = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			AttackBasedXSpeed = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			AttackBasedYSpeed = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SelfInducedGroundXSpeed = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,8,0 })
				return ParseOffset;
			HitlagFramesRemaining = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,11,0 })
				return ParseOffset;
			AnimationIndex = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(sizeof(Event_PostFrameUpdate)+1, 0);
			size_t ParseOffset = 1;
			ReturnValue[0] = char(m_Type);
			uint8_t* OutBuffer = (uint8_t*)ReturnValue.data();
			MBParsing::WriteBigEndianInteger(OutBuffer, FrameNumber, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, PlayerIndex, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, IsFollower, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, uint8_t(InternalCharID), 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, uint16_t(ActionStateID), 2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, XPosition, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, YPosition, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, FacingDirection, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, Percent, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, ShieldSize, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, uint8_t(LastHittingAttackID), 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, CurrentComboCount, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, LastHitBy, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, StocksRemaining, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 0,2,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, ActionStateFrameCounter, 4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 2,0,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags, 5, ParseOffset, &ParseOffset);
			//MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags1, 1, ParseOffset, &ParseOffset);
			//MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags2, 1, ParseOffset, &ParseOffset);
			//MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags3, 1, ParseOffset, &ParseOffset);
			//MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags4, 1, ParseOffset, &ParseOffset);
			//MBParsing::WriteBigEndianInteger(OutBuffer, StateBitFlags5, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, MiscAS, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, Airborne, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, LastGroundID, 2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, JumpsRemaining, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, LCancelStatus, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 2,1,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, HurtboxCollisionState, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,5,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, SelfInducedAirXSpeed, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, SelfInducedYSpeed, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, AttackBasedXSpeed, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, AttackBasedYSpeed, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, SelfInducedGroundXSpeed, 4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,8,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, HitlagFramesRemaining, 4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,11,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, AnimationIndex, 4, ParseOffset, &ParseOffset);	
			ReturnValue.resize(ParseOffset);
			return(ReturnValue);
		}
		int32_t 	FrameNumber				= 0;
		uint8_t 	PlayerIndex				 = 0;
		bool 		IsFollower					 = 0;
		InternalCharacterID	InternalCharID	= InternalCharacterID(0);
		ActionState ActionStateID				 = ActionState(0);
		float 		XPosition					 = 0;
		float 		YPosition					 = 0;
		float		FacingDirection			 = 0;
		float 		Percent						 = 0;
		float 		ShieldSize					 = 0;
		AttackID    LastHittingAttackID		 = AttackID(0);
		uint8_t 	CurrentComboCount			 = 0;
		uint8_t 	LastHitBy					 = 0; //player index
		uint8_t 	StocksRemaining			 = 0;
		//0.2.0
		float 		ActionStateFrameCounter	 = 0;
		//2.0.0
		uint64_t StateBitFlags = 0;
		//uint8_t 	StateBitFlags1			 = 0;
		//uint8_t 	StateBitFlags2			 = 0;
		//uint8_t 	StateBitFlags3			 = 0;
		//uint8_t 	StateBitFlags4			 = 0;
		//uint8_t 	StateBitFlags5			 = 0;
		float 		MiscAS	 = 0;
		bool 		Airborne			 = 0;
		uint16_t 	LastGroundID				 = 0;
		uint8_t 	JumpsRemaining				 = 0;
		uint8_t 	LCancelStatus			 = 0;
		//2.1.0
		uint8_t 	HurtboxCollisionState		 = 0;
		//3.5.0
		float 		SelfInducedAirXSpeed	 = 0;
		float 		SelfInducedYSpeed		 = 0;
		float 		AttackBasedXSpeed		 = 0;
		float 		AttackBasedYSpeed		 = 0;
		float 		SelfInducedGroundXSpeed= 0;
		//3.8.0
		float 		HitlagFramesRemaining		 = 0;
		//3.11.0
		uint32_t 	AnimationIndex				 = 0;
	};
	enum class StateBitFlags : uint64_t
	{
		ReflectActive =					1ull << uint64_t(32 + 4),
		Invincible =					1ull << uint64_t(24 + 2),
		FastFalling =					1ull << uint64_t(24 + 3),
		InHitlag =						1ull << uint64_t(24 + 5),
		ShieldActive =					1ull << uint64_t(16 + 7),
		InHitstun =						1ull << uint64_t(8 + 1),
		HitboxTouchingShieldBubble =	1ull << uint64_t(8 + 2),
		PowershieldActive =				1ull << uint64_t(8 + 5),
		IsFollower =					1ull << uint64_t(0 + 3),
		IsInActionStateSleep =			1ull << uint64_t(0 + 4),
		Dead =							1ull << uint64_t(0 + 6),
		OffScreen =						1ull << uint64_t(0 + 7),

	};

	class Event_GameEnd : public EventData
	{
	public:
		Event_GameEnd()
		{
			m_Type = EventType::GameEnd;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_GameEnd > ReturnValue = std::make_unique<Event_GameEnd>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			GameEndMethod = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,0,0 })
				return ParseOffset;
			LRASInitiator = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(3, 0);
			ReturnValue[0] = char(m_Type);
			ReturnValue[1] = char(GameEndMethod);
			if (Version < ParseVersion{ 2,0,0 })
			{
				ReturnValue.resize(2);
				return(ReturnValue);
			}
			ReturnValue[2] = char(LRASInitiator);
			return(ReturnValue);
		}
		uint8_t GameEndMethod = -1;
		int8_t LRASInitiator = -1;
	};

	class Event_FrameStart : public EventData
	{
	public:
		Event_FrameStart()
		{
			m_Type = EventType::FrameStart;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_FrameStart > ReturnValue = std::make_unique<Event_FrameStart>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			RandomSeed = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,10,0 })
				return ParseOffset;
			SceneFrameCounter = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(13, 0);
			ReturnValue[0] = char(m_Type);
			size_t ParseOffset = 1;
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), FrameNumber,4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), RandomSeed,4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,10,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), SceneFrameCounter,4, ParseOffset, &ParseOffset);
			return(ReturnValue);
		}
		int32_t FrameNumber = 0;
		uint32_t RandomSeed = 0;
		uint32_t SceneFrameCounter = 0;
	};

	class Event_ItemUpdate : public EventData
	{
	public:
		Event_ItemUpdate()
		{
			m_Type = EventType::ItemUpdate;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_ItemUpdate > ReturnValue = std::make_unique<Event_ItemUpdate>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			TypeID = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			State = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			XVelocity = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YVelocity = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			XPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			DamageTaken = MBParsing::ParseBigEndianInteger(Data,2, ParseOffset, &ParseOffset);
			ExpirationTimer = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SpawnID = MBParsing::ParseBigEndianIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,2,0 })
				return ParseOffset;
			Misc1 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc2 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc3 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc4 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,6,0 })
				return ParseOffset; 
			Owner = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(sizeof(Event_ItemUpdate)+1, 0);
			size_t ParseOffset = 1;
			uint8_t* OutBuffer = (uint8_t*) ReturnValue.data();
			ReturnValue[0] = char(m_Type);
			MBParsing::WriteBigEndianInteger(OutBuffer, FrameNumber, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, TypeID, 2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, State, 1, ParseOffset, &ParseOffset);

			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, FacingDirection, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, XVelocity, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, YVelocity, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, XPosition, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, YPosition, 4, ParseOffset, &ParseOffset);

			MBParsing::WriteBigEndianInteger(OutBuffer, DamageTaken, 2, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianIEEE754Float(OutBuffer, ExpirationTimer, 4, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, SpawnID, 4, ParseOffset, &ParseOffset);

			if (Version< ParseVersion{ 3,2,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, Misc1, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, Misc2, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, Misc3, 1, ParseOffset, &ParseOffset);
			MBParsing::WriteBigEndianInteger(OutBuffer, Misc4, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,6,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(OutBuffer, Owner, 1, ParseOffset, &ParseOffset);

			ReturnValue.resize(ParseOffset);
			return(ReturnValue);
		}
		int32_t		FrameNumber 		= -1;
		uint16_t	TypeID 				= -1;
		uint8_t		State 				= -1;
		float		FacingDirection 	= -1;
		float 		XVelocity 			= -1;
		float 	 	YVelocity 			= -1;
		float 	 	XPosition 			= -1;
		float 	 	YPosition 			= -1;
		uint16_t	DamageTaken 		= -1;
		float 	 	ExpirationTimer 	= -1;
		uint32_t 	SpawnID 			= -1;
		//3.2.0
		uint8_t 	Misc1 				= -1;
		uint8_t	 	Misc2 				= -1;
		uint8_t	 	Misc3 				= -1;
		uint8_t	 	Misc4 				= -1;
		//3.6.0
		int8_t 		Owner 				= -1;
	};

	class Event_FrameBookend : public EventData
	{
	public:
		Event_FrameBookend()
		{
			m_Type = EventType::FrameBockend;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_FrameBookend> ReturnValue = std::make_unique<Event_FrameBookend>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,7,0 })
				return ParseOffset;
			LatestFinalizedFrame = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			return(ParseOffset);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = std::string(9, 0);
			size_t ParseOffset = 1;
			ReturnValue[0] = char(m_Type);
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), FrameNumber, 4, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,7,0 })
			{
				ReturnValue.resize(ParseOffset);
				return(ReturnValue);
			}
			MBParsing::WriteBigEndianInteger(ReturnValue.data(), LatestFinalizedFrame, 4, ParseOffset, &ParseOffset);
			return(ReturnValue);
		}
		int32_t	FrameNumber = -1;
		//3.7.0
		int32_t LatestFinalizedFrame = -1;
	};

	class Event_GeckoList : public EventData
	{
	public:
		std::string RawData = "";
		Event_GeckoList()
		{
			m_Type = EventType::GeckoList;
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_GeckoList> ReturnValue = std::make_unique<Event_GeckoList>();
			*ReturnValue = *this;
			return(ReturnValue);
		}
		size_t ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			RawData = std::string(DataSize, 0);
			std::memcpy(RawData.data(), Data, DataSize);
			return(DataSize);
		}
		std::string Serialize(ParseVersion const& Version)const override
		{
			std::string ReturnValue = "";
			size_t ProcessedBytesOffset = 0;
			while (ProcessedBytesOffset < RawData.size())
			{
				Event_MessageSplit NewMessage;
				size_t BytesToTransfer = std::min(RawData.size() - ProcessedBytesOffset,size_t(512));
				NewMessage.ActualSize = BytesToTransfer;
				NewMessage.InternalCommand = uint8_t(m_Type);
				NewMessage.LastMessage = false;
				std::memcpy(NewMessage.FixedSizeBlock, RawData.data() + ProcessedBytesOffset, BytesToTransfer);
				ProcessedBytesOffset += BytesToTransfer;
				if (ProcessedBytesOffset == RawData.size())
				{
					NewMessage.LastMessage = true;
				}
				ReturnValue += NewMessage.Serialize(Version);
				if (ProcessedBytesOffset == RawData.size())
				{
					break;
				}
			}

			return(ReturnValue);
		}
	};

	class Event
	{
	private:
		std::unique_ptr<EventData> m_Data = nullptr;
	public:
		Event(Event const& EventToCopy);
		Event(Event&& EventToSteal) noexcept;
		Event& operator=(Event EventToSteal);
		Event(std::unique_ptr<EventData> InitialData);
		EventType GetType() const;
		
		std::string Serialize(ParseVersion const& Version)
		{
			return(m_Data->Serialize(Version));
		}

		template<typename T>
		T& GetEventData()
		{
			return(dynamic_cast<T&>(*m_Data));
		}
		template<typename T>
		T const& GetEventData() const
		{
			return(dynamic_cast<T const&>(*m_Data));
		}
	};
	inline void UpdateFrameIndex(EventData& EventToUpdate, int32_t NewFrameIndex)
	{
		EventType CurrentType = EventToUpdate.GetType();
		if (CurrentType == EventType::FrameBockend)
		{
			Event_FrameBookend& Data = dynamic_cast<Event_FrameBookend&>(EventToUpdate);
			if (Data.LatestFinalizedFrame != Data.LatestFinalizedFrame)
			{
				throw std::runtime_error("Implement the case for LatestFinalizedFrame");
			}
			Data.FrameNumber = NewFrameIndex;
			Data.LatestFinalizedFrame = NewFrameIndex;
		}
		else if (CurrentType == EventType::FrameStart)
		{
			Event_FrameStart& Data = dynamic_cast<Event_FrameStart&>(EventToUpdate);
			Data.FrameNumber = NewFrameIndex;
		}
		else if (CurrentType == EventType::PostFrameUpdate)
		{
			Event_PostFrameUpdate& Data = dynamic_cast<Event_PostFrameUpdate&>(EventToUpdate);
			Data.FrameNumber = NewFrameIndex;
		}
		else if (CurrentType == EventType::PreFrameUpdate)
		{
			Event_PreFrameUpdate& Data = dynamic_cast<Event_PreFrameUpdate&>(EventToUpdate);
			Data.FrameNumber = NewFrameIndex;
		}
		else if (CurrentType == EventType::ItemUpdate)
		{
			Event_ItemUpdate& Data = dynamic_cast<Event_ItemUpdate&>(EventToUpdate);
			Data.FrameNumber = NewFrameIndex;
		}
	}
	inline void UpdateFrameIndex(Event& EventToUpdate, int32_t NewFrameIndex)
	{
		UpdateFrameIndex(EventToUpdate.GetEventData<EventData>(), NewFrameIndex);
	}
}