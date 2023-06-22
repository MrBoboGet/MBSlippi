#pragma once
#include "SlippiSpecParser.h"
#include <MBLSP/MBLSP.h>
#include "MBMeleeID.h"
#include <MBSystem/BiDirectionalSubProcess.h>
#include <MBSystem/MBSystem.h>
namespace MBSlippi
{
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
    class MQL_Module;
    class MQL_Scope
    {
        std::vector<std::shared_ptr<MQL_Module>> m_OverlayedModules;
        std::map<std::string,std::shared_ptr<MQL_Module>> m_BoundModules;

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
        static std::vector<GameIntervall> PlayerFlags(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> HasHitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Offstage(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Length(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        static std::vector<GameIntervall> Cornered(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect);
        std::unordered_map<std::string,BuiltinFilterType> m_BuiltinFilters = 
        {
            {"Punishes",BiggestPunishes},
            {"HasMove",HasMove},
            {"Move",Move},
            {"InShield",InShield},
            {"Expand",Expand},
            {"HitBy",HitBy},
            {"HasHitBy",HasHitBy},
            {"PlayerFlags",PlayerFlags},
            {"HasProjectile",HasProjectile},
            {"ActionState",ActionState},
            {"Until",Until},
            {"Offstage",Offstage},
            {"Length",Length},
            {"Cornered",Cornered},
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

        bool p_SatisfiesPlayerAssignment(MQL_Module& AssociatedModule,SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate);
        bool p_EvaluateGameSelection(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate);
        bool p_GetPlayerAssignments(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4]);
        void p_ApplyAssignment(MeleeGame& GameToModify,char InAssignments[4]);

        std::vector<MeleeGame> p_RetrieveSpecGames(MQL_Module& AssociatedModule,GameSelection const& GameSelection);
        std::vector<GameIntervall> p_EvaluateGameIntervalls(MQL_Module& AssociatedModule,Filter_Component const& FilterToUse,
                std::vector<GameIntervall> const& InputIntervalls,MeleeGame const& GameToFilter);

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
