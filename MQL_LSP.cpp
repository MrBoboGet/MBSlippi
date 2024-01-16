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


    //BEGIN CompletionsExtractor
    bool CompletionsExtractor::p_Empty(MBCC::TokenPosition Pos)
    {
        return Pos.ByteOffset == 0 && Pos.Line == 0;
    }
    bool CompletionsExtractor::p_In(MBLSP::Position Left,MBLSP::Position Right,MBLSP::Position Cursor)
    {
        bool ReturnValue = false;
        if(Left <= Cursor)
        {
            if((Right.character == 0 && Right.line == 0) || Cursor <= Right)
            {
                ReturnValue = true;
            }
        }
        return ReturnValue;
    }
    bool CompletionsExtractor::p_In(MBCC::TokenPosition Left,MBCC::TokenPosition Right,MBLSP::Position Cursor)
    {
        return p_In(h_Convert(Left),h_Convert(Right),Cursor);
    }
    bool CompletionsExtractor::p_In(Identifier const& Ident,MBLSP::Position Cursor)
    {
        bool ReturnValue = false;
        if(Ident.Parts.size() > 0)
        {
            return p_In(h_Convert(Ident.Parts.front().Position),h_Convert(Ident.End),Cursor);
        }
        return ReturnValue;
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, std::vector<MQL_Statement> const& Statements)
    {
        for(auto const& Statement : Statements)
        {
            if(Statement.IsType<MQL_Import>())
            {
                auto const& Import = Statement.GetType<MQL_Import>();
                if(p_In(Import.Begin,Import.End,CursorPosition))
                {
                    p_GetCompletions(CursorPosition,Import);
                }
            }
            else if(Statement.IsType<MQL_VariableAssignment>())
            {
                auto const& Assignment = Statement.GetType<MQL_VariableAssignment>();
                if(p_In(Assignment.Begin,Assignment.End,CursorPosition))
                {
                    p_GetCompletions(CursorPosition,Assignment);
                }
            }
            else if(Statement.IsType<MQL_Selection>())
            {
                auto const& Selection = Statement.GetType<MQL_Selection>();
                p_GetCompletions(CursorPosition,Selection);
            }
        }   
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, MQL_Import const& Statement)
    {
        if(p_In(Statement.ModuleIdentifier,CursorPosition))
        {
            p_GetImportCompletions(CursorPosition,Statement.ModuleIdentifier);
        }
    }
    void CompletionsExtractor::p_GetImportCompletions(MBLSP::Position Cursor, Identifier const& ImportPath)
    {
        auto SearchDirectories = m_Evaluator.GetModuleSearchDirectories(m_Document->ModulePath);
        std::filesystem::path BasePath;
        for(auto const& Token : ImportPath.Parts)
        {
            if(p_In( h_Convert(Token.Position),h_Convert(Token.Position)+Token.Value.size()+1,Cursor))
            {
                break;
            }
            BasePath /= Token.Value;
        }
        for(auto const& Dir : SearchDirectories)
        {
            std::filesystem::directory_iterator DirIterator(Dir/BasePath);
            for(auto const& Entry : DirIterator)
            {
                if(Entry.is_regular_file())
                {
                    if(Entry.path().extension() == ".slpspec")
                    {
                        m_Result.push_back(MBUnicode::PathToUTF8(Entry.path().filename().replace_extension("")));
                    }
                }
                else if(Entry.is_directory())
                {
                    m_Result.push_back(MBUnicode::PathToUTF8(Entry.path().filename()));
                }
            }
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, MQL_VariableAssignment const& Statement)
    {
        if(std::holds_alternative<MQL_Variable_GameInfoPredicate>(Statement.Value.Data))
        {
            auto const& Predicate = std::get<MQL_Variable_GameInfoPredicate>(Statement.Value.Data);
            if(p_In(Predicate.Predicate.Begin,Predicate.Predicate.End,CursorPosition))
            {
                p_GetCompletions(CursorPosition,Predicate.Predicate,Predicate.IsPlayerAssignment);
            }
        }
        else if(std::holds_alternative<MQL_LazyGameList>(Statement.Value.Data))
        {
            auto const& GameList = std::get<MQL_LazyGameList>(Statement.Value.Data);
            p_GetCompletions(CursorPosition,GameList.GamesToRetrieve);
        }
        else if(std::holds_alternative<std::shared_ptr<MQL_FilterDefinition>>(Statement.Value.Data))
        {
            auto const& FilterDef = std::get<std::shared_ptr<MQL_FilterDefinition>>(Statement.Value.Data);
            ArgumentList EmptyArgList;
            p_GetCompletions(CursorPosition,EmptyArgList,FilterDef->Arguments);
            p_GetCompletions(CursorPosition,FilterDef->Arguments,*FilterDef->Component);
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, MQL_Selection const& Statement)
    {
        ArgumentList EmptyArgList;
        p_GetCompletions(CursorPosition,EmptyArgList,Statement.Filter);
        p_GetCompletions(CursorPosition,Statement.Games);
        p_GetCompletions(CursorPosition,Statement.Output);
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, ArgumentList const& ParentArgList ,MQL_Filter const& Statement)
    {
        if(p_In(Statement.Begin,Statement.End,CursorPosition))
        {
            if(Statement.IsType<MQL_FilterCombiner>())
            {
                auto const& Combiner = Statement.GetType<MQL_FilterCombiner>();
                for(auto const& SubFilter : Combiner.Operands)
                {
                    p_GetCompletions(CursorPosition,ParentArgList,SubFilter);
                }
            }
            else if(Statement.IsType<MQL_MetricCombiner>())
            {
                auto const& Combiner = Statement.GetType<MQL_MetricCombiner>();
                for(auto const& SubFilter : Combiner.Operands)
                {
                    p_GetCompletions(CursorPosition,ParentArgList,SubFilter);
                }
            }
            else if(Statement.IsType<MQL_IntervallExtractor>())
            {
                auto const& Filter = Statement.GetType<MQL_IntervallExtractor>();
                p_GetIdentifierCompletions(CursorPosition,Filter.Name);
                p_GetCompletions(CursorPosition,ParentArgList,Filter.Args);
            }
            else if(Statement.IsType<MQL_InvalidFilter>())
            {
                auto const& Filter = Statement.GetType<MQL_InvalidFilter>();
                p_GetIdentifierCompletions(CursorPosition,Filter.Name);
                p_GetCompletions(CursorPosition,ParentArgList,Filter.Args);
            }
            else if(Statement.IsType<MQL_FilterReference>())
            {
                auto const& Filter = Statement.GetType<MQL_FilterReference>();
                p_GetIdentifierCompletions(CursorPosition,Filter.Name);
                p_GetCompletions(CursorPosition,ParentArgList,Filter.Args);
            }
            else if(Statement.IsType<MQL_Metric>())
            {
                auto const& Filter = Statement.GetType<MQL_Metric>();
                p_GetIdentifierCompletions(CursorPosition,Filter.Name);
                p_GetCompletions(CursorPosition,ParentArgList,Filter.Args);
            }
            else if(Statement.IsType<MQL_Filter_Literal>())
            {
                auto const& Literal = Statement.GetType<MQL_Filter_Literal>().Value;
                if(Literal.IsType<Literal_Symbol>())
                {
                    auto const& Symbol = Literal.GetType<Literal_Symbol>();
                    auto EnvirSymbols = ParentArgList.GetKeys();
                    m_Result.insert(m_Result.end(),EnvirSymbols.begin(),EnvirSymbols.end());
                    if(p_In(Symbol.ValuePosition,Symbol.ValuePosition+Symbol.Value.size(),CursorPosition))
                    {
                        Identifier NewIdentifier;
                        NewIdentifier.Parts.push_back(Token());
                        NewIdentifier.Parts.back().Position = Symbol.ValuePosition;
                        NewIdentifier.Parts.back().Value = Symbol.Value;
                        p_GetIdentifierCompletions(CursorPosition,NewIdentifier);
                    }
                }
            }
        }
    }
    void CompletionsExtractor::p_GetIdentifierCompletions(MBLSP::Position CursorPosition, Identifier const& Statement)
    {
        if(!p_In(Statement,CursorPosition))
        {
            return;   
        }
        int TargetPosition = p_GetIdentifierIndex(CursorPosition,Statement);
        MQL_Scope* TargetScope = &m_Document->ResultModule.ModuleScope;
        for(int i  = 0; i < TargetPosition;i++)
        {
            if(TargetScope->HasBinding(Statement.Parts[i].Value))
            {
                TargetScope = &TargetScope->GetBinding(Statement.Parts[i].Value)->ModuleScope;
            }
            else
            {
                return;   
            }
        }
        auto PossibleVars = TargetScope->GetPossibleVars();
        m_Result.insert(m_Result.end(),PossibleVars.begin(),PossibleVars.end());
        if(TargetPosition == 0)
        {
            auto Extra = m_Evaluator.BuiltinNames();
            m_Result.insert(m_Result.end(),Extra.begin(),Extra.end());
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, GameInfoPredicate const& Predicate,bool IsPlayerAssignment)
    {
        if(!p_In(Predicate.Begin,Predicate.End,CursorPosition))
        {
            return;
        }
        p_GetAttributeCompletions(CursorPosition,Predicate.Attribute,IsPlayerAssignment);
        if(Predicate.Data.IsType<GameInfoPredicate_Direct>())
        {
            p_GetAttributeValueCompletions(CursorPosition,Predicate.Attribute,Predicate.Data.GetType<GameInfoPredicate_Direct>(),IsPlayerAssignment);
        }
        for(auto const& SubPredicate : Predicate.ExtraTerms)
        {
            p_GetCompletions(CursorPosition,SubPredicate,IsPlayerAssignment);
        }
    }
    void CompletionsExtractor::p_GetAttributeCompletions(MBLSP::Position CursorPosition, Identifier const& Attribute,bool IsPlayerAssignment)
    {
        if(!p_In(Attribute,CursorPosition))
        {
            return;   
        }
        int PartIndex = p_GetIdentifierIndex(CursorPosition,Attribute);
        if(PartIndex == 0)
        {
            m_Result.push_back("Stage");
            m_Result.push_back("Date");
            if(IsPlayerAssignment)
            {
                m_Result.push_back("Character");
                m_Result.push_back("Tag");
                m_Result.push_back("Code");
            }
            else
            {
                m_Result.push_back("Tag");   
                m_Result.push_back("Code");   
                m_Result.push_back("Character");   
            }
        }
        else if(PartIndex == 1)
        {
            m_Result.push_back("Tag");   
            m_Result.push_back("Code");   
            m_Result.push_back("Character");   
        }
    }
    int CompletionsExtractor::p_GetIdentifierIndex(MBLSP::Position CursorPosition,Identifier const& Ident)
    {
        int ReturnValue = 0;
        for(int i = 0; i < Ident.Parts.size();i++)
        {
            if(p_In(Ident.Parts[i].Position,Ident.Parts[i].Position+Ident.Parts[i].Value.size(),CursorPosition))
            {
                ReturnValue = i;
                break;
            }
            ReturnValue = i;
        }
        return ReturnValue;
    }

    void CompletionsExtractor::p_GetAttributeValueCompletions(MBLSP::Position CursorPosition, Identifier const& Attribute, GameInfoPredicate_Direct const& Value,bool IsPlayerAssignment)
    {
        if(p_In(Value.ValuePosition,Value.ValuePosition+Value.Value.size()+2,CursorPosition))
        {
            if(Attribute.Parts.size() == 1)
            {
                if(Attribute.Parts[0].Value  == "Stage")
                {
                    m_Result = PossibleStageStrings();
                }
            }
            int PlayerAttributeIndex = IsPlayerAssignment ? 0 : 1;
            if(Attribute.Parts.size() >= PlayerAttributeIndex+1)
            {
                if(Attribute.Parts[PlayerAttributeIndex].Value == "Character")
                {
                    m_Result = PossibleMBCharacterStrings();
                }
            }
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition,ArgumentList const& ParentArgList,ArgumentList const& VerifiedArgs)
    {
        for(auto const& Key : VerifiedArgs.GetKeys())
        {
            p_GetCompletions(CursorPosition,ParentArgList,VerifiedArgs.GetRawVariable(Key));
        }
        for(int i = 0; i < VerifiedArgs.PositionalCount();i++)
        {
            p_GetCompletions(CursorPosition,ParentArgList,VerifiedArgs.GetRawVariable(i));
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, GameSelection const& Selection)
    {
        p_GetCompletions(CursorPosition,Selection.Using);
        p_GetCompletions(CursorPosition,Selection.Assignment.PlayerCondition,true);
        p_GetCompletions(CursorPosition,Selection.GameCondition,false);
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, UsingDirective const& Using)
    {
        if(Using.GameSets.size() > 0 && p_In(Using.GameSets.front().Position,Using.End, CursorPosition))
        {
            auto ReplayDir = MBSystem::GetUserHomeDirectory()/".mbslippi/Replays";
            std::filesystem::directory_iterator DirIt(ReplayDir);
            for(auto const& Entry : DirIt)
            {
                if(Entry.is_directory())
                {
                    m_Result.push_back(MBUnicode::PathToUTF8(Entry.path().filename()));
                }   
            }
        }
    }
    void CompletionsExtractor::p_GetCompletions(MBLSP::Position CursorPosition, MQL_Result const& Output)
    {
        if(Output.IsType<MQL_Result_Tabulate>())
        {
            auto const& Tabulate = Output.GetType<MQL_Result_Tabulate>();
            for(auto const& Col : Tabulate.Columns)
            {
                ArgumentList EmptyArgList;
                p_GetCompletions(CursorPosition,EmptyArgList,Col.second);
            }
        }
    }
    std::vector<std::string> CompletionsExtractor::GetCompletions(DocumentInfo& Document,MBLSP::Position CursorPosition)
    {
        m_Result = {};
        m_Document = &Document;
        p_GetCompletions(CursorPosition,Document.Statements);
        return m_Result;
    }
    //END Completion extarctor
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
    DocumentInfo SlippiLSP::p_CreateDocumentInfo(std::string const& Content,std::filesystem::path DocumentPath)
    {
        DocumentInfo ReturnValue;
        ReturnValue.ModulePath = DocumentPath;
        m_Tokenizer.SetText(Content);
        try
        {
            //ReturnValue.ParsedModule = ParseModule(m_Tokenizer);
            try
            {
                FillModule(ReturnValue.ParsedModule,m_Tokenizer);
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
            MQLEvaluator Evaluator;
            Evaluator.SetDBAdapter(&m_TempHandler);
            Evaluator.SetRecorder(&m_TempHandler);
            ReturnValue.ResultModule = Evaluator.GetModule(Evaluator.LoadEmptyModule());
            ReturnValue.ResultModule.ModulePath = DocumentPath;
            ReturnValue.Statements = Evaluator.VerifyAndUpdateModule(ReturnValue.ResultModule,ReturnValue.ParsedModule,ReturnValue.Diagnostics);
            if(!m_Tokenizer.IsEOF(m_Tokenizer.Peek()))
            {
                ReturnValue.Diagnostics.push_back(MBLSP::Diagnostic());
                ReturnValue.Diagnostics.back().range.start.line = m_Tokenizer.Peek().Position.Line;
                ReturnValue.Diagnostics.back().range.start.character = m_Tokenizer.Peek().Position.ByteOffset;
                ReturnValue.Diagnostics.back().range.end = ReturnValue.Diagnostics.back().range.start+m_Tokenizer.Peek().Value.size();
                ReturnValue.Diagnostics.back().message = "trailing junk";
            }
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
        ReturnValue.result = MBLSP::Completion_Result();
        auto DocIt = m_OpenedDocuments.find(Request.params.textDocument.uri);
        if(DocIt == m_OpenedDocuments.end())
        {
            throw std::runtime_error("Attempting semanticTokens/range on unopened document");   
        }
        CompletionsExtractor Extractor;
        auto Completions = Extractor.GetCompletions(DocIt->second,Request.params.position);
        for(auto const& Completion : Completions)
        {
            MBLSP::CompletionItem Item;
            Item.label = Completion;
            ReturnValue.result->items.push_back(std::move(Item));
        }
        return ReturnValue;
    }

}
