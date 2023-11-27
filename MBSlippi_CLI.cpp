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
#include <iomanip>


#include "SlippiSpecParser.h"
#include "SlippiSpec.h"

#include <MBSystem/MBSystem.h>
#include <MBUtility/MBFunctional.h>

#include <MBUtility/StreamReader.h>
#include <MBUtility/FileStreams.h>

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
                    //m_Evaluator->EvaluateStatement(StatementToExecute,Errors);
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
                        //m_Evaluator->VerifyStatement(StatementsToExecute,Errors);
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
    std::vector<SlippiGameInfo> MBSlippiCLIHandler::RetrieveGames(std::string const& WhereCondition,std::vector<std::string> const& GameSets)
    {
        std::vector<SlippiGameInfo> ReturnValue;
        for(auto const& Path : m_ReplayDirectories)
        {
            if(GameSets.size() != 0 && std::find(GameSets.begin(),GameSets.end(),Path.filename()) == GameSets.end())
            {
                continue;
            }
            MBDB::MrBoboDatabase Database(MBUnicode::PathToUTF8(Path/"SlippiGames.db"), MBDB::DBOpenOptions::ReadOnly);
            std::string QueryString = "SELECT * FROM GAMES";
            if(WhereCondition != "")
            {
                QueryString += "WHERE "+WhereCondition;   
            }
            QueryString += " ORDER BY GameID ASC;";
            MBError QueryError = true;
            std::vector<MBDB::MBDB_RowData> Result = Database.GetAllRows(QueryString,&QueryError);
            ReturnValue.reserve(ReturnValue.size() + Result.size());
            auto PlayerQuery = Database.GetSQLStatement("SELECT * FROM PLAYERS ORDER BY GameID ASC,Port ASC;");
            auto PlayerRows = Database.GetAllRows(PlayerQuery,&QueryError); 
            if(PlayerRows.size() != Result.size()*4)
            {
                throw std::runtime_error("Error in retrieving games from database at '"+
                        MBUnicode::PathToUTF8(Path)+"': Player rows not 4 times the game rows");
            }
            int PlayerRowIndex = 0;
            for(MBDB::MBDB_RowData const& Row : Result)
            {
                SlippiGameInfo NewGameInfo;
                MBDB::IntType GameID = Row.GetColumnData<MBDB::IntType>(0);
                NewGameInfo.AbsolutePath = MBUnicode::PathToUTF8( Path/Row.GetColumnData<std::string>(1));
                NewGameInfo.Date = Row.GetColumnData<MBDB::IntType>(2);
                NewGameInfo.Stage = Row.GetColumnData<std::string>(3);
                NewGameInfo.FrameDuration = Row.GetColumnData<MBDB::IntType>(4);


                for(int i = PlayerRowIndex; i < PlayerRowIndex+4;i++)
                {
                    auto const& PlayerRow = PlayerRows[i];
                    int Index = PlayerRow.GetColumnData<MBDB::IntType>(1);
                    if(Index < 0 || Index > 3)
                    {
                        throw std::runtime_error("Error retrieving specgames: player port above 3 or below 0 in database index");
                    }
                    NewGameInfo.PlayerInfo[Index].Code = PlayerRow.GetColumnData<std::string>(2);
                    NewGameInfo.PlayerInfo[Index].Tag = PlayerRow.GetColumnData<std::string>(3);
                    NewGameInfo.PlayerInfo[Index].Character = PlayerRow.GetColumnData<std::string>(4);
                }
                PlayerRowIndex += 4;
                ReturnValue.push_back(NewGameInfo);
            }
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
        //write the json info
        std::filesystem::path JSONPath = OutPath;
        JSONPath.replace_extension("json");
        std::ofstream OutJSON(JSONPath);
        MBParsing::JSONObject ObjectToWrite(MBParsing::JSONObjectType::Aggregate);
        std::vector<MBParsing::JSONObject> Frames; 
        std::vector<MBParsing::JSONObject> SituationBegin; 
        int TotalFrameIndex = 0;
        for(auto const& Recording : GamesToRecord)
        {
            for(auto const& Intervall : Recording.IntervallsToRecord)
            {
                for(int i = Intervall.FirstFrame; i < Intervall.LastFrame;i++)
                {
                    Frames.push_back(Recording.GameData.Frames[i].ToJSON());
                }   
                SituationBegin.push_back(TotalFrameIndex);
                //-2 because empirical evidence
                TotalFrameIndex += Intervall.LastFrame-Intervall.FirstFrame-2;
            }
        }
        ObjectToWrite["frames"] = std::move(Frames);
        ObjectToWrite["SituationBegin"] = std::move(SituationBegin);
        OutJSON<<ObjectToWrite.ToString();
    }
    bool MBSlippiCLIHandler::GameSetExists(std::string const& WhereCondition)
    {
        bool ReturnValue = false;
        for(auto const& Directory : m_ReplayDirectories)
        {
            if(Directory.filename() == WhereCondition)
            {
                return true;
            }
        }
        return ReturnValue;
    }
    void MBSlippiCLIHandler::LoadGlobalConfig()
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
            //Every directory in the ~/.mbslippi/Replays/ counts as a replay directory
            std::filesystem::directory_iterator DirIterator(MBSystem::GetUserHomeDirectory()/".mbslippi/Replays");
            for(auto const& Entry : DirIterator)
            {
                if(Entry.is_directory())
                {
                    m_ReplayDirectories.push_back(Entry.path());   
                }
            }
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
        auto const& PlayersData = TotalGameData["metadata"]["players"];
        while (CurrentEvent.GetType() != EventType::Null)
        {
            if (CurrentEvent.GetType() == EventType::GameStart)
            {
                Event_GameStart const& CurrentEventData = CurrentEvent.GetEventData<Event_GameStart>();
                GameInfoBlock GameInfo(CurrentEventData.GameInfoBlock,sizeof(CurrentEventData.GameInfoBlock));
                if(!PlayersData.HasAttribute("names"))
                {
                    for(int i = 0; i< 4;i++)
                    {
                        if(!(GameInfo.PlayerInfo[i].Type == PlayerType::Human))
                        {
                            continue;
                        }
                        ReturnValue.PlayerInfo[i].Tag = CurrentEventData.NameTags[i];
                        ReturnValue.PlayerInfo[i].Code = CurrentEventData.ConnectCode[i];
                        ReturnValue.PlayerInfo[i].Character = CharacterToString( GameInfo.PlayerInfo[i].Character);
                    }
                }
                ReturnValue.Stage = StageIDToString(GameInfo.Stage);
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

        if(PlayersData.HasAttribute("names"))
        {
            if (PlayersData.GetMapData().begin()->second["names"].HasAttribute("code"))
            {
                if(PlayersData.HasAttribute("0"))
                    ReturnValue.PlayerInfo[0].Code = PlayersData["0"]["names"].HasAttribute("code") ? PlayersData["0"]["names"]["code"].GetStringData() : "";
                if (PlayersData.HasAttribute("1"))
                    ReturnValue.PlayerInfo[1].Code = PlayersData["1"]["names"].HasAttribute("code") ? PlayersData["1"]["names"]["code"].GetStringData() : "";
                if (PlayersData.HasAttribute("2"))
                    ReturnValue.PlayerInfo[2].Code = PlayersData["2"]["names"].HasAttribute("code") ? PlayersData["2"]["names"]["code"].GetStringData() : "";
                if (PlayersData.HasAttribute("3"))
                    ReturnValue.PlayerInfo[3].Code = PlayersData["3"]["names"].HasAttribute("code") ? PlayersData["3"]["names"]["code"].GetStringData() : "";
            }
            if (PlayersData.GetMapData().begin()->second["names"].HasAttribute("netplay"))
            {
                if (PlayersData.HasAttribute("0"))
                    ReturnValue.PlayerInfo[0].Tag = PlayersData["0"]["names"].HasAttribute("netplay") ? PlayersData["0"]["names"]["netplay"].GetStringData() : "";
                if (PlayersData.HasAttribute("1"))
                    ReturnValue.PlayerInfo[1].Tag = PlayersData["1"]["names"].HasAttribute("netplay") ? PlayersData["1"]["names"]["netplay"].GetStringData() : "";
                if (PlayersData.HasAttribute("2"))
                    ReturnValue.PlayerInfo[2].Tag = PlayersData["2"]["names"].HasAttribute("netplay") ? PlayersData["2"]["names"]["netplay"].GetStringData() : "";
                if (PlayersData.HasAttribute("3"))
                    ReturnValue.PlayerInfo[3].Tag = PlayersData["3"]["names"].HasAttribute("netplay") ? PlayersData["3"]["names"]["netplay"].GetStringData() : "";
            }
        }
        if (PlayersData.GetMapData().size() != 0 && PlayersData.GetMapData().begin()->second.HasAttribute("characters"))
        {
            if (PlayersData.HasAttribute("0") && PlayersData["0"].HasAttribute("characters"))
            {
                ReturnValue.PlayerInfo[0].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(PlayersData["0"]["characters"].GetMapData().begin()->first)));
            }
            if (PlayersData.HasAttribute("1") && PlayersData["1"].HasAttribute("characters"))
            {
                ReturnValue.PlayerInfo[1].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(PlayersData["1"]["characters"].GetMapData().begin()->first)));
            }
            if (PlayersData.HasAttribute("2") && PlayersData["2"].HasAttribute("characters"))
            {
                ReturnValue.PlayerInfo[2].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(PlayersData["2"]["characters"].GetMapData().begin()->first)));
            }
            if (PlayersData.HasAttribute("3") && PlayersData["3"].HasAttribute("characters"))
            {
                ReturnValue.PlayerInfo[3].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(PlayersData["3"]["characters"].GetMapData().begin()->first)));
            }
        }
        return(ReturnValue);
    }
    void MBSlippiCLIHandler::p_CreateDatabase(std::string const& DirectoryToCreate)
    {
        if (!std::filesystem::exists(DirectoryToCreate))
        {
            throw std::runtime_error("Invalid replay directory");
        }
        MBDB::MrBoboDatabase Database(DirectoryToCreate +"/SlippiGames.db",MBDB::DBOpenOptions::ReadWrite);
        std::string GamesTable = "CREATE TABLE GAMES (GameID INTEGER PRIMARY KEY,RelativePath VARCHAR(65535),Date INTEGER,Stage VARCHAR(255),FrameDuration INTEGER);";
        std::string PlayerTable = "CREATE TABLE Players(GameID INTEGER,Port INTEGER,Code VARCHAR(255),Tag VARCHAR(255),Character VARCHAR(255));";
        MBDB::SQLStatement Statement = Database.GetSQLStatement(GamesTable);
        MBError Result = true;
        Database.GetAllRows(Statement,&Result);
        Statement = Database.GetSQLStatement(PlayerTable);
        Result = true;
        Database.GetAllRows(Statement,&Result);
    }
    void MBSlippiCLIHandler::p_UpdateGameSQLDatabase(MBDB::MrBoboDatabase& DatabaseToUpdate,std::filesystem::path const& ReplayDirectory,
                std::vector<std::string> const& NewFiles)    
    {
        uint32_t ParsedFiles = 0;
        uint32_t TotalFiles = 0;
        for(auto const& NewFile : NewFiles)
        {
            if (!std::filesystem::exists(ReplayDirectory/NewFile))
            {
                continue;
            }
            //can assume files are valid slp files, that is ends with .slp
            std::string TotalFileData = MBUtility::ReadWholeFile(MBUnicode::PathToUTF8( ReplayDirectory/NewFile));
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
            
            //Should actually check for errors...
            std::string QuerryToInsert = "INSERT INTO GAMES (RelativePath,Date,Stage,FrameDuration) VALUES (?,?,?,?);";
            MBDB::SQLStatement Statement = DatabaseToUpdate.GetSQLStatement(QuerryToInsert);
            Statement.BindString(NewFile, 1);
            Statement.BindInt(NewGame.Date, 2);
            Statement.BindString(NewGame.Stage, 3);
            Statement.BindInt(NewGame.FrameDuration, 4);
            MBError Result = true;
            DatabaseToUpdate.GetAllRows(Statement, &Result);
            for(int i = 0; i < 4; i++)
            {
                std::string PlayerQuery = "INSERT INTO PLAYERS (GameID,Port,Code,Tag,Character) VALUES (?,?,?,?,?);";
                //how to get GameID?
                std::vector<MBDB::MBDB_RowData> GameIDResult = DatabaseToUpdate.GetAllRows("SELECT MAX(GameID) FROM GAMES;");
                if(GameIDResult.size() == 0)
                {
                    throw std::runtime_error("Error retrieving GameID from database");
                }
                MBDB::IntType GameID = GameIDResult[0].GetColumnData<MBDB::IntType>(0);
                Statement = std::move(DatabaseToUpdate.GetSQLStatement(PlayerQuery));
                Statement.BindInt(GameID,1);
                Statement.BindInt(i,2);
                Statement.BindString(NewGame.PlayerInfo[i].Code,3);
                Statement.BindString(NewGame.PlayerInfo[i].Tag,4);
                Statement.BindString(NewGame.PlayerInfo[i].Character,5);
                DatabaseToUpdate.GetAllRows(Statement, &Result);
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
        for(auto const& ReplayDirectory : m_ReplayDirectories)
        {
            if (!std::filesystem::exists(ReplayDirectory) || !std::filesystem::directory_entry(ReplayDirectory).is_directory())
            {
                m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
                m_Terminal.Print("Invalid replay directory: "+MBUnicode::PathToUTF8(ReplayDirectory) + " doesn't exist");
                return;
            }
            std::filesystem::path DatabasePath = ReplayDirectory/"SlippiGames.db";
            if (!std::filesystem::exists(DatabasePath) || Input.CommandOptions.find("rebuild") != Input.CommandOptions.end())
            {
                //in case of rebuild
                if(std::filesystem::exists(DatabasePath))
                {
                    std::filesystem::remove(DatabasePath);
                }
                p_CreateDatabase(MBUnicode::PathToUTF8(ReplayDirectory));
            }
            MBDB::MrBoboDatabase Database(MBUnicode::PathToUTF8(DatabasePath),MBDB::DBOpenOptions::ReadWrite);
            std::vector<std::string> StoredGames;
            std::vector<MBDB::MBDB_RowData> ResultRows = Database.GetAllRows("SELECT RelativePath FROM GAMES ORDER BY RelativePath;"); 
            for(auto const & Row : ResultRows)
            {
                StoredGames.push_back(Row.GetColumnData<std::string>(0));
            }
            //get local games
            std::vector<std::string> GamesInDirectory;
            std::filesystem::recursive_directory_iterator DirIterator(ReplayDirectory);
            for(auto const& Entry : DirIterator)
            {
                if(Entry.is_regular_file() && Entry.path().extension() == ".slp")
                {
                    GamesInDirectory.push_back(MBUnicode::PathToUTF8(std::filesystem::relative(Entry.path(),ReplayDirectory)));
                }
            }
            std::sort(GamesInDirectory.begin(),GamesInDirectory.end());

            std::vector<std::string> NewFiles;
            std::set_difference(GamesInDirectory.begin(),GamesInDirectory.end(),StoredGames.begin(),StoredGames.end(),
                    std::inserter(NewFiles,NewFiles.begin()));
            p_UpdateGameSQLDatabase(Database,ReplayDirectory,NewFiles);
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
        try
        {
            Evaluator.SetDBAdapter(this);
            Evaluator.SetRecorder(this);
            //DEBUG
            ModuleID ID = Evaluator.LoadModule(SlippiScriptToExecute);
            MQL_Module& AssociatedModule = Evaluator.GetModule(ID);
            if(AssociatedModule.LoadErrors.size() > 0)
            {
                m_Terminal.PrintLine("Semantic error evaluating SlippiSpec:");
                for(auto const& Diagnostic : AssociatedModule.LoadErrors)
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
    void MBSlippiCLIHandler::p_Handle_Record(MBCLI::ProcessedCLInput const& Input)
    {
        if(Input.TopCommandArguments.size() != 2)
        {
            m_Terminal.PrintLine("record requires exactly 2 arguments: the input filepath, and the output filepath");   
            std::exit(1);
        }
        std::filesystem::path InputPath = Input.TopCommandArguments[0];
        std::filesystem::path OutputPath = Input.TopCommandArguments[1];
        if(!std::filesystem::exists(InputPath))
        {
            m_Terminal.PrintLine("cannot find input file \""+Input.TopCommandArguments[0]+"\"");
            std::exit(1);
        }
        MBUtility::StreamReader Reader = MBUtility::StreamReader(std::make_unique<MBUtility::InputFileStream>(InputPath));
        MBParsing::CSVFile File = MBParsing::CSVFile::ParseCSVFile(Reader);
        Vec<MBParsing::CsvIntType>* Begin = nullptr;
        Vec<MBParsing::CsvIntType>* End = nullptr;
        Vec<MBParsing::CsvStringType>* Paths = nullptr;
        if(!File.HasColName("Begin"))
        {
            throw std::runtime_error("CSV file need to have a column named Begin");
            auto& Col = File["Begin"];
            if(!Col.IsType<MBParsing::CsvIntType>())
            {
                throw std::runtime_error("Begin column must be of type integer");
            }
        }
        Begin = &File["Begin"].GetType<MBParsing::CsvIntType>();
        if(!File.HasColName("End"))
        {
            throw std::runtime_error("CSV file need to have a column named End");
            auto& Col = File["End"];
            if(!Col.IsType<MBParsing::CsvIntType>())
            {
                throw std::runtime_error("End column must be of type integer");
            }
        }
        End = &File["End"].GetType<MBParsing::CsvIntType>();
        if(!File.HasColName("File"))
        {
            throw std::runtime_error("CSV file need to have a column named File");
            auto& Col = File["File"];
            if(!Col.IsType<MBParsing::CsvStringType>())
            {
                throw std::runtime_error("End column must be of type string");
            }
        }
        Paths = &File["File"].GetType<MBParsing::CsvStringType>();
        p_Record(*Paths,*Begin,*End,OutputPath);
    }
    void MBSlippiCLIHandler::p_Record(Vec<MBParsing::CsvStringType> const& Path, Vec<MBParsing::CsvIntType> const& Begin,Vec<MBParsing::CsvIntType> const& End, std::filesystem::path const& OutPath)
    {
        std::unordered_map<std::string,RecordingInfo> Recordings;
        std::vector<std::string> FileOrder;
        for(size_t i = 0; i < Path.size();i++)
        {
            RecordingInfo& Recording = Recordings[Path[i]];
            if(Recording.GamePath == "")
            {
                if(!std::filesystem::exists(Path[i]))
                {
                    throw std::runtime_error("Filepath in File column doesnt exist: \""+Path[i]+"\"");
                }
                FileOrder.push_back(Path[i]);
                Recording.GamePath = Path[i];
                MBUtility::MBFileInputStream InputStream(Path[i]);
                MBError Result = MeleeGame::ParseSlippiGame(InputStream,Recording.GameData);
                if(!Result)
                {
                    throw std::runtime_error("Error parsing slippi game at path \""+Path[i]+"\": "+Result.ErrorMessage);   
                }
            }
            GameIntervall NewIntervall;
            NewIntervall.FirstFrame = Begin[i];
            NewIntervall.LastFrame = End[i];
            Recording.IntervallsToRecord.push_back(NewIntervall);
        }
        std::vector<RecordingInfo> GamesToRecord;
        for(auto& File : FileOrder)
        {
            GamesToRecord.push_back(std::move(Recordings[File]));
        }
        RecordGames(GamesToRecord,OutPath);
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
            //DEPRECATED Input.TopCommand
            if (Input.TopCommand == "index")
            {
                p_Handle_UpdateIndex(Input);
            }
            else if (Input.TopCommand == "execute")
            {
                p_Handle_Execute(Input);
            }
            else if (Input.TopCommand == "record")
            {
                p_Handle_Record(Input);
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
