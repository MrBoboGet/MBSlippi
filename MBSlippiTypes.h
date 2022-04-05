#pragma once
#include <memory>
#include <vector>
#include <string>
#include <MBParsing/MBParsing.h>
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
		virtual void ParseData(ParseVersion const& Version, void const* Data, size_t DataSize) = 0;
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
		virtual void ParseData(ParseVersion const& Version, void const* Data, size_t DataSize)
		{

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
			m_Type = EventType::Null;
		}
		virtual std::unique_ptr<EventData> Copy() const
		{
			return(std::make_unique<Event_MessageSplit>());
		}
		virtual void ParseData(ParseVersion const& Version, void const* Data, size_t DataSize)
		{
			size_t ParseOffset = 0;
			std::memcpy(FixedSizeBlock, Data, 512);
			ParseOffset += 512;
			ActualSize = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			InternalCommand = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			LastMessage = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
		}
	};
	class Event_Payloads : public EventData
	{
	public:
		uint8_t Size = 0;
		std::vector<std::pair<EventType, uint16_t>> EventSizes = {};
		virtual void ParseData(ParseVersion const& , void const* Data, size_t DataSize) override
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
		}
		std::unique_ptr<EventData> Copy() const override
		{
			std::unique_ptr<Event_Payloads> ReturnValue = std::make_unique<Event_Payloads>();
			ReturnValue->Size = Size;
			ReturnValue->EventSizes = EventSizes;
			return(ReturnValue);
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
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
				return;
			}
			for (size_t i = 0; i < 4; i++)
			{
				DashBackFix[i] = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			}
			for (size_t i = 0; i < 4; i++)
			{
				ShieldDropFix[i] = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			}
			if (Version < ParseVersion{ 1,5,0,0 })
			{
				return;
			}
			IsPal = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 2,0,0 })
			{
				return;
			}
			IsFrozenPS = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);;
			if (Version < ParseVersion{ 3,7,0 })
			{
				return;
			}
			MinorScene = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			MajorScene = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (Version < ParseVersion{ 3,9,0 })
			{
				return;
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
				return;
			}
			for (size_t i = 0; i < 4; i++)
			{
				SlippiUID[i] = std::string((const char*)ByteData + ParseOffset);
				ParseOffset += 29;
			}
			if (Version < ParseVersion{ 3,12,0 })
			{
				return;
			}
			LanguageOption = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
		}
		//0.1.0
		ParseVersion Version;
		uint8_t GameInfoBlock[312];
		uint32_t RandomSeed = 0;
		//1.0.0
		uint32_t DashBackFix[4];
		uint32_t ShieldDropFix[4];
		//1.3.0
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PlayIndex = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			IsFollower = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			RandomSeed = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			ActionStateID = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			XPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			JoystickX = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			JoystickY = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			CStickX = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			CStickY = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Trigger = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			ProcessedButtons = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PhysicalButtons = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			PhysicalLTrigger = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			PhysicalRTrigger = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 1,2,0 })
				return;
			XAnalog = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 1,4,0 })
				return;
			Percent = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
		}
		int32_t FrameNumber = -1;
		uint8_t PlayIndex = -1;
		bool IsFollower = false;
		uint32_t RandomSeed = 0;
		uint16_t ActionStateID = 0;
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
		uint8_t XAnalog = 0;
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			PlayerIndex = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			IsFollower = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			InternalCharacterID = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			ActionStateID = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			XPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Percent = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			ShieldSize = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			LastHittingAttackID = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			CurrentComboCount = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			LastHitBy = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			StocksRemaining = MBParsing::ParseBigEndianInteger(Data, 1	, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 0,2,0 })
				return;
			ActionStateFrameCounter = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,0,0 })
				return;
			StateBitFlags1 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			StateBitFlags2 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			StateBitFlags3 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			StateBitFlags4 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			StateBitFlags5 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			MiscAS = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			Airborne = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			LastGroundID = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			JumpsRemaining = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			LCancelStatus = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,1,0 })
				return;
			HurtboxCollisionState = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,5,0 })
				return;
			SelfInducedAirXSpeed = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SelfInducedYSpeed = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			AttackBasedXSpeed = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			AttackBasedYSpeed = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SelfInducedGroundXSpeed = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,8,0 })
				return;
			HitlagFramesRemaining = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,11,0 })
				return;
			AnimationIndex = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
		}
		int32_t 	FrameNumber				= 0;
		uint8_t 	PlayerIndex				 = 0;
		bool 		IsFollower					 = 0;
		uint8_t		InternalCharacterID		 = 0;
		uint16_t 	ActionStateID				 = 0;
		float 		XPosition					 = 0;
		float 		YPosition					 = 0;
		float		FacingDirection			 = 0;
		float 		Percent						 = 0;
		float 		ShieldSize					 = 0;
		uint8_t		LastHittingAttackID		 = 0;
		uint8_t 	CurrentComboCount			 = 0;
		uint8_t 	LastHitBy					 = 0;
		uint8_t 	StocksRemaining			 = 0;
		//0.2.0
		float 		ActionStateFrameCounter	 = 0;
		//2.0.0
		uint8_t 	StateBitFlags1			 = 0;
		uint8_t 	StateBitFlags2			 = 0;
		uint8_t 	StateBitFlags3			 = 0;
		uint8_t 	StateBitFlags4			 = 0;
		uint8_t 	StateBitFlags5			 = 0;
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			GameEndMethod = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 2,0,0 })
				return;
			LRASInitiator = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			RandomSeed = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,10,0 })
				return;
			SceneFrameCounter = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			TypeID = MBParsing::ParseBigEndianInteger(Data, 2, ParseOffset, &ParseOffset);
			State = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			FacingDirection = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			XVelocity = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YVelocity = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			XPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			YPosition = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			DamageTaken = MBParsing::ParseBigEndianInteger(Data,2, ParseOffset, &ParseOffset);
			ExpirationTimer = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			SpawnID = MBParsing::ParseIEEE754Float(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,2,0 })
				return;
			Misc1 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc2 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc3 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			Misc4 = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,6,0 })
				return;
			Owner = MBParsing::ParseBigEndianInteger(Data, 1, ParseOffset, &ParseOffset);
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
		uint8_t 	Misc1 				= -1;
		uint8_t	 	Misc2 				= -1;
		uint8_t	 	Misc3 				= -1;
		uint8_t	 	Misc4 				= -1;
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{
			size_t ParseOffset = 0;
			FrameNumber = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
			if (CurrentVersion < ParseVersion{ 3,7,0 })
				return;
			LatestFinalizedFrame = MBParsing::ParseBigEndianInteger(Data, 4, ParseOffset, &ParseOffset);
		}
		int32_t	FrameNumber = -1;
		int32_t LatestFinalizedFrame = -1;
	};

	class Event_GeckoList : public EventData
	{
	public:
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
		void ParseData(ParseVersion const& CurrentVersion, void const* Data, size_t DataSize) override
		{

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
}