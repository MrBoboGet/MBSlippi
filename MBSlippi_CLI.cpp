#include "MBSlippi_CLI.h"
#include "MBCLI/MBCLI.h"
#include <MBUtility/MBFiles.h>
#include <cstdlib>
#include <MBPacketManager/MB_PacketProtocol.h>
#include <MrBoboDatabase/MrBoboDatabase.h>
#include "MeleeID.h"

#include <MBUtility/InterfaceAdaptors.h>

#include <chrono>
#include <exception>
#include <filesystem>
#include <sstream>

#include "SlippiSpecParser.h"
#include "SlippiSpec.h"

#include <MBSystem/MBSystem.h>
namespace MBSlippi
{
    void MQLServer::SendMessage(MBUtility::MBOctetOutputStream& OutStream,MBParsing::JSONObject const& ObjectToSend)
    {
        std::string DataToWrite = ObjectToSend.ToString();
        MBParsing::WriteBigEndianInteger(OutStream,DataToWrite.size(),4);
        size_t WrittenBytes = OutStream.Write(DataToWrite.data(),DataToWrite.size());
        if(WrittenBytes < DataToWrite.size())
        {
            throw std::runtime_error("error writing request response");
        }
    }
    void MQLServer::p_EvaluatorThread()
    {
        while(m_IsRunning.load())
        {
            std::unique_lock<std::mutex> Lock(m_StatementLock);
            while(m_IsRunning.load() && m_StatementsToEvaluate.size() == 0)
            {
                m_Evalutating.store(false);
                m_NotifyConditional.wait(Lock);
            }
            if(m_IsRunning.load() == false)
            {
                break;
            }
            m_Evalutating.store(true);
            Statement StatementToExecute = std::move(m_StatementsToEvaluate.front());
            m_StatementsToEvaluate.pop_front();
            {
                std::vector<MBLSP::Diagnostic> Errors;
                {
                    std::lock_guard<std::mutex> EvalLock(m_EvaluatorMutex);
                    m_Evaluator->EvaluateStatement(StatementToExecute,Errors);
                }
                MBParsing::JSONObject Result(MBParsing::JSONObjectType::Aggregate);
                Result["method"] = "execute/result";
                Result["jsonrpc"] = "2.0";
                if(Errors.size() != 0)
                {
                    Result["params"] = std::map<std::string,MBParsing::JSONObject>{{"result","Error: "+Errors[0].message}};
                }
                else
                {
                    Result["params"] = std::map<std::string,MBParsing::JSONObject>{{"result",""}};
                }
                m_AssociatedHandler->SendRawNotificaction(Result);
                //assert(Errors.size() == 0 && "p_EvaluatorThread should only have");
            }
        }
    }
    void MQLServer::p_ExecuteAsync(Statement StatementToExecute)
    {
        {
            std::lock_guard<std::mutex> Lock(m_StatementLock);
            m_StatementsToEvaluate.push_back(std::move(StatementToExecute));
            m_NotifyConditional.notify_one();
        }
    }
    MBParsing::JSONObject MQLServer::HandleGenericRequest(MBParsing::JSONObject const& GenericRequest)
    {
        auto Tokenizer = GetTokenizer();
        MBParsing::JSONObject MessageToSend(MBParsing::JSONObjectType::Aggregate);
        MessageToSend["jsonrpc"] = "2.0";
        try
        {
            MBError ParseResult = true;
            if(!ParseResult)
            {
                MessageToSend["error"] =
                std::map<std::string,MBParsing::JSONObject>{{"message", "Error parsing JSON object"},{"code",1}};
                return(MessageToSend);
            }
            if(GenericRequest.GetType() != MBParsing::JSONObjectType::Aggregate)
            {
                MessageToSend["error"] =
                std::map<std::string,MBParsing::JSONObject>{{"message", "Error interpreting json object: object is not of aggregate type"},{"code",1}};
                return(MessageToSend);
            }
            if(GenericRequest["method"].GetStringData() == "execute")
            {
                std::string const& StringToExecute = GenericRequest["params"]["statement"].GetStringData();
                Tokenizer.SetText(StringToExecute);
                if(m_Evalutating.load())
                {
                    MessageToSend["error"] = 
                        std::map<std::string,MBParsing::JSONObject>{{"message","Already evaluating statement"},{"code",1}};
                }
                try
                {
                    Statement StatementsToExecute = ParseStatement(Tokenizer);
                    std::vector<MBLSP::Diagnostic> Errors;
                    {
                        std::lock_guard<std::mutex> Lock(m_EvaluatorMutex);
                        m_Evaluator->VerifyStatement(StatementsToExecute,Errors);
                    }
                    if(Errors.size() > 0)
                    {
                        MessageToSend["error"] = 
                        std::map<std::string,MBParsing::JSONObject>{{"message",Errors[0].message},{"code",1}};
                    }
                    else
                    {
                        MessageToSend["result"] = "executing...";
                    }
                    p_ExecuteAsync(std::move(StatementsToExecute));
                }
                catch(std::exception const& e)
                {
                    MessageToSend["error"] = 
                std::map<std::string,MBParsing::JSONObject>{{"message",std::string(e.what())},{"code",1}};
                }
            }
            else if(GenericRequest["method"].GetStringData() == "exit")
            {
                return(0);
            }
            else
            {
                MessageToSend["error"] =
                std::map<std::string,MBParsing::JSONObject>{{"message","Unkown method \""+GenericRequest["method"].GetStringData()+"\""},{"code",1}};
            }
        }
        catch(std::exception const& e)
        {
            std::cerr <<"Error when executing server: "<<e.what()<<std::endl;
        }
        return(MessageToSend);
    }
    std::vector<SlippiGameInfo> MBSlippiCLIHandler::RetrieveGames(std::string const& WhereCondition)
    {
        std::vector<SlippiGameInfo> ReturnValue;
        MBDB::MrBoboDatabase Database(m_Config.ReplaysDirectory + "/SlippiGames.db", MBDB::DBOpenOptions::ReadWrite);
        std::string QueryString = "SELECT * FROM GAMES";
        if(WhereCondition != "")
        {
            QueryString += "WHERE "+WhereCondition;   
        }
        MBError QueryError = true;
        std::vector<MBDB::MBDB_RowData> Result = Database.GetAllRows(QueryString,&QueryError);
        for(MBDB::MBDB_RowData const& Row : Result)
        {
            SlippiGameInfo NewGameInfo;
            MBDB::MaxInt GameID = Row.GetColumnData<int>(0);
            NewGameInfo.AbsolutePath = m_Config.ReplaysDirectory + "/" + Row.GetColumnData<std::string>(1);
            NewGameInfo.Date = Row.GetColumnData<int>(2);
            NewGameInfo.Stage = Row.GetColumnData<std::string>(3);
            NewGameInfo.FrameDuration = Row.GetColumnData<int>(4);


            auto PlayerQuery = Database.GetSQLStatement("SELECT * FROM PLAYERS WHERE GameID = ?");
            PlayerQuery->BindInt(GameID,1);
            auto Rows = Database.GetAllRows(PlayerQuery,&QueryError);
            Database.FreeSQLStatement(PlayerQuery);
            for(auto const& Row : Rows)
            {
                int Index = Row.GetColumnData<int>(1);
                if(Index < 0 || Index > 3)
                {
                    throw std::runtime_error("Error retrieving specgames: player port above 3 or below 0 in database index");
                }
                NewGameInfo.PlayerInfo[Index].Code = Row.GetColumnData<std::string>(2);
                NewGameInfo.PlayerInfo[Index].Tag = Row.GetColumnData<std::string>(3);
                NewGameInfo.PlayerInfo[Index].Code = Row.GetColumnData<std::string>(4);
            }
            ReturnValue.push_back(NewGameInfo);
        }
        return(ReturnValue);
    }
    
