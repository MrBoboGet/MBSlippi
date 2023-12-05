#include <MBLSP/MBLSP.h>
#include "MBLSP/LSP_Structs.h"
#include "MBLSP/TextChanges.h"
#include "MQL.h"
#include "MBSlippi_CLI.h"
#include <MBLSP/SemanticTokens.h>
namespace MBSlippi
{
    class SlippiLSP : public MBLSP::LSP_Server
    {
    private:
        struct DocumentInfo
        {
            bool CorrectParsing = true;
            Module ParsedModule;
            std::vector<MBLSP::Diagnostic> Diagnostics;
            std::vector<MBLSP::SemanticToken> Tokens;
            std::vector<int> SemanticTokens;
        };
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

        void p_PushDiagnostics(DocumentInfo& DocumentData,std::string const& URI);
        std::vector<MBLSP::SemanticToken> p_ExtractTokens(Module const& Spec);
        DocumentInfo p_CreateDocumentInfo(std::string const& Content);
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

    };
}
