#include "SlippiSpecLSP.h"
#include "MBLSP/LSP_Structs.h"
#include "SlippiSpecParser.h"
#include "SlippiSpec.h"
#include <assert.h>
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
        if(!FilterToExamine.Component.Data.IsEmpty())
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                        h_Convert(FilterToExamine.FilterPosition),6));
            p_ExtractTokens(OutTokens,FilterToExamine.Component);
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_Component const& ComponentToExamine)
    {
        if(ComponentToExamine.Data.IsType<Filter_Component_Function>())
        {
            auto const& FunctionData = ComponentToExamine.Data.GetType<Filter_Component_Function>();
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Function,
                        h_Convert(FunctionData.NamePosition),FunctionData.FilterName.size()));

            if(ComponentToExamine.ArgumentList.Arguments.size() != 0)
            {
                p_ExtractTokens(OutTokens,ComponentToExamine.ArgumentList);
            }
        }
        else if(ComponentToExamine.Data.IsType<Filter_Component_Variable>())
        {
            auto const& VariableData = ComponentToExamine.Data.GetType<Filter_Component_Variable>();
            OutTokens.push_back(MBLSP::SemanticToken(
                        MBLSP::TokenType::Keyword,h_Convert(VariableData.VariablePosition),VariableData.VariableName.size()+1));
        }
        for(auto const& SubComponent : ComponentToExamine.ExtraTerms)
        {
            p_ExtractTokens(OutTokens,SubComponent);
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_ArgList const& ComponentToExamine)
    {
        for(auto const& Arg : ComponentToExamine.Arguments)
        {
            if(Arg.IsType<Filter_Arg_Named>())
            {
                auto const& NamedArg = Arg.GetType<Filter_Arg_Named>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(NamedArg.NamePosition),NamedArg.Name.size()));
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(NamedArg.ValuePosition),NamedArg.Value.size()+2));
            }   
            else if(Arg.IsType<Filter_Arg_Positional>())
            {
                auto const& PositionalArg  = Arg.GetType<Filter_Arg_Positional>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(PositionalArg.ValuePosition),PositionalArg.Value.size()+2));
            }
            else
            {
                assert(false && "p_ExtractTokens in LSP doesn't cover all cases for Filter_ArgList");   
            }
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameInfoPredicate const& PredicateToExamine)
    {
        if(PredicateToExamine.Data.IsType<GameInfoPredicate_Direct>())
        {
            auto const& DirectData = PredicateToExamine.Data.GetType<GameInfoPredicate_Direct>();
            for(auto const& Attribute : DirectData.Attribute)
            {
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(Attribute.NamePosition),Attribute.Name.size()));
            }
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(DirectData.ValuePosition),DirectData.Value.size() + 2));
        }
        else if(PredicateToExamine.Data.IsType<GameInfoPredicate_Variable>())
        {
            auto const& VariableData = PredicateToExamine.Data.GetType<GameInfoPredicate_Variable>();
            OutTokens.push_back(MBLSP::SemanticToken(
                        MBLSP::TokenType::Keyword,h_Convert(VariableData.VariablePosition),VariableData.VariableName.size()+1));
               
        }
        for(auto const& SubComponent : PredicateToExamine.ExtraTerms)
        {
            p_ExtractTokens(OutTokens,SubComponent);   
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameSelection const& SelectionToExamine)
    {
        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(SelectionToExamine.SelectPosition),6));
        p_ExtractTokens(OutTokens,SelectionToExamine.GameCondition);
        p_ExtractTokens(OutTokens,SelectionToExamine.Assignment);
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
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,
                        MBLSP::Position{RecordResult.FilePosition.Line,RecordResult.FilePosition.ByteOffset},
                        RecordResult.OutFile.size()+2));
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Statement const& StatementToExamine)
    {
        if(StatementToExamine.IsType<Selection>())
        {
            p_ExtractTokens(OutTokens,StatementToExamine.GetType<Selection>());
        }
        else if(StatementToExamine.IsType<VariableDeclaration_Base>())
        {
            auto const& VariableType = StatementToExamine.GetType<VariableDeclaration_Base>();
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(VariableType.NamePosition),VariableType.Name.size()));
            if(StatementToExamine.IsType<VariableDeclaration_Filter>())
            {
                auto const& FilterType = StatementToExamine.GetType<VariableDeclaration_Filter>();
                p_ExtractTokens(OutTokens,FilterType.Component);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_GameInfoPredicate>())
            {
                auto const& PredicateVariable = StatementToExamine.GetType<VariableDeclaration_GameInfoPredicate>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(PredicateVariable.GamePosition),4));
                p_ExtractTokens(OutTokens,PredicateVariable.Predicate);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_PlayerSelection>())
            {
                auto const& PredicateVariable = StatementToExamine.GetType<VariableDeclaration_PlayerSelection>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(PredicateVariable.GamePosition),6));
                p_ExtractTokens(OutTokens,PredicateVariable.Predicate);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_GameList>())
            {
                auto const& GameListVariable = StatementToExamine.GetType<VariableDeclaration_GameList>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(GameListVariable.GamePosition),8));
                p_ExtractTokens(OutTokens,GameListVariable.Selection);
            }
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Selection const& SelectionToExamine)
    {
        p_ExtractTokens(OutTokens,SelectionToExamine.Games);
        p_ExtractTokens(OutTokens,SelectionToExamine.SituationFilter);
        p_ExtractTokens(OutTokens,SelectionToExamine.Output);

    }
    std::vector<MBLSP::SemanticToken> SlippiLSP::p_ExtractTokens(Module const& Spec)
    {
        std::vector<MBLSP::SemanticToken> ReturnValue;
        for(auto const& Statement : Spec.Statements)
        {
            p_ExtractTokens(ReturnValue,Statement);
        }
        std::sort(ReturnValue.begin(),ReturnValue.end());
        return(ReturnValue);
    }
    SlippiLSP::DocumentInfo SlippiLSP::p_CreateDocumentInfo(std::string const& Content)
    {
        DocumentInfo ReturnValue;
        m_Tokenizer.SetText(Content);
        try
        {
            ReturnValue.ParsedModule = ParseModule(m_Tokenizer);
            ReturnValue.Tokens = p_ExtractTokens(ReturnValue.ParsedModule);
            ReturnValue.SemanticTokens = MBLSP::CalculateSemanticTokens(ReturnValue.Tokens);
            SpecEvaluator Evaluator;
            Evaluator.VerifyModule(ReturnValue.ParsedModule,ReturnValue.Diagnostics);
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
            ReturnValue.CorrectParsing = false;
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.range.start = h_Convert(ParseException.Position);
            NewDiagnostic.range.end = NewDiagnostic.range.start +m_Tokenizer.Peek().Value.size();
            NewDiagnostic.message = "Error parsing "+ParseException.NonterminalName+": expected "+ParseException.ExpectedType;
            ReturnValue.Diagnostics.push_back(NewDiagnostic);
        }
        catch(std::exception const& e)
        {
            //Range, xd, arbitrary
            ReturnValue.CorrectParsing = false;
            ReturnValue.Diagnostics.push_back(MBLSP::Diagnostic());
            ReturnValue.Diagnostics.back().range.end.line = 0;
            ReturnValue.Diagnostics.back().range.end.character = 0;
            ReturnValue.Diagnostics.back().range.start.line = 1;
            ReturnValue.Diagnostics.back().range.start.character = 0;
            ReturnValue.Diagnostics.back().message = e.what();
        }
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
        Legend.tokenTypes.push_back("TestTest");
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
        DocumentInfo NewDocumentInfo = p_CreateDocumentInfo(NewContent);
        m_OpenedDocuments[URI] = std::move(NewDocumentInfo);
        p_PushDiagnostics(m_OpenedDocuments[URI],URI);
    }
    void SlippiLSP::DocumentChanged(std::string const& URI,std::string const& NewContent,std::vector<MBLSP::TextChange> const& Changes)
    {
        DocumentInfo NewDocumentInfo = p_CreateDocumentInfo(NewContent);
        if(!NewDocumentInfo.CorrectParsing && m_OpenedDocuments[URI].SemanticTokens.size() != 0)
        {
            NewDocumentInfo.SemanticTokens = MBLSP::UpdateSemanticTokens(m_OpenedDocuments[URI].SemanticTokens,Changes);
        }
        m_OpenedDocuments[URI] = std::move(NewDocumentInfo);
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
        ReturnValue.result->data = m_OpenedDocuments[Request.params.textDocument.uri].SemanticTokens;
        return(ReturnValue);
    }
    MBLSP::SemanticTokensRange_Response SlippiLSP::HandleRequest(MBLSP::SemanticTokensRange_Request const& Request) 
    {
        MBLSP::SemanticTokensRange_Response ReturnValue;
        auto DocIt = m_OpenedDocuments.find(Request.params.textDocument.uri);
        if(DocIt == m_OpenedDocuments.end())
        {
            throw std::runtime_error("Attempting semanticTokens/range on unopened document");   
        }
        ReturnValue.result = MBLSP::SemanticTokens();
        ReturnValue.result->data = MBLSP::GetTokenRange(DocIt->second.SemanticTokens,Request.params.range);
        return(ReturnValue);
    }

}
