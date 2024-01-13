#include "MQL_LSP.h"
#include "MBLSP/LSP_Structs.h"
#include "MQLParser.h"
#include "MQL.h"
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

    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Literal const& LiteralToExamine)
    {
        OutTokens.push_back(GetToken(LiteralToExamine));
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens, Filter const& FilterToExamine)
    {
        if(!FilterToExamine.Component.IsEmpty())
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                        h_Convert(FilterToExamine.FilterPosition),6));
            p_ExtractTokens(OutTokens,FilterToExamine.Component);
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_Component const& ComponentToExamine)
    {
        if(ComponentToExamine.IsType<Filter_Component_Func>())
        {
            auto const& Func = ComponentToExamine.GetType<Filter_Component_Func>();
            if(Func.FilterName.Parts.size() != 0)
            {

                for(int i = 0; i < Func.FilterName.Parts.size();i++)
                {
                    auto const& CurrentPart = Func.FilterName.Parts[i];
                    if(i + 1 < Func.FilterName.Parts.size())
                    {
                        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Namespace,
                                    h_Convert(CurrentPart.Position),CurrentPart.Value.size()));
                    }
                    else
                    {
                        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Function,
                                    h_Convert(CurrentPart.Position),CurrentPart.Value.size()));
                    }
                }
                if(Func.ArgumentList.Arguments.size() != 0)
                {
                    p_ExtractTokens(OutTokens,Func.ArgumentList);
                }
            }
        }
        else if(ComponentToExamine.IsType<Filter_OperatorList>())
        {
            for(auto const& SubFilter : ComponentToExamine.GetType<Filter_OperatorList>().Components)
            {
                p_ExtractTokens(OutTokens,SubFilter);
            }
        }
        else if(ComponentToExamine.IsType<Filter_Component_Literal>())
        {
            OutTokens.push_back(GetToken(ComponentToExamine.GetType<Filter_Component_Literal>().Value));
        }
    }
    MBLSP::SemanticToken SlippiLSP::GetToken(Literal const& LiteralToConvert)
    {
        MBLSP::SemanticToken ReturnValue;
        if(LiteralToConvert.IsType<Literal_String>())
        {
            auto const& Literal = LiteralToConvert.GetType<Literal_String>();
            ReturnValue = MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(Literal.ValuePosition),Literal.Value.size()+2);
        }
        else if(LiteralToConvert.IsType<Literal_Symbol>())
        {
            auto const& Literal = LiteralToConvert.GetType<Literal_Symbol>();
            ReturnValue = MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(Literal.ValuePosition),Literal.Value.size());
        }
        else if(LiteralToConvert.IsType<Literal_Number>())
        {
            auto const& Literal = LiteralToConvert.GetType<Literal_Number>();
            ReturnValue = MBLSP::SemanticToken(MBLSP::TokenType::Number,h_Convert(Literal.ValuePosition),std::to_string(Literal.Value).size());
        }
        else
        {
            assert(false && "LSP doesnt cover all cases for literal type");
        }
        return ReturnValue;
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Filter_ArgList const& ComponentToExamine)
    {
        for(auto const& Arg : ComponentToExamine.Arguments)
        {
            if(Arg.IsType<Filter_Arg_Named>())
            {
                auto const& NamedArg = Arg.GetType<Filter_Arg_Named>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(NamedArg.NamePosition),NamedArg.Name.size()));
                p_ExtractTokens(OutTokens,NamedArg.Argument);
            }   
            else if(Arg.IsType<Filter_Arg_Positional>())
            {
                auto const& PositionalArg  = Arg.GetType<Filter_Arg_Positional>();
                p_ExtractTokens(OutTokens,PositionalArg.Argument);
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
            for(auto const& Attribute : PredicateToExamine.Attribute.Parts)
            {
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(Attribute.Position),Attribute.Value.size()));
            }
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,h_Convert(DirectData.ValuePosition),DirectData.Value.size() + 2));
        }
        else if(PredicateToExamine.Data.IsEmpty())
        {
            for(int i = 0; i < PredicateToExamine.Attribute.Parts.size();i++)
            {
                auto const& Attribute = PredicateToExamine.Attribute.Parts[i];
                MBLSP::TokenType NewTokenType = MBLSP::TokenType::Property;
                if(i + 1 < PredicateToExamine.Attribute.Parts.size())
                {
                    NewTokenType = MBLSP::TokenType::Namespace;
                }
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(Attribute.Position),Attribute.Value.size()));
            }
        }
        for(auto const& SubComponent : PredicateToExamine.ExtraTerms)
        {
            p_ExtractTokens(OutTokens,SubComponent);   
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,GameSelection const& SelectionToExamine)
    {
        if(SelectionToExamine.Using.GameSets.size() != 0)
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(SelectionToExamine.Using.UsingPosition),5));
        }
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
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,ColumnSpec const& ColumnToExamine)
    {
        if(ColumnToExamine.Name.Value != "")
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,
                        MBLSP::Position{ColumnToExamine.Name.Position.Line,ColumnToExamine.Name.Position.ByteOffset},
                        ColumnToExamine.Name.Value.size()));
        }
        p_ExtractTokens(OutTokens,ColumnToExamine.Metric);
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
        else if(ResultToExamine.IsType<Result_Tabulate>())
        {
            auto const& TabulateResult = ResultToExamine.GetType<Result_Tabulate>();
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                        MBLSP::Position{TabulateResult.TabulatePos.Line,TabulateResult.TabulatePos.ByteOffset},
                        8));
            for(auto const& Column : TabulateResult.Columns)
            {
                p_ExtractTokens(OutTokens,Column);
            }
            if(TabulateResult.OutFile.Value != "")
            {
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,
                            MBLSP::Position{TabulateResult.IntoPos.Line,TabulateResult.IntoPos.ByteOffset},
                            4));
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::String,
                            MBLSP::Position{TabulateResult.OutFile.ValuePosition.Line,TabulateResult.OutFile.ValuePosition.ByteOffset},
                            TabulateResult.OutFile.Value.size()+2));

            }
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Statement const& StatementToExamine)
    {
        if(StatementToExamine.IsType<Selection>())
        {
            p_ExtractTokens(OutTokens,StatementToExamine.GetType<Selection>());
        }
        else if(StatementToExamine.IsType<Import>())
        {
            p_ExtractTokens(OutTokens,StatementToExamine.GetType<Import>());
        }
        else if(StatementToExamine.IsType<VariableDeclaration>())
        {
            auto const& VariableType = StatementToExamine.GetType<VariableDeclaration>().GetBase();
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Property,h_Convert(VariableType.NamePosition),VariableType.Name.size()));

            //typedef Statement::i_BaseType<VariableDeclaration>::type Test;
            typedef MBCC::BaseType<VariableDeclaration>::type Test; 
            typedef Statement::TypeBaseParent_t<VariableDeclaration> Test2;
            
            if(StatementToExamine.IsType<VariableDeclaration_Filter>())
            {
                auto const& FilterType = StatementToExamine.GetType<VariableDeclaration_Filter>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(FilterType.FilterPosition),6));
                if(FilterType.Arguments.Arguments.size() != 0)
                {
                    p_ExtractTokens(OutTokens,FilterType.Arguments);
                }
                p_ExtractTokens(OutTokens,FilterType.Component);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_GameInfoPredicate>())
            {
                auto const& PredicateVariable = StatementToExamine.GetType<VariableDeclaration_GameInfoPredicate>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(PredicateVariable.GamePosition),13));
                p_ExtractTokens(OutTokens,PredicateVariable.Predicate);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_PlayerSelection>())
            {
                auto const& PredicateVariable = StatementToExamine.GetType<VariableDeclaration_PlayerSelection>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(PredicateVariable.GamePosition),15));
                p_ExtractTokens(OutTokens,PredicateVariable.Predicate);
            }
            else if(StatementToExamine.IsType<VariableDeclaration_GameList>())
            {
                auto const& GameListVariable = StatementToExamine.GetType<VariableDeclaration_GameList>();
                OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Class,h_Convert(GameListVariable.GamePosition),5));
                p_ExtractTokens(OutTokens,GameListVariable.Selection);
            }
        }
    }
    void SlippiLSP::p_ExtractTokens(std::vector<MBLSP::SemanticToken>& OutTokens,Import const& ImportToExamine)
    {
        OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(ImportToExamine.ImportPosition),6));
        for(auto const& Part : ImportToExamine.ImportPath.Parts)
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Namespace,h_Convert(Part.Position),Part.Value.size()));
        }
        if(ImportToExamine.Binding.ImportName.Value != "")
        {
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Keyword,h_Convert(ImportToExamine.Binding.AsPosition),2));
            OutTokens.push_back(MBLSP::SemanticToken(MBLSP::TokenType::Namespace,h_Convert(ImportToExamine.Binding.ImportName.Position),
                        ImportToExamine.Binding.ImportName.Value.size()));
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
    SlippiLSP::DocumentInfo SlippiLSP::p_CreateDocumentInfo(std::string const& Content,std::filesystem::path DocumentPath)
    {
        DocumentInfo ReturnValue;
        m_Tokenizer.SetText(Content);
        try
        {
            //ReturnValue.ParsedModule = ParseModule(m_Tokenizer);
            FillModule(ReturnValue.ParsedModule,m_Tokenizer);
            MQLEvaluator Evaluator;
            Evaluator.SetDBAdapter(&m_TempHandler);
            Evaluator.SetRecorder(&m_TempHandler);
            MQL_Module TempModule = Evaluator.GetModule(Evaluator.LoadEmptyModule());
            TempModule.ModulePath = DocumentPath;
            Evaluator.VerifyModule(TempModule,ReturnValue.ParsedModule,ReturnValue.Diagnostics);
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
        ReturnValue.Tokens = p_ExtractTokens(ReturnValue.ParsedModule);
        ReturnValue.SemanticTokens = MBLSP::CalculateSemanticTokens(ReturnValue.Tokens);
        return(ReturnValue);
    }
    MBLSP::Initialize_Response SlippiLSP::HandleRequest(MBLSP::InitializeRequest const& Request)
    {
        MBLSP::Initialize_Response ReturnValue;
        ReturnValue.result = MBLSP::Initialize_Result();
        ReturnValue.result->capabilities.textDocumentSync = MBLSP::TextDocumentSyncOptions();
        //ReturnValue.result->capabilities.declarationProvider = true;
        ReturnValue.result->capabilities.semanticTokensProvider = MBLSP::SemanticTokensOptions();
        MBLSP::SemanticTokensLegend Legend;
        Legend.tokenTypes = MBLSP::GetTokenLegend();
        Legend.tokenTypes.push_back("TestTest");
        ReturnValue.result->capabilities.semanticTokensProvider->legend = Legend;
        ReturnValue.result->capabilities.semanticTokensProvider->full = true;
        ReturnValue.result->capabilities.semanticTokensProvider->range = false;
        ReturnValue.result->capabilities.completionProvider = MBLSP::CompletionOptions();
        return(ReturnValue); 
    }
    void SlippiLSP::OpenedDocument(std::string const& URI,std::string const& Content)
    {
        m_OpenedDocuments[URI] = p_CreateDocumentInfo(Content, MBLSP::URLDecodePath(URI));
        p_PushDiagnostics(m_OpenedDocuments[URI],URI);
    }
    void SlippiLSP::ClosedDocument(std::string const& URI)
    {
        m_OpenedDocuments.erase(URI);
    }
    void SlippiLSP::DocumentChanged(std::string const& URI,std::string const& NewContent)
    {
        DocumentInfo NewDocumentInfo = p_CreateDocumentInfo(NewContent, MBLSP::URLDecodePath(URI));
        m_OpenedDocuments[URI] = std::move(NewDocumentInfo);
        p_PushDiagnostics(m_OpenedDocuments[URI],URI);
    }
    void SlippiLSP::DocumentChanged(std::string const& URI,std::string const& NewContent,std::vector<MBLSP::TextChange> const& Changes)
    {
        DocumentInfo NewDocumentInfo = p_CreateDocumentInfo(NewContent, MBLSP::URLDecodePath(URI));
        if(!NewDocumentInfo.CorrectParsing && m_OpenedDocuments[URI].SemanticTokens.size() != 0)
        {
            NewDocumentInfo.Tokens =MBLSP::CombineTokens(std::move(NewDocumentInfo.Tokens),  MBLSP::UpdateSemanticTokens(m_OpenedDocuments[URI].Tokens,Changes));
            NewDocumentInfo.SemanticTokens = MBLSP::CalculateSemanticTokens(NewDocumentInfo.Tokens);
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
    MBLSP::Completion_Response SlippiLSP::HandleRequest(MBLSP::Completion_Request const& Request)
    {
        MBLSP::Completion_Response ReturnValue;
        auto DocIt = m_OpenedDocuments.find(Request.params.textDocument.uri);
        if(DocIt == m_OpenedDocuments.end())
        {
            throw std::runtime_error("Attempting semanticTokens/range on unopened document");   
        }
        auto const& AST = DocIt->second.ParsedModule;

        return ReturnValue;
    }

}
