#pragma once
#include "MBUtility/MBInterfaces.h"
#include "MQLParser.h"
#include <MBLSP/MBLSP.h>
#include "MBMeleeID.h"
#include <MBSystem/BiDirectionalSubProcess.h>
#include <MBSystem/MBSystem.h>
#include <typeindex>
#include <type_traits>

#include <unordered_set>
namespace MBSlippi
{
    typedef int  ModuleID;
    class MQL_Filter;
    class MQL_MetricVariable;
    class ArgumentList
    {
        ArgumentList const* m_ParentArgList = nullptr;
        std::vector<MQL_Filter>  m_PositionalArguments;

        std::vector<MQL_Filter> m_KeyArgs;
        std::unordered_map<std::string,size_t> m_KeyPositions;

        //kinda hacky but walla walla
        mutable std::vector<MQL_Filter> m_EvaluatedValues;

        mutable std::unordered_map<std::string,size_t> m_EvaluatedKeyPositions;
        mutable std::unordered_map<size_t,size_t> m_EvaluatedPositionalPositions;

        void p_AddKey(std::string const& Key,MQL_Filter Value);
        
        MQL_Filter p_EvaluateConstExpr(MQL_Filter const& Expr) const; 
        static Literal p_VariableToLiteral(MQL_MetricVariable const& Var);

        MQL_Filter const* p_LookUpVar(MQL_Filter const& Value) const; 
        MQL_Filter const* p_GetEvaluatedValue(size_t AssociatedIndex) const; 
        MQL_Filter const* p_GetNamedVariable(std::string const& StringToSearch) const; 

    public:
        bool Empty() const;
        std::vector<std::string> GetKeys() const;

        ArgumentList() = default;

        void SetParentArgList(ArgumentList const* ParentList);
        ArgumentList const* GetParentArgList() const
        {
            return m_ParentArgList;   
        }

        ArgumentList(std::vector<MQL_Filter> PositionalArguments,std::unordered_map<std::string,MQL_Filter>& KeyArgs);
        //includes parent scope
        ArgumentList(ArgumentList const& DefinitionBindings,ArgumentList const& SuppliedArguments);

        MQL_Filter const& GetRawVariable(std::string const& VariableToGet) const;
        MQL_Filter const& GetRawVariable(size_t IndexToGet) const;
        bool HasNamedVariable(std::string const& VariableToCheck) const;
        std::string GetNamedVariableString(std::string const& VariableName) const;
        std::string GetPositionalArgumentString(int Index) const;
        size_t PositionalCount() const;

        MQL_Filter const& operator[](size_t Index) const;
        MQL_Filter const& operator[](std::string const& Name) const;
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
        for(int i = IntervallToInspect.FirstFrame; i <= IntervallToInspect.LastFrame;i++)
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
        ArgumentList Arguments;
        ModuleID AssociatedModule = -1;
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
    struct ArgumentErrors
    {
        std::unordered_map<int,std::string> PositionalErrors;
        std::unordered_map<std::string,std::string> KeyErrors;
        std::unordered_set<std::string> ValidKeys;
        std::vector<std::string> GenericErrors;
    };
    typedef void (*ArgumentCheckerType)(ArgumentErrors& OutErrors,ArgumentList const& Arguments);
    struct BuiltinFilterInfo
    {
        BuiltinFilterType Filter = nullptr;
        std::vector<ArgumentCheckerType> ArgCheckers;
    };
    struct BuiltinMetric
    {
        MetricFuncType Func = nullptr;
        std::type_index ResultType = std::type_index(typeid(nullptr));
        std::vector<ArgumentCheckerType> ArgCheckers;
        bool IsConstexpr = false;
    };
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
        bool  IsConstexpr = false;
        std::type_index LhsType = typeid(nullptr);
        std::type_index RhsType = typeid(nullptr);
        std::type_index ResultType = typeid(nullptr);
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
        BuiltinFilterInfo Filter;
    };
    struct MQL_Metric
    {
        bool IsConstexpr = false;
        std::type_index ResultType = typeid(nullptr);
        ArgumentList Args;
        BuiltinMetric Metric;
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
       

