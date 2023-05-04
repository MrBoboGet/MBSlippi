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
        MeleeGame GameData;
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

    template <typename PredType>
    std::vector<GameIntervall> ExtractSequences(MeleeGame const& GameToInspect, GameIntervall IntervallToInspect,PredType Predicate)
    {
        std::vector<GameIntervall> ReturnValue;
        bool WasTrue = false;
        int FirstTrue = IntervallToInspect.FirstFrame;
        for(int i = IntervallToInspect.FirstFrame; i < IntervallToInspect.LastFrame;i++)
        {
            if(Predicate(GameToInspect.Frames[i]))
            {
                if(!WasTrue)
                {
                    FirstTrue = i;
                }
                WasTrue = true;
            }
            else
            {
                if(WasTrue)
                {
                    ReturnValue.push_back(GameIntervall(FirstTrue,i));   
                }
                WasTrue = false;
            }
        }
        return(ReturnValue);
    }

    struct MQL_LazyGameList
    {
        bool Evaluated = false;
        GameSelection GamesToRetrieve;
        std::vector<MeleeGame> Games;
    };
    struct MQL_Variable_GameInfoPredicate
    {
        bool IsPlayerAssignment = false;
        GameInfoPredicate Predicate;
    };
    class MQL_Variable
    {
    public:
        std::variant<MQL_LazyGameList,MQL_Variable_GameInfoPredicate,Filter_Component>  Data;
    };
    class MQL_Scope
    {
        MQL_Scope* m_ParentScope = nullptr;
        std::unordered_map<std::string,MQL_Variable> m_Variables;
    public:
        void SetParentScope(MQL_Scope* Parent)
        {
            m_ParentScope = Parent;
        }
        //doesn't verify that the variable doesn't already exist
        void AddVariable(std::string const& Name,MQL_Variable Value)
        {
            m_Variables[Name] = std::move(Value);
        }
        MQL_Variable& GetVariable(std::string const& VariableName)
        {
            auto It = m_Variables.find(VariableName);
            if(It == m_Variables.end())
            {
                if(m_ParentScope != nullptr)
                {
                    return(m_ParentScope->GetVariable(VariableName));
                }
                else
                {
                    throw std::runtime_error(VariableName + " doesn't exist in current scope");
                }
            }
            return(It->second);
        }
        bool HasVariable(std::string const& VariableName)
        {
            bool ReturnValue = m_Variables.find(VariableName) != m_Variables.end();
            if(!ReturnValue && m_ParentScope != nullptr)
            {
                ReturnValue = m_ParentScope->HasVariable(VariableName);
            }
            return(ReturnValue);
        }
    };
    struct MQL_Context
    {
        MQL_Scope GlobalScope;
    };
    class SpecEvaluator
    {
    private:
        typedef std::vector<GameIntervall> (* BuiltinFilterType)(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);

        static int GetPlayerIndex(ArgumentList const& ExtraArguments);
    
        static std::vector<GameIntervall> BiggestPunishes(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasMove(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Move(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> InShield(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Expand(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> ActionState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Until(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> SpecialState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasHitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        std::unordered_map<std::string,BuiltinFilterType> m_BuiltinFilters = 
        {
            {"Punishes",BiggestPunishes},
            {"HasMove",HasMove},
            {"Move",Move},
            {"InShield",InShield},
            {"Expand",Expand},
            {"HitBy",HitBy},
            {"HasHitBy",HitBy},
            {"SpecialState",SpecialState},
            {"HasProjectile",HasProjectile},
            {"ActionState",ActionState},
            {"Until",Until},
        };

        std::vector<SpecServer> m_SpecServers;
        std::unordered_map<std::string,int> m_FilterToServer;

        MQL_Context m_TopContext;
 
        MeleeGameDBAdapter* m_DBAdapter = nullptr;
        MeleeGameRecorder* m_Recorder = nullptr;

        void p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilterComponent(Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilter(Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

        void p_VerifyGameInfoPredicate_Direct(GameInfoPredicate_Direct& PredicateToVerify,
                bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameInfoPredicate(GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyPlayerAssignment(PlayerAssignment& AssignmentToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameSelection(GameSelection& SelectionToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

        bool p_SatisfiesPlayerAssignment(SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate);
        bool p_EvaluateGameSelection(SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate);
        //Retunrs wheter or not any player satisfies the condition
        //bool p_IsPlayersSwapped(SlippiGameInfo const& GameInfo, GameInfoPredicate const& PredicateToEvaluate,bool& IsSwapped);
        bool p_GetPlayerAssignments(SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4]);
        void p_ApplyAssignment(MeleeGame& GameToModify,char InAssignments[4]);

        std::vector<MeleeGame> p_RetrieveSpecGames(GameSelection const& GameSelection);
        std::vector<GameIntervall> p_EvaluateGameIntervalls(Filter_Component const& FilterToUse,GameIntervall CurrentIntervall,MeleeGame const& GameToFilter);

        void p_InitializeServers();
    public:
        void SetDBAdapter(MeleeGameDBAdapter* NewAdapter);
        void SetRecorder(MeleeGameRecorder* NewRecorder);
        void InitializeServers(std::vector<ServerInitilizationData> const& ServersToInitialize);

        bool VerifyVariableDeclaration(Statement& DeclarationToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool UpdateState=false);
        bool VerifyStatement(Statement& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        bool VerifySelection(Selection& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        //special semantics, in that it modifies the scope, so that modules can be verified correct
        //without needing to actually execute the actions
        bool VerifyModule(Module& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);


        void EvaluateStatement(Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateSelection(Selection& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateVariableDeclaration(Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateModule(Module& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
    };
}
