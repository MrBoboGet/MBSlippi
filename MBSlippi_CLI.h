#pragma once
#include "MBSlippiScript.h"
#include "MBSlippiConfig.h"
#include <MBCLI/MBCLI.h>
#include <MBPacketManager/MB_PacketProtocol.h>
#include <deque>
#include "SlippiSpec.h"

#include <MrBoboDatabase/MrBoboDatabase.h>


namespace MBSlippi
{
    //LSP in syntax transporatation only
    class MQLServer : public MBLSP::LSP_Server
    {
    private:
        void SendMessage(MBUtility::MBOctetOutputStream& OutStream,MBParsing::JSONObject const& ObjectToSend);
        MBLSP::LSP_ServerHandler* m_AssociatedHandler = nullptr;

        std::mutex m_StatementLock;
        std::deque<Statement> m_StatementsToEvaluate;
        std::condition_variable m_NotifyConditional;
        std::thread m_EvaluatorThread;
        std::atomic<bool> m_Evalutating{false};
        std::atomic<bool> m_IsRunning{true};

        std::mutex m_EvaluatorMutex;
        SpecEvaluator* m_Evaluator = nullptr;
        
        void p_EvaluatorThread();
        void p_ExecuteAsync(Statement StatementToExecute);
    public:
        MQLServer(SpecEvaluator* Evalator)
        {
            m_Evaluator = Evalator;   
            m_EvaluatorThread = std::thread(&MQLServer::p_EvaluatorThread,this);
        }
        virtual MBLSP::Initialize_Response HandleRequest(MBLSP::InitializeRequest const& Request)
        {
            MBLSP::Initialize_Response ReturnValue;
            return ReturnValue;
        }
        virtual void OpenedDocument(std::string const& URI,std::string const& Content) 
        {
               
        }
        virtual void ClosedDocument(std::string const& URI) 
        {
               
        }
        virtual void DocumentChanged(std::string const& URI,std::string const& NewContent) 
        {
               
        }
        virtual void SetHandler(MBLSP::LSP_ServerHandler* AssociatedHandler) 
        {
            m_AssociatedHandler = AssociatedHandler;
        }
        virtual MBParsing::JSONObject HandleGenericRequest(MBParsing::JSONObject const& GenericRequest);
        virtual void HandleGenericNotification(MBParsing::JSONObject const& GenericNotification)
        {
               
        }
    };

	class MBSlippiCLIHandler : public MeleeGameRecorder,public MeleeGameDBAdapter
	{
	private:
        void p_WriteReplayInfo(std::vector<RecordingInfo> const& RecordingToWrite,MBUtility::MBOctetOutputStream& OutStream);
        void p_RestoreDolphinConfigs(std::string const& DumpDirectory, DolphinConfigParser const& DolphinINI, DolphinConfigParser const& DolphinGFX);
        std::string p_UpdateDolphinConfigs(DolphinConfigParser* OutOriginalIni,DolphinConfigParser* OutOriginalGFX);
        virtual std::vector<SlippiGameInfo> RetrieveGames(std::string const& WhereCondition,std::vector<std::string> const& GameSets) override;
        virtual bool GameSetExists(std::string const& WhereCondition) override;
        virtual void RecordGames(std::vector<RecordingInfo> const& GamesToRecord,std::filesystem::path const& OutPath) override;


        std::vector<std::filesystem::path> m_ReplayDirectories;
		MBParsing::JSONObject m_GlobalConfig;
		MBSlippiConfig m_Config;

		MBCLI::MBTerminal m_Terminal;
		void LoadGlobalConfig();

		void p_CreateDatabase(std::string const& DirectoryToCreate);
		SlippiGameInfo p_SlippiFileToGameInfo(MBParsing::JSONObject const& TotalGameData);
		void p_UpdateGameSQLDatabase(MBDB::MrBoboDatabase& DatabaseToUpdate,std::filesystem::path const& ReplayDirectory,
                std::vector<std::string> const& NewFiles);

		void p_Handle_UpdateIndex(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Execute_Legacy(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Execute(MBCLI::ProcessedCLInput const& Input);
        int p_HandleServer(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Play(MBCLI::ProcessedCLInput const& Input);
		void p_Handle_Query(MBCLI::ProcessedCLInput const& Input);
	public:
        MBSlippiCLIHandler()
        {
            LoadGlobalConfig();
        }
		int Run(int argc, const char** argv);
	};
}
