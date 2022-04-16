#include "MBSlippiScript.h"

namespace MBSlippi
{
	//BEGIN MBS_Slippi Objects
	MBS_SlippiPlayerMetadata::MBS_SlippiPlayerMetadata(MBScript::ObjectType TypeToUse,std::unordered_map<std::string, MBScript::MBSObjectStore> FieldsToUse)
	{
		m_Fields = std::move(FieldsToUse);
		m_Type = TypeToUse;
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiPlayerMetadata::Copy() const
	{
		std::unique_ptr<MBS_SlippiPlayerMetadata> ReturnValue = std::make_unique<MBS_SlippiPlayerMetadata>();
		ReturnValue->m_Type = m_Type;
		ReturnValue->m_Fields = m_Fields;
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiPlayerMetadata::DotOperator(std::string const& FieldName)
	{
		if (m_Fields.find(FieldName) == m_Fields.end())
		{
			throw MBScript::MBSRuntimeException("Field \"" + FieldName + "\" not defined in object");
		}
		return(m_Fields[FieldName].GetReferenceObject());
	}
	//
	MBS_SlippiPlayerFrameInfo::MBS_SlippiPlayerFrameInfo(Event PostFrameUpdate, MBS_SlippiModule& AssociatedModule)
	{
		m_Type = AssociatedModule.GetTypeConversion(MBSSlippiTypes::PlayerFrameInfo);
		Event_PostFrameUpdate const& EventData = PostFrameUpdate.GetEventData<Event_PostFrameUpdate>();
		//m_Fields["State"] = ;
		m_Fields["State"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(ActionStateToString(EventData.ActionStateID)));
		m_Fields["Shielding"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::ShieldActive)));
		m_Fields["InHitlag"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::InHitlag)));
		m_Fields["InHitstun"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::InHitstun)));
		m_Fields["InShieldStun"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.ActionStateID == ActionState::GuardSetOff));
		m_Fields["LastHitBy"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(AttackIDToString(EventData.LastHittingAttackID)));
		if (EventData.ActionStateID == ActionState::GuardSetOff)
		{
			int hej = 2;
		}
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiPlayerFrameInfo::Copy() const
	{
		std::unique_ptr<MBS_SlippiPlayerFrameInfo> ReturnValue = std::make_unique<MBS_SlippiPlayerFrameInfo>();
		ReturnValue->m_Type = m_Type;
		ReturnValue->m_Fields = m_Fields;
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiPlayerFrameInfo::DotOperator(std::string const& FieldName)
	{
		if (m_Fields.find(FieldName) == m_Fields.end())
		{
			throw MBScript::MBSRuntimeException("Field \"" + FieldName + "\" not defined in object");
		}
		return(m_Fields[FieldName].GetReferenceObject());
	}
	//
	bool h_PlayerCompare(Event const& Left, Event const& Right)
	{
		return(Left.GetEventData<Event_PostFrameUpdate>().PlayerIndex < Right.GetEventData<Event_PostFrameUpdate>().PlayerIndex);
	}
	MBS_SlippiFrame::MBS_SlippiFrame(std::vector<Event> FrameEvents, MBS_SlippiModule& AssociatedModule)
	{
		std::vector<Event> PostFrameUpdates;
		for (size_t i = 0; i < FrameEvents.size(); i++)
		{
			if (FrameEvents[i].GetType() == EventType::PostFrameUpdate)
			{
				PostFrameUpdates.push_back(std::move(FrameEvents[i]));
			}
		}
		std::sort(PostFrameUpdates.begin(), PostFrameUpdates.end(), h_PlayerCompare);
		std::unique_ptr<MBSObject> PlayerFrameDataList = std::make_unique<MBScript::MBSObject_List>();
		for (size_t i = 0; i < PostFrameUpdates.size(); i++)
		{
			*PlayerFrameDataList += std::make_unique<MBS_SlippiPlayerFrameInfo>(std::move(PostFrameUpdates[i]),AssociatedModule);
		}
		m_Fields["PlayerInfo"] = MBScript::MBSObjectStore(std::move(PlayerFrameDataList));
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiFrame::Copy() const
	{
		std::unique_ptr<MBS_SlippiFrame> ReturnValue = std::make_unique<MBS_SlippiFrame>();
		ReturnValue->m_Type = m_Type;
		ReturnValue->m_Fields = m_Fields;
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiFrame::DotOperator(std::string const& FieldName)
	{
		if (m_Fields.find(FieldName) == m_Fields.end())
		{
			throw MBScript::MBSRuntimeException("Field \"" + FieldName + "\" not defined in object");
		}
		return(m_Fields[FieldName].GetReferenceObject());
	}
	//
	MBS_SlippiGame::MBS_SlippiGame(std::string const& GamePath, MBS_SlippiModule* AssociatedModule)
	{
		std::ifstream FileStream = std::ifstream(GamePath, std::ios::in | std::ios::binary);
		MBUtility::MBFileInputStream InputStream= MBUtility::MBFileInputStream(&FileStream);
		MBError Result = true;
		MBParsing::JSONObject TotalFileData = MBParsing::ParseUBJSON(&InputStream,&Result);
		if (!Result)
		{
			return;
		}
		std::string const& raw = TotalFileData["raw"].GetStringData();
		std::unique_ptr<MBUtility::MBBufferInputStream> BufferStream = std::unique_ptr<MBUtility::MBBufferInputStream>(new MBUtility::MBBufferInputStream(raw.data(), raw.size()));
		SlippiEventParser EventParser(std::move(BufferStream));
		EventParser.GetNextEvent();
		Event GameStart = EventParser.GetNextEvent();
		if (GameStart.GetType() != EventType::GameStart)
		{
			return;
		}
		std::unordered_map < std::string, MBScript::MBSObjectStore> MetaData = {};
		std::map<std::string, MBParsing::JSONObject> const& PlayerArray = TotalFileData["metadata"]["players"].GetMapData();
		std::unique_ptr<MBScript::MBSObject> PlayerList = std::make_unique<MBScript::MBSObject_List>();
		for (auto const& Element : PlayerArray)
		{
			//kanske borde göra en aggregate klass...
			std::unordered_map<std::string, MBScript::MBSObjectStore> PlayerData;
			PlayerData["Code"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(Element.second.GetAttribute("names").GetAttribute("code").GetStringData()));
			PlayerData["Tag"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(Element.second.GetAttribute("names").GetAttribute("netplay").GetStringData()));
			PlayerData["Character"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(CharacterToString(InternalCharacterID(std::stoi(Element.second.GetAttribute("characters").GetMapData().begin()->first)))));
			*PlayerList += std::make_unique<MBS_SlippiPlayerMetadata>(AssociatedModule->GetTypeConversion(MBSSlippiTypes::PlayerMetadata),std::move(PlayerData));
		}
		MetaData["Players"] = std::move(PlayerList);
		m_Fields["MetaData"] = MBScript::MBSObjectStore(std::make_unique<MBS_SlippiPlayerMetadata>(AssociatedModule->GetTypeConversion(MBSSlippiTypes::PlayerMetadata), std::move(MetaData)));


		//frames grejer
		std::unique_ptr<MBScript::MBSObject> FrameList = std::make_unique<MBScript::MBSObject_List>();
		FrameParser LocalFrameParser;
		LocalFrameParser.SetVersion(EventParser.GetVersion());
		while (true)
		{
			Event NewEvent = EventParser.GetNextEvent();
			if (NewEvent.GetType() == EventType::Null)
			{
				break;
			}
			LocalFrameParser.InsertEvent(std::move(NewEvent));
			if (LocalFrameParser.AvailableFrames() > 0)
			{
				*FrameList += std::make_unique<MBS_SlippiFrame>(LocalFrameParser.ExtractFrame(),*AssociatedModule);
			}
		}
		if (LocalFrameParser.AvailableFrames() > 0)
		{
			*FrameList += std::make_unique<MBS_SlippiFrame>(LocalFrameParser.ExtractFrame(),*AssociatedModule);
		}
		m_Fields["Frames"] = MBScript::MBSObjectStore(std::move(FrameList));
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiGame::Copy() const
	{
		std::unique_ptr<MBS_SlippiGame> ReturnValue = std::make_unique<MBS_SlippiGame>();
		ReturnValue->m_Type = m_Type;
		ReturnValue->m_Fields = m_Fields;
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiGame::DotOperator(std::string const& FieldName)
	{
		if (m_Fields.find(FieldName) == m_Fields.end())
		{
			throw MBScript::MBSRuntimeException("Field \"" + FieldName + "\" not defined in object");
		}
		return(m_Fields[FieldName].GetReferenceObject());
	}
	//END MBS_Slippi Objects




	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::ActionableFrames(MBScript::ArgumentList Argumnets)
	{
		if (Argumnets.Arguments.size() > 0)
		{
			throw MBScript::MBSRuntimeException("Actionable frames takes only 1 argument");
		}
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::InShieldStun(MBScript::ArgumentList Argumnets)
	{
		if (Argumnets.Arguments.size() > 0)
		{
			throw MBScript::MBSRuntimeException("InShieldStun frames takes only 1 argument");
		}
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::LoadGame(MBScript::ArgumentList Argumnets)
	{
		if (Argumnets.Arguments.size() > 1 || Argumnets.Arguments.size() == 0)
		{
			throw MBScript::MBSRuntimeException("InShieldStun frames takes only 1 argument");
		}
		std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBS_SlippiGame>(MBScript::CastObject<MBScript::MBSObject_String>(*Argumnets.Arguments[0]).Value,this);
		return(ReturnValue);
	}
	MBScript::ObjectType MBS_SlippiModule::GetTypeConversion(MBSSlippiTypes TypeToConvert)
	{
		return(m_TypeMap.at(TypeToConvert));
	}
	std::vector<std::string> MBS_SlippiModule::GetExportedFunctions() const
	{
		std::vector<std::string> ReturnValue = {};
		for (auto const& Element : m_StaticFunctions)
		{
			ReturnValue.push_back(Element.first);
		}
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::ExecuteFunction(std::string const& FunctionName, MBScript::ArgumentList Arguments)
	{
		return(((*this).*m_StaticFunctions.at(FunctionName))(std::move(Arguments)));
	}
	void MBS_SlippiModule::OnCreate(MBScript::ExecutionEnvironment& AssociatedEnvironment)
	{
		m_TypeMap[MBSSlippiTypes::Frame] = AssociatedEnvironment.GetUniqueTypeID();
		m_TypeMap[MBSSlippiTypes::PlayerFrameInfo] = AssociatedEnvironment.GetUniqueTypeID();
		m_TypeMap[MBSSlippiTypes::Metadata] = AssociatedEnvironment.GetUniqueTypeID();
		m_TypeMap[MBSSlippiTypes::PlayerMetadata] = AssociatedEnvironment.GetUniqueTypeID();
	}
}