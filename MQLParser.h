#pragma once
#include <MBCC/MBCC.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <memory>
class Statement;class VariableDeclaration;class VariableDeclaration_Filter;class VariableDeclaration_GameList;class VariableDeclaration_PlayerSelection;class Identifier;class VariableDeclaration_GameInfoPredicate;class Module;class Operator;class Token;class UsingDirective;class GameSelection;class GameInfoPredicate_Conjunction;class AttributeComponent;class GameInfoPredicate_Data;class GameInfoPredicate_Direct;class GameInfoPredicate;class GameInfoPredicate_OperatorList;class PlayerAssignment;class Filter_ArgList;class Filter_Arg;class Literal;class Literal_String;class Literal_Symbol;class Literal_PosRef;class Literal_Number;class Filter_Arg_Positional;class Filter_Arg_Named;class AttributeList;class Filter_Component;class Filter_Component_Func;class Filter_Component_Literal;class Filter_OperatorList;class Filter;class Result;class Result_Record;class Result_Print;class ColumnSpec;class Result_Tabulate;class ImportBinding;class Import;class Selection;class Statement_Base :  public MBCC::AST_Base
{
public:
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Statement : public MBCC::PolyBase<Statement_Base,VariableDeclaration,Import,Selection>
{
public:
typedef MBCC::PolyBase<Statement_Base,VariableDeclaration,Import,Selection> Base;
typedef Statement_Base BASE;
using Base::Base;
template<typename T>
Statement& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

};
class Token :  public MBCC::AST_Base
{
public:
MBCC::TokenPosition Position;
std::string Value;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal_Base :  public MBCC::AST_Base
{
public:
MBCC::TokenPosition ValuePosition;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal : public MBCC::PolyBase<Literal_Base,Literal_String,Literal_Symbol,Literal_PosRef,Literal_Number>
{
public:
typedef MBCC::PolyBase<Literal_Base,Literal_String,Literal_Symbol,Literal_PosRef,Literal_Number> Base;
typedef Literal_Base BASE;
using Base::Base;
template<typename T>
Literal& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

};
class Result_Base :  public MBCC::AST_Base
{
public:
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Result : public MBCC::PolyBase<Result_Base,Result_Record,Result_Print,Result_Tabulate>
{
public:
typedef MBCC::PolyBase<Result_Base,Result_Record,Result_Print,Result_Tabulate> Base;
typedef Result_Base BASE;
using Base::Base;
template<typename T>
Result& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

};
class Filter_Component_Base :  public MBCC::AST_Base
{
public:
MBCC::TokenPosition Begin;
MBCC::TokenPosition End;
bool Negated =  false;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Filter_Component : public MBCC::PolyBase<Filter_Component_Base,Filter_Component_Func,Filter_Component_Literal,Filter_OperatorList>
{
public:
typedef MBCC::PolyBase<Filter_Component_Base,Filter_Component_Func,Filter_Component_Literal,Filter_OperatorList> Base;
typedef Filter_Component_Base BASE;
using Base::Base;
template<typename T>
Filter_Component& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

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
class GameInfoPredicate_Data : public MBCC::PolyBase<GameInfoPredicate_Data_Base,GameInfoPredicate_Direct>
{
public:
typedef MBCC::PolyBase<GameInfoPredicate_Data_Base,GameInfoPredicate_Direct> Base;
typedef GameInfoPredicate_Data_Base BASE;
using Base::Base;
template<typename T>
GameInfoPredicate_Data& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

};
class Filter_Arg_Base :  public MBCC::AST_Base
{
public:
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Filter_Arg : public MBCC::PolyBase<Filter_Arg_Base,Filter_Arg_Positional,Filter_Arg_Named>
{
public:
typedef MBCC::PolyBase<Filter_Arg_Base,Filter_Arg_Positional,Filter_Arg_Named> Base;
typedef Filter_Arg_Base BASE;
using Base::Base;
template<typename T>
Filter_Arg& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

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
class VariableDeclaration : public MBCC::PolyBase<VariableDeclaration_Base,VariableDeclaration_Filter,VariableDeclaration_GameList,VariableDeclaration_PlayerSelection,VariableDeclaration_GameInfoPredicate>
{
public:
typedef MBCC::PolyBase<VariableDeclaration_Base,VariableDeclaration_Filter,VariableDeclaration_GameList,VariableDeclaration_PlayerSelection,VariableDeclaration_GameInfoPredicate> Base;
typedef VariableDeclaration_Base BASE;
using Base::Base;
template<typename T>
VariableDeclaration& operator=(T&& ObjectToStore)
{
static_cast<Base&>(*this) = std::forward<T>(ObjectToStore);
return *this;
}

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
class Result_Record : public Result_Base
{
public:
MBCC::TokenPosition RecordPosition;
MBCC::TokenPosition FilePosition;
std::string OutFile;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Identifier :  public MBCC::AST_Base
{
public:
std::vector<Token> Parts;
MBCC::TokenPosition End;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Result_Print : public Result_Base
{
public:
MBCC::TokenPosition PrintPosition;
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
MBCC::TokenPosition End;
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
class Filter_Arg_Positional : public Filter_Arg_Base
{
public:
Filter_Component Argument;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal_String : public Literal_Base
{
public:
std::string Value;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal_Symbol : public Literal_Base
{
public:
std::string Value;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal_PosRef : public Literal_Base
{
public:
int Pos =  0;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class Literal_Number : public Literal_Base
{
public:
int Value;
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
class Filter_Component_Literal : public Filter_Component_Base
{
public:
Literal Value;
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
MBCC::TokenPosition Begin;
MBCC::TokenPosition End;
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
MBCC::TokenPosition End;
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
class PlayerAssignment :  public MBCC::AST_Base
{
public:
MBCC::TokenPosition WithPosition;
MBCC::TokenPosition PlayerPosition;
std::string AffectedPlayer;
GameInfoPredicate PlayerCondition;
std::unique_ptr<AST_Base> Copy() const override{return(MBCC::CopyAST(*this));}

};
class GameInfoPredicate_OperatorList :  public MBCC::AST_Base
{
public:
std::vector<GameInfoPredicate> Terms;
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
template<> inline int MBCC::GetTypeBegin<Statement>(){return(0);}
template<> inline int MBCC::GetTypeEnd<Statement>(){return(8);}
template<> inline int MBCC::GetTypeBegin<VariableDeclaration>(){return(1);}
template<> inline int MBCC::GetTypeEnd<VariableDeclaration>(){return(6);}
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
template<> inline int MBCC::GetTypeBegin<GameInfoPredicate_Data>(){return(16);}
template<> inline int MBCC::GetTypeEnd<GameInfoPredicate_Data>(){return(18);}
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
template<> inline int MBCC::GetTypeBegin<Filter_Arg>(){return(22);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg>(){return(25);}
template<> inline int MBCC::GetTypeBegin<Literal>(){return(25);}
template<> inline int MBCC::GetTypeEnd<Literal>(){return(30);}
template<> inline int MBCC::GetTypeBegin<Literal_String>(){return(26);}
template<> inline int MBCC::GetTypeEnd<Literal_String>(){return(27);}
template<> inline int MBCC::GetTypeBegin<Literal_Symbol>(){return(27);}
template<> inline int MBCC::GetTypeEnd<Literal_Symbol>(){return(28);}
template<> inline int MBCC::GetTypeBegin<Literal_PosRef>(){return(28);}
template<> inline int MBCC::GetTypeEnd<Literal_PosRef>(){return(29);}
template<> inline int MBCC::GetTypeBegin<Literal_Number>(){return(29);}
template<> inline int MBCC::GetTypeEnd<Literal_Number>(){return(30);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Positional>(){return(23);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Positional>(){return(24);}
template<> inline int MBCC::GetTypeBegin<Filter_Arg_Named>(){return(24);}
template<> inline int MBCC::GetTypeEnd<Filter_Arg_Named>(){return(25);}
template<> inline int MBCC::GetTypeBegin<AttributeList>(){return(30);}
template<> inline int MBCC::GetTypeEnd<AttributeList>(){return(31);}
template<> inline int MBCC::GetTypeBegin<Filter_Component>(){return(31);}
template<> inline int MBCC::GetTypeEnd<Filter_Component>(){return(35);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Func>(){return(32);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Func>(){return(33);}
template<> inline int MBCC::GetTypeBegin<Filter_Component_Literal>(){return(33);}
template<> inline int MBCC::GetTypeEnd<Filter_Component_Literal>(){return(34);}
template<> inline int MBCC::GetTypeBegin<Filter_OperatorList>(){return(34);}
template<> inline int MBCC::GetTypeEnd<Filter_OperatorList>(){return(35);}
template<> inline int MBCC::GetTypeBegin<Filter>(){return(35);}
template<> inline int MBCC::GetTypeEnd<Filter>(){return(36);}
template<> inline int MBCC::GetTypeBegin<Result>(){return(36);}
template<> inline int MBCC::GetTypeEnd<Result>(){return(40);}
template<> inline int MBCC::GetTypeBegin<Result_Record>(){return(37);}
template<> inline int MBCC::GetTypeEnd<Result_Record>(){return(38);}
template<> inline int MBCC::GetTypeBegin<Result_Print>(){return(38);}
template<> inline int MBCC::GetTypeEnd<Result_Print>(){return(39);}
template<> inline int MBCC::GetTypeBegin<ColumnSpec>(){return(40);}
template<> inline int MBCC::GetTypeEnd<ColumnSpec>(){return(41);}
template<> inline int MBCC::GetTypeBegin<Result_Tabulate>(){return(39);}
template<> inline int MBCC::GetTypeEnd<Result_Tabulate>(){return(40);}
template<> inline int MBCC::GetTypeBegin<ImportBinding>(){return(41);}
template<> inline int MBCC::GetTypeEnd<ImportBinding>(){return(42);}
template<> inline int MBCC::GetTypeBegin<Import>(){return(6);}
template<> inline int MBCC::GetTypeEnd<Import>(){return(7);}
template<> inline int MBCC::GetTypeBegin<Selection>(){return(7);}
template<> inline int MBCC::GetTypeEnd<Selection>(){return(8);}
Operator ParseComparator(MBCC::Tokenizer& Tokenizer);
void FillComparator(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_0(MBCC::Tokenizer& Tokenizer);
void FillComparator_0(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_1(MBCC::Tokenizer& Tokenizer);
void FillComparator_1(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_2(MBCC::Tokenizer& Tokenizer);
void FillComparator_2(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_3(MBCC::Tokenizer& Tokenizer);
void FillComparator_3(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_4(MBCC::Tokenizer& Tokenizer);
void FillComparator_4(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_5(MBCC::Tokenizer& Tokenizer);
void FillComparator_5(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseComparator_6(MBCC::Tokenizer& Tokenizer);
void FillComparator_6(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator(MBCC::Tokenizer& Tokenizer);
void FillExprOperator(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_0(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_0(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_1(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_1(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_2(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_2(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_3(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_3(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_4(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_4(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_5(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_5(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_6(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_6(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_7(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_7(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_8(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_8(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_9(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_9(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_10(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_10(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseExprOperator_11(MBCC::Tokenizer& Tokenizer);
void FillExprOperator_11(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer);
void FillCombinator(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_0(MBCC::Tokenizer& Tokenizer);
void FillCombinator_0(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_1(MBCC::Tokenizer& Tokenizer);
void FillCombinator_1(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Operator ParseCombinator_2(MBCC::Tokenizer& Tokenizer);
void FillCombinator_2(Operator& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg(Filter_Arg& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_0(Filter_Arg& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_1(Filter_Arg& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_Named(Filter_Arg_Named& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg_Named ParseFilter_Arg_Named_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_Named_0(Filter_Arg_Named& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_Positional(Filter_Arg_Positional& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Arg_Positional ParseFilter_Arg_Positional_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Arg_Positional_0(Filter_Arg_Positional& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer);
void FillFilter_ArgList(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_ArgList_0(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L1(MBCC::Tokenizer& Tokenizer);
void Fill_L1(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L1_0(MBCC::Tokenizer& Tokenizer);
void Fill_L1_0(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L2(MBCC::Tokenizer& Tokenizer);
void Fill_L2(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_ArgList Parse_L2_0(MBCC::Tokenizer& Tokenizer);
void Fill_L2_0(Filter_ArgList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral(MBCC::Tokenizer& Tokenizer);
void FillLiteral(Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_0(MBCC::Tokenizer& Tokenizer);
void FillLiteral_0(Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_1(MBCC::Tokenizer& Tokenizer);
void FillLiteral_1(Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal ParseLiteral_2(MBCC::Tokenizer& Tokenizer);
void FillLiteral_2(Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_String ParseLiteral_String(MBCC::Tokenizer& Tokenizer);
void FillLiteral_String(Literal_String& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_String ParseLiteral_String_0(MBCC::Tokenizer& Tokenizer);
void FillLiteral_String_0(Literal_String& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_Symbol ParseLiteral_Symbol(MBCC::Tokenizer& Tokenizer);
void FillLiteral_Symbol(Literal_Symbol& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_Symbol ParseLiteral_Symbol_0(MBCC::Tokenizer& Tokenizer);
void FillLiteral_Symbol_0(Literal_Symbol& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_Number ParseLiteral_Number(MBCC::Tokenizer& Tokenizer);
void FillLiteral_Number(Literal_Number& ReturnValue,MBCC::Tokenizer& Tokenizer);
Literal_Number ParseLiteral_Number_0(MBCC::Tokenizer& Tokenizer);
void FillLiteral_Number_0(Literal_Number& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier2(MBCC::Tokenizer& Tokenizer);
void FillIdentifier2(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier2_0(MBCC::Tokenizer& Tokenizer);
void FillIdentifier2_0(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier Parse_L3(MBCC::Tokenizer& Tokenizer);
void Fill_L3(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier Parse_L3_0(MBCC::Tokenizer& Tokenizer);
void Fill_L3_0(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Token ParseToken2(MBCC::Tokenizer& Tokenizer);
void FillToken2(Token& ReturnValue,MBCC::Tokenizer& Tokenizer);
Token ParseToken2_0(MBCC::Tokenizer& Tokenizer);
void FillToken2_0(Token& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component_Func ParseFilter_Component_Func(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component_Func(Filter_Component_Func& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component_Func ParseFilter_Component_Func_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component_Func_0(Filter_Component_Func& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term(MBCC::Tokenizer& Tokenizer);
void FillFilter_Term(Filter_Component& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Term_0(Filter_Component& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_1(MBCC::Tokenizer& Tokenizer);
void FillFilter_Term_1(Filter_Component& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_2(MBCC::Tokenizer& Tokenizer);
void FillFilter_Term_2(Filter_Component& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Term_3(MBCC::Tokenizer& Tokenizer);
void FillFilter_Term_3(Filter_Component& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component_Literal ParseFilter_Component_Literal(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component_Literal(Filter_Component_Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_Component_Literal ParseFilter_Component_Literal_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component_Literal_0(Filter_Component_Literal& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_Component(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component(Filter_OperatorList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_Component_0(Filter_OperatorList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_OperatorList Parse_L4(MBCC::Tokenizer& Tokenizer);
void Fill_L4(Filter_OperatorList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter_OperatorList Parse_L4_0(MBCC::Tokenizer& Tokenizer);
void Fill_L4_0(Filter_OperatorList& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter ParseFilter(MBCC::Tokenizer& Tokenizer);
void FillFilter(Filter& ReturnValue,MBCC::Tokenizer& Tokenizer);
Filter ParseFilter_0(MBCC::Tokenizer& Tokenizer);
void FillFilter_0(Filter& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Or(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Or_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_And(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_And_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Direct(GameInfoPredicate_Direct& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Direct_0(GameInfoPredicate_Direct& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Base(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Base_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicateOr(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicateOr(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicateOr_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicateOr_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Term(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Term_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_1(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Term_1(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Term_2(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_Term_2(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer);
void FillGameInfoPredicate_0(GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameSelection ParseGameSelection(MBCC::Tokenizer& Tokenizer);
void FillGameSelection(GameSelection& ReturnValue,MBCC::Tokenizer& Tokenizer);
GameSelection ParseGameSelection_0(MBCC::Tokenizer& Tokenizer);
void FillGameSelection_0(GameSelection& ReturnValue,MBCC::Tokenizer& Tokenizer);
PlayerAssignment ParsePlayerAssignment(MBCC::Tokenizer& Tokenizer);
void FillPlayerAssignment(PlayerAssignment& ReturnValue,MBCC::Tokenizer& Tokenizer);
PlayerAssignment ParsePlayerAssignment_0(MBCC::Tokenizer& Tokenizer);
void FillPlayerAssignment_0(PlayerAssignment& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Record ParseResult_Record(MBCC::Tokenizer& Tokenizer);
void FillResult_Record(Result_Record& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Record ParseResult_Record_0(MBCC::Tokenizer& Tokenizer);
void FillResult_Record_0(Result_Record& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Print ParseResult_Print(MBCC::Tokenizer& Tokenizer);
void FillResult_Print(Result_Print& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Print ParseResult_Print_0(MBCC::Tokenizer& Tokenizer);
void FillResult_Print_0(Result_Print& ReturnValue,MBCC::Tokenizer& Tokenizer);
ColumnSpec ParseColumnSpec(MBCC::Tokenizer& Tokenizer);
void FillColumnSpec(ColumnSpec& ReturnValue,MBCC::Tokenizer& Tokenizer);
ColumnSpec ParseColumnSpec_0(MBCC::Tokenizer& Tokenizer);
void FillColumnSpec_0(ColumnSpec& ReturnValue,MBCC::Tokenizer& Tokenizer);
ColumnSpec Parse_L5(MBCC::Tokenizer& Tokenizer);
void Fill_L5(ColumnSpec& ReturnValue,MBCC::Tokenizer& Tokenizer);
ColumnSpec Parse_L5_0(MBCC::Tokenizer& Tokenizer);
void Fill_L5_0(ColumnSpec& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate ParseResult_Tabulate(MBCC::Tokenizer& Tokenizer);
void FillResult_Tabulate(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate ParseResult_Tabulate_0(MBCC::Tokenizer& Tokenizer);
void FillResult_Tabulate_0(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L6(MBCC::Tokenizer& Tokenizer);
void Fill_L6(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L6_0(MBCC::Tokenizer& Tokenizer);
void Fill_L6_0(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L7(MBCC::Tokenizer& Tokenizer);
void Fill_L7(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result_Tabulate Parse_L7_0(MBCC::Tokenizer& Tokenizer);
void Fill_L7_0(Result_Tabulate& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result ParseResult(MBCC::Tokenizer& Tokenizer);
void FillResult(Result& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result ParseResult_0(MBCC::Tokenizer& Tokenizer);
void FillResult_0(Result& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result ParseResult_1(MBCC::Tokenizer& Tokenizer);
void FillResult_1(Result& ReturnValue,MBCC::Tokenizer& Tokenizer);
Result ParseResult_2(MBCC::Tokenizer& Tokenizer);
void FillResult_2(Result& ReturnValue,MBCC::Tokenizer& Tokenizer);
Selection ParseSelection(MBCC::Tokenizer& Tokenizer);
void FillSelection(Selection& ReturnValue,MBCC::Tokenizer& Tokenizer);
Selection ParseSelection_0(MBCC::Tokenizer& Tokenizer);
void FillSelection_0(Selection& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_Filter ParseVariableDeclaration_Filter(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_Filter(VariableDeclaration_Filter& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_Filter ParseVariableDeclaration_Filter_0(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_Filter_0(VariableDeclaration_Filter& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameList ParseVariableDeclaration_GameList(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_GameList(VariableDeclaration_GameList& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameList ParseVariableDeclaration_GameList_0(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_GameList_0(VariableDeclaration_GameList& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_PlayerSelection(VariableDeclaration_PlayerSelection& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection_0(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_PlayerSelection_0(VariableDeclaration_PlayerSelection& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_GameInfoPredicate(VariableDeclaration_GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate_0(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_GameInfoPredicate_0(VariableDeclaration_GameInfoPredicate& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration(VariableDeclaration& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_0(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_0(VariableDeclaration& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_1(VariableDeclaration& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_2(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_2(VariableDeclaration& ReturnValue,MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_3(MBCC::Tokenizer& Tokenizer);
void FillVariableDeclaration_3(VariableDeclaration& ReturnValue,MBCC::Tokenizer& Tokenizer);
Module ParseModule(MBCC::Tokenizer& Tokenizer);
void FillModule(Module& ReturnValue,MBCC::Tokenizer& Tokenizer);
Module ParseModule_0(MBCC::Tokenizer& Tokenizer);
void FillModule_0(Module& ReturnValue,MBCC::Tokenizer& Tokenizer);
Module ParseSlugma(MBCC::Tokenizer& Tokenizer);
void FillSlugma(Module& ReturnValue,MBCC::Tokenizer& Tokenizer);
Module ParseSlugma_0(MBCC::Tokenizer& Tokenizer);
void FillSlugma_0(Module& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier(MBCC::Tokenizer& Tokenizer);
void FillIdentifier(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier ParseIdentifier_0(MBCC::Tokenizer& Tokenizer);
void FillIdentifier_0(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier Parse_L8(MBCC::Tokenizer& Tokenizer);
void Fill_L8(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Identifier Parse_L8_0(MBCC::Tokenizer& Tokenizer);
void Fill_L8_0(Identifier& ReturnValue,MBCC::Tokenizer& Tokenizer);
Token ParseToken(MBCC::Tokenizer& Tokenizer);
void FillToken(Token& ReturnValue,MBCC::Tokenizer& Tokenizer);
Token ParseToken_0(MBCC::Tokenizer& Tokenizer);
void FillToken_0(Token& ReturnValue,MBCC::Tokenizer& Tokenizer);
UsingDirective ParseUsingDirective(MBCC::Tokenizer& Tokenizer);
void FillUsingDirective(UsingDirective& ReturnValue,MBCC::Tokenizer& Tokenizer);
UsingDirective ParseUsingDirective_0(MBCC::Tokenizer& Tokenizer);
void FillUsingDirective_0(UsingDirective& ReturnValue,MBCC::Tokenizer& Tokenizer);
UsingDirective Parse_L9(MBCC::Tokenizer& Tokenizer);
void Fill_L9(UsingDirective& ReturnValue,MBCC::Tokenizer& Tokenizer);
UsingDirective Parse_L9_0(MBCC::Tokenizer& Tokenizer);
void Fill_L9_0(UsingDirective& ReturnValue,MBCC::Tokenizer& Tokenizer);
ImportBinding ParseImportBinding(MBCC::Tokenizer& Tokenizer);
void FillImportBinding(ImportBinding& ReturnValue,MBCC::Tokenizer& Tokenizer);
ImportBinding ParseImportBinding_0(MBCC::Tokenizer& Tokenizer);
void FillImportBinding_0(ImportBinding& ReturnValue,MBCC::Tokenizer& Tokenizer);
Import ParseImport(MBCC::Tokenizer& Tokenizer);
void FillImport(Import& ReturnValue,MBCC::Tokenizer& Tokenizer);
Import ParseImport_0(MBCC::Tokenizer& Tokenizer);
void FillImport_0(Import& ReturnValue,MBCC::Tokenizer& Tokenizer);
Statement ParseStatement(MBCC::Tokenizer& Tokenizer);
void FillStatement(Statement& ReturnValue,MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer);
void FillStatement_0(Statement& ReturnValue,MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer);
void FillStatement_1(Statement& ReturnValue,MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_2(MBCC::Tokenizer& Tokenizer);
void FillStatement_2(Statement& ReturnValue,MBCC::Tokenizer& Tokenizer);
inline MBCC::Tokenizer GetTokenizer()
{
MBCC::Tokenizer ReturnValue("((( |\\t|\\n|\\r)*)|(#.*\\n))*",{"\\(","\\)","\\{","\\}","\\[","\\]","[[:digit:]]+","$\"((\\\\.|[^\"\\\\])*)\"","true|false","WITH","IMPORT","USING","AS","SELECT","RECORD","Games","GamePredicate","PlayerSelection","Filter","PRINT","TABULATE","INTO","$\\$([[:alnum:]_]+)","[[:alpha:]_]+[[:alnum:]_]*",";","<=","<","!",">=",">","!=","\\+","-","\\*","/",":",",","==","=","\\.","\\|\\|","\\|","&&","&","\\?",});
return(ReturnValue);
}