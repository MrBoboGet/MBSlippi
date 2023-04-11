#pragma once
#include <MBCC/MBCC.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <memory>
class Statement_Base
{
    public:
    
};
class Statement
{
    private:
     std::unique_ptr<Statement_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> Statement(T ObjectToStore)
    {
        m_Data = std::unique_ptr<Statement_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    Statement() = default;
    Statement(Statement&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(Statement&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    Statement_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Statement_Base&>(*m_Data);
    }
    Statement_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Statement_Base const&>(*m_Data);
    }
    
};
class Filter_Arg_Base
{
    public:
    
};
class Filter_Arg
{
    private:
     std::unique_ptr<Filter_Arg_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> Filter_Arg(T ObjectToStore)
    {
        m_Data = std::unique_ptr<Filter_Arg_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    Filter_Arg() = default;
    Filter_Arg(Filter_Arg&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(Filter_Arg&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    Filter_Arg_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Filter_Arg_Base&>(*m_Data);
    }
    Filter_Arg_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Filter_Arg_Base const&>(*m_Data);
    }
    
};
class GameInfoPredicate_Data_Base
{
    public:
    
};
class GameInfoPredicate_Data
{
    private:
     std::unique_ptr<GameInfoPredicate_Data_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> GameInfoPredicate_Data(T ObjectToStore)
    {
        m_Data = std::unique_ptr<GameInfoPredicate_Data_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    GameInfoPredicate_Data() = default;
    GameInfoPredicate_Data(GameInfoPredicate_Data&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(GameInfoPredicate_Data&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    GameInfoPredicate_Data_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<GameInfoPredicate_Data_Base&>(*m_Data);
    }
    GameInfoPredicate_Data_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<GameInfoPredicate_Data_Base const&>(*m_Data);
    }
    
};
class AttributeComponent
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    
};
class Filter_Component_Data_Base
{
    public:
    
};
class Filter_Component_Data
{
    private:
     std::unique_ptr<Filter_Component_Data_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> Filter_Component_Data(T ObjectToStore)
    {
        m_Data = std::unique_ptr<Filter_Component_Data_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    Filter_Component_Data() = default;
    Filter_Component_Data(Filter_Component_Data&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(Filter_Component_Data&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    Filter_Component_Data_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Filter_Component_Data_Base&>(*m_Data);
    }
    Filter_Component_Data_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Filter_Component_Data_Base const&>(*m_Data);
    }
    
};
class Operator
{
    public:
    std::string Op;
    
};
class Result_Base
{
    public:
    
};
class Result
{
    private:
     std::unique_ptr<Result_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> Result(T ObjectToStore)
    {
        m_Data = std::unique_ptr<Result_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    Result() = default;
    Result(Result&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(Result&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    Result_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Result_Base&>(*m_Data);
    }
    Result_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<Result_Base const&>(*m_Data);
    }
    
};
class Filter_Component_Function : public Filter_Component_Data_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string FilterName;
    
};
class Filter_Component_Variable : public Filter_Component_Data_Base
{
    public:
    MBCC::TokenPosition VariablePosition;
    std::string VariableName;
    
};
class Result_Record : public Result_Base
{
    public:
    MBCC::TokenPosition RecordPosition;
    MBCC::TokenPosition FilePosition;
    std::string OutFile;
    
};
class Result_Print : public Result_Base
{
    public:
    MBCC::TokenPosition PrintPosition;
    
};
class Result_Tabulate : public Result_Base
{
    public:
    
};
class AttributeList
{
    public:
    std::vector<AttributeComponent> Attributes;
    
};
class ArgList
{
    public:
    std::vector<Filter_Arg> Args;
    
};
class Filter_Arg_Named : public Filter_Arg_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    
};
class Filter_Arg_Positional : public Filter_Arg_Base
{
    public:
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    
};
class VariableDeclaration_Base : public Statement_Base
{
    public:
    MBCC::TokenPosition NamePosition;
    std::string Name;
    
};
class VariableDeclaration
{
    private:
     std::unique_ptr<VariableDeclaration_Base> m_Data;
    size_t m_TypeID = 0;
    template<typename T> static size_t p_GetTypeID(){return size_t(&p_GetTypeID<T>);}
    public:
    template<typename T> void Accept(T& Visitor);
    template<typename T> void Accept(T& Visitor) const;
    template<typename T> VariableDeclaration(T ObjectToStore)
    {
        m_Data = std::unique_ptr<VariableDeclaration_Base>(new T(std::move(ObjectToStore)));
        m_TypeID = p_GetTypeID<T>();
    }
    VariableDeclaration() = default;
    VariableDeclaration(VariableDeclaration&&) = default;
    template<typename T> bool IsType() const
    {
        return m_TypeID == p_GetTypeID<T>();
    }
    bool IsEmpty() const
    {
        return m_Data == nullptr;
    }
    void operator=(VariableDeclaration&& StructToMove)
    {
        std::swap(m_TypeID,StructToMove.m_TypeID);
        std::swap(m_Data,StructToMove.m_Data);
    }
    template<typename T> T const& GetType() const
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T const&>(*m_Data);
    }
    template<typename T> T& GetType()
    {
        if(!IsType<T>() || m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class");
        }return static_cast<T&>(*m_Data);
    }
    VariableDeclaration_Base& GetBase()
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<VariableDeclaration_Base&>(*m_Data);
    }
    VariableDeclaration_Base const& GetBase() const
    {
        if(m_Data == nullptr)
        {
            throw std::runtime_error("Invalid type access for abstract AST class: data is null");
        }return static_cast<VariableDeclaration_Base const&>(*m_Data);
    }
    
};
class Filter_ArgList
{
    public:
    std::vector<Filter_Arg> Arguments;
    
};
class GameInfoPredicate
{
    public:
    GameInfoPredicate_Data Data;
    std::string Operator;
    std::vector<GameInfoPredicate> ExtraTerms;
    
};
class GameInfoPredicate_Direct : public GameInfoPredicate_Data_Base
{
    public:
    std::vector<AttributeComponent> Attribute;
    MBCC::TokenPosition ValuePosition;
    std::string Value;
    std::string Comparison;
    uint64_t DateValue;
    
};
class GameInfoPredicate_Variable : public GameInfoPredicate_Data_Base
{
    public:
    MBCC::TokenPosition VariablePosition;
    std::string VariableName;
    
};
class Module
{
    public:
    std::vector<Statement> Statements;
    
};
class PlayerAssignment
{
    public:
    MBCC::TokenPosition WithPosition;
    MBCC::TokenPosition PlayerPosition;
    std::string AffectedPlayer;
    GameInfoPredicate PlayerCondition;
    
};
class VariableDeclaration_PlayerSelection : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameInfoPredicate Predicate;
    
};
class VariableDeclaration_GameInfoPredicate : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameInfoPredicate Predicate;
    
};
class Filter_Component
{
    public:
    Filter_Component_Data Data;
    Filter_ArgList ArgumentList;
    std::string Operator;
    std::vector<Filter_Component> ExtraTerms;
    
};
class GameInfoPredicate_Conjunction
{
    public:
    std::string Conjunction;
    GameInfoPredicate RHS;
    
};
class GameInfoPredicate_OperatorList
{
    public:
    std::vector<GameInfoPredicate> Terms;
    
};
class GameSelection
{
    public:
    PlayerAssignment Assignment;
    GameInfoPredicate GameCondition;
    
};
class Filter_OperatorList
{
    public:
    std::vector<Filter_Component> Components;
    
};
class Filter
{
    public:
    MBCC::TokenPosition FilterPosition;
    Filter_Component Component;
    
};
class VariableDeclaration_Filter : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition FilterPosition;
    Filter_Component Component;
    
};
class VariableDeclaration_GameList : public VariableDeclaration_Base
{
    public:
    MBCC::TokenPosition GamePosition;
    GameSelection Selection;
    
};
class Selection : public Statement_Base
{
    public:
    MBCC::TokenPosition SelectPosition;
    GameSelection Games;
    Filter SituationFilter;
    Result Output;
    
};
template<typename T> void Statement::Accept(T& Visitor)
{
    if(p_GetTypeID<VariableDeclaration>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration&>(*m_Data));
    }
    else if(p_GetTypeID<Selection>() == m_TypeID)
    {
        Visitor(static_cast<Selection&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Statement::Accept(T& Visitor) const
{
    if(p_GetTypeID<VariableDeclaration>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration const&>(*m_Data));
    }
    else if(p_GetTypeID<Selection>() == m_TypeID)
    {
        Visitor(static_cast<Selection const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Filter_Arg::Accept(T& Visitor)
{
    if(p_GetTypeID<Filter_Arg_Positional>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Arg_Positional&>(*m_Data));
    }
    else if(p_GetTypeID<Filter_Arg_Named>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Arg_Named&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Filter_Arg::Accept(T& Visitor) const
{
    if(p_GetTypeID<Filter_Arg_Positional>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Arg_Positional const&>(*m_Data));
    }
    else if(p_GetTypeID<Filter_Arg_Named>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Arg_Named const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void GameInfoPredicate_Data::Accept(T& Visitor)
{
    if(p_GetTypeID<GameInfoPredicate_Variable>() == m_TypeID)
    {
        Visitor(static_cast<GameInfoPredicate_Variable&>(*m_Data));
    }
    else if(p_GetTypeID<GameInfoPredicate_Direct>() == m_TypeID)
    {
        Visitor(static_cast<GameInfoPredicate_Direct&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void GameInfoPredicate_Data::Accept(T& Visitor) const
{
    if(p_GetTypeID<GameInfoPredicate_Variable>() == m_TypeID)
    {
        Visitor(static_cast<GameInfoPredicate_Variable const&>(*m_Data));
    }
    else if(p_GetTypeID<GameInfoPredicate_Direct>() == m_TypeID)
    {
        Visitor(static_cast<GameInfoPredicate_Direct const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Filter_Component_Data::Accept(T& Visitor)
{
    if(p_GetTypeID<Filter_Component_Variable>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Component_Variable&>(*m_Data));
    }
    else if(p_GetTypeID<Filter_Component_Function>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Component_Function&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Filter_Component_Data::Accept(T& Visitor) const
{
    if(p_GetTypeID<Filter_Component_Variable>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Component_Variable const&>(*m_Data));
    }
    else if(p_GetTypeID<Filter_Component_Function>() == m_TypeID)
    {
        Visitor(static_cast<Filter_Component_Function const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Result::Accept(T& Visitor)
{
    if(p_GetTypeID<Result_Record>() == m_TypeID)
    {
        Visitor(static_cast<Result_Record&>(*m_Data));
    }
    else if(p_GetTypeID<Result_Print>() == m_TypeID)
    {
        Visitor(static_cast<Result_Print&>(*m_Data));
    }
    else if(p_GetTypeID<Result_Tabulate>() == m_TypeID)
    {
        Visitor(static_cast<Result_Tabulate&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void Result::Accept(T& Visitor) const
{
    if(p_GetTypeID<Result_Record>() == m_TypeID)
    {
        Visitor(static_cast<Result_Record const&>(*m_Data));
    }
    else if(p_GetTypeID<Result_Print>() == m_TypeID)
    {
        Visitor(static_cast<Result_Print const&>(*m_Data));
    }
    else if(p_GetTypeID<Result_Tabulate>() == m_TypeID)
    {
        Visitor(static_cast<Result_Tabulate const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void VariableDeclaration::Accept(T& Visitor)
{
    if(p_GetTypeID<VariableDeclaration_Filter>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_Filter&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_GameList>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_GameList&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_PlayerSelection>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_PlayerSelection&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_GameInfoPredicate>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_GameInfoPredicate&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
template<typename T> void VariableDeclaration::Accept(T& Visitor) const
{
    if(p_GetTypeID<VariableDeclaration_Filter>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_Filter const&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_GameList>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_GameList const&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_PlayerSelection>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_PlayerSelection const&>(*m_Data));
    }
    else if(p_GetTypeID<VariableDeclaration_GameInfoPredicate>() == m_TypeID)
    {
        Visitor(static_cast<VariableDeclaration_GameInfoPredicate const&>(*m_Data));
    }
    else 
    {
        throw std::runtime_error("Invalid object stored in AST abstract class");
    }
    
}
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
Filter_Component ParseFilter_Component_And(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_And_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Or(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Or_0(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_OperatorList(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_OperatorList_0(MBCC::Tokenizer& Tokenizer);
Filter_OperatorList ParseFilter_OperatorList_1(MBCC::Tokenizer& Tokenizer);
Filter_Component_Function ParseFilter_Component_Function(MBCC::Tokenizer& Tokenizer);
Filter_Component_Function ParseFilter_Component_Function_0(MBCC::Tokenizer& Tokenizer);
Filter_Component_Variable ParseFilter_Component_Variable(MBCC::Tokenizer& Tokenizer);
Filter_Component_Variable ParseFilter_Component_Variable_0(MBCC::Tokenizer& Tokenizer);
Filter_Component_Data ParseFilter_Component_Data(MBCC::Tokenizer& Tokenizer);
Filter_Component_Data ParseFilter_Component_Data_0(MBCC::Tokenizer& Tokenizer);
Filter_Component_Data ParseFilter_Component_Data_1(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Base(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_Base_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer);
Filter_Component ParseFilter_Component_1(MBCC::Tokenizer& Tokenizer);
Filter ParseFilter(MBCC::Tokenizer& Tokenizer);
Filter ParseFilter_0(MBCC::Tokenizer& Tokenizer);
AttributeComponent ParseAttribute_Base(MBCC::Tokenizer& Tokenizer);
AttributeComponent ParseAttribute_Base_0(MBCC::Tokenizer& Tokenizer);
AttributeComponent ParseAttribute_Extension(MBCC::Tokenizer& Tokenizer);
AttributeComponent ParseAttribute_Extension_0(MBCC::Tokenizer& Tokenizer);
AttributeList ParseAttribute_List(MBCC::Tokenizer& Tokenizer);
AttributeList ParseAttribute_List_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Or_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_And_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_1(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Data ParseGameInfoPredicate_Data(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Data ParseGameInfoPredicate_Data_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate_Data ParseGameInfoPredicate_Data_1(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer);
GameInfoPredicate ParseGameInfoPredicate_1(MBCC::Tokenizer& Tokenizer);
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
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_0(MBCC::Tokenizer& Tokenizer);
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer);
Module ParseModule(MBCC::Tokenizer& Tokenizer);
Module ParseModule_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer);
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer);
inline MBCC::Tokenizer GetTokenizer()
{
    MBCC::Tokenizer ReturnValue("( |\\t|\\n|\\r)*",{"\\(","\\)","\\{","\\}","\\[","\\]",";","[[:digit:]]+","$\"((\\\\.|[^\"\\\\])*)\"","true|false","WITH","SELECT","RECORD","GAMES","FILTER","PRINT","$[[:alnum:]_]+","[[:alpha:]_]+[[:alnum:]_]*",";","<=","<",">=",">","!=",":",",","=","\\.","\\|\\|","\\|","&&","&","\\?",});
    return(ReturnValue);
}
