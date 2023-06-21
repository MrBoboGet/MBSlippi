#pragma once
#include <MBCC/MBCC.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <memory>
class Statement_Base :  public MBCC::AST_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Statement : public MBCC::PolyBase<Statement_Base>
{
    public:
    typedef MBCC::PolyBase<Statement_Base> Base;
    using Base::Base;
    
};
class Filter_Arg_Base :  public MBCC::AST_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Arg : public MBCC::PolyBase<Filter_Arg_Base>
{
    public:
    typedef MBCC::PolyBase<Filter_Arg_Base> Base;
    using Base::Base;
    
};
class AttributeComponent :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate_Data_Base :  public MBCC::AST_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate_Data : public MBCC::PolyBase<GameInfoPredicate_Data_Base>
{
    public:
    typedef MBCC::PolyBase<GameInfoPredicate_Data_Base> Base;
    using Base::Base;
    
};
class Operator :  public MBCC::AST_Base
{
    public:
    std::string Op;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Token :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition Position;
    std::string Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component_Data_Base :  public MBCC::AST_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component_Data : public MBCC::PolyBase<Filter_Component_Data_Base>
{
    public:
    typedef MBCC::PolyBase<Filter_Component_Data_Base> Base;
    using Base::Base;
    
};
class Result_Base :  public MBCC::AST_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Result : public MBCC::PolyBase<Result_Base>
{
    public:
    typedef MBCC::PolyBase<Result_Base> Base;
    using Base::Base;
    
};
class ImportBinding :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition AsPosition;
    std::string ImportName;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_Base : public Statement_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration : public MBCC::PolyBase<VariableDeclaration_Base>
{
    public:
    typedef MBCC::PolyBase<VariableDeclaration_Base> Base;
    using Base::Base;
    operator Statement()&&
    {
        return(Statement(std::move(m_Data),m_TypeID));
    }
    
};
class Identifier :  public MBCC::AST_Base
{
    public:
    std::vector<Token> Parts;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate_Direct : public GameInfoPredicate_Data_Base
{
    public:
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    std::string Comparison;
    uint64_t DateLowerBound;
    uint64_t DateHigherBound;
    int ComponentCount =  0;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Module :  public MBCC::AST_Base
{
    public:
    std::vector<Statement> Statements;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class UsingDirective :  public MBCC::AST_Base
{
    public:
    std::vector<Token> GameSets;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_ArgList :  public MBCC::AST_Base
{
    public:
    std::vector<Filter_Arg> Arguments;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Arg_Positional : public Filter_Arg_Base
{
    public:
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Arg_Named : public Filter_Arg_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class ArgList :  public MBCC::AST_Base
{
    public:
    std::vector<Filter_Arg> Args;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class AttributeList :  public MBCC::AST_Base
{
    public:
    std::vector<AttributeComponent> Attributes;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component_Variable : public Filter_Component_Data_Base
{
    public:
    MBCC::TokenPosition VariablePosition;
    std::string VariableName;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component_Function : public Filter_Component_Data_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string FilterName;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Result_Record : public Result_Base
{
    public:
    MBCC::TokenPosition RecordPosition;
    MBCC::TokenPosition FilePosition;
    std::string OutFile;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Result_Print : public Result_Base
{
    public:
    MBCC::TokenPosition PrintPosition;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Result_Tabulate : public Result_Base
{
    public:
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Import : public Statement_Base
{
    public:
    MBCC::TokenPosition ImportPosition;
    std::vector<Token> ImportPath;
    ImportBinding Binding;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate :  public MBCC::AST_Base
{
    public:
    Identifier Attribute;
    GameInfoPredicate_Data Data;
    std::string Operator;
    bool Negated =  false;
    std::vector<GameInfoPredicate> ExtraTerms;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    Identifier FilterName;
    bool Negated =  false;
    Filter_ArgList ArgumentList;
    std::string Operator;
    std::vector<Filter_Component> ExtraTerms;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_Filter : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition FilterPosition;
    Filter_Component Component;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_OperatorList :  public MBCC::AST_Base
{
    public:
    std::vector<Filter_Component> Components;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition FilterPosition;
    Filter_Component Component;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_PlayerSelection : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameInfoPredicate Predicate;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class FilterList :  public MBCC::AST_Base
{
    public:
    std::vector<Filter_Component> Components;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_GameInfoPredicate : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameInfoPredicate Predicate;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate_Conjunction :  public MBCC::AST_Base
{
    public:
    std::string Conjunction;
    GameInfoPredicate RHS;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameInfoPredicate_OperatorList :  public MBCC::AST_Base
{
    public:
    std::vector<GameInfoPredicate> Terms;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class PlayerAssignment :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition WithPosition;
    MBCC::TokenPosition PlayerPosition;
    std::string AffectedPlayer;
    GameInfoPredicate PlayerCondition;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class GameSelection :  public MBCC::AST_Base
{
    public:
    UsingDirective Using;
    PlayerAssignment Assignment;
    MBCC::TokenPosition SelectPosition;
    GameInfoPredicate GameCondition;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_GameList : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameSelection Selection;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Selection : public Statement_Base
{
    public:
    GameSelection Games;
    Filter SituationFilter;
    Result Output;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
template<> inline int MBCC::GetTypeBegin<Statement_Base>(){return(0);}
template<> inline int MBCC::GetTypeEnd<Statement_Base>(){return(8);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_Base>(){return(1);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_Base>(){return(6);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_Filter>(){return(2);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_Filter>(){return(3);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_GameList>(){return(3);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_GameList>(){return(4);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_PlayerSelection>(){return(4);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_PlayerSelection>(){return(5);}
template<> inline int MBCC::GetTypeBegin<Identifier>(){return(8);}
template<> inline int MBCC::GetTypeEnd<Identifier>(){return(9);}
template<> inline int MBCC::GetTypeBegin<FilterList>(){return(9);}
template<> inline int MBCC::GetTypeEnd<FilterList>(){return(10);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_GameInfoPredicate>(){return(5);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_GameInfoPredicate>(){return(6);}
template<> inline int MBCC::GetTypeBegin<Module>(){return(10);}
template<> inline int MBCC::GetTypeEnd<Module>(){return(11);}
template<> inline int MBCC::GetTypeBegin<Operator>(){return(11);}
template<> inline int MBCC::GetTypeEnd<Operator>(){return(12);}
template<> inline int MBCC::GetTypeBegin<Token>(){return(12);}
template<> inline int MBCC::GetTypeEnd<Token>(){return(13);}
template<> inline int MBCC::GetTypeBegin<UsingDirective>(){return(13);}
template<> inline int MBCC::GetTypeEnd<UsingDirective>(){return(14);}
template<> inline int MBCC::GetTypeBegin<GameSelection>(){return(14);}
template<> inline int MBCC::GetTypeEnd<GameSelection>(){return(15);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Conjunction>(){return(15);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Conjunction>(){return(16);}
template<> inline int MBCC::GetTypeBegin<AttributeComponent>(){return(16);}
template<> inline int MBCC::GetTypeEnd<AttributeComponent>(){return(17);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Data_Base>(){return(17);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Data_Base>(){return(19);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Direct>(){return(18);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Direct>(){return(19);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate>(){return(19);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate>(){return(20);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_OperatorList>(){return(20);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_OperatorList>(){return(21);}
template<> inline int MBCC::GetTypeBegin<PlayerAssignment>(){return(21);}
template<> inline int MBCC::GetTypeEnd<PlayerAssignment>(){return(22);}
template<> inline int MBCC::GetTypeBegin<Filter_ArgList>(){return(22);}
template<> inline int MBCC::GetTypeEnd<Filter_ArgList>(){return(23);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Base>(){return(23);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Base>(){return(26);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Positional>(){return(24);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Positional>(){return(25);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Named>(){return(25);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Named>(){return(26);}
template<> inline int MBCC::GetTypeBegin<ArgList>(){return(26);}
template<> inline int MBCC::GetTypeEnd<ArgList>(){return(27);}
template<> inline int MBCC::GetTypeBegin<AttributeList>(){return(27);}
template<> inline int MBCC::GetTypeEnd<AttributeList>(){return(28);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Data_Base>(){return(28);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Data_Base>(){return(31);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Variable>(){return(29);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Variable>(){return(30);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Function>(){return(30);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Function>(){return(31);}
template<> inline int MBCC::GetTypeBegin<Filter_Component>(){return(31);}
template<> inline int MBCC::GetTypeEnd<Filter_Component>(){return(32);}
template<> inline int MBCC::GetTypeBegin<Filter_OperatorList>(){return(32);}
template<> inline int MBCC::GetTypeEnd<Filter_OperatorList>(){return(33);}
template<> inline int MBCC::GetTypeBegin<Filter>(){return(33);}
template<> inline int MBCC::GetTypeEnd<Filter>(){return(34);}
template<> inline int MBCC::GetTypeBegin<Result_Base>(){return(34);}
template<> inline int MBCC::GetTypeEnd<Result_Base>(){return(38);}
template<> inline int MBCC::GetTypeBegin<Result_Record>(){return(35);}
template<> inline int MBCC::GetTypeEnd<Result_Record>(){return(36);}
template<> inline int MBCC::GetTypeBegin<Result_Print>(){return(36);}
template<> inline int MBCC::GetTypeEnd<Result_Print>(){return(37);}
template<> inline int MBCC::GetTypeBegin<Result_Tabulate>(){return(37);}
template<> inline int MBCC::GetTypeEnd<Result_Tabulate>(){return(38);}
template<> inline int MBCC::GetTypeBegin<ImportBinding>(){return(38);}
template<> inline int MBCC::GetTypeEnd<ImportBinding>(){return(39);}
template<> inline int MBCC::GetTypeBegin<Import>(){return(6);}
template<> inline int MBCC::GetTypeEnd<Import>(){return(7);}
template<> inline int MBCC::GetTypeBegin<Selection>(){return(7);}
template<> inline int MBCC::GetTypeEnd<Selection>(){return(8);}
Operator ParseOperator(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_2(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_3(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_4(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_5(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_6(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_7(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_8(MBCC::Tokenizer& Tokenizer);
Operator ParseOperator_9(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_2(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_3(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_4(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_5(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_6(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_2(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_Extra(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_Extra_0(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_0(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named_0(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional_0(MBCC::Tokenizer& Tokenizer);
ArgList ParseFilter_ArgList_Arguments(MBCC::Tokenizer& Tokenizer);
ArgList ParseFilter_ArgList_Arguments_0(MBCC::Tokenizer& Tokenizer);
ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer);
ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Or(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Or_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Base(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Base_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_1(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_2(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilterOr(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilterOr_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilterAdd(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilterAdd_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer);
Filter ParseFilter(MBCC::Tokenizer& Tokenizer);
Filter ParseFilter_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicateOr(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicateOr_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_1(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_2(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer);
GameSelection ParseGameSelection(MBCC::Tokenizer& Tokenizer);
GameSelection ParseGameSelection_0(MBCC::Tokenizer& Tokenizer);
PlayerAssignment ParsePlayerAssignment(MBCC::Tokenizer& Tokenizer);
PlayerAssignment ParsePlayerAssignment_0(MBCC::Tokenizer& Tokenizer);
Result_Record ParseResult_Record(MBCC::Tokenizer& Tokenizer);
Result_Record ParseResult_Record_0(MBCC::Tokenizer& Tokenizer);
Result_Print ParseResult_Print(MBCC::Tokenizer& Tokenizer);
Result_Print ParseResult_Print_0(MBCC::Tokenizer& Tokenizer);
Result ParseResult(MBCC::Tokenizer& Tokenizer);
Result ParseResult_0(MBCC::Tokenizer& Tokenizer);
Result ParseResult_1(MBCC::Tokenizer& Tokenizer);
Selection ParseSelection(MBCC::Tokenizer& Tokenizer);
Selection ParseSelection_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_Filter ParseVariableDeclaration_Filter(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_Filter ParseVariableDeclaration_Filter_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameList ParseVariableDeclaration_GameList(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameList ParseVariableDeclaration_GameList_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate(MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_2(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_3(MBCC::Tokenizer& Tokenizer);
Module ParseModule(MBCC::Tokenizer& Tokenizer);
Module ParseModule_0(MBCC::Tokenizer& Tokenizer);
Token ParseIdentifierPart(MBCC::Tokenizer& Tokenizer);
Token ParseIdentifierPart_0(MBCC::Tokenizer& Tokenizer);
Token ParseIdentifierPart_Rest(MBCC::Tokenizer& Tokenizer);
Token ParseIdentifierPart_Rest_0(MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier(MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer);
inline MBCC::Tokenizer GetTokenizer()
{
    MBCC::Tokenizer ReturnValue("((( |\\t|\\n|\\r)*)|(#.*\\n))*",{"\\(","\\)","\\{","\\}","\\[","\\]","[[:digit:]]+","$\"((\\\\.|[^\"\\\\])*)\"","true|false","WITH","SELECT","RECORD","Games","GamePredicate","PlayerSelection","Filter","PRINT","$\\$([[:alnum:]_]+)","[[:alpha:]_]+[[:alnum:]_]*",";","<=","<","!",">=",">","!=",":",",","=","\\.","\\|\\|","\\|","&&","&","\\?",});
    return(ReturnValue);
}