        std::type_index GetTypeID() const
        {
            if(IsLiteral())
            {
                auto const& Literal = GetType<MQL_Filter_Literal>().Value;
                if(Literal.IsType<Literal_String>())
                {
                    return typeid(std::string);
                }
                else if(Literal.IsType<Literal_Number>())
                {
                    return typeid(int);   
                }
            }
            else if(IsConstExpr())
            {
                if(IsType<MQL_MetricCombiner>())
                {
                    return GetType<MQL_MetricCombiner>().ResultType;
                }
                else  if(IsType<MQL_Metric>())
                {
                    return GetType<MQL_Metric>().ResultType;
                }
                else
                {
                    assert(false && "GetTypeID doesn't cover all cases");
                }
            }
            else
            {
                return typeid(MQL_Filter);   
            }

            return typeid(nullptr);
        }
        
        bool IsFilter() const
        {
            return IsType<MQL_FilterCombiner>() || IsType<MQL_FilterReference>() || IsType<MQL_IntervallExtractor>();
        }
        bool IsConstExpr() const
        {
            bool ReturnValue = false;
            if(!IsMetric())
            {
                return false;   
            }
            if(IsType<MQL_MetricCombiner>() && GetType<MQL_MetricCombiner>().IsConstexpr)
            {
                ReturnValue = true;   
            }
            if(IsType<MQL_Metric>() && GetType<MQL_Metric>().IsConstexpr)
            {
                ReturnValue = true;   
            }
            return ReturnValue;
        }
        bool IsLiteral() const
        {
            return IsType<MQL_Filter_Literal>();
        }
        bool IsSymbol() const
        {
            return IsLiteral() && std::get<MQL_Filter_Literal>(m_Data).Value.IsType<Literal_Symbol>();
        }
        bool IsMetric() const
        {
            return IsType<MQL_Metric>() || IsType<MQL_MetricCombiner>() || IsType<MQL_Filter_Literal>();
        }
        
        template<typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T const& GetType() const
        {
            return std::get<T>(m_Data);
        }
        template<typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T& GetType()
        {
            return std::get<T>(m_Data);
        }
        template<typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        bool IsType() const
        {
            return std::holds_alternative<T>(m_Data);   
        }
    };

    #define FILTER_ARGLIST (MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallsToInspect,CallContext& Context)
    #define METRIC_ARGLIST (MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallToInspect)
   

    class MQLEvaluator;
    class CallContext
    {
        MQL_Module& m_Module;
        MQLEvaluator& m_Evaluator;
    public:
        CallContext(MQL_Module& AssociatedModule,MQLEvaluator& Evaluator) : m_Module(AssociatedModule),m_Evaluator(Evaluator)
        {

        }
        MQL_Module& GetModule()
        {
            return m_Module;   
        }
        MQLEvaluator& GetEvaluator()
        {
            return m_Evaluator;   
        }
    };
    class MQLEvaluator
    {
    private:
        static bool p_IsInteger(MQL_Filter const& Value);
        static int p_GetInteger(MQL_Filter const& Value);
        static bool p_IsString(MQL_Filter const& Value);
        static std::string p_GetString(MQL_Filter const& Value);
        static bool p_IsFloat(MQL_Filter const& Value);
        static float p_GetFloat(MQL_Filter const& Value);

        static void PlayerArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void MoveArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void ShieldArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void ExpandArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void PlayerFlagArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void ActionStateArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void UntilArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void HasArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void PunishArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void CorneredArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);
        static void OtherArgChecker(ArgumentErrors& OutError,ArgumentList const& Args);

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
        static std::vector<GameIntervall> Has FILTER_ARGLIST;
        static std::vector<GameIntervall> Normalize FILTER_ARGLIST;


