#include "SlippiSpecParser.h"
const bool LOOKTable[41][1][26] = {{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,true,true,true,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,true,true,true,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,true,true,true,true,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,true,true,true,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,true,true,true,true,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,true,true,true,true,true,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,true,true,true,true,true,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,true,true,true,true,true,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,true,true,true,true,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},};
Operator ParseOperator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[1][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_0(Tokenizer);
    }
    else if (LOOKTable[2][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_1(Tokenizer);
    }
    else if (LOOKTable[3][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_2(Tokenizer);
    }
    else if (LOOKTable[4][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_3(Tokenizer);
    }
    else if (LOOKTable[5][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_4(Tokenizer);
    }
    else if (LOOKTable[6][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_5(Tokenizer);
    }
    else if (LOOKTable[7][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_6(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","Operator");
    }
    return(ReturnValue);
}
Operator ParseOperator_0(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","less");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_1(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","pipe");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_2(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","AND");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_3(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 20)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","OR");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_4(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","add");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_5(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","great");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_6(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 19)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","eq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if (LOOKTable[9][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg_0(Tokenizer);
    }
    else if (LOOKTable[10][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg");
    }
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(LOOKTable[11][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg_Named(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Named");
    }
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(LOOKTable[13][0][Tokenizer.Peek().Type])
    {
        ParseFilter_Arg_Positional(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Positional");
    }
    return(ReturnValue);
}
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Named ReturnValue;
    ReturnValue = ParseFilter_Arg_Named_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Named ParseFilter_Arg_Named_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Named ReturnValue;
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 8)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Positional ReturnValue;
    ReturnValue = ParseFilter_Arg_Positional_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Positional ParseFilter_Arg_Positional_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Positional ReturnValue;
    if(Tokenizer.Peek().Type != 8)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer)
{
    Filter_ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_0(Tokenizer);
    return(ReturnValue);
}
Filter_ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_ArgList ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
    }
    Tokenizer.ConsumeToken();
    while(LOOKTable[8][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Arguments.push_back(ParseFilter_Arg(Tokenizer));
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Conjunction(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_Conjunction_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Conjunction_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    std::string Operator;
    if(LOOKTable[0][0][Tokenizer.Peek().Type])
    {
        Operator = ParseOperator(Tokenizer).Op;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Conjunction","Operator");
    }
    if(LOOKTable[22][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Conjunction","Filter_Component");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if (LOOKTable[20][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Base_0(Tokenizer);
    }
    else if (LOOKTable[21][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Base_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","Filter_Component_Base");
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[22][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","Filter_Component");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","rpar");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","idf");
    }
    ReturnValue.FilterName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[15][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ArgumentList = ParseFilter_ArgList(Tokenizer);
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(LOOKTable[19][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component_Base");
    }
    std::vector<Filter_Component> ExtraTerms;
    while(LOOKTable[17][0][Tokenizer.Peek().Type])
    {
        ExtraTerms.push_back(ParseFilter_Component_Conjunction(Tokenizer));
    }
    ReturnValue.ExtraTerms = std::move(ExtraTerms);
    return(ReturnValue);
}
Filter ParseFilter(MBCC::Tokenizer& Tokenizer)
{
    Filter ReturnValue;
    ReturnValue = ParseFilter_0(Tokenizer);
    return(ReturnValue);
}
Filter ParseFilter_0(MBCC::Tokenizer& Tokenizer)
{
    Filter ReturnValue;
    if(Tokenizer.Peek().Type != 13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[22][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Component = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Conjunction(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Conjunction_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Conjunction_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    std::string Operator;
    if(LOOKTable[0][0][Tokenizer.Peek().Type])
    {
        Operator = ParseOperator(Tokenizer).Op;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Conjunction","Operator");
    }
    if(LOOKTable[31][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Conjunction","GameInfoPredicate");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if (LOOKTable[29][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Base_0(Tokenizer);
    }
    else if (LOOKTable[30][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Base_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","GameInfoPredicate_Base");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[31][0][Tokenizer.Peek().Type])
    {
        ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","GameInfoPredicate");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","rpar");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    while(Tokenizer.Peek().Type == 14)
    {
        ReturnValue.Attribute.push_back(Tokenizer.Peek().Value);
        Tokenizer.ConsumeToken();
        
    }
    if(LOOKTable[0][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Operator = ParseOperator(Tokenizer).Op;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","Operator");
    }
    if(Tokenizer.Peek().Type != 8)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(LOOKTable[28][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate_Base");
    }
    std::vector<GameInfoPredicate> ExtraTerms;
    while(LOOKTable[26][0][Tokenizer.Peek().Type])
    {
        ExtraTerms.push_back(ParseGameInfoPredicate_Conjunction(Tokenizer));
    }
    ReturnValue.ExtraTerms = std::move(ExtraTerms);
    return(ReturnValue);
}
GameSelection ParseGameSelection(MBCC::Tokenizer& Tokenizer)
{
    GameSelection ReturnValue;
    ReturnValue = ParseGameSelection_0(Tokenizer);
    return(ReturnValue);
}
GameSelection ParseGameSelection_0(MBCC::Tokenizer& Tokenizer)
{
    GameSelection ReturnValue;
    if(Tokenizer.Peek().Type != 11)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[31][0][Tokenizer.Peek().Type])
    {
        ReturnValue.GameCondition = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","GameInfoPredicate");
    }
    return(ReturnValue);
}
PlayerAssignment ParsePlayerAssignment(MBCC::Tokenizer& Tokenizer)
{
    PlayerAssignment ReturnValue;
    ReturnValue = ParsePlayerAssignment_0(Tokenizer);
    return(ReturnValue);
}
PlayerAssignment ParsePlayerAssignment_0(MBCC::Tokenizer& Tokenizer)
{
    PlayerAssignment ReturnValue;
    if(Tokenizer.Peek().Type != 10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","WITH");
    }
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[31][0][Tokenizer.Peek().Type])
    {
        ReturnValue.PlayerCondition = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
    }
    return(ReturnValue);
}
Result_Record ParseResult(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    ReturnValue = ParseResult_0(Tokenizer);
    return(ReturnValue);
}
Result_Record ParseResult_0(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    if(Tokenizer.Peek().Type != 12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","RECORD");
    }
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 8)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","string");
    }
    ReturnValue.OutFile = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
SlippiSpec ParseSlippiSpec(MBCC::Tokenizer& Tokenizer)
{
    SlippiSpec ReturnValue;
    ReturnValue = ParseSlippiSpec_0(Tokenizer);
    return(ReturnValue);
}
SlippiSpec ParseSlippiSpec_0(MBCC::Tokenizer& Tokenizer)
{
    SlippiSpec ReturnValue;
    if(LOOKTable[35][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Assignment = ParsePlayerAssignment(Tokenizer);
    }
    if(LOOKTable[33][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Games = ParseGameSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"SlippiSpec","GameSelection");
    }
    if(LOOKTable[24][0][Tokenizer.Peek().Type])
    {
        ReturnValue.SituationFilter = ParseFilter(Tokenizer);
    }
    if(LOOKTable[37][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Output = ParseResult(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"SlippiSpec","Result");
    }
    return(ReturnValue);
}
