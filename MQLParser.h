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
class Token :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition Position;
    std::string Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
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
class Literal_Base :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition ValuePosition;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Literal : public MBCC::PolyBase<Literal_Base>
{
    public:
    typedef MBCC::PolyBase<Literal_Base> Base;
    using Base::Base;
    
};
class Filter_Component_Base :  public MBCC::AST_Base
{
    public:
    bool Negated =  false;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component : public MBCC::PolyBase<Filter_Component_Base>
{
    public:
    typedef MBCC::PolyBase<Filter_Component_Base> Base;
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
class Operator :  public MBCC::AST_Base
{
    public:
    std::string Op;
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
class Module :  public MBCC::AST_Base
{
    public:
    std::vector<Statement> Statements;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class UsingDirective :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition UsingPosition;
    std::vector<Token> GameSets;
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
class Filter_ArgList :  public MBCC::AST_Base
{
    public:
    std::vector<Filter_Arg> Arguments;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Literal_Number : public Literal_Base
{
    public:
    int Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Arg_Positional : public Filter_Arg_Base
{
    public:
    Filter_Component Argument;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Arg_Named : public Filter_Arg_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    Filter_Component Argument;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class AttributeList :  public MBCC::AST_Base
{
    public:
    std::vector<AttributeComponent> Attributes;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Literal_String : public Literal_Base
{
    public:
    std::string Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_Component_Literal : public Filter_Component_Base
{
    public:
    Literal Value;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Filter_OperatorList : public Filter_Component_Base
{
    public:
    std::vector<std::string> Operators;
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
class Literal_Symbol : public Literal_Base
{
    public:
    std::string Value;
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
class ColumnSpec :  public MBCC::AST_Base
{
    public:
    Token Name;
    Filter_Component Metric;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class ImportBinding :  public MBCC::AST_Base
{
    public:
    MBCC::TokenPosition AsPosition;
    Token ImportName;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_Filter : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition FilterPosition;
    Filter_Component Component;
    Filter_ArgList Arguments;
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
class Filter_Component_Func : public Filter_Component_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    Identifier FilterName;
    Filter_ArgList ArgumentList;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Result_Tabulate : public Result_Base
{
    public:
    MBCC::TokenPosition TabulatePos;
    std::vector<ColumnSpec> Columns;
    MBCC::TokenPosition IntoPos;
    Literal_String OutFile;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class Import : public Statement_Base
{
    public:
    MBCC::TokenPosition ImportPosition;
    Identifier ImportPath;
    ImportBinding Binding;
    std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}
    
};
class VariableDeclaration_PlayerSelection : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameInfoPredicate Predicate;
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
template<> inline int MBCC::GetTypeBegin<VariableDeclaration_GameInfoPredicate>(){return(5);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration_GameInfoPredicate>(){return(6);}
template<> inline int MBCC::GetTypeBegin<Module>(){return(9);}
template<> inline int MBCC::GetTypeEnd<Module>(){return(10);}
template<> inline int MBCC::GetTypeBegin<Operator>(){return(10);}
template<> inline int MBCC::GetTypeEnd<Operator>(){return(11);}
template<> inline int MBCC::GetTypeBegin<Token>(){return(11);}
template<> inline int MBCC::GetTypeEnd<Token>(){return(12);}
template<> inline int MBCC::GetTypeBegin<UsingDirective>(){return(12);}
template<> inline int MBCC::GetTypeEnd<UsingDirective>(){return(13);}
template<> inline int MBCC::GetTypeBegin<GameSelection>(){return(13);}
template<> inline int MBCC::GetTypeEnd<GameSelection>(){return(14);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Conjunction>(){return(14);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Conjunction>(){return(15);}
template<> inline int MBCC::GetTypeBegin<AttributeComponent>(){return(15);}
template<> inline int MBCC::GetTypeEnd<AttributeComponent>(){return(16);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Data_Base>(){return(16);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Data_Base>(){return(18);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Direct>(){return(17);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Direct>(){return(18);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate>(){return(18);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate>(){return(19);}
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_OperatorList>(){return(19);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_OperatorList>(){return(20);}
template<> inline int MBCC::GetTypeBegin<PlayerAssignment>(){return(20);}
template<> inline int MBCC::GetTypeEnd<PlayerAssignment>(){return(21);}
template<> inline int MBCC::GetTypeBegin<Filter_ArgList>(){return(21);}
template<> inline int MBCC::GetTypeEnd<Filter_ArgList>(){return(22);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Base>(){return(22);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Base>(){return(25);}
template<> inline int MBCC::GetTypeBegin<Literal_Base>(){return(25);}
template<> inline int MBCC::GetTypeEnd<Literal_Base>(){return(29);}
template<> inline int MBCC::GetTypeBegin<Literal_String>(){return(26);}
template<> inline int MBCC::GetTypeEnd<Literal_String>(){return(27);}
template<> inline int MBCC::GetTypeBegin<Literal_Symbol>(){return(27);}
template<> inline int MBCC::GetTypeEnd<Literal_Symbol>(){return(28);}
template<> inline int MBCC::GetTypeBegin<Literal_Number>(){return(28);}
template<> inline int MBCC::GetTypeEnd<Literal_Number>(){return(29);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Positional>(){return(23);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Positional>(){return(24);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Named>(){return(24);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Named>(){return(25);}
template<> inline int MBCC::GetTypeBegin<AttributeList>(){return(29);}
template<> inline int MBCC::GetTypeEnd<AttributeList>(){return(30);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Base>(){return(30);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Base>(){return(34);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Func>(){return(31);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Func>(){return(32);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Literal>(){return(32);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Literal>(){return(33);}
template<> inline int MBCC::GetTypeBegin<Filter_OperatorList>(){return(33);}
template<> inline int MBCC::GetTypeEnd<Filter_OperatorList>(){return(34);}
template<> inline int MBCC::GetTypeBegin<Filter>(){return(34);}
template<> inline int MBCC::GetTypeEnd<Filter>(){return(35);}
template<> inline int MBCC::GetTypeBegin<Result_Base>(){return(35);}
template<> inline int MBCC::GetTypeEnd<Result_Base>(){return(39);}
template<> inline int MBCC::GetTypeBegin<Result_Record>(){return(36);}
template<> inline int MBCC::GetTypeEnd<Result_Record>(){return(37);}
template<> inline int MBCC::GetTypeBegin<Result_Print>(){return(37);}
template<> inline int MBCC::GetTypeEnd<Result_Print>(){return(38);}
template<> inline int MBCC::GetTypeBegin<ColumnSpec>(){return(39);}
template<> inline int MBCC::GetTypeEnd<ColumnSpec>(){return(40);}
template<> inline int MBCC::GetTypeBegin<Result_Tabulate>(){return(38);}
template<> inline int MBCC::GetTypeEnd<Result_Tabulate>(){return(39);}
template<> inline int MBCC::GetTypeBegin<ImportBinding>(){return(40);}
template<> inline int MBCC::GetTypeEnd<ImportBinding>(){return(41);}
template<> inline int MBCC::GetTypeBegin<Import>(){return(6);}
template<> inline int MBCC::GetTypeEnd<Import>(){return(7);}
template<> inline int MBCC::GetTypeBegin<Selection>(){return(7);}
template<> inline int MBCC::GetTypeEnd<Selection>(){return(8);}
Operator ParseComparator(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_2(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_3(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_4(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_5(MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_6(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_2(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_3(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_4(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_5(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_6(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_7(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_8(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_9(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_10(MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_11(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_0(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_1(MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_2(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_0(MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named_0(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional_0(MBCC::Tokenizer& Tokenizer);
Filter_ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer);
Filter_ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L1(MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L1_0(MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L2(MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L2_0(MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral(MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_0(MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_1(MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_2(MBCC::Tokenizer& Tokenizer);
Literal_String ParseLiteral_String(MBCC::Tokenizer& Tokenizer);
Literal_String ParseLiteral_String_0(MBCC::Tokenizer& Tokenizer);
Literal_Symbol ParseLiteral_Symbol(MBCC::Tokenizer& Tokenizer);
Literal_Symbol ParseLiteral_Symbol_0(MBCC::Tokenizer& Tokenizer);
Literal_Number ParseLiteral_Number(MBCC::Tokenizer& Tokenizer);
Literal_Number ParseLiteral_Number_0(MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier2(MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier2_0(MBCC::Tokenizer& Tokenizer);
Identifier Parse_L3(MBCC::Tokenizer& Tokenizer);
Identifier Parse_L3_0(MBCC::Tokenizer& Tokenizer);
Token ParseToken2(MBCC::Tokenizer& Tokenizer);
Token ParseToken2_0(MBCC::Tokenizer& Tokenizer);
Filter_Component_Func ParseFilter_Component_Func(MBCC::Tokenizer& Tokenizer);
Filter_Component_Func ParseFilter_Component_Func_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_1(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_2(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_3(MBCC::Tokenizer& Tokenizer);
Filter_Component_Literal ParseFilter_Component_Literal(MBCC::Tokenizer& Tokenizer);
Filter_Component_Literal ParseFilter_Component_Literal_0(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_Component(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList Parse_L4(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList Parse_L4_0(MBCC::Tokenizer& Tokenizer);
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
ColumnSpec ParseColumnSpec(MBCC::Tokenizer& Tokenizer);
ColumnSpec ParseColumnSpec_0(MBCC::Tokenizer& Tokenizer);
ColumnSpec Parse_L5(MBCC::Tokenizer& Tokenizer);
ColumnSpec Parse_L5_0(MBCC::Tokenizer& Tokenizer);
Result_Tabulate ParseResult_Tabulate(MBCC::Tokenizer& Tokenizer);
Result_Tabulate ParseResult_Tabulate_0(MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L6(MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L6_0(MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L7(MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L7_0(MBCC::Tokenizer& Tokenizer);
Result ParseResult(MBCC::Tokenizer& Tokenizer);
Result ParseResult_0(MBCC::Tokenizer& Tokenizer);
Result ParseResult_1(MBCC::Tokenizer& Tokenizer);
Result ParseResult_2(MBCC::Tokenizer& Tokenizer);
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
Identifier ParseIdentifier(MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier_0(MBCC::Tokenizer& Tokenizer);
Identifier Parse_L8(MBCC::Tokenizer& Tokenizer);
Identifier Parse_L8_0(MBCC::Tokenizer& Tokenizer);
Token ParseToken(MBCC::Tokenizer& Tokenizer);
Token ParseToken_0(MBCC::Tokenizer& Tokenizer);
UsingDirective ParseGameList(MBCC::Tokenizer& Tokenizer);
UsingDirective ParseGameList_0(MBCC::Tokenizer& Tokenizer);
UsingDirective Parse_L9(MBCC::Tokenizer& Tokenizer);
UsingDirective Parse_L9_0(MBCC::Tokenizer& Tokenizer);
UsingDirective ParseUsingDirective(MBCC::Tokenizer& Tokenizer);
UsingDirective ParseUsingDirective_0(MBCC::Tokenizer& Tokenizer);
ImportBinding ParseImportBinding(MBCC::Tokenizer& Tokenizer);
ImportBinding ParseImportBinding_0(MBCC::Tokenizer& Tokenizer);
Import ParseImport(MBCC::Tokenizer& Tokenizer);
Import ParseImport_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_2(MBCC::Tokenizer& Tokenizer);
inline MBCC::Tokenizer GetTokenizer()
{
    MBCC::Tokenizer ReturnValue("((( |\\t|\\n|\\r)*)|(#.*\\n))*",{"\\(","\\)","\\{","\\}","\\[","\\]","[[:digit:]]+","$\"((\\\\.|[^\"\\\\])*)\"","true|false","WITH","IMPORT","USING","AS","SELECT","RECORD","Games","GamePredicate","PlayerSelection","Filter","PRINT","TABULATE","INTO","$\\$([[:alnum:]_]+)","[[:alpha:]_]+[[:alnum:]_]*",";","<=","<","!",">=",">","!=","\\+","-","\\*","/",":",",","==","=","\\.","\\|\\|","\\|","&&","&","\\?",});
    return(ReturnValue);
}