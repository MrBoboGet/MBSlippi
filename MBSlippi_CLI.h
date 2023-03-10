#include "MBSlippiScript.h"
#include "MBSlippiConfig.h"
#include <MBCLI/MBCLI.h>
#include <MBPacketManager/MB_PacketProtocol.h>

namespace MBSlippi
{
	
    struct SlippiGamePlayerInfo
    {
        std::string Code;
        std::string Tag;
        std::string Character;
    };
	struct SlippiGameInfo
	{
		std::string RelativePath;
		uint64_t Date;
		std::string Stage;
        SlippiGamePlayerInfo PlayerInfo[2];
		uint64_t FrameDuration = 0;
	};


	class MBSlippiCLIHandler
	{
	private:
		MBParsing::JSONObject m_GlobalConfig;
		MBSlippiConfig m_Config;

		MBCLI::MBTerminal m_Terminal;
		void p_LoadGlobalConfig();

		void p_CreateDatabase(std::string const& DirectoryToCreate);
		SlippiGameInfo p_SlippiFileToGameInfo(MBParsing::JSONObject const& TotalGameData);
		void p_UpdateGameSQLDatabase(std::string const& ReplayDirectory,MBPM::MBPP_FileInfoReader const& FileInfo);

		void p_Handle_UpdateIndex(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Execute(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Play(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Query(MBCLI::ProcessedCLInput const& Input);
	public:

		void Run(int argc, const char** argv);
	};
}
