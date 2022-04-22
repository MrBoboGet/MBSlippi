#include "MBSlippiScript.h"
#include "MBMeleeID.h"
#include <filesystem>
#include <MBUnicode/MBUnicode.h>
#include <MBUtility/MBStrings.h>

#include <MrBoboDatabase/MrBoboDatabase.h>
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
		m_Fields["MBState"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(MBActionStateToString(StateToMBActionState(EventData))));
		m_Fields["Attack"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(MBAttackIDToString(StateToMBAttackID(EventData))));
		m_Fields["LastHitBy"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(MBAttackIDToString(StateToMBAttackID(EventData))));
		m_Fields["FrameNumber"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Integer>(EventData.FrameNumber));
		m_Fields["Shielding"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::ShieldActive)));
		m_Fields["InHitlag"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::InHitlag)));
		m_Fields["InHitstun"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.StateBitFlags & uint64_t(StateBitFlags::InHitstun)));
		m_Fields["InShieldStun"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_Bool>(EventData.ActionStateID == ActionState::GuardSetOff));
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
			if (Element.second["names"].HasAttribute("code"))
			{
				PlayerData["Code"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(Element.second.GetAttribute("names").GetAttribute("code").GetStringData()));
			}
			if (Element.second["names"].HasAttribute("tag"))
			{
				PlayerData["Tag"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(Element.second.GetAttribute("names").GetAttribute("netplay").GetStringData()));
			}
			if (Element.second["names"].HasAttribute("characters"))
			{
				PlayerData["Character"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(CharacterToString(InternalCharacterID(std::stoi(Element.second.GetAttribute("characters").GetMapData().begin()->first)))));
			}
			*PlayerList += std::make_unique<MBS_SlippiPlayerMetadata>(AssociatedModule->GetTypeConversion(MBSSlippiTypes::PlayerMetadata),std::move(PlayerData));
		}
		MetaData["Players"] = std::move(PlayerList);
		m_Fields["MetaData"] = MBScript::MBSObjectStore(std::make_unique<MBS_SlippiPlayerMetadata>(AssociatedModule->GetTypeConversion(MBSSlippiTypes::PlayerMetadata), std::move(MetaData)));
		m_Fields["Path"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(MBUnicode::PathToUTF8(std::filesystem::absolute(GamePath))));

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

	MBS_SlippiReplayInfo::MBS_SlippiReplayInfo(MBS_SlippiModule& AssociatedModule,MBScript::ArgumentList Arguments)
	{
		if (Arguments.Arguments.size() < 2)
		{
			throw MBScript::MBSRuntimeException("ReplayInfo requires atleast 2 arguments");
		}
		m_Type = AssociatedModule.GetTypeConversion(MBSSlippiTypes::ReplayInfo);
		//för att garantera att den är en string mest
		m_Fields["Path"] = MBScript::MBSObjectStore(std::make_unique<MBScript::MBSObject_String>(MBScript::CastObject<MBScript::MBSObject_String>(*Arguments.Arguments[0]).Value));
		if (Arguments.Arguments[1]->GetType() == MBScript::ObjectType::Reference)
		{
			m_Fields["Intervalls"] = Arguments.Arguments[1]->Copy();
		}
		else
		{
			m_Fields["Intervalls"] = std::move(Arguments.Arguments[1]);
		}
		
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiReplayInfo::Copy() const 
	{
		std::unique_ptr<MBS_SlippiReplayInfo> ReturnValue = std::make_unique<MBS_SlippiReplayInfo>();
		ReturnValue->m_Type = m_Type;
		ReturnValue->m_Fields = m_Fields;
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiReplayInfo::DotOperator(std::string const& FieldName)
	{
		if (m_Fields.find(FieldName) == m_Fields.end())
		{
			throw MBScript::MBSRuntimeException("Field \"" + FieldName + "\" not defined in object");
		}
		return(m_Fields[FieldName].GetReferenceObject());
	}
	//END MBS_Slippi Objects


	//BEGIN DolphinConfigParsser
	DolphinConfigParser::DolphinConfigParser(std::string const& FileToReadPath)
	{
		std::ifstream InputStream(FileToReadPath);
		std::string CurrentLine;

		std::string CurrentOptionType;
		while (std::getline(InputStream,CurrentLine))
		{
			if (CurrentLine == "")
			{
				continue;
			}
			if (CurrentLine[0] == '[')
			{
				CurrentOptionType = CurrentLine.substr(1, CurrentLine.size()- 2);
				m_Options.push_back(std::pair<std::string, std::vector<DolphinConfigOption>>(CurrentOptionType, {}));
			}
			else
			{
				size_t FirstEqual = CurrentLine.find('=');
				if(FirstEqual == CurrentLine.npos)
				{
					continue;
				}
				DolphinConfigOption NewOption;
				NewOption.Name = MBUtility::ReplaceAll(CurrentLine.substr(0, FirstEqual)," ","");
				NewOption.Value = CurrentLine.substr(FirstEqual + 1);
				m_Options.back().second.push_back(NewOption);
			}
		}
	}
	void DolphinConfigParser::InsertValue(std::string const& OptionType, std::string const& OptionName, std::string const& OptionValue)
	{
		size_t OptionTypeIndex = -1;
		for (size_t i = 0; i < m_Options.size(); i++)
		{
			if (m_Options[i].first == OptionType)
			{
				OptionTypeIndex = i;
				break;
			}
		}
		if (OptionTypeIndex == -1)
		{
			m_Options.push_back(std::pair<std::string, std::vector<DolphinConfigOption>>(OptionType, {}));
			OptionTypeIndex = m_Options.size() - 1;
		}
		std::vector<DolphinConfigOption> const& ArrayToTraverse = m_Options[OptionTypeIndex].second;
		size_t ArrayOptionIndex = -1;
		for (size_t i = 0; i < ArrayToTraverse.size(); i++)
		{
			if (ArrayToTraverse[i].Name == OptionName)
			{
				ArrayOptionIndex = i;
				break;
			}
		}
		if (ArrayOptionIndex == -1)
		{
			m_Options[OptionTypeIndex].second.push_back(DolphinConfigOption{ OptionName,OptionValue });
		}
		else
		{
			m_Options[OptionTypeIndex].second[ArrayOptionIndex] = DolphinConfigOption{ OptionName,OptionValue };
		}
	}
	void DolphinConfigParser::RemoveValue(std::string const& OptionType, std::string const& OptionName)
	{
		size_t OptionTypeIndex = -1;
		for (size_t i = 0; i < m_Options.size(); i++)
		{
			if (m_Options[i].first == OptionType)
			{
				OptionTypeIndex = i;
				break;
			}
		}
		if (OptionTypeIndex == -1)
		{
			return;
		}
		std::vector<DolphinConfigOption> const& ArrayToTraverse = m_Options[OptionTypeIndex].second;
		size_t ArrayOptionIndex = -1;
		for (size_t i = 0; i < ArrayToTraverse.size(); i++)
		{
			if(ArrayToTraverse[i].Name == OptionName)
			{
				ArrayOptionIndex = i;
				break;
			}
		}
		if (ArrayOptionIndex == -1)
		{
			return;
		}
		m_Options[OptionTypeIndex].second.erase(m_Options[OptionTypeIndex].second.begin() + ArrayOptionIndex);
	}
	void DolphinConfigParser::WriteValues(std::string const& Path) const
	{
		std::ofstream OutputStream(Path);
		for (size_t i = 0; i < m_Options.size(); i++)
		{
			OutputStream << "[" << m_Options[i].first << "]" << std::endl;
			for (size_t j = 0; j < m_Options[i].second.size(); j++)
			{
				OutputStream << m_Options[i].second[j].Name << " = " << m_Options[i].second[j].Value << std::endl;
			}
		}
		
	}
	//END DolphinConfigParser




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
		std::string GamePath = MBScript::CastObject<MBScript::MBSObject_String>(*Argumnets.Arguments[0]).Value;
		std::error_code Code;
		if (!std::filesystem::exists(GamePath,Code))
		{
			if (std::filesystem::exists(m_Config.ReplaysDirectory + "/" + GamePath))
			{
				GamePath = m_Config.ReplaysDirectory + "/" + GamePath;
			}
			else
			{
				throw MBScript::MBSRuntimeException("Game path doesnt exist");
			}
		}
		std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBS_SlippiGame>(GamePath,this);
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::ReplayInfo(MBScript::ArgumentList Arguments)
	{
		std::unique_ptr<MBS_SlippiReplayInfo> ReturnValue = std::make_unique<MBS_SlippiReplayInfo>(*this,std::move(Arguments));
		return(ReturnValue);
	}
	struct ReplayIntervall
	{
		int FirstFrame;
		int LastFrame;
	};
	bool h_ReplayIntervallComparison(ReplayIntervall const& Left, ReplayIntervall const& Right)
	{
		return(Left.FirstFrame < Right.FirstFrame);
	}
	std::vector<ReplayIntervall> h_NormalizeIntervalls(std::vector<ReplayIntervall>IntervallsToNormalize)
	{
		std::vector<ReplayIntervall> ReturnValue;
		if (IntervallsToNormalize.size() == 0)
		{
			throw std::runtime_error("Need atleast 1 intervall to normalize");
		}
		std::sort(IntervallsToNormalize.begin(), IntervallsToNormalize.end(), h_ReplayIntervallComparison);
		ReturnValue.push_back(IntervallsToNormalize.front());
		for (size_t i = 1; i < IntervallsToNormalize.size(); i++)
		{
			if (IntervallsToNormalize[i].FirstFrame <= ReturnValue.back().LastFrame)
			{
				ReturnValue.back().LastFrame = IntervallsToNormalize[i].LastFrame;
			}
			else
			{
				ReturnValue.push_back(IntervallsToNormalize[i]);
			}
		}
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::WriteReplayInfo(MBScript::ArgumentList Arguments)
	{
		std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBScript::MBSObject>();
		MBParsing::JSONObject JSONToWrite(MBParsing::JSONObjectType::Aggregate);
		JSONToWrite["mode"] = "queue";
		JSONToWrite["replay"] = "";
		JSONToWrite["isRealTimeMode"] = false;
		JSONToWrite["outputOverlayFiles"] = true;
		std::vector<MBParsing::JSONObject> QueueElements;

		MBScript::MBSObject_List& ReplayInfoList = MBScript::CastObject<MBScript::MBSObject_List>(*Arguments.Arguments[0]);
		for (size_t i = 0; i < ReplayInfoList.size(); i++)
		{
			std::string Path = MBUnicode::PathToUTF8(std::filesystem::absolute(MBScript::CastObject<MBScript::MBSObject_String>(*ReplayInfoList[i].DotOperator("Path")).Value));
			std::unique_ptr<MBScript::MBSObject> IntervallListReference = ReplayInfoList[i].DotOperator("Intervalls");
			MBScript::MBSObject_List& IntervallList = MBScript::CastObject<MBScript::MBSObject_List>(*IntervallListReference);
			std::vector<ReplayIntervall> IntervallsToWrite = {};
			for (size_t j = 0; j < IntervallList.size(); j++)
			{
				MBScript::MBSObject_List const& IntervallReference = MBScript::CastObject<MBScript::MBSObject_List>(IntervallList[j]);
				ReplayIntervall NewIntervall;
				NewIntervall.FirstFrame = MBScript::CastObject<MBScript::MBSObject_Integer>(IntervallReference[0]).Value;
				NewIntervall.LastFrame = MBScript::CastObject<MBScript::MBSObject_Integer>(IntervallReference[1]).Value;
				if (NewIntervall.FirstFrame > NewIntervall.LastFrame)
				{
					std::swap(NewIntervall.FirstFrame, NewIntervall.LastFrame);
				}
				IntervallsToWrite.push_back(NewIntervall);
			}
			IntervallsToWrite = h_NormalizeIntervalls(IntervallsToWrite);
			for (size_t i = 0; i < IntervallsToWrite.size(); i++)
			{
				MBParsing::JSONObject NewQueueElement(MBParsing::JSONObjectType::Aggregate);
				NewQueueElement["path"] = Path;
				NewQueueElement["startFrame"] =int64_t(IntervallsToWrite[i].FirstFrame);
				NewQueueElement["endFrame"] = int64_t( IntervallsToWrite[i].LastFrame);
				QueueElements.push_back(std::move(NewQueueElement));
			}
		}
		JSONToWrite["queue"] = std::move(QueueElements);

		std::string OutputPath = MBScript::CastObject<MBScript::MBSObject_String>(*Arguments.Arguments[1]).Value;

		std::ofstream OutStream(OutputPath);
		OutStream << JSONToWrite.ToString();
		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::GetGameQuery(MBScript::ArgumentList Arguments)
	{
		if (Arguments.Arguments.size() != 1)
		{
			throw MBScript::MBSRuntimeException("GetGameQuery requires exactly one argument: an SQL query to execute");
		}
		std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBScript::MBSObject_List>();
		MBDB::MrBoboDatabase DataBase(m_Config.ReplaysDirectory + "/SlippiGames.db", uint64_t(MBDB::DBOpenOptions::ReadOnly));
		std::string GameQuery = MBScript::CastObject<MBScript::MBSObject_String>(*Arguments.Arguments[0]).Value;
		
		MBDB::SQLStatement* Statement = DataBase.GetSQLStatement(GameQuery);
		MBError ErrorResult = true;
		std::vector<MBDB::MBDB_RowData> Result = DataBase.GetAllRows(Statement, &ErrorResult);
		if (!ErrorResult)
		{
			throw MBScript::MBSRuntimeException("Error executing querry: "+ErrorResult.ErrorMessage);
		}
		for (auto const& Row : Result)
		{
				std::unique_ptr<MBScript::MBSObject> NewList = std::make_unique<MBScript::MBSObject_List>();
			for (int i = 0; i < Row.GetNumberOfColumns(); i++)
			{
				if (Row.GetColumnValueType(i) == MBDB::MBDB_ColumnValueTypes::Int32)
				{
					*NewList += std::make_unique<MBScript::MBSObject_Integer>(Row.GetColumnData<int>(i));
				}
				else if (Row.GetColumnValueType(i) == MBDB::MBDB_ColumnValueTypes::Int64)
				{
					*NewList += std::make_unique<MBScript::MBSObject_Integer>(Row.GetColumnData<long long>(i));
				}
				else if (Row.GetColumnValueType(i) == MBDB::MBDB_ColumnValueTypes::Double)
				{
					assert(false);
				}
				else if (Row.GetColumnValueType(i) == MBDB::MBDB_ColumnValueTypes::Float)
				{
					assert(false);
				}
				else if (Row.GetColumnValueType(i) == MBDB::MBDB_ColumnValueTypes::Text)
				{
					*NewList += std::make_unique<MBScript::MBSObject_String>(Row.GetColumnData<std::string>(i));
				}
			}
			*ReturnValue += std::move(NewList);
		}

		return(ReturnValue);
	}
	std::unique_ptr<MBScript::MBSObject> MBS_SlippiModule::RecordReplay(MBScript::ArgumentList Arguments)
	{
		if (Arguments.Arguments.size() < 2 || (Arguments.Arguments[0]->GetType() != MBScript::ObjectType::String || Arguments.Arguments[1]->GetType() != MBScript::ObjectType::String))
		{
			throw MBScript::MBSRuntimeException("RecordReplay requires exactly 2 strings, path to ReplayInfo.json and name of outputfile");
		}
		std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBScript::MBSObject>();
		std::string ReplayInfoFile = MBScript::CastObject<MBScript::MBSObject_String>(*Arguments.Arguments[0]).Value;
		std::string OutputVideo = MBScript::CastObject<MBScript::MBSObject_String>(*Arguments.Arguments[1]).Value;
		DolphinConfigParser OriginalIni;
		DolphinConfigParser OriginalGFX;
		std::string DumpDirectory = p_UpdateDolphinConfigs(&OriginalIni, &OriginalGFX);
		std::string DolphinCommand = "\"\"" + m_Config.ReplayDolphinDirectory + "\\SlippiMBFix.exe\"" + " -b -e " + "\"" + m_Config.MeleeISOPath + "\" -i " + ReplayInfoFile+"\"";
		int DolphinResult = std::system(DolphinCommand.c_str());
		std::string FFMPEGCommand = "ffmpeg -y -i " + DumpDirectory + "/Frames/framedump0.avi -i " + DumpDirectory + "/Audio/dspdump.wav -map 0:v:0 -map 1:a:0 " + OutputVideo+" > nul 2> nul";
		int FFMPEGResult = std::system(FFMPEGCommand.c_str());
		p_RestoreDolphinConfigs(DumpDirectory,OriginalIni,OriginalGFX);
		return(ReturnValue);
	}
	std::string MBS_SlippiModule::p_UpdateDolphinConfigs(DolphinConfigParser* OutOriginalIni,DolphinConfigParser* OutOriginalGFX)
	{
		std::string DumpPath = ".__DolphinDumpDirectory";
		std::filesystem::create_directory(DumpPath);
		
		std::string AbsoluteDumpPath = MBUnicode::PathToUTF8(std::filesystem::absolute(DumpPath));
		
		if (!std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini") || !std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini"))
		{
			throw MBScript::MBSRuntimeException("Dolphin configs doesnt exist. Invalid dolphin replay path specified?");
		}
		DolphinConfigParser DolphinIniConfigs = DolphinConfigParser(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini");
		if (OutOriginalIni)
		{
			*OutOriginalIni = DolphinIniConfigs;
		}
		DolphinConfigParser DolphinGFXConfigs = DolphinConfigParser(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini");
		if (OutOriginalGFX)
		{
			*OutOriginalGFX = DolphinGFXConfigs;
		}
		DolphinIniConfigs.InsertValue("Core", "SlippiPlaybackExitOnFinished", "True");
		DolphinIniConfigs.InsertValue("Core", "EmulationSpeed", "0.00000000");
		DolphinIniConfigs.InsertValue("Movie", "DumpFrames", "True");
		DolphinIniConfigs.InsertValue("Movie", "DumpFramesSilent", "True");
		DolphinIniConfigs.InsertValue("DSP", "DumpAudio", "True");
		DolphinIniConfigs.InsertValue("DSP", "DumpAudioSilent", "True");
		DolphinIniConfigs.InsertValue("DSP", "Volume", "0");
		DolphinIniConfigs.InsertValue("General", "DumpPath", AbsoluteDumpPath);

		//DolphinIniConfigs.InsertValue("Display", "RenderWindowWidth", "1280");
		//DolphinIniConfigs.InsertValue("Display", "RenderWindowHeight", "1052");
		//DolphinIniConfigs.InsertValue("Display", "RenderWindowAutoSize", "True");

		//DolphinGFXConfigs.InsertValue("Settings", "EFBScale", "1");
		DolphinGFXConfigs.InsertValue("Settings", "BitrateKbps", "10000");

		DolphinGFXConfigs.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini");
		DolphinIniConfigs.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini");
		return(DumpPath);
	}
	void MBS_SlippiModule::p_RestoreDolphinConfigs(std::string const& DumpDirectory, DolphinConfigParser const& DolphinINI, DolphinConfigParser const& DolphinGFX)
	{
		std::filesystem::remove_all(DumpDirectory);
		if (!std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini") || !std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini"))
		{
			throw MBScript::MBSRuntimeException("Dolphin configs doesnt exist. Invalid dolphin replay path specified?");
		}
		DolphinGFX.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini");
		DolphinINI.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini");
	}
	MBS_SlippiModule::MBS_SlippiModule(MBSlippiConfig ConfigObject)
	{
		m_Config = std::move(ConfigObject);
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
		m_TypeMap[MBSSlippiTypes::ReplayInfo] = AssociatedEnvironment.GetUniqueTypeID();
	}
}