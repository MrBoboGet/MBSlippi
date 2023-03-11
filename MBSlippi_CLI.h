#pragma once
#include "MBSlippiScript.h"
#include "MBSlippiConfig.h"
#include <MBCLI/MBCLI.h>
#include <MBPacketManager/MB_PacketProtocol.h>

#include "SlippiSpec.h"
namespace MBSlippi
{
	

	class MBSlippiCLIHandler : MeleeGameRecorder,MeleeGameDBAdapter
	{
	private:
        void p_WriteReplayInfo(std::vector<RecordingInfo> const& RecordingToWrite,MBUtility::MBOctetOutputStream& OutStream);
        void p_RestoreDolphinConfigs(std::string const& DumpDirectory, DolphinConfigParser const& DolphinINI, DolphinConfigParser const& DolphinGFX);
        std::string p_UpdateDolphinConfigs(DolphinConfigParser* OutOriginalIni,DolphinConfigParser* OutOriginalGFX);
        virtual std::vector<SlippiGameInfo> RetrieveGames(std::string const& WhereCondition) override;
        virtual void RecordGames(std::vector<RecordingInfo> const& GamesToRecord,std::filesystem::path const& OutPath) override;

		MBParsing::JSONObject m_GlobalConfig;
		MBSlippiConfig m_Config;

		MBCLI::MBTerminal m_Terminal;
		void p_LoadGlobalConfig();

		void p_CreateDatabase(std::string const& DirectoryToCreate);
		SlippiGameInfo p_SlippiFileToGameInfo(MBParsing::JSONObject const& TotalGameData);
		void p_UpdateGameSQLDatabase(std::string const& ReplayDirectory,MBPM::MBPP_FileInfoReader const& FileInfo);

		void p_Handle_UpdateIndex(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Execute_Legacy(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Execute(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Play(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Query(MBCLI::ProcessedCLInput const& Input);
	public:
		void Run(int argc, const char** argv);
	};
}
