#include "SlippiSpecLSP.h"
#include "MBLSP/LSP_Structs.h"
#include "SlippiSpecParser.h"
#include "SlippiSpec.h"
namespace MBSlippi
{
    MBLSP::Position h_Convert(MBCC::TokenPosition PositionToConvert)
    {
        MBLSP::Position ReturnValue;
        ReturnValue.line = PositionToConvert.Line;
        ReturnValue.character = PositionToConvert.ByteOffset;
        return(ReturnValue);
    }

    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens, Filter const& FilterToExamine)
    {
        if(FilterToExamine.Component.FilterName != "")
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                        h_Convert(FilterToExamine.FilterPosition),6));
            p_ExtractTokens(OutTokens,FilterToExamine.Component);
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_Component const& ComponentToExamine)
    {
        if(ComponentToExamine.FilterName != "")
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Function,
                        h_Convert(ComponentToExamine.NamePosition),ComponentToExamine.FilterName.size()));
            for(auto const& SubComponent : ComponentToExamine.ExtraTerms)
            {
                p_ExtractTokens(OutTokens,SubComponent);
            }
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameInfoPredicate const& PredicateToExamine)
    {
        for(auto const& Attribute : PredicateToExamine.Attribute)
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(Attribute.NamePosition),Attribute.Name.size()));
        }
        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(PredicateToExamine.ValuePosition),PredicateToExamine.Value.size() + 2));
        for(auto const& SubComponent : PredicateToExamine.ExtraTerms)
        {
            p_ExtractTokens(OutTokens,SubComponent);   
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameSelection const& SelectionToExamine)
    {
        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(SelectionToExamine.SelectPosition),6));
        p_ExtractTokens(OutTokens,SelectionToExamine.GameCondition);
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,PlayerAssignment const& AssignmentToExamine)
    {
        if(AssignmentToExamine.AffectedPlayer != "")
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(AssignmentToExamine.WithPosition),4));
            p_ExtractTokens(OutTokens,AssignmentToExamine.PlayerCondition);
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Result const& ResultToExamine)
    {
        if(ResultToExamine.IsType<Result_Record>())
        {
            auto const& RecordResult = ResultToExamine.GetType<Result_Record>();
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                        MBLSP::Position{RecordResult.RecordPosition.Line,RecordResult.RecordPosition.ByteOffset},
                        6));
        }
    }
    std::vector<MBLSP::SemanticToken> SlippiLSP::p_ExtractTokens(SlippiSpec const& Spec)
    {
        std::vector<MBLSP::SemanticToken> ReturnValue;

        p_ExtractTokens(ReturnValue,Spec.Assignment);
        p_ExtractTokens(ReturnValue,Spec.Games);
        p_ExtractTokens(ReturnValue,Spec.SituationFilter);
        p_ExtractTokens(ReturnValue,Spec.Output);

        std::sort(ReturnValue.begin(),ReturnValue.end());
        return(ReturnValue);
    }
    SlippiLSP::DocumentInfo SlippiLSP::p_CreateDocumentInfo(std::string const& Content)
    {
        DocumentInfo ReturnValue;
        m_Tokenizer.SetText(Content);
        try
        {
            ReturnValue.ParsedSpec = ParseSlippiSpec(m_Tokenizer);
            ReturnValue.SemanticTokens = p_ExtractTokens(ReturnValue.ParsedSpec);
            SpecEvaluator Evaluator;
            Evaluator.VerifySpec(ReturnValue.ParsedSpec,ReturnValue.Diagnostics);
            if(!m_Tokenizer.IsEOF(m_Tokenizer.Peek()))
            {

                ReturnValue.Diagnostics.push_back(MBLSP::Diagnostic());
                ReturnValue.Diagnostics.back().range.start.line = m_Tokenizer.Peek().Position.Line;
                ReturnValue.Diagnostics.back().range.start.character = m_Tokenizer.Peek().Position.ByteOffset;
                ReturnValue.Diagnostics.back().range.end = ReturnValue.Diagnostics.back().range.start+m_Tokenizer.Peek().Value.size();
                ReturnValue.Diagnostics.back().message = "trailing junk";
            }
        }
        catch(MBCC::ParsingException const& ParseException)
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.range.start = h_Convert(ParseException.Position);
            NewDiagnostic.range.end = NewDiagnostic.range.start +m_Tokenizer.Peek().Value.size();
            NewDiagnostic.message = "Error parsing "+ParseException.NonterminalName+": expected "+ParseException.ExpectedType;
            ReturnValue.Diagnostics.push_back(NewDiagnostic);
        }
        catch(std::exception const& e)
        {
            //Range, xd, arbitrary
            ReturnValue.Diagnostics.push_back(MBLSP::Diagnostic());
            ReturnValue.Diagnostics.back().range.end.line = 0;
            ReturnValue.Diagnostics.back().range.end.character = 0;
            ReturnValue.Diagnostics.back().range.start.line = 1;
            ReturnValue.Diagnostics.back().range.start.character = 0;
            ReturnValue.Diagnostics.back().message = e.what();
        }
        return(ReturnValue);
        return(ReturnValue);
    }
    MBLSP::Initialize_Response SlippiLSP::HandleRequest(MBLSP::InitializeRequest const& Request)
    {
        MBLSP::Initialize_Response ReturnValue;
        ReturnValue.result = MBLSP::Initialize_Result();
        ReturnValue.result->capabilities.textDocumentSync = MBLSP::TextDocumentSyncOptions();
        ReturnValue.result->capabilities.declarationProvider = true;
        ReturnValue.result->capabilities.semanticTokensProvider = MBLSP::SemanticTokensOptions();
        MBLSP::SemanticTokensLegend Legend;
        Legend.tokenTypes = MBLSP::GetTokenLegend();
        ReturnValue.result->capabilities.semanticTokensProvider->legend = Legend;
        ReturnValue.result->capabilities.semanticTokensProvider->full = true;
        ReturnValue.result->capabilities.semanticTokensProvider->range = false;
        return(ReturnValue); 
    }
    void SlippiLSP::OpenedDocument(std::string const& URI,std::string const& Content)
    {
        m_OpenedDocuments[URI] = p_CreateDocumentInfo(Content);
        p_PushDiagnostics(m_OpenedDocuments[URI],URI);
    }
    void SlippiLSP::ClosedDocument(std::string const& URI)
    {
        m_OpenedDocuments.erase(URI);
    }
    void SlippiLSP::DocumentChanged(std::string const& URI,std::string const& NewContent)
    {
        m_OpenedDocuments[URI] = p_CreateDocumentInfo(NewContent);
        p_PushDiagnostics(m_OpenedDocuments[URI],URI);
    }
    void SlippiLSP::p_PushDiagnostics(DocumentInfo& DocumentData,std::string const& URI)
    {
        MBLSP::PublishDiagnostics_Notification Notification; 
        Notification.params.uri = URI;
        Notification.params.diagnostics = std::move(DocumentData.Diagnostics);
        m_AssociatedHandler->SendNotification(Notification);
    }
    void SlippiLSP::SetHandler(MBLSP::LSP_ServerHandler* AssociatedHandler)
    {
        m_AssociatedHandler = AssociatedHandler;
    }
    //Skip the mandatory initialized notification, probably only necessary
    //with dynamic registration

    MBLSP::GotoDefinition_Response SlippiLSP::HandleRequest(MBLSP::GotoDefinition_Request const& Request)
    {
        throw MBLSP::LSP_Not_Implemented();   
    }
    MBLSP::SemanticToken_Response SlippiLSP::HandleRequest(MBLSP::SemanticToken_Request const& Request)
    {
        MBLSP::SemanticToken_Response ReturnValue;
        ReturnValue.result = MBLSP::SemanticTokens();
        ReturnValue.result->data = MBLSP::CalculateSemanticTokens(m_OpenedDocuments[Request.params.textDocument.uri].SemanticTokens);
        return(ReturnValue);
    }

}
