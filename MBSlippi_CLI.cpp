#include "MBSlippi_CLI.h"
#include "MBCLI/MBCLI.h"
#include <MBUtility/MBFiles.h>
#include <cstdlib>
#include <MBPacketManager/MB_PacketProtocol.h>
#include <MrBoboDatabase/MrBoboDatabase.h>
#include "MeleeID.h"

#include <chrono>
#include <filesystem>
#include <sstream>

#include "SlippiSpecParser.h"
namespace MBSlippi
{
	void MBSlippiCLIHandler::p_LoadGlobalConfig()
	{
		try
		{
			std::string TotalConfigData = MBUtility::ReadWholeFile("MBSlippiConfig.json");
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
		}
		if (TotalGameData["metadata"]["players"]["0"]["names"].HasAttribute("netplay"))
		{
			ReturnValue.PlayerInfo[0].Tag = TotalGameData["metadata"]["players"]["0"]["names"]["netplay"].GetStringData();
			ReturnValue.PlayerInfo[1].Tag = TotalGameData["metadata"]["players"]["1"]["names"]["netplay"].GetStringData();
		}
		if (TotalGameData["metadata"]["players"]["0"].HasAttribute("characters"))
		{
			ReturnValue.PlayerInfo[0].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["0"]["characters"].GetMapData().begin()->first)));
			ReturnValue.PlayerInfo[1].Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["1"]["characters"].GetMapData().begin()->first)));
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
		std::string DatabaseStatement = "CREATE TABLE GAMES (RelativePath VARCHAR(65535),Date INTEGER,Stage VARCHAR(255),Player1Code VARCHAR(255),Player1Tag VARCHAR(255),Player1Character VARCHAR(255),"
			"Player2Code VARCHAR(255),Player2Tag VARCHAR(255),Player2Character VARCHAR(255), FrameDuration INTEGER);";

		MBDB::SQLStatement* Statement = Database.GetSQLStatement(DatabaseStatement);
		MBError Result = true;
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
			NewGame.RelativePath =  FileIterator.GetCurrentDirectory() + "/" + FileIterator->FileName;
			
			
			std::string QuerryToInsert = "INSERT INTO GAMES VALUES(?,?,?,?,?,?,?,?,?,?);";
			MBDB::SQLStatement* Statement = Database.GetSQLStatement(QuerryToInsert);
			Statement->BindString(NewGame.RelativePath, 1);
			Statement->BindInt(NewGame.Date, 2);
			Statement->BindString(NewGame.Stage, 3);
			Statement->BindString(NewGame.PlayerInfo[0].Code, 4);
			Statement->BindString(NewGame.PlayerInfo[0].Tag, 5);
			Statement->BindString(NewGame.PlayerInfo[0].Character, 6);
			Statement->BindString(NewGame.PlayerInfo[1].Code, 7);
			Statement->BindString(NewGame.PlayerInfo[1].Tag, 8);
			Statement->BindString(NewGame.PlayerInfo[1].Character, 9);
			Statement->BindInt(NewGame.FrameDuration, 10);
			MBError Result = true;
			Database.GetAllRows(Statement, &Result);
			Database.FreeSQLStatement(Statement);

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
			p_UpdateGameSQLDatabase(m_Config.ReplaysDirectory, UpdatedFiles);
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
	void MBSlippiCLIHandler::Run(int argc, const char** argv)
	{
		MBCLI::ProcessedCLInput Input(argc, argv);
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
		else if (Input.TopCommand == "play")
		{
			p_Handle_Play(Input);
		}
		else if (Input.TopCommand == "query")
		{
			p_Handle_Query(Input);
		}
	}
}
