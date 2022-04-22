#include "MBSlippi_CLI.h"
#include "MBCLI/MBCLI.h"
#include <MBUtility/MBFiles.h>
#include <cstdlib>
#include <MBPacketManager/MB_PacketProtocol.h>
#include <MrBoboDatabase/MrBoboDatabase.h>
#include "MeleeID.h"

#include <chrono>
#include <sstream>
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
				std::exit(1);
			}
			MBError ParseResult = true;
			MBParsing::JSONObject ConfigJSON = MBParsing::ParseJSONObject(TotalConfigData, 0, 0, &ParseResult);
			if (!ParseResult)
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Error parsing MBSlippiConfig file: " + ParseResult.ErrorMessage);
				std::exit(1);
			}
			m_Config = MBSlippiConfig(ConfigJSON);
		}
		catch (std::exception const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Error loading config: " +std::string(e.what()));
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
			ReturnValue.Player1Code = TotalGameData["metadata"]["players"]["0"]["names"]["code"].GetStringData();
			ReturnValue.Player2Code = TotalGameData["metadata"]["players"]["1"]["names"]["code"].GetStringData();
		}
		if (TotalGameData["metadata"]["players"]["0"]["names"].HasAttribute("netplay"))
		{
			ReturnValue.Player1Tag = TotalGameData["metadata"]["players"]["0"]["names"]["netplay"].GetStringData();
			ReturnValue.Player2Tag = TotalGameData["metadata"]["players"]["1"]["names"]["netplay"].GetStringData();
		}
		if (TotalGameData["metadata"]["players"]["0"].HasAttribute("characters"))
		{
			ReturnValue.Player1Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["0"]["characters"].GetMapData().begin()->first)));
			ReturnValue.Player2Character = MBSlippi::CharacterToString(InternalCharacterID(std::stoi(TotalGameData["metadata"]["players"]["1"]["characters"].GetMapData().begin()->first)));
		}
		return(ReturnValue);
	}
	void MBSlippiCLIHandler::p_CreateDatabase(std::string const& DirectoryToCreate)
	{
		if (!std::filesystem::exists(DirectoryToCreate))
		{
			throw std::runtime_error("Invalid replay directory");
		}
		MBDB::MrBoboDatabase Database(DirectoryToCreate + "\\SlippiGames.db",uint64_t(MBDB::DBOpenOptions::ReadWrite));
		std::string DatabaseStatement = "CREATE TABLE GAMES (RelativePath VARCHAR(65535),Date INTEGER,Stage VARCHAR(255),Player1Code VARCHAR(255),Player1Tag VARCHAR(255),Player1Character VARCHAR(255),"
			"Player2Code VARCHAR(255),Player2Tag VARCHAR(255),Player2Character VARCHAR(255), FrameDuration INTEGER);";

		MBDB::SQLStatement* Statement = Database.GetSQLStatement(DatabaseStatement);
		MBError Result = true;
		Database.GetAllRows(Statement,&Result);
	}
	void MBSlippiCLIHandler::p_UpdateGameSQLDatabase(std::string const& ReplayDirectory, MBPM::MBPP_FileInfoReader const& FileInfo)
	{
		std::vector<SlippiGameInfo> NewGames = {};
		auto FileIterator = FileInfo.GetDirectoryInfo("/")->begin();
		auto End = FileInfo.GetDirectoryInfo("/")->end();
		uint32_t TotalFiles = 0;
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
			NewGames.push_back(std::move(NewGame));
		}
		if (!std::filesystem::exists(ReplayDirectory + "/SlippiGames.db") || !std::filesystem::directory_entry(ReplayDirectory + "/SlippiGames.db").is_regular_file())
		{
			p_CreateDatabase(ReplayDirectory);
		}
		for (auto const& NewGame : NewGames)
		{
			MBDB::MrBoboDatabase Database(ReplayDirectory + "/SlippiGames.db",0);
			std::string QuerryToInsert = "INSERT INTO GAMES VALUES(?,?,?,?,?,?,?,?,?,?);";
			MBDB::SQLStatement* Statement = Database.GetSQLStatement(QuerryToInsert);
			Statement->BindString(NewGame.RelativePath, 1);
			Statement->BindInt(NewGame.Date, 2);
			Statement->BindString(NewGame.Stage, 3);
			Statement->BindString(NewGame.Player1Code, 4);
			Statement->BindString(NewGame.Player1Tag, 5);
			Statement->BindString(NewGame.Player1Character, 6);
			Statement->BindString(NewGame.Player2Code, 7);
			Statement->BindString(NewGame.Player2Tag, 8);
			Statement->BindString(NewGame.Player2Character,9);
			Statement->BindInt(NewGame.FrameDuration, 10);
			MBError Result = true;
			Database.GetAllRows(Statement,&Result);
			Database.FreeSQLStatement(Statement);
		}
		if (TotalFiles > NewGames.size())
		{
			m_Terminal.PrintLine("Totalfiles: " + std::to_string(TotalFiles) + " Parsed Files: " + std::to_string(NewGames.size()));
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
		if (!std::filesystem::exists(m_Config.ReplaysDirectory+"/MBPM_FileInfo"))
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
			MBPM::MBPP_FileInfoReader UpdatedFiles = MBPM::MBPP_FileInfoReader::GetLocalInfoDifference(m_Config.ReplaysDirectory);
			if (UpdatedFiles.GetDirectoryInfo("/") == nullptr)
			{
				m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
				m_Terminal.Print("Error getting local file info difference");
				return;
			}
			p_UpdateGameSQLDatabase(m_Config.ReplaysDirectory, UpdatedFiles);
			MBPM::MBPP_FileInfoReader OldInfo = MBPM::MBPP_FileInfoReader(m_Config.ReplaysDirectory + "/MBPM_FileInfo");
			OldInfo.UpdateInfo(UpdatedFiles);
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
				std::exit(1);
			}
		}
		catch (MBScript::MBSRuntimeException const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Unhandled MBScript runtime error: " + std::string(e.what()));
			std::exit(1);
		}
		catch (std::runtime_error const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Error with MBScript implementation: " + std::string(e.what()));
			std::exit(1);
		}
		catch (std::exception const& e)
		{
			m_Terminal.SetTextColor(MBCLI::ANSITerminalColor::Red);
			m_Terminal.Print("Unkown error occured: " + std::string(e.what()));
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
			std::exit(1);
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