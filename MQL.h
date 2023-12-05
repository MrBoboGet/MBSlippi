#pragma once
#include "MBUtility/MBInterfaces.h"
#include "MQLParser.h"
#include <MBLSP/MBLSP.h>
#include "MBMeleeID.h"
#include <MBSystem/BiDirectionalSubProcess.h>
#include <MBSystem/MBSystem.h>
#include <typeindex>
#include <type_traits>
namespace MBSlippi
{
    class MQL_Filter;
    class ArgumentList
    {
        ArgumentList const* m_ParentArgList = nullptr;
        std::vector<MQL_Filter>  m_PositionalArguments;
        std::vector<MQL_Filter> m_KeyArgs;
        std::unordered_map<std::string,size_t> m_KeyPositions;
    public:
        ArgumentList() = default;

        void SetParentArgList(ArgumentList const* ParentList);
        ArgumentList(Filter_ArgList const& ListToConvert);
        //includes parent scope
        ArgumentList(ArgumentList const& DefinitionBindings,ArgumentList const& SuppliedArguments);
        bool HasNamedVariable(std::string const& VariableToCheck) const;
        std::string GetNamedVariableString(std::string const& VariableName) const;
        std::string GetPositionalArgumentString(int Index) const;
        size_t PositionalCount() const;
    };
    inline std::string IdentifierToString(Identifier const& Idf)
    {
        std::string ReturnValue = "";   
        if(Idf.Parts.size() == 0)
        {
            return(ReturnValue);   
        }
        ReturnValue = Idf.Parts[0].Value;
        for(auto const& Part : Idf.Parts)
        {
            ReturnValue += '.';   
            ReturnValue  += Part.Value;
        }
        return(ReturnValue);
    }
    inline int IdentifierLength(Identifier const& Idf)
    {
        //assumes that identifier is on a single line...
        int ReturnValue = 0;
        MBCC::TokenPosition StartPosition = Idf.Parts.front().Position;
        MBCC::TokenPosition EndPosition = Idf.Parts.back().Position;
        if(StartPosition.Line != EndPosition.Line)
        {
            return(Idf.Parts.front().Value.size());
        }
        else
        {
            ReturnValue = (EndPosition.ByteOffset-StartPosition.ByteOffset)+Idf.Parts.back().Value.size();
        }
        return(ReturnValue);
    }
    struct GameIntervall
    {
        int FirstFrame = 0;   
        int LastFrame = 0;
        GameIntervall() = default;
        GameIntervall& operator=(GameIntervall const&) = default;
        GameIntervall(int NewFirstFrame,int NewLastFrame)
        {
            FirstFrame = NewFirstFrame;   
            LastFrame = NewLastFrame;   
        }
        bool operator<(GameIntervall const& rhs) const
        {
            return(FirstFrame < rhs.FirstFrame);   
        }
        bool In(GameIntervall const& OtherIntervall) const
        {
            return(FirstFrame <= OtherIntervall.LastFrame && LastFrame >= OtherIntervall.FirstFrame);
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
        virtual std::vector<SlippiGameInfo> RetrieveGames(std::string const& WhereCondition,std::vector<std::string> const& GameSets) = 0;
        virtual bool GameSetExists(std::string const& GameSet) = 0;
    };
    class MeleeGameRecorder
    {

