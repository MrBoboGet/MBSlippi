#include <MBLSP/MBLSP.h>
#include "MBLSP/LSP_Structs.h"
#include "MBLSP/TextChanges.h"
#include "MQL.h"
#include "MBSlippi_CLI.h"
#include <MBLSP/SemanticTokens.h>
namespace MBSlippi
{

    struct Scope
    {
           
    };

    struct DocumentInfo
    {
        bool CorrectParsing = true;
        std::filesystem::path ModulePath;
        MQL_Module ResultModule;
        Module ParsedModule;
        std::vector<MQL_Statement> Statements;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        std::vector<MBLSP::SemanticToken> Tokens;
        //std::vector<std::string> Completions;
        std::vector<int> SemanticTokens;


    };

    class CompletionsExtractor
    {
        std::vector<std::string> m_Result;
        MQLEvaluator m_Evaluator;
        DocumentInfo * m_Document = nullptr;
        
        void p_GetCompletions(MBLSP::Position CursorPosition, std::vector<MQL_Statement> const& Statements);

        void p_GetCompletions(MBLSP::Position CursorPosition, MQL_Import const& Statement);
        void p_GetImportCompletions(MBLSP::Position CursorPosition, Identifier const& ImportPath);

        void p_GetCompletions(MBLSP::Position CursorPosition, MQL_VariableAssignment const& Statement);
        void p_GetCompletions(MBLSP::Position CursorPosition, MQL_Selection const& Statement);

        void p_GetCompletions(MBLSP::Position CursorPosition, MQL_Filter const& Statement);
        void p_GetIdentifierCompletions(MBLSP::Position CursorPosition, Identifier const& Statement);


        void p_GetCompletions(MBLSP::Position CursorPosition, GameInfoPredicate const& Predicate,bool IsPlayerAssignment);
        void p_GetAttributeCompletions(MBLSP::Position CursorPosition, Identifier const& Attribute,bool IsPlayerAssignment);
        int p_GetIdentifierIndex(MBLSP::Position CursorPosition,Identifier const& Ident);

        void p_GetAttributeValueCompletions(MBLSP::Position CursorPosition, Identifier const& Attribute, GameInfoPredicate_Direct const& Valu,bool IsPlayerAssignment);

        void p_GetCompletions(MBLSP::Position CursorPosition, ArgumentList const& VerifiedArgs);
        void p_GetCompletions(MBLSP::Position CursorPosition, GameSelection const& Selection);
        void p_GetCompletions(MBLSP::Position CursorPosition, UsingDirective const& Using);
        void p_GetCompletions(MBLSP::Position CursorPosition, MQL_Result const& Output);


        bool p_Empty(MBCC::TokenPosition Pos);
        bool p_In(MBLSP::Position Left,MBLSP::Position Right,MBLSP::Position Cursor);
        bool p_In(MBCC::TokenPosition Left,MBCC::TokenPosition Right,MBLSP::Position Cursor);
        bool p_In(Identifier const& Ident,MBLSP::Position Cursor);
    public:
        std::vector<std::string> GetCompletions(DocumentInfo& Document,MBLSP::Position CursorPosition);
    };

    
    class SlippiLSP : public MBLSP::LSP_Server
    {
    private:
        MBLSP::LSP_ServerHandler* m_AssociatedHandler = nullptr;
        MBCC::Tokenizer m_Tokenizer = GetTokenizer();
        

        MBSlippiCLIHandler m_TempHandler;

        //PlayerAssignment Assignment;
        //GameSelection Games;
        //Filter SituationFilter;
        //Result Output;


        static MBLSP::SemanticToken GetToken(Literal const& LiteralToConvert);
        
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Literal const& LiteralToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter const& FilterToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_Component const& ComponentToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_ArgList const& ComponentToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameInfoPredicate const& PredicateToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameSelection const& SelectionToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,PlayerAssignment const& AssignmentToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,ColumnSpec const& ColumnToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Result const& ResultToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Statement const& StatementToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Selection const& SelectionToExamine);
        void p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Import const& ImportToExamine);

        void p_GetCompletions(std::vector<std::string>& Completions,MQL_Scope const& Context ,Identifier const& Identifier);
        void p_GetStringCompletion(std::vector<std::string>& Completions,MQL_Scope const& Context ,Identifier const& Identifier);


        void p_PushDiagnostics(DocumentInfo& DocumentData,std::string const& URI);
        std::vector<MBLSP::SemanticToken> p_ExtractTokens(Module const& Spec);
        DocumentInfo p_CreateDocumentInfo(std::string const& Content,std::filesystem::path DocumentPath);
        std::unordered_map<std::string,DocumentInfo> m_OpenedDocuments;
    public:
        //Mandatory to support
        virtual MBLSP::Initialize_Response HandleRequest(MBLSP::InitializeRequest const& Request) override;
        virtual void OpenedDocument(std::string const& URI,std::string const& Content) override;
        virtual void ClosedDocument(std::string const& URI) override;
        virtual void DocumentChanged(std::string const& URI,std::string const& NewContent) override;
        virtual void DocumentChanged(std::string const& URI,std::string const& NewContent,std::vector<MBLSP::TextChange> const& Changes) override;

        virtual void SetHandler(MBLSP::LSP_ServerHandler* AssociatedHandler) override;
        //Skip the mandatory initialized notification, probably only necessary
        //with dynamic registration

        virtual MBLSP::GotoDefinition_Response HandleRequest(MBLSP::GotoDefinition_Request const& Request) override;
        virtual MBLSP::SemanticToken_Response HandleRequest(MBLSP::SemanticToken_Request const& Request) override;
        virtual MBLSP::SemanticTokensRange_Response HandleRequest(MBLSP::SemanticTokensRange_Request const& Request) override;
        virtual MBLSP::Completion_Response HandleRequest(MBLSP::Completion_Request const& Request) override;

    };
}
