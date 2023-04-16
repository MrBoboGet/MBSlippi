#include "SlippiSpecParser.h"
const bool LOOKTable[95][1][33] = {{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,true,false,false,true,false,true,true,true,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},};
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
    else if (LOOKTable[8][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_7(Tokenizer);
    }
    else if (LOOKTable[9][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_8(Tokenizer);
    }
    else if (LOOKTable[10][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseOperator_9(Tokenizer);
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
    if(Tokenizer.Peek().Type != 19)
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
    if(Tokenizer.Peek().Type != 28)
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
    if(Tokenizer.Peek().Type != 29)
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
    if(Tokenizer.Peek().Type != 27)
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
    if(Tokenizer.Peek().Type != 30)
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
    if(Tokenizer.Peek().Type != 21)
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
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","eq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_7(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","leq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_8(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 20)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","geq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_9(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","neq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_Extra(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    ReturnValue = ParseFilter_Arg_Extra_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_Extra_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(Tokenizer.Peek().Type != 24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Extra","comma");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[13][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Extra","Filter_Arg");
    }
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if (LOOKTable[14][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg_0(Tokenizer);
    }
    else if (LOOKTable[15][0][Tokenizer.Peek().Type])
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
    if(LOOKTable[16][0][Tokenizer.Peek().Type])
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
    if(LOOKTable[18][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Arg_Positional(Tokenizer);
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
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","eq");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
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
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_Arguments(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_Arguments_0(Tokenizer);
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_Arguments_0(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    if(LOOKTable[13][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Args.push_back(ParseFilter_Arg(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList_Arguments","Filter_Arg");
    }
    while(LOOKTable[11][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Args.push_back(ParseFilter_Arg_Extra(Tokenizer));
    }
    return(ReturnValue);
}
ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_0(Tokenizer);
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[20][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_ArgList_Arguments(Tokenizer);
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_And(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_And_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_And_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_And","add");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_And","Filter_Component");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Or(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_Or_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Or_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Or","pipe");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Or","Filter_Component");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    if (LOOKTable[29][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_OperatorList_0(Tokenizer);
    }
    else if (LOOKTable[30][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_OperatorList_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_OperatorList","Filter_OperatorList");
    }
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    do
    {
        ReturnValue.Components.push_back(ParseFilter_Component_And(Tokenizer));
    }
    while(LOOKTable[24][0][Tokenizer.Peek().Type]);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    do
    {
        ReturnValue.Components.push_back(ParseFilter_Component_Or(Tokenizer));
    }
    while(LOOKTable[26][0][Tokenizer.Peek().Type]);
    return(ReturnValue);
}
Filter_Component_Function ParseFilter_Component_Function(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Function ReturnValue;
    ReturnValue = ParseFilter_Component_Function_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Function ParseFilter_Component_Function_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Function ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Function","idf");
    }
    ReturnValue.FilterName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component_Variable ParseFilter_Component_Variable(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Variable ReturnValue;
    ReturnValue = ParseFilter_Component_Variable_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Variable ParseFilter_Component_Variable_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Variable ReturnValue;
    ReturnValue.VariablePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Variable","variable");
    }
    ReturnValue.VariableName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component_Data ParseFilter_Component_Data(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Data ReturnValue;
    if (LOOKTable[36][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Data_0(Tokenizer);
    }
    else if (LOOKTable[37][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Data_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Data","Filter_Component_Data");
    }
    return(ReturnValue);
}
Filter_Component_Data ParseFilter_Component_Data_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Data ReturnValue;
    if(LOOKTable[31][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Function(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Data","Filter_Component_Function");
    }
    return(ReturnValue);
}
Filter_Component_Data ParseFilter_Component_Data_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Data ReturnValue;
    if(LOOKTable[33][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Variable(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Data","Filter_Component_Variable");
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_Base_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(LOOKTable[35][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Data = ParseFilter_Component_Data(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","Filter_Component_Data");
    }
    if(LOOKTable[22][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ArgumentList.Arguments = ParseFilter_ArgList(Tokenizer).Args;
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if (LOOKTable[41][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_0(Tokenizer);
    }
    else if (LOOKTable[42][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseFilter_Component(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","rpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[28][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ExtraTerms = ParseFilter_OperatorList(Tokenizer).Components;
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(LOOKTable[38][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseFilter_Component_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component_Base");
    }
    std::vector<Filter_Component> ExtraTerms;
    if(LOOKTable[28][0][Tokenizer.Peek().Type])
    {
        ExtraTerms = ParseFilter_OperatorList(Tokenizer).Components;
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
    ReturnValue.FilterPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Component = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
    }
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Base(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue = ParseAttribute_Base_0(Tokenizer);
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Base_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Base","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Extension(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue = ParseAttribute_Extension_0(Tokenizer);
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Extension_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    if(Tokenizer.Peek().Type != 26)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Extension","dot");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Extension","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
AttributeList ParseAttribute_List(MBCC::Tokenizer& Tokenizer)
{
    AttributeList ReturnValue;
    ReturnValue = ParseAttribute_List_0(Tokenizer);
    return(ReturnValue);
}
AttributeList ParseAttribute_List_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeList ReturnValue;
    if(LOOKTable[45][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Attributes.push_back(ParseAttribute_Base(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_List","Attribute_Base");
    }
    while(LOOKTable[47][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Attributes.push_back(ParseAttribute_Extension(Tokenizer));
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Or_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Or_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","OR");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[67][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","GameInfoPredicate");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_And_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_And_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 29)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","AND");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[67][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","GameInfoPredicate");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    if (LOOKTable[56][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_OperatorList_0(Tokenizer);
    }
    else if (LOOKTable[57][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_OperatorList_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_OperatorList","GameInfoPredicate_OperatorList");
    }
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    do
    {
        ReturnValue.Terms.push_back(ParseGameInfoPredicate_Or(Tokenizer));
    }
    while(LOOKTable[51][0][Tokenizer.Peek().Type]);
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    do
    {
        ReturnValue.Terms.push_back(ParseGameInfoPredicate_And(Tokenizer));
    }
    while(LOOKTable[53][0][Tokenizer.Peek().Type]);
    return(ReturnValue);
}
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Variable ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Variable_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Variable ReturnValue;
    ReturnValue.VariablePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Variable","variable");
    }
    ReturnValue.VariableName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Direct ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Direct_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Direct ReturnValue;
    if(LOOKTable[49][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Attribute = ParseAttribute_List(Tokenizer).Attributes;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Attribute_List");
    }
    if(LOOKTable[0][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Comparison = ParseOperator(Tokenizer).Op;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Operator");
    }
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if (LOOKTable[63][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Data_0(Tokenizer);
    }
    else if (LOOKTable[64][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Data_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Data");
    }
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if(LOOKTable[58][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Variable(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Variable");
    }
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if(LOOKTable[60][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Direct(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Direct");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Base_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(LOOKTable[62][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Data = ParseGameInfoPredicate_Data(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","GameInfoPredicate_Data");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if (LOOKTable[68][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_0(Tokenizer);
    }
    else if (LOOKTable[69][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[67][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicate(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","rpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[55][0][Tokenizer.Peek().Type])
    {
        ReturnValue.ExtraTerms = ParseGameInfoPredicate_OperatorList(Tokenizer).Terms;
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(LOOKTable[65][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseGameInfoPredicate_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate_Base");
    }
    std::vector<GameInfoPredicate> ExtraTerms;
    if(LOOKTable[55][0][Tokenizer.Peek().Type])
    {
        ExtraTerms = ParseGameInfoPredicate_OperatorList(Tokenizer).Terms;
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
    if(LOOKTable[72][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Assignment = ParsePlayerAssignment(Tokenizer);
    }
    ReturnValue.SelectPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[67][0][Tokenizer.Peek().Type])
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
    ReturnValue.WithPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 9)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","WITH");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.PlayerPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[67][0][Tokenizer.Peek().Type])
    {
        ReturnValue.PlayerCondition = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
    }
    return(ReturnValue);
}
Result_Record ParseResult_Record(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    ReturnValue = ParseResult_Record_0(Tokenizer);
    return(ReturnValue);
}
Result_Record ParseResult_Record_0(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    ReturnValue.RecordPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 11)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","RECORD");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.FilePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","string");
    }
    ReturnValue.OutFile = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Result_Print ParseResult_Print(MBCC::Tokenizer& Tokenizer)
{
    Result_Print ReturnValue;
    ReturnValue = ParseResult_Print_0(Tokenizer);
    return(ReturnValue);
}
Result_Print ParseResult_Print_0(MBCC::Tokenizer& Tokenizer)
{
    Result_Print ReturnValue;
    ReturnValue.PrintPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Print","PRINT");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Result ParseResult(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if (LOOKTable[79][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseResult_0(Tokenizer);
    }
    else if (LOOKTable[80][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseResult_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result");
    }
    return(ReturnValue);
}
Result ParseResult_0(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if(LOOKTable[74][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseResult_Record(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Record");
    }
    return(ReturnValue);
}
Result ParseResult_1(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if(LOOKTable[76][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseResult_Print(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Print");
    }
    return(ReturnValue);
}
Selection ParseSelection(MBCC::Tokenizer& Tokenizer)
{
    Selection ReturnValue;
    ReturnValue = ParseSelection_0(Tokenizer);
    return(ReturnValue);
}
Selection ParseSelection_0(MBCC::Tokenizer& Tokenizer)
{
    Selection ReturnValue;
    if(LOOKTable[70][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Games = ParseGameSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","GameSelection");
    }
    if(LOOKTable[43][0][Tokenizer.Peek().Type])
    {
        ReturnValue.SituationFilter = ParseFilter(Tokenizer);
    }
    if(LOOKTable[78][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Output = ParseResult(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","Result");
    }
    if(Tokenizer.Peek().Type != 17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","semi");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_Filter ParseVariableDeclaration_Filter(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_Filter ReturnValue;
    ReturnValue = ParseVariableDeclaration_Filter_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_Filter ParseVariableDeclaration_Filter_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_Filter ReturnValue;
    ReturnValue.FilterPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","FILTER");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Component = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_Component");
    }
    if(Tokenizer.Peek().Type != 17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","semi");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_GameList ParseVariableDeclaration_GameList(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameList ReturnValue;
    ReturnValue = ParseVariableDeclaration_GameList_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_GameList ParseVariableDeclaration_GameList_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameList ReturnValue;
    ReturnValue.GamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GAMES");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[70][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Selection = ParseGameSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GameSelection");
    }
    if(Tokenizer.Peek().Type != 17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","semi");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if (LOOKTable[88][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseVariableDeclaration_0(Tokenizer);
    }
    else if (LOOKTable[89][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseVariableDeclaration_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration");
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(LOOKTable[83][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseVariableDeclaration_Filter(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_Filter");
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(LOOKTable[85][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseVariableDeclaration_GameList(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameList");
    }
    return(ReturnValue);
}
Module ParseModule(MBCC::Tokenizer& Tokenizer)
{
    Module ReturnValue;
    ReturnValue = ParseModule_0(Tokenizer);
    return(ReturnValue);
}
Module ParseModule_0(MBCC::Tokenizer& Tokenizer)
{
    Module ReturnValue;
    while(LOOKTable[92][0][Tokenizer.Peek().Type])
    {
        ReturnValue.Statements.push_back(ParseStatement(Tokenizer));
    }
    return(ReturnValue);
}
Statement ParseStatement(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if (LOOKTable[93][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseStatement_0(Tokenizer);
    }
    else if (LOOKTable[94][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseStatement_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Statement");
    }
    return(ReturnValue);
}
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(LOOKTable[87][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseVariableDeclaration(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","VariableDeclaration");
    }
    return(ReturnValue);
}
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(LOOKTable[81][0][Tokenizer.Peek().Type])
    {
        ReturnValue = ParseSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Selection");
    }
    return(ReturnValue);
}