        std::unordered_map<std::string,BuiltinFilterInfo> m_BuiltinFilters = 
        {
            //{"HasMove",HasMove},
            //{"HasHitBy",HasHitBy},
            //{"HasProjectile",HasProjectile},
            //{"HasState",HasState},
            {"Punishes",{BiggestPunishes,{PlayerArgChecker,PunishArgChecker}}},
            {"Move",{Move,{PlayerArgChecker,MoveArgChecker}}},
            {"InShield",{InShield,{PlayerArgChecker,ShieldArgChecker}}},
            {"Expand",{Expand,{ExpandArgChecker}}},
            {"HitBy",{InShield,{PlayerArgChecker,MoveArgChecker}}},
            {"PlayerFlags",{PlayerFlags,{PlayerArgChecker,PlayerFlagArgChecker}}},
            {"ActionState",{ActionState,{PlayerArgChecker,ActionStateArgChecker}}},
            {"Until",{Until,{PlayerArgChecker,UntilArgChecker}}},
            {"Offstage",{Offstage,{PlayerArgChecker}}},
            {"Cornered",{Cornered,{PlayerArgChecker,CorneredArgChecker}}},
            {"Has",{Has,{HasArgChecker}}},
            {"Normalize",{Normalize,{}}},
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
        static std::vector<MQL_MetricVariable> ActionableFrames METRIC_ARGLIST;
        static std::vector<MQL_MetricVariable> Other METRIC_ARGLIST;
        std::unordered_map<std::string,BuiltinMetric> m_BuiltinMetrics = 
        {
            {"Percent",{Percent,typeid(float),{PlayerArgChecker}}},
            {"Delay",{Delay,typeid(int),{PlayerArgChecker}}},
            {"End",{End,typeid(int),{}}},
            {"Begin",{Begin,typeid(int),{}}},
            {"File",{File,typeid(std::string),{}}},
            {"MoveName",{MoveName,typeid(std::string),{PlayerArgChecker}}},
            {"PercentDiff",{PercentDiff,typeid(float),{PlayerArgChecker}}},
            {"Length",{Length,typeid(int),{}}},
            {"ActionableFrames",{ActionableFrames,typeid(int),{PlayerArgChecker}}},
            {"Other",{Other,typeid(int),{OtherArgChecker},true}},
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

        void p_FillError(ArgumentErrors& OutError,ArgumentList const& FilterArgs, Literal_Symbol const& ErrorSym,std::string_view Message);
        void p_FillErrors(ArgumentErrors& OutError,ArgumentList const& ArgDef,ArgumentList const& FilterArgs ,ArgumentErrors const& InError);
        void p_CombineErrors(ArgumentErrors& OutError,ArgumentErrors InError);
        ArgumentErrors p_VerifyReferenceArguments(MQL_Module& AssociatedModule, ArgumentList const& ArgDef,ArgumentList const& CalledArguments,MQL_Filter& FilterToVerify);
        void p_TypeCheckArgs(ArgumentErrors& OutErrors,ArgumentList const& ArgDef,ArgumentList const& CalledArguments);
        void p_AddDiagnostics(Filter_Component_Func const& Func,ArgumentList const& ParentArgList,ArgumentList const& ErrorArgList,ArgumentErrors const& Errors,std::vector<MBLSP::Diagnostic>& OutDiagnostics);

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

      
        std::vector<GameIntervall> p_EvaluateSubIntervalls(
                MQL_Module& AssociatedModule,
                MeleeGame const& InputGame,
                std::vector<GameIntervall> const& InputIntervalls,
                ArgumentList const& ArgList,
                MQL_Filter const& Filter);
        std::vector<GameIntervall> p_EvaluateGameIntervalls(
                MQL_Module& AssociatedModule,
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
                Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool AllowLiterals = false);
        MQL_Filter p_ConvertFilterComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
                Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool AllowLiterals = false);

        MQL_Filter p_ConvertArgComponent(MQL_Module& AssociatedModule, ArgumentList& ParentArgList,Filter_Component const& Filter,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
        ArgumentList p_ConvertArgList(MQL_Module& AssociatedModule, ArgumentList& ParentArgList,Filter_ArgList const& ArgsToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
      
        static MBLSP::Position p_GetBegin(Literal const& Component);
        static MBLSP::Position p_GetEnd(Literal const& Component);
        static MBLSP::Position p_GetBegin(Filter_Component const& Component);
        static MBLSP::Position p_GetEnd(Filter_Component const& Component);
        
        static void p_AddKeyError(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,std::string const& UnknownKey,std::string_view Error);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,size_t ArgIndex,std::string_view Error);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,MBCC::TokenPosition Begin,int Length,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,std::string const& Key,std::string_view Error);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Literal const& ErrorLiteral,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Identifier const& ErrorIdentifier,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_OperatorList const& ErrorList,int Begin,int End,std::string_view Message);
        static void p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component const& ErrorFilter,std::string_view Message);
        
        void p_InitializeServers();

        bool p_EvaluateImport(MQL_Module& AssociatedModule,Import& ImportStatement,std::vector<MBLSP::Diagnostic>& OutDiagnostics);
    public:
        static std::vector<MQL_MetricVariable> EvaluateMetric(
                MeleeGame const& InputGame,
                std::vector<GameIntervall> const& InputIntervalls,
                ArgumentList const& ArgList,
                MQL_Filter const& Filter);

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