    public:
        virtual void RecordGames(std::vector<RecordingInfo> const& GamesToRecord,std::filesystem::path const& OutPath) = 0;
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
    void ExtractSequences(MeleeGame const& GameToInspect, GameIntervall IntervallToInspect,PredType Predicate,std::vector<GameIntervall>& OutIntervalls)
    {
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
                    OutIntervalls.push_back(GameIntervall(FirstTrue,i));   
                }
                WasTrue = false;
            }
        }
    }
    template <typename PredType>
    std::vector<GameIntervall> ExtractSequences(MeleeGame const& GameToInspect, GameIntervall IntervallToInspect,PredType Predicate)
    {
        std::vector<GameIntervall> ReturnValue;
        ExtractSequences(GameToInspect,IntervallToInspect,Predicate,ReturnValue);
        return(ReturnValue);
    }
    template <typename PredType>
    std::vector<GameIntervall> ExtractSequences(MeleeGame const& GameToInspect, std::vector<GameIntervall> const& IntervallsToInspect,PredType Predicate)
    {
        std::vector<GameIntervall> ReturnValue;
        for(auto const& Intervall : IntervallsToInspect)
        {
            ExtractSequences(GameToInspect,Intervall,Predicate,ReturnValue);
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
    class MQL_Filter;
    struct MQL_FilterDefinition
    {
        std::shared_ptr<MQL_Filter> Component;
        Filter_ArgList Arguments;
    };
    class MQL_Variable
    {
    public:
        std::variant<MQL_LazyGameList,MQL_Variable_GameInfoPredicate,std::shared_ptr<MQL_FilterDefinition>>  Data;
    };
    class MQL_MetricVariable
    {
    public:
        std::variant<std::string,float,int,bool> Data;

        MQL_MetricVariable(){};
        template<typename T>
        MQL_MetricVariable(T&& Data)
        {
            this->Data = std::forward<T>(Data);
        }
    };
    inline MBUtility::MBOctetOutputStream& operator<<(MBUtility::MBOctetOutputStream& OutStream,MQL_MetricVariable const& Rhs)
    {
        if(std::holds_alternative<std::string>(Rhs.Data))
        {
            OutStream<<std::get<std::string>(Rhs.Data);
        }
        else if(std::holds_alternative<float>(Rhs.Data))
        {
            OutStream<<std::to_string(std::get<float>(Rhs.Data));
        }
        else if(std::holds_alternative<int>(Rhs.Data))
        {
            OutStream<<std::to_string(std::get<int>(Rhs.Data));
        }
        else if(std::holds_alternative<bool>(Rhs.Data))
        {
            if(std::get<bool>(Rhs.Data))
            {
                OutStream<<"true";   
            }
            else
            {
                OutStream<<"false";   
            }
        }
        return OutStream;
    }
    class MQL_Module;
    class MQL_Scope
    {
        std::vector<std::shared_ptr<MQL_Module>> m_OverlayedModules;
        std::map<std::string,std::vector<std::shared_ptr<MQL_Module>>> m_BoundModules;

        std::unordered_map<std::string,MQL_Variable> m_Variables;

        MQL_Variable* p_GetVariable(std::string const& VariableName);
        MQL_Variable* p_GetVariable(Identifier const& Idf,int Offset);
        bool p_HasVariable(std::string const& VariableName);
        bool p_HasVariable(Identifier const& idf,int Offset);
    public:
        void OverlayScope(std::shared_ptr<MQL_Module> ScopeToOverlay);
        void BindScope(std::string ScopeName,std::shared_ptr<MQL_Module> ScopeToOverlay);
        //doesn't verify that the variable doesn't already exist
        void AddVariable(std::string const& Name,MQL_Variable Value);
        MQL_Variable& GetVariable(Identifier const& Idf);
        bool HasVariable(std::string const& Idf);
        bool HasBinding(std::string const& Idf);
        bool HasVariable(Identifier const& Idf);
    };
    struct MQL_Context
    {
        MQL_Scope GlobalScope;
    };
    typedef int  ModuleID;
    //TODO fix module functionality
    struct MQL_Module
    {
        ModuleID ID = -1;
        std::filesystem::path ModulePath;
        //std::vector<std::filesystem::path> 
        MQL_Scope ModuleScope;
        Module Contents;
        std::vector<MBLSP::Diagnostic> LoadErrors;

        void Reset()
        {
            ModuleScope = MQL_Scope();
            Contents = Module();
            LoadErrors.clear();
        }
    };


    class CallContext;
    typedef std::vector<GameIntervall> (* BuiltinFilterType)(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallsToInspect,CallContext& Context);
    typedef std::vector<MQL_MetricVariable> (* MetricFuncType)(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const&  IntervallToInspect);
    enum class OperatorType
    {
        Add,
        Pipe,
        Minus,
        Plus,
        Times,
        
        //comparisons
        eq,
        leq,
        geq,
        le,
        ge,

        Null
    };
    class MQL_Filter;
    struct MQL_FilterCombiner
    {
        bool Negated = false;
        OperatorType Type = OperatorType::Null;
        std::vector<MQL_Filter> Operands;
    };
    struct MQL_MetricCombiner
    {
        std::vector<MQL_Filter> Operands;
        std::type_index LhsType = typeid(nullptr);
        std::type_index RhsType = typeid(nullptr);
        OperatorType Type = OperatorType::Null;
        bool Negated = false;
    };
    struct MQL_Filter_Literal
    {
        Literal Value;
    };
    struct MQL_FilterReference
    {
        bool Negated = false;
        std::shared_ptr<MQL_FilterDefinition> Filter;
        ArgumentList Args;
    };
    struct MQL_IntervallExtractor
    {
        bool Negated = false;
        ArgumentList Args;
        BuiltinFilterType Filter;
    };
    struct MQL_Metric
    {
        ArgumentList Args;
        MetricFuncType Metric;
    };

    std::ostream& operator<<(std::ostream& OutStream, MQL_Filter const& FilterToPrint);

    class MQL_Filter
    {
        std::variant<MQL_FilterCombiner, MQL_FilterReference,MQL_Filter_Literal,MQL_MetricCombiner,MQL_IntervallExtractor,MQL_Metric> m_Data;
        typedef decltype(m_Data) VariantType;
        public:

        MQL_Filter(MQL_Filter const&) = default;
        MQL_Filter(MQL_Filter&&) noexcept = default;
        MQL_Filter& operator=(MQL_Filter const&) = default;
        MQL_Filter& operator=(MQL_Filter&&) = default;
        template<typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        MQL_Filter(T&& Data)
        {
            m_Data = std::forward<T>(Data);
        }
        MQL_Filter(){};
        
        template<typename T>
        T const& GetType() const
        {
            return std::get<T>(m_Data);
        }
        template<typename T>
        T& GetType()
        {
            return std::get<T>(m_Data);
        }
        template<typename T>
        bool IsType() const
        {
            return std::holds_alternative<T>(m_Data);   
        }
    };

    #define FILTER_ARGLIST (MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallsToInspect,CallContext& Context)
    #define METRIC_ARGLIST (MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallToInspect)
   

    class CallContext
    {
        MQL_Module& m_Module;
    public:
        CallContext(MQL_Module& AssociatedModule) : m_Module(AssociatedModule)
        {

        }
        MQL_Module& GetModule()
        {
            return m_Module;   
        }
    };
    class MQLEvaluator
    {
    private:

        static int GetPlayerIndex(ArgumentList const& ExtraArguments);
    
        //static std::vector<GameIntervall> HasMove(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        //static std::vector<GameIntervall> HasState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        //static std::vector<GameIntervall> HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        //static std::vector<GameIntervall> HasHitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> BiggestPunishes FILTER_ARGLIST;
        static std::vector<GameIntervall> Move FILTER_ARGLIST;
        static std::vector<GameIntervall> InShield FILTER_ARGLIST;
        static std::vector<GameIntervall> Expand FILTER_ARGLIST;
        static std::vector<GameIntervall> ActionState FILTER_ARGLIST;
        static std::vector<GameIntervall> Until FILTER_ARGLIST;
        static std::vector<GameIntervall> PlayerFlags FILTER_ARGLIST;
        static std::vector<GameIntervall> HitBy FILTER_ARGLIST;
        static std::vector<GameIntervall> Offstage FILTER_ARGLIST;
        static std::vector<GameIntervall> Cornered FILTER_ARGLIST;
        std::unordered_map<std::string,BuiltinFilterType> m_BuiltinFilters = 
        {
            //{"HasMove",HasMove},
            //{"HasHitBy",HasHitBy},
            //{"HasProjectile",HasProjectile},
            //{"HasState",HasState},
            {"Punishes",BiggestPunishes},
            {"Move",Move},
            {"InShield",InShield},
            {"Expand",Expand},
            {"HitBy",HitBy},
            {"PlayerFlags",PlayerFlags},
            {"ActionState",ActionState},
            {"Until",Until},
            {"Offstage",Offstage},
            {"Cornered",Cornered},
        };
        struct BuiltinMetric
        {
            MetricFuncType Func = nullptr;
            std::type_index ResultType;
        };

        //Metrics
        static std::vector<MQL_MetricVariable> Percent METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> Delay METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> Begin METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> End METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> File METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> MoveName METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> PercentDiff METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> Length METRIC_ARGLIST;
        std::unordered_map<std::string,BuiltinMetric> m_BuiltinMetrics = 
        {
            {"Percent",{Percent,typeid(float)}},
            {"Delay",{Delay,typeid(int)}},
            {"End",{End,typeid(int)}},
            {"Begin",{Begin,typeid(int)}},
            {"File",{File,typeid(std::string)}},
            {"MoveName",{MoveName,typeid(std::string)}},
            {"PercentDiff",{PercentDiff,typeid(float)}},
            {"Length",{Length,typeid(int)}},
        };

        std::vector<SpecServer> m_SpecServers;
        std::unordered_map<std::string,int> m_FilterToServer;

        ModuleID m_CurrentModuleID = 1;
      
        MBCC::Tokenizer m_Tokenizer = GetTokenizer();
        
        std::unordered_map<ModuleID,std::shared_ptr<MQL_Module>> m_LoadedModules;
        std::unordered_map<std::string,ModuleID> m_LoadedModulePaths;
        std::vector<std::filesystem::path> m_ExtraModuleSearchPaths = {MBSystem::GetUserHomeDirectory()/".mbslippi/Libs"};
        
        MeleeGameDBAdapter* m_DBAdapter = nullptr;
        MeleeGameRecorder* m_Recorder = nullptr;

        void p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilterComponent(MQL_Module& AssociatedModule,Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyFilter(MQL_Module& AssociatedModule,Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameInfoPredicate_Direct(MQL_Module& AssociatedModule,Identifier const& Attribute,GameInfoPredicate_Direct& PredicateToVerify,
                bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameInfoPredicate(MQL_Module& AssociatedModule,GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyPlayerAssignment(MQL_Module& AssociatedModule,PlayerAssignment& AssignmentToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyGameSelection(MQL_Module& AssociatedModule,GameSelection& SelectionToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void p_VerifyResult(MQL_Module& AssociatedModule,Result& ResultToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

        bool p_SatisfiesPlayerAssignment(MQL_Module& AssociatedModule,SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate);
        bool p_EvaluateGameSelection(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate);
        bool p_GetPlayerAssignments(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4]);
        void p_ApplyAssignment(MeleeGame& GameToModify,char InAssignments[4]);
        void p_EvaluateTabulate(MQL_Module& AssociatedModule ,std::vector<RecordingInfo> const& FilterResult,Result_Tabulate const& TabulateInfo);
        std::string p_MetricToName(Filter_Component const& FilterToConvert);

        std::vector<MeleeGame> p_RetrieveSpecGames(MQL_Module& AssociatedModule,GameSelection const& GameSelection);
        //std::vector<GameIntervall> p_EvaluateGameIntervalls(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,Filter_Component const& FilterToUse,
        //        std::vector<GameIntervall> const& InputIntervalls,MeleeGame const& GameToFilter);

        
        std::vector<GameIntervall> p_EvaluateGameIntervalls(
                MQL_Module& AssociatedModule,
                MeleeGame const& InputGame,
                std::vector<GameIntervall> const& InputIntervalls,
                ArgumentList const& ArgList,
                MQL_Filter const& Filter);

        std::vector<MQL_MetricVariable> p_EvaluateMetric(
                MeleeGame const& InputGame,
                std::vector<GameIntervall> const& InputIntervalls,
                ArgumentList const& ArgList,
                MQL_Filter const& Filter);
        
        //converts and verifies
        struct PrecedenceInfo
        {
            OperatorType Operator;
            std::vector<int> OperatorPositions;
        };

        struct OperatorPrecedence
        {
            int Precedence = -1;
            OperatorType ResultOperator;
            bool Binary = true;
        };
        std::unordered_map<std::string,OperatorPrecedence> m_Operators = 
        {
            {"&",{10,OperatorType::Add,false}},
            {"|",{9,OperatorType::Pipe,false}},
            {"<",{8,OperatorType::le,true}},
            {"<=",{8,OperatorType::leq,true}},
            {">",{8,OperatorType::ge,true}},
            {">=",{8,OperatorType::geq,true}},
            {"*",{7,OperatorType::Times,true}},
            {"+",{6,OperatorType::Plus,true}},
            {"-",{6,OperatorType::Minus,true}},
        };
        
        PrecedenceInfo p_GetPrecedenceInfo(std::vector<std::string> const& Operators,int BeginIndex,int EndIndex);

        bool p_OperatorIsValid(OperatorType Op,std::type_index Lhs,std::type_index Rhs,std::type_index& OutType);
        MQL_Filter p_ConvertMetricOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
                Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,
                std::vector<MBLSP::Diagnostic>& OutDiagnostics,std::type_index& OutType);
        MQL_Filter p_ConvertMetricComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
                Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics,std::type_index& OutType);
        MQL_Filter p_ConvertFilterOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
                Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        MQL_Filter p_ConvertFilterComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
                Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

      
        static MBLSP::Position p_GetBegin(Literal const& Component);
        static MBLSP::Position p_GetEnd(Literal const& Component);
        static MBLSP::Position p_GetBegin(Filter_Component const& Component);
        static MBLSP::Position p_GetEnd(Filter_Component const& Component);
        
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Literal const& ErrorLiteral,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Identifier const& ErrorIdentifier,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_OperatorList const& ErrorList,int Begin,int End,std::string_view Message);
        
        void p_InitializeServers();

        bool p_EvaluateImport(MQL_Module& AssociatedModule,Import& ImportStatement,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
    public:
        void SetDBAdapter(MeleeGameDBAdapter* NewAdapter);
        void SetRecorder(MeleeGameRecorder* NewRecorder);
        void InitializeServers(std::vector<ServerInitilizationData> const& ServersToInitialize);

        //Top function for external uses, handles parsing and makes sure that paths etc are correct
        ModuleID LoadModule(std::filesystem::path const& ModulePath);
        ModuleID LoadEmptyModule();
        MQL_Module& GetModule(ModuleID ID);

        //
        bool VerifyVariableDeclaration(MQL_Module& AssociatedModule,Statement& DeclarationToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool UpdateState=false);
        bool VerifyStatement(MQL_Module& AssociatedModule,Statement& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        bool VerifySelection(MQL_Module& AssociatedModule,Selection& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

        //special semantics, in that it modifies the scope, so that modules can be verified correct
        //without needing to actually execute the actions, relatively hacky
        bool VerifyModule(MQL_Module& AssociatedModule,Module& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateStatement(MQL_Module& AssociatedModule,Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateImport(MQL_Module& AssociatedModule,Import& ImportToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateSelection(MQL_Module& AssociatedModule,Selection& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateVariableDeclaration(MQL_Module& AssociatedModule,Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        void EvaluateModule(MQL_Module& AssociatedModule,Module& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
    };
}