#pragma once
#include "SlippiSpecParser.h"
#include <MBLSP/MBLSP.h>
#include "MBMeleeID.h"
#include <MBSystem/BiDirectionalSubProcess.h>
namespace MBSlippi
{
    struct GameIntervall
    {
        int FirstFrame = 0;   
        int LastFrame = 0;
        GameIntervall() = default;
        GameIntervall(int NewFirstFrame,int NewLastFrame)
        {
            FirstFrame = NewFirstFrame;   
            LastFrame = NewLastFrame;   
        }
        bool operator<(GameIntervall rhs) const
        {
            return(FirstFrame < rhs.FirstFrame);   
        }
    };
    struct RecordingInfo
    {
        std::string GamePath;
        std::vector<GameIntervall> IntervallsToRecord;
    };
    class MeleeGameDBAdapter
    {
    public:      
        virtual std::vector<SlippiGameInfo> RetrieveGames(std::string const& WhereCondition) = 0;
    };
    class MeleeGameRecorder
    {

    public:
        virtual void RecordGames(std::vector<RecordingInfo> const& GamesToRecord,std::filesystem::path const& OutPath) = 0;
    };

    class ArgumentList
    {
    public:
        ArgumentList() = default;
        ArgumentList(Filter_ArgList const& ListToConvert)
        {
            for(auto const& Argument : ListToConvert.Arguments)
            {
                if(Argument.IsType<Filter_Arg_Positional>())
                {
                    PositionalArguments.push_back(Argument.GetType<Filter_Arg_Positional>().Value);
                }
                else if(Argument.IsType<Filter_Arg_Named>())
                {
                    auto const& KeyArgument = Argument.GetType<Filter_Arg_Named>();
                    KeyArguments[KeyArgument.Name] = KeyArgument.Value;
                }
            }   
        }
        std::vector<std::string>  PositionalArguments;
        std::unordered_map<std::string,std::string> KeyArguments;
    };
    

    class SpecServer
    {
        std::unique_ptr<MBSystem::BiDirectionalSubProcess> m_SubProcess;
        std::vector<std::string> m_ExportedFilters;

        MBParsing::JSONObject p_SendRequest(MBParsing::JSONObject const& Request);
        void p_SendInitialize();
    public:
        SpecServer(std::string const& ExecutableName,std::vector<std::string> const& Args);
        std::vector<std::string> GetExportedFilters();
        std::vector<GameIntervall> ExecuteFilter(std::string const& FilterName,MeleeGame const& GameToFilter,GameIntervall const& Intervall);
    };
    
    struct ServerInitilizationData
    {
        std::string ExecutableName;
        std::vector<std::string> ExecutableArguments;
    };
    class SpecEvaluator
    {
    private:
        typedef std::vector<GameIntervall> (* BuiltinFilterType)(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);


        static std::vector<GameIntervall> BiggestPunishes(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasMove(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        std::unordered_map<std::string,BuiltinFilterType> m_BuiltinFilters = 
        {
            {"Punishes",BiggestPunishes},
            {"HasMove",HasMove}
        };

        std::vector<SpecServer> m_SpecServers;
        std::unordered_map<std::string,int> m_FilterToServer;

        MeleeGameDBAdapter* m_DBAdapter;
        MeleeGameRecorder* m_Recorder;

        void p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilterComponent(Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilter(Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameInfoPredicate(GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

        bool p_EvaluateGameSelection(SlippiGameInfo const& GameInfo,bool IsSwapped,GameInfoPredicate const& PredicateToEvaluate);
        //Retunrs wheter or not any player satisfies the condition
        bool p_IsPlayersSwapped(SlippiGameInfo const& GameInfo, GameInfoPredicate const& PredicateToEvaluate,bool& IsSwapped);

        std::vector<MeleeGame> p_RetrieveSpecGames(SlippiSpec const& SpecToEvalaute);
        std::vector<GameIntervall> p_EvaluateGameIntervalls(Filter_Component const& FilterToUse,GameIntervall CurrentIntervall,MeleeGame const& GameToFilter);

        void p_InitializeServers();
    public:
        void SetDBAdapter(MeleeGameDBAdapter* NewAdapter);
        void SetRecorder(MeleeGameRecorder* NewRecorder);
        void InitializeServers(std::vector<ServerInitilizationData> const& ServersToInitialize);

        bool VerifySpec(SlippiSpec& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateSpec(SlippiSpec& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
    };
}