    void MBSlippiCLIHandler::p_WriteReplayInfo(std::vector<RecordingInfo> const& RecordingsToWrite,MBUtility::MBOctetOutputStream& OutStream)
    {
		MBParsing::JSONObject JSONToWrite(MBParsing::JSONObjectType::Aggregate);
		JSONToWrite["mode"] = "queue";
		JSONToWrite["replay"] = "";
		JSONToWrite["isRealTimeMode"] = false;
		JSONToWrite["outputOverlayFiles"] = true;
		std::vector<MBParsing::JSONObject> QueueElements;
		for(auto const& Recording : RecordingsToWrite)
		{
			std::string Path = Recording.GamePath;
			for (auto const& Intervall : Recording.IntervallsToRecord)
			{
				MBParsing::JSONObject NewQueueElement(MBParsing::JSONObjectType::Aggregate);
				NewQueueElement["path"] = Path;
                //replays start at -123, lmao, just hardcode it here.
				NewQueueElement["startFrame"] =int64_t(Intervall.FirstFrame-123);
				NewQueueElement["endFrame"] = int64_t(Intervall.LastFrame-123);
				QueueElements.push_back(std::move(NewQueueElement));
			}
		}
		JSONToWrite["queue"] = std::move(QueueElements);
        OutStream<<JSONToWrite.ToString();
        OutStream.Flush();
    }
	void MBSlippiCLIHandler::p_RestoreDolphinConfigs(std::string const& DumpDirectory, DolphinConfigParser const& DolphinINI, DolphinConfigParser const& DolphinGFX)
	{
		std::filesystem::remove_all(DumpDirectory);
		if (!std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini") || !std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini"))
		{
			throw MBScript::MBSRuntimeException("Dolphin configs doesnt exist. Invalid dolphin replay path specified?");
		}
		DolphinGFX.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini");
		DolphinINI.WriteValues(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini");
	}
    std::string MBSlippiCLIHandler::p_UpdateDolphinConfigs(DolphinConfigParser* OutOriginalIni,DolphinConfigParser* OutOriginalGFX)
    {
        std::string DumpPath = ".__DolphinDumpDirectory";
        std::filesystem::create_directory(DumpPath);

        std::string AbsoluteDumpPath = MBUnicode::PathToUTF8(std::filesystem::absolute(DumpPath));

        if (!std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/Dolphin.ini") || !std::filesystem::exists(m_Config.ReplayDolphinDirectory + "/User/Config/GFX.ini"))
        {
            throw std::runtime_error("Dolphin configs doesnt exist. Invalid dolphin replay path specified?");
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
    void MBSlippiCLIHandler::RecordGames(std::vector<RecordingInfo> const& GamesToRecord,std::filesystem::path const& OutPath)
    {
        if(GamesToRecord.size() == 0)
        {
            m_Terminal.PrintLine("Empty list of games to record, skipping recording");
            return;   
        }
        std::unique_ptr<MBScript::MBSObject> ReturnValue = std::make_unique<MBScript::MBSObject>();
        //std::string ReplayInfoFile = MBUnicode::PathToUTF8(OutPath);
        std::string OutputVideo = MBUnicode::PathToUTF8(OutPath);
        DolphinConfigParser OriginalIni;
        DolphinConfigParser OriginalGFX;
        std::string DumpDirectory = p_UpdateDolphinConfigs(&OriginalIni, &OriginalGFX);
        
        {
            std::ofstream ReplayInfoFile = std::ofstream(".__DolphinDumpDirectory/ReplayInfo.json");
            if(!ReplayInfoFile.is_open())
            {
                throw std::runtime_error("Error opening \".__DolphinDumpDirectory/ReplayInfo.json\", needed to create replay file");
            }
            MBUtility::MBFileOutputStream OutStream(&ReplayInfoFile);
            p_WriteReplayInfo(GamesToRecord,OutStream);
        }
        std::string DolphinCommand = "\"\"" + m_Config.ReplayDolphinDirectory + "\\SlippiMBFix.exe\"" + " -b -e " + "\"" + m_Config.MeleeISOPath + "\" -i " + ".__DolphinDumpDirectory/ReplayInfo.json"+"\"";
        int DolphinResult = std::system(DolphinCommand.c_str());
        std::string FFMPEGCommand = "ffmpeg -y -i " + DumpDirectory + "/Frames/framedump0.avi -i " + DumpDirectory + "/Audio/dspdump.wav -map 0:v:0 -map 1:a:0 " + OutputVideo+" > nul 2> nul";
        int FFMPEGResult = std::system(FFMPEGCommand.c_str());
        p_RestoreDolphinConfigs(DumpDirectory,OriginalIni,OriginalGFX);
    }
	void MBSlippiCLIHandler::p_LoadGlobalConfig()
	{
		try
		{
            std::filesystem::path FileToRead = MBSystem::GetUserHomeDirectory()/".mbslippi/MBSlippiConfig.json";
			std::string TotalConfigData = MBUtility::ReadWholeFile(MBUnicode::PathToUTF8(FileToRead));
			if (TotalConfigData == "")
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Couldnt find or load data from MBSlippiConfig.json");
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
				std::exit(1);
			}
			MBError ParseResult = true;
			MBParsing::JSONObject ConfigJSON = MBParsing::ParseJSONObject(TotalConfigData, 0, 0, &ParseResult);
			if (!ParseResult)
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Error parsing MBSlippiConfig file: " + ParseResult.ErrorMessage);
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
				std::exit(1);
			}
			m_Config = MBSlippiConfig(ConfigJSON);
		}
		catch (std::exception const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Error loading config: " +std::string(e.what()));
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
	}
	SlippiGameInfo MBSlippiCLIHandler::p_SlippiFileToGameInfo(MBParsing::JSONObject const& TotalGameData)
	{
		SlippiGameInfo ReturnValue;
		
		std::string const& RawData = TotalGameData["raw"].GetStringData();
		std::unique_ptr<MBUtility::MBOctetInputStream> InputStream = std::make_unique<MBUtility::MBBufferInputStream>(RawData.data(), RawData.size());
		SlippiEventParser EventParser(std::move(InputStream));
		Event CurrentEvent = EventParser.GetNextEvent();
		while (CurrentEvent.GetType() != EventType::Null)
		{
			if (CurrentEvent.GetType() == EventType::GameStart)
			{
				Event_GameStart const& CurrentEventData = CurrentEvent.GetEventData<Event_GameStart>();
				ReturnValue.Stage = StageIDToString(GameInfoBlock(CurrentEventData.GameInfoBlock, sizeof(CurrentEventData.GameInfoBlock)).Stage);
				break;
			}
			CurrentEvent = EventParser.GetNextEvent();
		}
		std::tm StoredTime{};
		std::istringstream(TotalGameData["metadata"]["startAt"].GetStringData()) >> std::get_time(&StoredTime, "%Y-%m-%d");
		StoredTime.tm_isdst = -1;
		ReturnValue.Date = uint64_t(std::mktime(&StoredTime));
		
		ReturnValue.FrameDuration = TotalGameData.GetAttribute("metadata").GetAttribute("lastFrame").GetIntegerData();
		
		//std::cout << TotalGameData["metadata"].ToString() << std::endl;

		if (TotalGameData["metadata"]["players"]["0"]["names"].HasAttribute("code"))
		{
			ReturnValue.PlayerInfo[0].Code = TotalGameData["metadata"]["players"]["0"]["names"]["code"].GetStringData();
			ReturnValue.PlayerInfo[1].Code = TotalGameData["metadata"]["players"]["1"]["names"]["code"].GetStringData();
			ReturnValue.PlayerInfo[2].Code = TotalGameData["metadata"]["players"]["2"]["names"]["code"].GetStringData();
			ReturnValue.PlayerInfo[3].Code = TotalGameData["metadata"]["players"]["3"]["names"]["code"].GetStringData();
		}
		if (TotalGameData["metadata"]["players"]["0"]["names"].HasAttribute("netplay"))
		{
			ReturnValue.PlayerInfo[0].Tag = TotalGameData["metadata"]["players"]["0"]["names"]["netplay"].GetStringData();
			ReturnValue.PlayerInfo[1].Tag = TotalGameData["metadata"]["players"]["1"]["names"]["netplay"].GetStringData();
			ReturnValue.PlayerInfo[2].Tag = TotalGameData["metadata"]["players"]["2"]["names"]["netplay"].GetStringData();
			ReturnValue.PlayerInfo[3].Tag = TotalGameData["metadata"]["players"]["3"]["names"]["netplay"].GetStringData();
		}
		if (TotalGameData["metadata"]["players"]["0"].HasAttribute("characters"))
		{
			ReturnValue.PlayerInfo[0].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["0"]["characters"].GetMapData().begin()->first)));
			ReturnValue.PlayerInfo[1].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["1"]["characters"].GetMapData().begin()->first)));
			ReturnValue.PlayerInfo[2].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["2"]["characters"].GetMapData().begin()->first)));
			ReturnValue.PlayerInfo[3].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["3"]["characters"].GetMapData().begin()->first)));
		}
		return(ReturnValue);
	}
	void MBSlippiCLIHandler::p_CreateDatabase(std::string const& DirectoryToCreate)
	{
		if (!std::filesystem::exists(DirectoryToCreate))
		{
			throw std::runtime_error("Invalid replay directory");
		}
		MBDB::MrBoboDatabase Database(DirectoryToCreate + "\\SlippiGames.db",MBDB::DBOpenOptions::ReadWrite);
		std::string GamesTable = "CREATE TABLE GAMES (GameID INTEGER PRIMARY KEY,RelativePath VARCHAR(65535),Date INTEGER,Stage VARCHAR(255),FrameDuration INTEGER);";
        std::string PlayerTable = "CREATE TABLE Players(GameID INTEGER,Port INTEGER,Code VARCHAR(255),Tag VARCHAR(255),Character VARCHAR(255));";
		MBDB::SQLStatement* Statement = Database.GetSQLStatement(GamesTable);
		MBError Result = true;
		Database.GetAllRows(Statement,&Result);
        Database.FreeSQLStatement(Statement);
		Statement = Database.GetSQLStatement(PlayerTable);
		Result = true;
		Database.GetAllRows(Statement,&Result);
        Database.FreeSQLStatement(Statement);
	}
	void MBSlippiCLIHandler::p_UpdateGameSQLDatabase(std::string const& ReplayDirectory, MBPM::MBPP_FileInfoReader const& FileInfo)
	{
		auto FileIterator = FileInfo.GetDirectoryInfo("/")->begin();
		auto End = FileInfo.GetDirectoryInfo("/")->end();
		uint32_t ParsedFiles = 0;
		uint32_t TotalFiles = 0;
		if (!std::filesystem::exists(ReplayDirectory + "/SlippiGames.db") || !std::filesystem::directory_entry(ReplayDirectory + "/SlippiGames.db").is_regular_file())
		{
			p_CreateDatabase(ReplayDirectory);
		}
		MBDB::MrBoboDatabase Database(ReplayDirectory + "/SlippiGames.db", MBDB::DBOpenOptions::ReadWrite);
		for(;FileIterator != End;FileIterator++)
		{
			if (!std::filesystem::exists(ReplayDirectory + FileIterator.GetCurrentDirectory() + "/" + FileIterator->FileName))
			{
				continue;
			}
			if (FileIterator->FileName.find('.') == FileIterator->FileName.npos || FileIterator->FileName.substr(FileIterator->FileName.find('.') + 1) != "slp")
			{
				continue;
			}
			std::string TotalFileData = MBUtility::ReadWholeFile(ReplayDirectory+FileIterator.GetCurrentDirectory() + "/" + FileIterator->FileName);
			if (TotalFileData == "")
			{
				continue;
			}
			TotalFiles++;
			MBError ParseResult = true;
			MBUtility::MBBufferInputStream InputStream(TotalFileData.data(), TotalFileData.size());
			MBParsing::JSONObject SlippiData = MBParsing::ParseUBJSON(&InputStream, &ParseResult);
			if (!ParseResult)
			{
				continue;
			}
			SlippiGameInfo NewGame;
			try
			{
				NewGame = p_SlippiFileToGameInfo(SlippiData);
			}
			catch(std::exception const& e)
			{
				continue;
			}
            std::string RelativePath = FileIterator.GetCurrentDirectory() + "/" + FileIterator->FileName;
            
            //Should actually check for errors...
			std::string QuerryToInsert = "INSERT INTO GAMES VALUES (RelativePath,Date,Stage,FrameDuration) (?,?,?,?);";
			MBDB::SQLStatement* Statement = Database.GetSQLStatement(QuerryToInsert);
			Statement->BindString(RelativePath, 1);
			Statement->BindInt(NewGame.Date, 2);
			Statement->BindString(NewGame.Stage, 3);
			Statement->BindInt(NewGame.FrameDuration, 10);
			MBError Result = true;
			Database.GetAllRows(Statement, &Result);
			Database.FreeSQLStatement(Statement);
            for(int i = 0; i < 4; i++)
            {
                std::string PlayerQuery = "INSERT INTO PLAYERS VALUES (GameID,Port,Code,Tag,Character)(?,?,?,?,?);";
                //how to get GameID?
                static_assert(false && "Fix GameID");
                Statement = Database.GetSQLStatement(QuerryToInsert);
                Statement->BindInt(i,2);
                Statement->BindString(NewGame.PlayerInfo[i].Code,3);
                Statement->BindString(NewGame.PlayerInfo[i].Tag,4);
                Statement->BindString(NewGame.PlayerInfo[i].Character,5);
                Database.GetAllRows(Statement, &Result);
                Database.FreeSQLStatement(Statement);
            }

			ParsedFiles += 1;
		}
		if (TotalFiles > ParsedFiles)
		{
			m_Terminal.PrintLine("Totalfiles: " + std::to_string(TotalFiles) + " Parsed Files: " + std::to_string(ParsedFiles));
		}

	}
	void MBSlippiCLIHandler::p_Handle_UpdateIndex(MBCLI::ProcessedCLInput const& Input)
	{
		if (!std::filesystem::exists(m_Config.ReplaysDirectory) || !std::filesystem::directory_entry(m_Config.ReplaysDirectory).is_directory())
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Cannot update Index: filesystem object doesnt exist or isn't a directory");
			return;
		}
		if (!std::filesystem::exists(m_Config.ReplaysDirectory+"/MBPM_FileInfo") || Input.CommandOptions.find("override") != Input.CommandOptions.end())
		{
			MBPM::MBPP_FileInfoReader ReplayFileInfo;
			MBPM::MBPP_FileInfoReader::CreateFileInfo(m_Config.ReplaysDirectory,&ReplayFileInfo);
			if (!ReplayFileInfo.ObjectExists("/"))
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Failed to create FileInfoIndex for replay directory");
				return;
			}
			p_UpdateGameSQLDatabase(m_Config.ReplaysDirectory, ReplayFileInfo);
			std::ofstream OutFile(m_Config.ReplaysDirectory + "/MBPM_FileInfo", std::ios::out | std::ios::binary);
			MBUtility::MBFileOutputStream OutStream(&OutFile);
			ReplayFileInfo.WriteData(&OutStream);
		}
		else
		{
			MBPM::MBPP_FileInfoReader UpdatedFiles = MBPM::MBPP_FileInfoReader::GetLocalUpdatedFiles(m_Config.ReplaysDirectory);
			if (UpdatedFiles.GetDirectoryInfo("/") == nullptr)
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Error getting local file info difference");
				return;
			}
			MBPM::MBPP_DirectoryInfoNode const* TopDirectory = UpdatedFiles.GetDirectoryInfo("/");
			MBPM::MBPP_FileInfoDiff FileDiff = MBPM::MBPP_FileInfoReader::GetLocalInfoDiff(m_Config.ReplaysDirectory);
			if ((TopDirectory->Files.size() == 0 && TopDirectory->Directories.size() == 0) && (FileDiff.RemovedFiles.size() == 0 && FileDiff.DeletedDirectories.size() == 0))
			{
				m_Terminal.Print("Index already up to date");
				return;
			}
            try
            {
                p_UpdateGameSQLDatabase(m_Config.ReplaysDirectory, UpdatedFiles);
            }
            catch(std::exception const& e)
            {
                m_Terminal.PrintLine("Error updating index: Error updateing SQL database: "+std::string(e.what()));   
                return;
            }
			MBPM::MBPP_FileInfoReader OldInfo = MBPM::MBPP_FileInfoReader(m_Config.ReplaysDirectory + "/MBPM_FileInfo");
			OldInfo.UpdateInfo(UpdatedFiles);
			OldInfo.DeleteObjects(FileDiff.RemovedFiles);
			OldInfo.DeleteObjects(FileDiff.DeletedDirectories);
			std::ofstream OutFile(m_Config.ReplaysDirectory+"/MBPM_FileInfo", std::ios::out | std::ios::binary);
			MBUtility::MBFileOutputStream OutStream(&OutFile);
			OldInfo.WriteData(&OutStream);

		}
	}
    void MBSlippiCLIHandler::p_Handle_Execute(MBCLI::ProcessedCLInput const& Input)
    {
		if (Input.TopCommandArguments.size() != 1)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("execute needs exactly 1 argument: the path to a MBScript file");
			std::exit(1);
		}
		std::string SlippiScriptToExecute = Input.TopCommandArguments[0];
        Module ModuleToEvaluate;
        SpecEvaluator Evaluator;
        auto Tokenizer = GetTokenizer();
        try
        {
            Evaluator.SetDBAdapter(this);
            Evaluator.SetRecorder(this);
            Tokenizer.SetText(MBUtility::ReadWholeFile(SlippiScriptToExecute));
            ModuleToEvaluate = ParseModule(Tokenizer);
            std::vector<MBLSP::Diagnostic> Diagnostics;
            //DEBUG
            std::vector<ServerInitilizationData> Servers;
            //Servers.emplace_back();
            //Servers.back().ExecutableName = "sbcl";
            //Servers.back().ExecutableArguments = {"--noinform","--core","TestServerSBCL.mem"};
            //Evaluator.InitializeServers(Servers);
            Evaluator.EvaluateModule(ModuleToEvaluate,Diagnostics);
            if(Diagnostics.size() > 0)
            {
                m_Terminal.PrintLine("Semantic error evaluating SlippiSpec:");
                for(auto const& Diagnostic : Diagnostics)
                {
                    m_Terminal.PrintLine(Diagnostic.message);
                }   
            }
        }
        catch(std::exception const& e)
        {
            m_Terminal.PrintLine("Error parsing SlippiSpec file: "+std::string(e.what()));
            std::exit(1);
        }
    }
    int MBSlippiCLIHandler::p_HandleServer(MBCLI::ProcessedCLInput const& Input)
    {
        std::unique_ptr<MBUtility::IndeterminateInputStream> InputStream = 
            std::unique_ptr<MBUtility::IndeterminateInputStream>(new MBUtility::NonOwningIndeterminateInputStream(&m_Terminal.GetInputStream()));
        std::unique_ptr<MBUtility::MBOctetOutputStream> OutStream = 
           std::unique_ptr<MBUtility::MBOctetOutputStream>(new MBUtility::NonOwningOutputStream(&m_Terminal.GetOutputStream()));
        SpecEvaluator Evaluator;
        std::unique_ptr<MBLSP::LSP_Server> Server = 
            std::unique_ptr<MBLSP::LSP_Server>( new MQLServer(&Evaluator));
        Evaluator.SetDBAdapter(this);
        Evaluator.SetRecorder(this);
        MBLSP::LSP_ServerHandler Handler = 
            MBLSP::LSP_ServerHandler(std::move(InputStream),std::move(OutStream),std::move(Server));
        return(Handler.Run());
    }
    void MBSlippiCLIHandler::p_Handle_Execute_Legacy(MBCLI::ProcessedCLInput const& Input)
	{
		if (Input.TopCommandArguments.size() != 1)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("execute needs exactly 1 argument: the path to a MBScript file");
			std::exit(1);
		}
		std::string SlippiScriptToExecute = Input.TopCommandArguments[0];
		MBError Result = true;
		std::vector<std::unique_ptr<MBScript::MBSStatement>> StatementsToExecute = MBScript::ParseFile(SlippiScriptToExecute, &Result);
		if (!Result)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Error parsing MBScript file: " + Result.ErrorMessage);
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
		MBScript::ExecutionEnvironment ExecutionEnvironment;
		ExecutionEnvironment.AddModule(std::unique_ptr<MBS_SlippiModule>(new MBS_SlippiModule(m_Config)));
		try
		{
			MBError ExecutionResult = ExecutionEnvironment.Execute(StatementsToExecute);
			if (!ExecutionResult)
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Error executing MBScript file" + ExecutionResult.ErrorMessage);
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
				std::exit(1);
			}
		}
		catch (MBScript::MBSRuntimeException const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Unhandled MBScript runtime error: " + std::string(e.what()));
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
		catch (std::runtime_error const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Error with MBScript implementation: " + std::string(e.what()));
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
		catch (std::exception const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Unkown error occured: " + std::string(e.what()));
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
	}
	void MBSlippiCLIHandler::p_Handle_Play(MBCLI::ProcessedCLInput const& Input)
	{
		if (Input.TopCommandArguments.size() != 1)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("play needs exactly 1 argument: the path to a recorded replay file");
			std::exit(1);
		}
	}
	void MBSlippiCLIHandler::p_Handle_Query(MBCLI::ProcessedCLInput const& Input)
	{
		if (Input.TopCommandArguments.size() != 1)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Query needs exactly 1 argument: SQL querry to run");
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::White);
			std::exit(1);
		}
        if(!std::filesystem::exists(m_Config.ReplaysDirectory+"/SlippiGames.db") || !std::filesystem::is_regular_file(m_Config.ReplaysDirectory+"/SlippiGames.db"))
        {
            m_Terminal.PrintLine("Can't find database, try to run mbslippi index-update");
            return;
        }
		MBDB::MrBoboDatabase Database(m_Config.ReplaysDirectory + "/SlippiGames.db", MBDB::DBOpenOptions::ReadOnly);
        MBError QueryResult = true;
        std::vector<MBDB::MBDB_RowData> QuerryRows = Database.GetAllRows(Input.TopCommandArguments[0],&QueryResult);
        if(!QueryResult)
        {
            m_Terminal.PrintLine("Error executing query: " + QueryResult.ErrorMessage);
            return;
        }
        for(MBDB::MBDB_RowData const& Row : QuerryRows)
        {
            for(int i = 0; i < Row.GetNumberOfColumns();i++)
            {
                m_Terminal.Print(Row.ColumnToString(i)+" ");
            }   
            m_Terminal.PrintLine("");
        }
	}
	int MBSlippiCLIHandler::Run(int argc, const char** argv)
	{
        int ReturnValue = 0;
		MBCLI::ProcessedCLInput Input(argc, argv);
        try
        {
		    p_LoadGlobalConfig();
            //DEPRECATED Input.TopCommand
            if (Input.TopCommand == "update-index")
            {
                p_Handle_UpdateIndex(Input);
            }
            else if (Input.TopCommand == "execute")
            {
                p_Handle_Execute(Input);
            }
            else if(Input.TopCommand == "server")
            {
                ReturnValue = p_HandleServer(Input);
            }
            else if (Input.TopCommand == "play")
            {
                p_Handle_Play(Input);
            }
            else if (Input.TopCommand == "query")
            {
                p_Handle_Query(Input);
            }
        }
        catch(std::exception const& e)
        {
            ReturnValue = 1;
            m_Terminal.PrintLine("Uncaught exception executing command: "+std::string(e.what())); 
        }
        return(ReturnValue);
	}
}
