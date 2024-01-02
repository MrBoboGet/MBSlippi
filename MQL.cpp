#include "MQL.h"
#include "MBMeleeID.h"
#include "MeleeID.h"
#include "MQLParser.h"
#include <memory>
#include <MBUtility/Merge.h>
#include <assert.h>
#include <sstream>
#include <iomanip>

#include "SlippiGQPStructs.h"

#include <MBLSP/SemanticTokens.h>
#include <MBUnicode/MBUnicode.h>
#include <MBUtility/MBFiles.h>
#include <cmath>
#include <functional>
namespace MBSlippi
{
    std::vector<GameIntervall> h_SplitIntervall(GameIntervall IntervallToSplit)
    {
        std::vector<GameIntervall> ReturnValue;
        ReturnValue.reserve(IntervallToSplit.LastFrame-IntervallToSplit.FirstFrame+1);
        for(int i = IntervallToSplit.FirstFrame; i <= IntervallToSplit.LastFrame;i++)
        {
            ReturnValue.push_back(GameIntervall(i,i));
        }
        return ReturnValue;
    }
    //BEGIN ArgumentList    
    bool ArgumentList::Empty() const
    {
        return m_PositionalArguments.size() == 0 && m_KeyArgs.size() == 0;
    }
    std::vector<std::string> ArgumentList::GetKeys() const
    {
        std::vector<std::string> ReturnValue;
        for(auto const& Keys : m_KeyPositions)
        {
            ReturnValue.push_back(Keys.first);
        }
        return ReturnValue;
    }
    void ArgumentList::SetParentArgList(ArgumentList const* ParentList)
    {
        m_ParentArgList = ParentList;
        m_EvaluatedKeyPositions.clear();
        m_EvaluatedPositionalPositions.clear();
        m_EvaluatedKeyPositions.clear();
    }
    ArgumentList::ArgumentList(std::vector<MQL_Filter> PositionalArguments,std::unordered_map<std::string,MQL_Filter>& KeyArgs)
    {
        m_PositionalArguments = std::move(PositionalArguments);
        for(auto& Arg : KeyArgs)
        {
            m_KeyPositions[Arg.first] = m_KeyArgs.size();
            m_KeyArgs.push_back(std::move(Arg.second));
        }   
    }
    void ArgumentList::p_AddKey(std::string const& Key,MQL_Filter Value)
    {
        if(auto It = m_KeyPositions.find(Key); It == m_KeyPositions.end())
        {
            m_KeyPositions[Key] = m_KeyArgs.size();
            m_KeyArgs.push_back(std::move(Value));
        }
        else
        {
            m_KeyArgs[It->second] = std::move(Value);   
        }
    }
    //includes parent scope
    ArgumentList::ArgumentList(ArgumentList const& DefinitionBindings,ArgumentList const& SuppliedArguments)
    {
        //ASSUMPTION value of DefinitionBindings positional arguments are all of type symbol
        int CurrentArgumentIndex = 0;
        *this = DefinitionBindings;
        for(auto const& Argument : SuppliedArguments.m_PositionalArguments)
        {
            if(CurrentArgumentIndex >= DefinitionBindings.PositionalCount())
            {
                break;
            }
            m_PositionalArguments[ CurrentArgumentIndex] = SuppliedArguments.m_PositionalArguments[CurrentArgumentIndex];
            Literal_PosRef Ref;
            Ref.Pos = CurrentArgumentIndex;
            p_AddKey(DefinitionBindings.m_PositionalArguments[CurrentArgumentIndex].GetType<MQL_Filter_Literal>().Value.GetType<Literal_Symbol>().Value, 
                    MQL_Filter_Literal{Ref});
            CurrentArgumentIndex++;
        }
        for(auto const& Argument : SuppliedArguments.m_KeyPositions)
        {
            p_AddKey(Argument.first,SuppliedArguments.m_KeyArgs[Argument.second]);
        }
    }

    MQL_Filter const& ArgumentList::GetRawVariable(size_t IndexToGet) const
    {
        if(IndexToGet >= m_PositionalArguments.size())
        {
            throw std::runtime_error("Index out of bound when accessing raw variable in ArgumentList");
        }
        return m_PositionalArguments[IndexToGet];
    }
    MQL_Filter const& ArgumentList::GetRawVariable(std::string const& VariableToGet) const
    {
        if(!HasNamedVariable(VariableToGet))
        {
            throw std::runtime_error("Key variable with name \""+ VariableToGet+"\" not in argument list");
        }
        return m_KeyArgs[m_KeyPositions.at(VariableToGet)];
    }
    bool ArgumentList::HasNamedVariable(std::string const& VariableToCheck) const
    {
        return(m_KeyPositions.find(VariableToCheck) != m_KeyPositions.end());
    }
    MQL_Filter ArgumentList::p_EvaluateConstExpr(MQL_Filter const& Expr) const
    {
        MQL_Filter_Literal ReturnValue;
        MeleeGame Game;
        std::vector<MQL_MetricVariable> Result;
        if(m_ParentArgList == nullptr)
        {
            Result = MQLEvaluator::EvaluateMetric(Game,{{0,0}},ArgumentList(),Expr);
        }
        else
        {
            Result = MQLEvaluator::EvaluateMetric(Game,{{0,0}},*m_ParentArgList,Expr);
        }
        assert(Result.size() == 1);
        ReturnValue.Value = p_VariableToLiteral(Result[0]);
        return ReturnValue;
    }
    Literal ArgumentList::p_VariableToLiteral(MQL_MetricVariable const& Var)
    {
        Literal ReturnValue;
        if(std::holds_alternative<bool>(Var.Data))
        {
            Literal_Number NewRetValue;
            NewRetValue.Value = std::get<bool>(Var.Data);
            ReturnValue = NewRetValue;
        }
        else if(std::holds_alternative<int>(Var.Data))
        {
            Literal_Number NewRetValue;
            NewRetValue.Value = std::get<int>(Var.Data);
            ReturnValue = NewRetValue;
        }
        else if(std::holds_alternative<std::string>(Var.Data))
        {
            Literal_String NewRetValue;
            NewRetValue.Value = std::get<std::string>(Var.Data);
            ReturnValue = NewRetValue;
        }
        else if(std::holds_alternative<float>(Var.Data))
        {
            Literal_Number NewRetValue;
            NewRetValue.Value = std::get<float>(Var.Data);
            ReturnValue = NewRetValue;
        }
        else
        {
            assert(false && "p_VariableToLiteral doesn't cover all cases");   
        }
        return ReturnValue;
    }
    MQL_Filter const* ArgumentList::p_LookUpVar(MQL_Filter const& Value) const
    {
        MQL_Filter const* ReturnValue = nullptr;
        if(!Value.IsLiteral())
        {
            return &Value;   
        }
        auto const& Literal = Value.GetType<MQL_Filter_Literal>().Value;
        if(Literal.IsType<Literal_Symbol>())
        {
            //look for symbol in partent scope
            if(m_ParentArgList == nullptr)
            {
                throw std::runtime_error("Symbol with name \"" + Literal.GetType<Literal_Symbol>().Value+"\"");
            }
            ReturnValue = m_ParentArgList->p_GetNamedVariable(Literal.GetType<Literal_Symbol>().Value);
        }
        else if(Literal.IsType<Literal_PosRef>())
        {
            auto const& PosRef = Literal.GetType<Literal_PosRef>();
            assert(PosRef.Pos < m_PositionalArguments.size());
            return p_LookUpVar(m_PositionalArguments[PosRef.Pos]);
        }
        else
        {
            return &Value;
        }
        return ReturnValue;
    }
    MQL_Filter const* ArgumentList::p_GetNamedVariable(std::string const& StringToSearch) const
    {
        MQL_Filter const*  ReturnValue = nullptr;
        auto VarIt = m_KeyPositions.find(StringToSearch);
        if(VarIt == m_KeyPositions.end())
        {
            return ReturnValue;
        }
        auto const& Var = m_KeyArgs[VarIt->second];
        auto const* LookedUpVar = p_LookUpVar(Var);
        if(LookedUpVar == nullptr)
        {
            return nullptr;   
        }
        assert(LookedUpVar != nullptr);
        auto const& Value = *LookedUpVar;
        if(Value.IsConstExpr())
        {
            //ensures that vector doesnt get resized
            m_EvaluatedValues.reserve(PositionalCount() + m_KeyArgs.size());
            if(auto It = m_EvaluatedKeyPositions.find(StringToSearch); It == m_EvaluatedKeyPositions.end())
            {
                auto EvaluatedValue = p_EvaluateConstExpr(Value);
                m_EvaluatedKeyPositions[StringToSearch] = m_EvaluatedValues.size();
                m_EvaluatedValues.push_back(std::move(EvaluatedValue));
                return &m_EvaluatedValues.back();
            }      
            else
            {
                return & m_EvaluatedValues[It->second];
            }
        }
        return &Value;   
        //return p_GetEvaluatedValue(Var);
    }
    MQL_Filter const* ArgumentList::p_GetEvaluatedValue(size_t AssociatedIndex) const
    {
        MQL_Filter const*  ReturnValue = nullptr;
        auto const& Var = m_PositionalArguments[AssociatedIndex];
        auto const* LookedUpVar = p_LookUpVar(Var);
        if(LookedUpVar == nullptr)
        {
            throw std::runtime_error("Error in variable lookup");
        }
        auto const& Value = *LookedUpVar;
        if(Value.IsConstExpr())
        {
            //ensures that vector doesnt get resized
            m_EvaluatedValues.reserve(PositionalCount() + m_KeyArgs.size());
            if(auto It = m_EvaluatedPositionalPositions.find(AssociatedIndex); It == m_EvaluatedPositionalPositions.end())
            {
                auto EvaluatedValue = p_EvaluateConstExpr(Value);
                m_EvaluatedPositionalPositions[AssociatedIndex] = m_EvaluatedValues.size();
                m_EvaluatedValues.push_back(std::move(EvaluatedValue));
                return &m_EvaluatedValues.back();
            }      
            else
            {
                return & m_EvaluatedValues[It->second];
            }
        }
        return &Value;   
        //return p_GetEvaluatedValue(Var);

    }
    std::string ArgumentList::GetNamedVariableString(std::string const& VariableName) const
    {
        std::string ReturnValue;
        MQL_Filter const& Result = (*this)[VariableName];
        if(!Result.IsLiteral())
        {
            throw std::runtime_error("Variable not of string or integer type");
        }
        auto const& Literal = Result.GetType<MQL_Filter_Literal>().Value;
        if(Literal.IsType<Literal_String>())
        {
            return Literal.GetType<Literal_String>().Value;
        }
        else if(Literal.IsType<Literal_Number>())
        {
            return std::to_string(Literal.GetType<Literal_Number>().Value);
        }
        else
        {
            if(!Result.IsLiteral())
            {
                throw std::runtime_error("Variable not of string or integer type");
            }
        }
        return ReturnValue;
    }
    std::string ArgumentList::GetPositionalArgumentString(int Index) const
    {
        std::string ReturnValue;
        MQL_Filter const& Result = (*this)[Index];
        if(!Result.IsLiteral())
        {
            throw std::runtime_error("Variable not of string or integer type");
        }
        auto const& Literal = Result.GetType<MQL_Filter_Literal>().Value;
        if(Literal.IsType<Literal_String>())
        {
            return Literal.GetType<Literal_String>().Value;
        }
        else if(Literal.IsType<Literal_Number>())
        {
            return std::to_string(Literal.GetType<Literal_Number>().Value);
        }
        else
        {
            if(!Result.IsLiteral())
            {
                throw std::runtime_error("Variable not of string or integer type");
            }
        }
        return ReturnValue;
    }
    size_t ArgumentList::PositionalCount() const
    {
        return(m_PositionalArguments.size());
    }
    MQL_Filter const& ArgumentList::operator[](size_t Index) const
    {
        if(Index >= m_PositionalArguments.size())
        {
            throw std::runtime_error("Variable access out of bounds in argument list");   
        }
        MQL_Filter const* ReturnValue = p_GetEvaluatedValue(Index);
        assert(ReturnValue != nullptr);
        return *ReturnValue;
    }
    MQL_Filter const& ArgumentList::operator[](std::string const& Name) const
    {
        auto VarRef = p_GetNamedVariable(Name);
        if(VarRef == nullptr)
        {
            throw std::runtime_error("Unable to find variable with name \"" + Name + "\" in argument list");
        }
        return *VarRef;
    }

    //END ArgumentList    
    
    int h_ParseExpandInteger(std::string const& StringToParse)
    {
        int ReturnValue = 0;
        try
        {
            ReturnValue = std::stoi(StringToParse);
        }   
        catch(std::exception const& e)
        {
            throw std::runtime_error("Error parsing integer: "+std::string(e.what()));
        }
        return(ReturnValue);
    }
    float h_ParseFloat(std::string const& StringToParse)
    {
        float ReturnValue = 0;
        try
        {
            ReturnValue = std::stof(StringToParse);
        }   
        catch(std::exception const& e)
        {
            throw std::runtime_error("Error parsing float: "+std::string(e.what()));
        }
        return(ReturnValue);
    }
    template<typename LHSType,typename RHSType>
    bool h_Comp(LHSType const& lhs, std::string const& Operator,RHSType const& rhs)
    {
        bool ReturnValue = true;
        if(Operator == "=" || Operator == "==")
        {
            ReturnValue = lhs == rhs;   
        }
        else if(Operator == "!=")
        {
            ReturnValue = lhs != rhs;   
        }
        else if(Operator == "<")
        {
            ReturnValue = lhs < rhs;   
        }
        else if(Operator == "<=")
        {
            ReturnValue = lhs <= rhs;   
        }
        else if(Operator == ">=")
        {
            ReturnValue = lhs >= rhs;   
        }
        else if(Operator == ">")
        {
            ReturnValue = lhs > rhs;   
        }
        else
        {
            assert(false && "Invalid operator encountered");   
        }

        return(ReturnValue);
    }
    void h_GetPosixTime(std::string const& DateToVerify,GameInfoPredicate_Direct& ObjectToModify)
    {
        uint64_t ReturnValue = 0;
		std::tm StoredTime{};
		std::istringstream Stream(DateToVerify);
        int ComponentCount = 0;
        int DashCount = std::count(DateToVerify.begin(),DateToVerify.end(),'-');
        if(DashCount == 0)
        {
            Stream >> std::get_time(&StoredTime, "%Y");
            ComponentCount = 1;
        }
        else if(DashCount == 1)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m");
            ComponentCount = 2;
        }
        else if(DashCount == 2)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m-%d");
            ComponentCount = 3;
        }
        else
        {
            assert(false && "Calling h_GetPosixTime with invalid date string");
        }
        StoredTime.tm_isdst = -1;
        ReturnValue = uint64_t(std::mktime(&StoredTime));
        ObjectToModify.DateLowerBound = ReturnValue;
        ObjectToModify.DateHigherBound = ReturnValue;
        if(ComponentCount == 3)
        {
            ObjectToModify.DateHigherBound += 24*3600;
        }
        else if(ComponentCount == 2)
        {
            ObjectToModify.DateHigherBound += 24*3600*31;
        }
        else if(ComponentCount == 1)
        {
            ObjectToModify.DateHigherBound += 24*3600*365;
        }
    }
    bool h_IsValidDate(std::string const& DateToVerify)
    {
        bool ReturnValue = true;
		std::tm StoredTime{};
		std::istringstream Stream(DateToVerify);
        int DashCount = std::count(DateToVerify.begin(),DateToVerify.end(),'-');
        if(DashCount == 0)
        {
            Stream >> std::get_time(&StoredTime, "%Y");
            if(Stream.fail())
            {
                ReturnValue = false;   
            }
        }
        else if(DashCount == 1)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m");
            if(Stream.fail())
            {
                ReturnValue = false;   
            }
               
        }
        else if(DashCount == 2)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m-%d");
            if(Stream.fail())
            {
                ReturnValue = false;   
            }
        }
        else
        {
            ReturnValue = false;
        }
        return(ReturnValue);       
    }
    bool h_IsValidStage(std::string const& StageToVerify)
    {
        return(true);    
    }

    //BEGIN MQL_Scope
    MQL_Variable* MQL_Scope::p_GetVariable(std::string const& VariableName)
    {
        MQL_Variable* ReturnValue = nullptr;
            auto It = m_Variables.find(VariableName);
        if(It == m_Variables.end())
        {
            for(auto const& OverlayedModule : m_OverlayedModules)
            {
                MQL_Variable* ParentVariable = OverlayedModule->ModuleScope.p_GetVariable(VariableName);
                if(ParentVariable != nullptr)
                {
                    return(ParentVariable);
                }
            }
        }
        else
        {
            ReturnValue = &It->second;
        }
        return(ReturnValue);
    }
    MQL_Variable* MQL_Scope::p_GetVariable(Identifier const& Idf,int Offset)
    {
        MQL_Variable* ReturnValue = nullptr;
        if(Offset + 1 == Idf.Parts.size())
        {
            ReturnValue = p_GetVariable(Idf.Parts[Offset].Value);
        }
        else
        {
            auto AssociatedModule = m_BoundModules.find(Idf.Parts[Offset].Value);
            if(AssociatedModule != m_BoundModules.end())
            {
                for(auto const& BoundModule : AssociatedModule->second)
                {
                    ReturnValue = BoundModule->ModuleScope.p_GetVariable(Idf,Offset+1);
                }
            }
        }
        return ReturnValue;
    }
    bool MQL_Scope::p_HasVariable(std::string const& VariableName)
    {
        return(p_GetVariable(VariableName) != nullptr);
    }
    bool  MQL_Scope::p_HasVariable(Identifier const& idf,int Offset)
    {
        return(p_GetVariable(idf,Offset) != nullptr);
    }
    void MQL_Scope::OverlayScope(std::shared_ptr<MQL_Module> ScopeToOverlay)
    {
        m_OverlayedModules.emplace_back(std::move(ScopeToOverlay));
    }
    void MQL_Scope::BindScope(std::string ScopeName,std::shared_ptr<MQL_Module> ScopeToOverlay)
    {
        m_BoundModules[ScopeName].push_back(std::move(ScopeToOverlay));

    }
    //doesn't verify that the variable doesn't already exist
    void MQL_Scope::AddVariable(std::string const& Name,MQL_Variable Value)
    {
        m_Variables[Name] = std::move(Value);
    }
    MQL_Variable& MQL_Scope::GetVariable(Identifier const& Idf)
    {
        if(Idf.Parts.size() == 0)
        {
            throw std::runtime_error("Cannot find empty variable in scope");   
        }
        MQL_Variable* VariablePointer = p_GetVariable(Idf,0);
        if(VariablePointer == nullptr)
        {
            throw std::runtime_error("Variable not found");   
        }
        return(*VariablePointer);
    }
    bool MQL_Scope::HasVariable(std::string const& Idf)
    {
        return(p_HasVariable(Idf));
    }
    bool MQL_Scope::HasBinding(std::string const& Idf)
    {
        return(m_BoundModules.find(Idf) != m_BoundModules.end());
    }
    bool MQL_Scope::HasVariable(Identifier const& Idf)
    {
        if(Idf.Parts.size() == 0)
        {
            throw std::runtime_error("Cannot find empty variable in scope");   
        }
        return(p_HasVariable(Idf,0));
    }
    //END MQL_Scope
    
    //BEGIN SpecServer
    void SpecServer::p_SendInitialize()
    {
        std::string DataToSend;
        GQP::Initialize_Request Request;
        MBParsing::JSONObject Result = p_SendRequest(Request.GetJSON());
        GQP::Initialize_Response Response;
        Response.FillObject(Result);
        for(auto const& Filter : Response.result.filters)
        {
            m_ExportedFilters.push_back(Filter.name);   
        }
    }
    MBParsing::JSONObject SpecServer::p_SendRequest(MBParsing::JSONObject const& Request)
    {
        MBParsing::JSONObject ReturnValue;
        std::string DataToSend = Request.ToString();
        MBParsing::WriteBigEndianInteger(*m_SubProcess,DataToSend.size(),4);
        *m_SubProcess<<DataToSend;
        char SizeData[4];
        MBUtility::ReadExact(*m_SubProcess,SizeData,4);
        uint64_t ResponseSize = MBParsing::ParseBigEndianInteger(SizeData,4,0,0);
        std::string Response = std::string(ResponseSize,0);
        MBUtility::ReadExact(*m_SubProcess,Response.data(),ResponseSize);
        MBError ParseResult = true;
        ReturnValue = MBParsing::ParseJSONObject(Response,0,nullptr,&ParseResult);
        if(!ParseResult)
        {
            throw std::runtime_error("Error parsing Request from server: "+ParseResult.ErrorMessage);   
        }
        return(ReturnValue);
    }
    SpecServer::SpecServer(std::string const& ExecutableName,std::vector<std::string> const& Args)
    {
        m_SubProcess = std::make_unique<MBSystem::BiDirectionalSubProcess>(ExecutableName,Args);
        p_SendInitialize();
    }
    std::vector<std::string> SpecServer::GetExportedFilters()
    {
        return(m_ExportedFilters);
    }
    std::vector<GameIntervall> SpecServer::ExecuteFilter(std::string const& FilterName,MeleeGame const& GameToFilter,GameIntervall const& Intervall)
    {
        std::vector<GameIntervall> ReturnValue;
        GQP::ExecuteFilter_Request Request;
        Request.params.filterName  = FilterName;
        MBParsing::JSONObject ObjectToSend = Request.GetJSON();
        std::vector<MBParsing::JSONObject>& Frames = ObjectToSend["params"]["frames"].GetArrayData();
        for(int i = Intervall.FirstFrame; i <= Intervall.LastFrame;i++)
        {
            Frames.push_back(std::vector<MBParsing::JSONObject>{GameToFilter.Frames[i].PlayerInfo[0].ToJSON(),GameToFilter.Frames[i].PlayerInfo[1].ToJSON()});
        }
        MBParsing::JSONObject Response = p_SendRequest(ObjectToSend);
        GQP::ExecuteFilter_Response Result;
        Result.FillObject(Response);
        if(Result.result.intervalls.size() % 2 != 0)
        {
            throw std::runtime_error("Invalid result intervalls: returned integers have to be even in count");
        }
        for(int i = 0; i + 1 < Result.result.intervalls.size();i+=2)
        {
            ReturnValue.push_back(GameIntervall(Result.result.intervalls[i]+Intervall.FirstFrame,Result.result.intervalls[i+1]+Intervall.FirstFrame));
        }
        return(ReturnValue);
    }
    //END SpecServer










    
    void MQLEvaluator::p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
    }
    void MQLEvaluator::p_VerifyGameInfoPredicate_Direct(MQL_Module& AssociatedModule,Identifier const& Idf,GameInfoPredicate_Direct& PredicateToVerify,
            bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        auto const& Attribute = Idf.Parts;
        if(Attribute.size() != 0)
        {
            int AttributeOffset = 0;
            if(!IsPlayerAssignment)
            {
                if(Attribute[0].Value == "Date" || Attribute[0].Value == "Stage")
                {
                    if(Attribute.size() > 1)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = Attribute[0].Value +" doesn't have any members";
                        NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                        NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                        return;
                    }
                    if(IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Only player attributes can be a part of a player specification";
                        NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                        NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                        return;
                    }
                }
                if(Attribute[0].Value == "Date")
                {
                    if(!(h_IsValidDate(PredicateToVerify.Value)))
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Invalid date for predicate string";
                        NewDiagnostic.range.start.line = PredicateToVerify.ValuePosition.Line;
                        NewDiagnostic.range.start.character = PredicateToVerify.ValuePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + PredicateToVerify.Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    }
                    else
                    {
                        h_GetPosixTime(PredicateToVerify.Value,PredicateToVerify);
                    }
                    return;
                }
                else if(Attribute[0].Value == "Stage")
                {
                    if(!(h_IsValidStage(PredicateToVerify.Value)))
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Invalid stage for predicate string";
                        NewDiagnostic.range.start.line = PredicateToVerify.ValuePosition.Line;
                        NewDiagnostic.range.start.character = PredicateToVerify.ValuePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + PredicateToVerify.Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    }
                    return;
                }
                else if(!(Attribute[0].Value == "Player1" || Attribute[0].Value == "Player2"))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Invalid attribute: \""+Attribute[0].Value+"\"";
                    NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                    NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();

                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    return;
                }
                AttributeOffset += 1;
            }
            if(AttributeOffset >= Attribute.size())
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Empy attribute is invalid";
                NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
            if(Attribute[AttributeOffset].Value == "Character")
            {
                MBCharacter Character = StringToMBCharacter(PredicateToVerify.Value);
                if(Character == MBCharacter::Null)
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Invalid character string";
                    NewDiagnostic.range.start.line = PredicateToVerify.ValuePosition.Line;
                    NewDiagnostic.range.start.character = PredicateToVerify.ValuePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + PredicateToVerify.Value.size()+2;
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
            }
            //Guaranteed to be player attributes at this point
            if(!(Attribute[AttributeOffset].Value == "Character" || Attribute[AttributeOffset].Value == "Tag" || Attribute[AttributeOffset].Value == "Code"))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Invalid player member: \""+Attribute[AttributeOffset].Value+"\"";
                NewDiagnostic.range.start.line = Attribute[AttributeOffset].Position.Line;
                NewDiagnostic.range.start.character = Attribute[AttributeOffset].Position.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[AttributeOffset].Value.size();

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
        }
    }
    void MQLEvaluator::p_VerifyGameInfoPredicate(MQL_Module& AssociatedModule,GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(PredicateToVerify.Data.IsType<GameInfoPredicate_Direct>())
        {
            p_VerifyGameInfoPredicate_Direct(AssociatedModule,PredicateToVerify.Attribute,PredicateToVerify.Data.GetType<GameInfoPredicate_Direct>(),IsPlayerAssignment,OutDiagnostics);
        }
        else if(PredicateToVerify.Attribute.Parts.size() != 0)
        {
            auto const& Attribute = PredicateToVerify.Attribute.Parts;
            if(!AssociatedModule.ModuleScope.HasVariable(PredicateToVerify.Attribute))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "No variable named \"" + IdentifierToString(PredicateToVerify.Attribute) + "\"";
                NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
            else
            {
                auto const& Variable = AssociatedModule.ModuleScope.GetVariable(PredicateToVerify.Attribute);
                if(std::holds_alternative<MQL_LazyGameList>(Variable.Data))
                {
                       
                }
                else if(!std::holds_alternative<MQL_Variable_GameInfoPredicate>(Variable.Data))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Variable isn't of type GameInfoPredicate";
                    NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                    NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
                else
                {
                    auto const& PredicateData = std::get<MQL_Variable_GameInfoPredicate>(Variable.Data);
                    if(IsPlayerAssignment && !PredicateData.IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Variable isn't of type PlayerAssignment";
                        NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                        NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    }
                    else  if(!IsPlayerAssignment && PredicateData.IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Variable isn't of type GameInfoPredicate";
                        NewDiagnostic.range.start.line = Attribute[0].Position.Line;
                        NewDiagnostic.range.start.character = Attribute[0].Position.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Value.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    }
                }
            }
        }
        else if(PredicateToVerify.Data.IsEmpty())
        {
           //Do nothing    
        }
        else
        {
            assert(false && "p_VerifyGameInfoPredicate doesn't cover all cases");   
        }
        for(auto& SubPredicate : PredicateToVerify.ExtraTerms)
        {
            p_VerifyGameInfoPredicate(AssociatedModule,SubPredicate,IsPlayerAssignment,OutDiagnostics);
        }
    }
    MBLSP::Position MQLEvaluator::p_GetBegin(Literal const& Component)
    {
        MBLSP::Position ReturnValue;
        ReturnValue.line = Component.GetBase().ValuePosition.Line;
        ReturnValue.character = Component.GetBase().ValuePosition.ByteOffset;
        return ReturnValue;
    }
    MBLSP::Position MQLEvaluator::p_GetEnd(Literal const& Component)
    {
        MBLSP::Position ReturnValue = p_GetBegin(Component);
        if(Component.IsType<Literal_String>())
        {
            ReturnValue = ReturnValue + Component.GetType<Literal_String>().Value.size()+2;
        }
        else if(Component.IsType<Literal_Number>())
        {
            ReturnValue = ReturnValue + std::to_string(Component.GetType<Literal_Number>().Value).size();
        }
        else if(Component.IsType<Literal_Symbol>())
        {
            ReturnValue = ReturnValue + Component.GetType<Literal_Symbol>().Value.size();
        }
        return ReturnValue;
    }
    MBLSP::Position MQLEvaluator::p_GetBegin(Filter_Component const& Component)
    {
        MBLSP::Position ReturnValue;
        if(Component.IsType<Filter_Component_Func>())
        {
            auto const& Func = Component.GetType<Filter_Component_Func>();
            ReturnValue.line = Func.FilterName.Parts.front().Position.Line;
            ReturnValue.character = Func.FilterName.Parts.front().Position.ByteOffset;
        }
        else if(Component.IsType<Filter_Component_Literal>())
        {
            return p_GetBegin(Component.GetType<Filter_Component_Literal>().Value);
        }
        else if(Component.IsType<Filter_OperatorList>())
        {
            return p_GetBegin(Component.GetType<Filter_OperatorList>().Components.front());
        }
        return ReturnValue;
    }
    MBLSP::Position MQLEvaluator::p_GetEnd(Filter_Component const& Component)
    {
        MBLSP::Position ReturnValue;
        if(Component.IsType<Filter_Component_Func>())
        {
            auto const& Func = Component.GetType<Filter_Component_Func>();
            ReturnValue.line = Func.FilterName.Parts.back().Position.Line;
            ReturnValue.character = Func.FilterName.Parts.back().Position.ByteOffset+Func.FilterName.Parts.back().Value.size();
        }
        else if(Component.IsType<Filter_Component_Literal>())
        {
            return p_GetEnd(Component.GetType<Filter_Component_Literal>().Value);
        }
        else if(Component.IsType<Filter_OperatorList>())
        {
            return p_GetEnd(Component.GetType<Filter_OperatorList>().Components.back());
        }
        return ReturnValue;
    }
    void MQLEvaluator::p_AddKeyError(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,std::string const& UnknownKey,std::string_view Error)
    {
        //p_AddDiagnostic(OutDiagnostics,Func.FilterName,Error);
        for(auto const& Arg : Func.ArgumentList.Arguments)
        {
            if(Arg.IsType<Filter_Arg_Named>())
            {
                auto const& KeyArg = Arg.GetType<Filter_Arg_Named>();
                if(KeyArg.Name == UnknownKey)
                {
                    p_AddDiagnostic(OutDiagnostics,KeyArg.NamePosition,KeyArg.Name.size(),Error);
                }
            }
        }
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,MBCC::TokenPosition Begin,int Length,std::string_view Message)
    {
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.range.start.line = Begin.Line;
        NewDiagnostic.range.start.character = Begin.ByteOffset;
        NewDiagnostic.range.end = NewDiagnostic.range.start + Length;
        NewDiagnostic.message = Message;
        OutDiagnostics.push_back(std::move(NewDiagnostic));
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,size_t ArgIndex,std::string_view Error)
    {
        for(auto const& Arg : Func.ArgumentList.Arguments)
        {
            size_t CurrentIndex = 0;
            if(Arg.IsType<Filter_Arg_Positional>())
            {
                auto const& PosArg = Arg.GetType<Filter_Arg_Positional>();
                if(CurrentIndex == ArgIndex)
                {
                    p_AddDiagnostic(OutDiagnostics,PosArg.Argument,Error);
                }
                CurrentIndex++;
            }
        }
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component_Func const& Func,std::string const& Key,std::string_view Error)
    {
        for(auto const& Arg : Func.ArgumentList.Arguments)
        {
            if(Arg.IsType<Filter_Arg_Named>())
            {
                auto const& KeyArg = Arg.GetType<Filter_Arg_Named>();
                if(KeyArg.Name == Key)
                {
                    p_AddDiagnostic(OutDiagnostics,KeyArg.Argument,Error);
                }
            }
        }
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Literal const& ErrorLiteral,std::string_view Message)
    {
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start = p_GetBegin(ErrorLiteral);
        NewDiagnostic.range.end = p_GetEnd(ErrorLiteral);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Identifier const& ErrorIdentifier,std::string_view Message)
    {
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start.line = ErrorIdentifier.Parts.front().Position.Line;
        NewDiagnostic.range.start.character = ErrorIdentifier.Parts.front().Position.ByteOffset;
        NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ErrorIdentifier);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_OperatorList const& ErrorList,int Begin,int End,std::string_view Message)
    {
        Filter_Component const& BeginError = ErrorList.Components[Begin];
        Filter_Component const& EndError = ErrorList.Components[End-1];
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start = p_GetBegin(ErrorList.Components[Begin]);
        NewDiagnostic.range.end = p_GetEnd(ErrorList.Components[End-1]);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    void MQLEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_Component const& ErrorFilter,std::string_view Message)
    {
        if(ErrorFilter.IsType<Filter_Component_Func>())
        {
            p_AddDiagnostic(OutDiagnostics,ErrorFilter.GetType<Filter_Component_Func>(),Message);
        }
        else if(ErrorFilter.IsType<Filter_Component_Literal>())
        {
            p_AddDiagnostic(OutDiagnostics,ErrorFilter.GetType<Filter_Component_Literal>().Value,Message);
        }
        else if(ErrorFilter.IsType<Filter_OperatorList>())
        {
            p_AddDiagnostic(OutDiagnostics,ErrorFilter.GetType<Filter_OperatorList>(),0, ErrorFilter.GetType<Filter_OperatorList>().Components.size() ,Message);
        }
    }
    MQLEvaluator::PrecedenceInfo MQLEvaluator::p_GetPrecedenceInfo(std::vector<std::string> const& Operators,int BeginIndex,int EndIndex)
    {
        PrecedenceInfo ReturnValue;
        assert(EndIndex-BeginIndex != 1);
        int OperatorIndex = BeginIndex;
        auto CurrentOperator = m_Operators.find(Operators[OperatorIndex]);
        ReturnValue.OperatorPositions.push_back(OperatorIndex+1);
        OperatorIndex += 1;
        ReturnValue.Operator = CurrentOperator->second.ResultOperator;
        assert(CurrentOperator != m_Operators.end());
        while(OperatorIndex < EndIndex - 1)
        {
            assert(OperatorIndex < Operators.size());
            auto NewOperator = m_Operators.find(Operators[OperatorIndex]);
            assert(NewOperator != m_Operators.end());
            if(NewOperator->second.Precedence > CurrentOperator->second.Precedence)
            {
                CurrentOperator = NewOperator;
                ReturnValue.Operator = NewOperator->second.ResultOperator;
                ReturnValue.OperatorPositions.clear();
                ReturnValue.OperatorPositions.push_back(OperatorIndex+1);
            }
            else if(NewOperator->second.ResultOperator == CurrentOperator->second.ResultOperator)
            {
                if(!NewOperator->second.Binary)
                {
                    ReturnValue.OperatorPositions.push_back(OperatorIndex+1);
                }
            }
            OperatorIndex += 1;
        }
        ReturnValue.OperatorPositions.push_back(EndIndex);
        return ReturnValue;
    }

    bool h_IsArithmeticOp(OperatorType Op)
    {
        bool ReturnValue = true;   
        if(!(Op == OperatorType::Plus || 
                    Op == OperatorType::Minus || 
                    Op == OperatorType::Times || 
                    Op == OperatorType::eq || 
                    Op == OperatorType::ge || 
                    Op == OperatorType::geq || 
                    Op == OperatorType::le || 
                    Op == OperatorType::leq ))
        {
            ReturnValue = false;
        }
        return ReturnValue;
    }
    bool MQLEvaluator::p_OperatorIsValid(OperatorType Op,std::type_index Lhs,std::type_index Rhs,std::type_index& OutType)
    {
        bool ReturnValue = true;
        if(Lhs == typeid(bool))
        {
            return false;   
        }
        else if(Lhs == typeid(int))
        {
            if(Rhs == typeid(float))
            {
                OutType = typeid(float);
            }
            else if(Rhs == typeid(int))
            {
                OutType = typeid(int);   
            }
            else 
            {
                ReturnValue = false;   
            }
            if(!h_IsArithmeticOp(Op))
            {
                ReturnValue = false;   
            }
        }
        else if(Lhs == typeid(float))
        {
            if(Rhs == typeid(int) || Rhs == typeid(float))
            {
                OutType = typeid(float);
            }
            else
            {
                ReturnValue = false;   
            }
            if(!h_IsArithmeticOp(Op))
            {
                ReturnValue = false;   
            }
        }
        else if(Lhs == typeid(std::string))
        {
            if(Rhs != typeid(std::string))
            {
                ReturnValue = false;   
            }
            if(!(Op == OperatorType::Plus || 
                 Op == OperatorType::Minus || 
                 Op == OperatorType::eq || 
                 Op == OperatorType::leq || 
                 Op == OperatorType::le || 
                 Op == OperatorType::ge || 
                 Op == OperatorType::geq
                ))
            {
                ReturnValue = false;
            }
        }
        if( Op == OperatorType::eq || 
                 Op == OperatorType::leq || 
                 Op == OperatorType::le || 
                 Op == OperatorType::ge || 
                 Op == OperatorType::geq)
        {
            OutType = typeid(bool);   
        }

        return ReturnValue;
    }
    MQL_Filter MQLEvaluator::p_ConvertMetricOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,std::vector<MBLSP::Diagnostic>& OutDiagnostics,std::type_index& OutType)
    {
        MQL_MetricCombiner ReturnValue;
        if(EndIndex - BeginIndex == 1)
        {
            return p_ConvertMetricComponent(AssociatedModule,ParentArgList,FilterToConvert.Components[BeginIndex],OutDiagnostics,OutType);
        }
        PrecedenceInfo PartOperator = p_GetPrecedenceInfo(FilterToConvert.Operators,BeginIndex,EndIndex);
        ReturnValue.Type = PartOperator.Operator;
        int PrevIndex = BeginIndex;
        std::type_index Lhs = typeid(nullptr);
        std::type_index Rhs = typeid(nullptr);
        std::type_index NewType = typeid(nullptr);
        bool IsConstExpr = true;
        for(auto Index : PartOperator.OperatorPositions)
        {
            ReturnValue.Operands.push_back(p_ConvertMetricOperatorList(AssociatedModule,
                        ParentArgList,FilterToConvert,PrevIndex,Index,OutDiagnostics,NewType));
            if(Lhs == typeid(nullptr))
            {
                Lhs = NewType;
            }
            else if(Rhs == typeid(nullptr))
            {
                Rhs = NewType;   
            }

            auto const& LastValue = ReturnValue.Operands.back();
            if(LastValue.IsType<MQL_MetricCombiner>())
            {
                IsConstExpr = IsConstExpr && LastValue.GetType<MQL_MetricCombiner>().IsConstexpr;
            }
            else if(LastValue.IsType<MQL_Metric>())
            {
                IsConstExpr = IsConstExpr && LastValue.GetType<MQL_Metric>().IsConstexpr;
            }
            //else if(ResultType != NewType)
            //{
            //    p_AddDiagnostic(OutDiagnostics,FilterToConvert,PrevIndex,Index,
            //            "Invalid type of expression: lhs has type "+std::string(ResultType.name())+" and rhs has type "+NewType.name());
            //}
            PrevIndex = Index;
        }
        ReturnValue.LhsType = Lhs;
        ReturnValue.RhsType = Rhs;
        ReturnValue.IsConstexpr = IsConstExpr;
        if(ReturnValue.Type == OperatorType::Add || ReturnValue.Type == OperatorType::Pipe)
        {
            p_AddDiagnostic(OutDiagnostics,FilterToConvert,BeginIndex,EndIndex,"Invalid operator for metric filter");
        }
        else
        {
            if(!p_OperatorIsValid(ReturnValue.Type,Lhs,Rhs,OutType))
            {
                p_AddDiagnostic(OutDiagnostics,FilterToConvert,BeginIndex,EndIndex,
                        "Invalid operator with lhs of type "+std::string(Lhs.name())+" and rhs of type "+ Rhs.name());
            }
        }
        ReturnValue.ResultType = OutType;
        return ReturnValue;
    }
    MQL_Filter MQLEvaluator::p_ConvertMetricComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics,std::type_index& OutType)
    {
        MQL_Filter ReturnValue;
        if(FilterToConvert.IsType<Filter_Component_Literal>())
        {
            MQL_Filter_Literal NewLiteral;
            Filter_Component_Literal const& Literal = FilterToConvert.GetType<Filter_Component_Literal>();
            NewLiteral.Value = Literal.Value;
            if(Literal.Value.IsType<Literal_String>())
            {
                OutType = typeid(std::string);
            }
            else if(Literal.Value.IsType<Literal_Number>())
            {
                OutType = typeid(int);
            }
            else if(Literal.Value.IsType<Literal_Symbol>())
            {   
                Literal_Symbol const& Symbol = Literal.Value.GetType<Literal_Symbol>();
                if(ParentArgList.HasNamedVariable(Symbol.Value))
                {
                    auto const& Var = ParentArgList[Symbol.Value];
                    assert(!Var.IsSymbol());
                    OutType = Var.GetTypeID();
                }
                else
                {
                    if(!ParentArgList.HasNamedVariable(NewLiteral.Value.GetType<Literal_Symbol>().Value))
                    {
                        p_AddDiagnostic(OutDiagnostics,Literal.Value,"Cannot find variable with name \"" + NewLiteral.Value.GetType<Literal_Symbol>().Value+"\"");
                    }
                    class Invalid{};
                    OutType = typeid(Invalid);
                }
                //auto& Variable = AssociatedModule.ModuleScope.GetVariable(Symbol.Value);
                //what type...
            }
            ReturnValue = NewLiteral;
        }
        else if(FilterToConvert.IsType<Filter_Component_Func>())
        {
            Filter_Component_Func const& Func = FilterToConvert.GetType<Filter_Component_Func>();
            if(Func.FilterName.Parts.size() != 1)
            {
                p_AddDiagnostic(OutDiagnostics,Func.FilterName,"Cannot find metric with name "+IdentifierToString(Func.FilterName));
            }
            else
            {
                auto MetricIt = m_BuiltinMetrics.find(Func.FilterName.Parts[0].Value);
                if(MetricIt == m_BuiltinMetrics.end())
                {
                    p_AddDiagnostic(OutDiagnostics,Func.FilterName,"Cannot find metric with name "+IdentifierToString(Func.FilterName));
                }
                else
                {
                    MQL_Metric NewMetric;
                    NewMetric.Args = p_ConvertArgList(AssociatedModule,ParentArgList,Func.ArgumentList,OutDiagnostics);
                    NewMetric.Metric = MetricIt->second;
                    OutType = MetricIt->second.ResultType;
                    NewMetric.IsConstexpr = MetricIt->second.IsConstexpr;
                    NewMetric.ResultType = MetricIt->second.ResultType;
                    if(NewMetric.Metric.ArgCheckers.size() > 0)
                    {
                        NewMetric.Args.SetParentArgList(&ParentArgList);
                        ArgumentErrors Errors;
                        for(auto const& ArgChecker : NewMetric.Metric.ArgCheckers)
                        {
                            ArgChecker(Errors,NewMetric.Args);
                        }
                        NewMetric.Args.SetParentArgList(nullptr);
                        p_AddDiagnostics(Func,ParentArgList,NewMetric.Args,Errors,OutDiagnostics);
                    }
                    ReturnValue = std::move(NewMetric);
                }
            }
        }
        else if(FilterToConvert.IsType<Filter_OperatorList>())
        {
            auto const& OperatorList = FilterToConvert.GetType<Filter_OperatorList>();
            MQL_FilterCombiner Combiner;
            if(OperatorList.Operators.size() == 0)
            {
                return p_ConvertMetricComponent(AssociatedModule,ParentArgList,OperatorList.Components[0],OutDiagnostics,OutType);
            }
            ReturnValue = p_ConvertMetricOperatorList(AssociatedModule,ParentArgList,OperatorList,0,OperatorList.Components.size(),OutDiagnostics,OutType);
        }
        return ReturnValue;
    }
    MQL_Filter MQLEvaluator::p_ConvertFilterOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool AllowLiterals)
    {
        MQL_FilterCombiner ReturnValue;
        ReturnValue.Negated = FilterToConvert.Negated;
        if(EndIndex-BeginIndex == 1)
        {
            return p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToConvert.Components[BeginIndex],OutDiagnostics,AllowLiterals);
        }
        PrecedenceInfo OperatorInfo = p_GetPrecedenceInfo(FilterToConvert.Operators,BeginIndex,EndIndex);
        ReturnValue.Type = OperatorInfo.Operator;
        int PrevIndex = BeginIndex;
        if(OperatorInfo.Operator == OperatorType::Add || OperatorInfo.Operator == OperatorType::Pipe)
        {
            for(auto Index : OperatorInfo.OperatorPositions)
            {
                ReturnValue.Operands.push_back(p_ConvertFilterOperatorList(AssociatedModule,ParentArgList,FilterToConvert,PrevIndex,Index,OutDiagnostics,AllowLiterals));
                PrevIndex = Index;
            }
        }
        else
        {
            std::type_index ReturnType = typeid(nullptr);
            MQL_Filter MetricValue = p_ConvertMetricOperatorList(AssociatedModule,ParentArgList,FilterToConvert,BeginIndex,EndIndex,OutDiagnostics,
                    ReturnType);
            if(ReturnType != typeid(bool))
            {
                p_AddDiagnostic(OutDiagnostics,FilterToConvert,BeginIndex,EndIndex,"Metric expression has to evaluate to a bool to be a part of a filter");
            }
            return MetricValue;
        }
        return ReturnValue;
    }
    MQL_Filter MQLEvaluator::p_ConvertArgComponent(MQL_Module& AssociatedModule, ArgumentList& ParentArgList, Filter_Component const& Filter,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        MQL_Filter ReturnValue;
        if(Filter.IsType<Filter_OperatorList>())
        {
            auto const& Operators = Filter.GetType<Filter_OperatorList>();
            if(Operators.Components.size() == 1)
            {
                return p_ConvertArgComponent(AssociatedModule,ParentArgList,Operators.Components[0],OutDiagnostics);
            }
        }
        else if(Filter.IsType<Filter_Component_Func>())
        {
            auto const& Func = Filter.GetType<Filter_Component_Func>();
            if(auto It = m_BuiltinMetrics.find(Func.FilterName.Parts[0].Value); It != m_BuiltinMetrics.end())
            {
                if(It->second.IsConstexpr)
                {
                    std::type_index OutType = typeid(nullptr);
                    return p_ConvertMetricComponent(AssociatedModule,ParentArgList,Filter,OutDiagnostics,OutType);
                }
            }
        }
        return p_ConvertFilterComponent(AssociatedModule,ParentArgList,Filter,OutDiagnostics,true);
    }
    ArgumentList MQLEvaluator::p_ConvertArgList(MQL_Module& AssociatedModule, ArgumentList& ParentArgList,Filter_ArgList const& ArgsToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        ArgumentList ReturnValue;
        std::vector<MQL_Filter> PositionalArguments;
        std::unordered_map<std::string,MQL_Filter> KeyArgs;
        for(auto const& Arg : ArgsToConvert.Arguments)
        {
            if(Arg.IsType<Filter_Arg_Positional>())
            {
                PositionalArguments.push_back(p_ConvertArgComponent(AssociatedModule,ParentArgList,Arg.GetType<Filter_Arg_Positional>().Argument,OutDiagnostics));
            }
            else if(Arg.IsType<Filter_Arg_Named>())
            {
                auto const& NewKeyArg = Arg.GetType<Filter_Arg_Named>();
                KeyArgs[NewKeyArg.Name] = p_ConvertArgComponent(AssociatedModule,ParentArgList,NewKeyArg.Argument,OutDiagnostics);
            }
        }
        return ArgumentList(std::move(PositionalArguments),KeyArgs);
    }
    MQL_Filter MQLEvaluator::p_ConvertFilterComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool AllowLiterals)
    {
        MQL_Filter ReturnValue;
        if(FilterToConvert.IsType<Filter_Component_Literal>())
        {
            auto& Literal = FilterToConvert.GetType<Filter_Component_Literal>();
            if(!AllowLiterals)
            {
                p_AddDiagnostic(OutDiagnostics,Literal.Value,"Expressions in filter component must be a part of a comparison");
            }
            else
            {
                MQL_Filter_Literal NewLiteral;
                NewLiteral.Value = Literal.Value;
                ReturnValue = NewLiteral;
                if(NewLiteral.Value.IsType<Literal_Symbol>())
                {
                    if(!ParentArgList.HasNamedVariable(NewLiteral.Value.GetType<Literal_Symbol>().Value))
                    {
                        p_AddDiagnostic(OutDiagnostics,Literal.Value,"Cannot find variable with name \"" + NewLiteral.Value.GetType<Literal_Symbol>().Value+"\"");
                    }
                }
            }
        }
        else if(FilterToConvert.IsType<Filter_Component_Func>())
        {
            //two cases depending on filter / metric
            Filter_Component_Func const& FuncFilter = FilterToConvert.GetType<Filter_Component_Func>();
            MQL_IntervallExtractor NewFilter;
            if(AssociatedModule.ModuleScope.HasVariable(FuncFilter.FilterName))
            {
                auto const& Variable = AssociatedModule.ModuleScope.GetVariable(FuncFilter.FilterName);   
                if(!std::holds_alternative<std::shared_ptr<MQL_FilterDefinition>>(Variable.Data))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Variable isn't of type Filter";
                    NewDiagnostic.range.start.line = FuncFilter.NamePosition.Line;
                    NewDiagnostic.range.start.character = FuncFilter.NamePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(FuncFilter.FilterName);
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
                else
                {
                    MQL_FilterReference NewValue;
                    NewValue.Filter = std::get<std::shared_ptr<MQL_FilterDefinition>>(Variable.Data);
                    NewValue.Negated = FuncFilter.Negated;
                    NewValue.Args = ArgumentList(NewValue.Filter->Arguments,p_ConvertArgList(AssociatedModule,ParentArgList,FuncFilter.ArgumentList,OutDiagnostics));
                    NewValue.Args.SetParentArgList(&ParentArgList);
                    auto ArgErrors = p_VerifyReferenceArguments(*m_LoadedModules[NewValue.Filter->AssociatedModule],
                            NewValue.Filter->Arguments,
                            NewValue.Args,
                            *NewValue.Filter->Component);
                    auto ValidKey = NewValue.Filter->Arguments.GetKeys();
                    ArgErrors.ValidKeys.insert(ValidKey.begin(),ValidKey.end());
                    p_TypeCheckArgs(ArgErrors, NewValue.Filter->Arguments,NewValue.Args);
                    NewValue.Args.SetParentArgList(nullptr);
                    p_AddDiagnostics(FuncFilter,ParentArgList,NewValue.Args,ArgErrors,OutDiagnostics);
                    return NewValue;
                }
            }
            else 
            {
                if(auto It = m_BuiltinFilters.find(FuncFilter.FilterName.Parts[0].Value); 
                        FuncFilter.FilterName.Parts.size() > 1 || It == m_BuiltinFilters.end())
                {
                       
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Can't find filter with name \""+IdentifierToString(FuncFilter.FilterName)+"\"";
                    NewDiagnostic.range.start.line = FuncFilter.NamePosition.Line;
                    NewDiagnostic.range.start.character = FuncFilter.NamePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(FuncFilter.FilterName);
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
                else
                {
                    MQL_IntervallExtractor NewValue;
                    NewValue.Args = p_ConvertArgList(AssociatedModule,ParentArgList,FuncFilter.ArgumentList,OutDiagnostics);
                    NewValue.Filter = It->second;
                    NewValue.Negated = FuncFilter.Negated;
                    if(NewValue.Filter.ArgCheckers.size() > 0)
                    {
                        NewValue.Args.SetParentArgList(&ParentArgList);
                        ArgumentErrors Errors;
                        for(auto const& ArgChecker : NewValue.Filter.ArgCheckers)
                        {
                            ArgChecker(Errors,NewValue.Args);
                        }
                        NewValue.Args.SetParentArgList(nullptr);
                        p_AddDiagnostics(FuncFilter,ParentArgList,NewValue.Args,Errors,OutDiagnostics);
                    }
                    return NewValue;
                }
            }

        }
        else if(FilterToConvert.IsType<Filter_OperatorList>())
        {
            //needs to convert filter first
            Filter_OperatorList const& OperatorList = FilterToConvert.GetType<Filter_OperatorList>();
            if(OperatorList.Operators.size() == 0)
            {
                return p_ConvertFilterComponent(AssociatedModule,ParentArgList,OperatorList.Components[0],OutDiagnostics,AllowLiterals);
            }
            return p_ConvertFilterOperatorList(AssociatedModule,ParentArgList,OperatorList,0,OperatorList.Components.size(),OutDiagnostics,AllowLiterals);
        }
        return ReturnValue;
    }
    void MQLEvaluator::p_CombineErrors(ArgumentErrors& OutError,ArgumentErrors InError)
    {
        if(OutError.GenericErrors.size() == 0 && InError.GenericErrors.size() > 0)
        {
            std::swap(OutError.GenericErrors,InError.GenericErrors);   
        }
        OutError.PositionalErrors.insert(std::make_move_iterator(InError.PositionalErrors.begin()),
                std::make_move_iterator(InError.PositionalErrors.end()));
        OutError.KeyErrors.insert(std::make_move_iterator(InError.KeyErrors.begin()),
                std::make_move_iterator(InError.KeyErrors.end()));
    }
    void MQLEvaluator::p_FillError(ArgumentErrors& OutError,ArgumentList const& FilterArgs, Literal_Symbol const& ErrorSym,std::string_view Message)
    {
        auto const& Literal = FilterArgs.GetRawVariable(ErrorSym.Value).GetType<MQL_Filter_Literal>();
        if(Literal.Value.IsType<Literal_PosRef>())
        {
            OutError.PositionalErrors[Literal.Value.GetType<Literal_PosRef>().Pos] = Message;
        }
        else
        {
            OutError.KeyErrors[ErrorSym.Value] = Message;
        }
    }
    void MQLEvaluator::p_FillErrors(ArgumentErrors& OutError,ArgumentList const& ArgDef,ArgumentList const& FilterArgs ,ArgumentErrors const& InError)
    {
        //OutError.GenericErrors.insert(OutError.GenericErrors.begin(),InError.GenericErrors.begin(),InError.GenericErrors.end());
        if(InError.GenericErrors.size() > 0 && OutError.GenericErrors.size() == 0)
        {
            OutError.GenericErrors.push_back("Errors in referenced filter");   
        }
        for(auto const& KeyError : InError.KeyErrors)
        {
            auto const& Var = FilterArgs.GetRawVariable(KeyError.first);
            if(Var.IsSymbol())
            {
                p_FillError(OutError,ArgDef,Var.GetType<MQL_Filter_Literal>().Value.GetType<Literal_Symbol>(),KeyError.second);
            }
            else if(OutError.GenericErrors.size() == 0)
            {
                OutError.GenericErrors.push_back("Errors in referenced filter");   
            }
        }
        for(auto const& PositionalError  : InError.PositionalErrors)
        {
            auto const& Var = FilterArgs.GetRawVariable(PositionalError.first);
            if(Var.IsSymbol())
            {
                p_FillError(OutError,ArgDef,Var.GetType<MQL_Filter_Literal>().Value.GetType<Literal_Symbol>(),PositionalError.second);
            }
            else if(OutError.GenericErrors.size() == 0)
            {
                OutError.GenericErrors.push_back("Errors in referenced filter");   
            }
        }
        //if(InError.UnknownKeys.size() > 0 && OutError.GenericErrors.size() == 0)
        //{
        //    OutError.GenericErrors.push_back("Errors in referenced filter");   
        //}
    }
    ArgumentErrors MQLEvaluator::p_VerifyReferenceArguments(MQL_Module& AssociatedModule,ArgumentList const& ArgDef , ArgumentList const& CalledArguments,MQL_Filter& FilterToVerify)
    {
        ArgumentErrors ReturnValue;
        if(FilterToVerify.IsType<MQL_FilterCombiner>())
        {
            auto& Combiner = FilterToVerify.GetType<MQL_FilterCombiner>();
            for(auto& Filter : Combiner.Operands)
            {
                p_CombineErrors(ReturnValue,p_VerifyReferenceArguments(AssociatedModule,ArgDef,CalledArguments,Filter));
            }
        }
        else if(FilterToVerify.IsType<MQL_IntervallExtractor>())
        {
            auto& Extractor = FilterToVerify.GetType<MQL_IntervallExtractor>();
            if(Extractor.Filter.ArgCheckers.size() > 0)
            {
                Extractor.Args.SetParentArgList(&CalledArguments);
                ArgumentErrors Errors;
                for(auto const& ArgChecker : Extractor.Filter.ArgCheckers)
                {
                    ArgChecker(Errors,Extractor.Args);
                }
                p_FillErrors(ReturnValue,CalledArguments,Extractor.Args,Errors);
                Extractor.Args.SetParentArgList(nullptr);
            }
        }
        else if(FilterToVerify.IsType<MQL_FilterReference>())
        {
            auto& Reference = FilterToVerify.GetType<MQL_FilterReference>();
            Reference.Args.SetParentArgList(&CalledArguments);

            auto const& FilterDefArgs = Reference.Filter->Arguments;
            //ensure that values supplied have the same type
            p_TypeCheckArgs(ReturnValue,FilterDefArgs,Reference.Args);
            p_FillErrors(ReturnValue,CalledArguments,Reference.Args,p_VerifyReferenceArguments(*m_LoadedModules[Reference.Filter->AssociatedModule],Reference.Filter->Arguments,Reference.Args,*Reference.Filter->Component));
            Reference.Args.SetParentArgList(nullptr);
        }
        else if(FilterToVerify.IsType<MQL_Metric>())
        {
            auto& Metric = FilterToVerify.GetType<MQL_Metric>();
            if(Metric.Metric.ArgCheckers.size() > 0)
            {
                Metric.Args.SetParentArgList(&CalledArguments);
                ArgumentErrors Errors;
                for(auto const& ArgChecker : Metric.Metric.ArgCheckers)
                {
                    ArgChecker(Errors,Metric.Args);
                }
                p_FillErrors(ReturnValue,CalledArguments,Metric.Args,Errors);
                Metric.Args.SetParentArgList(nullptr);
            }
        }
        else if(FilterToVerify.IsType<MQL_MetricCombiner>())
        {
               
            auto& Combiner = FilterToVerify.GetType<MQL_MetricCombiner>();
            for(auto& Filter : Combiner.Operands)
            {
                p_CombineErrors(ReturnValue,p_VerifyReferenceArguments(AssociatedModule,ArgDef,CalledArguments,Filter));
            }
        }
        //Filter only values that are relevant for the positional parts
        
        return ReturnValue;
    }
    void MQLEvaluator::p_TypeCheckArgs(ArgumentErrors& OutErrors,ArgumentList const& ArgDef,ArgumentList const& CalledArguments)
    {
        for(auto const& Key : CalledArguments.GetKeys())
        {
            if(ArgDef.HasNamedVariable(Key))
            {
                auto SuppliedType = CalledArguments[Key].GetTypeID();
                auto DefinedType = ArgDef[Key].GetTypeID();
                if(DefinedType != SuppliedType)
                {
                    OutErrors.KeyErrors[Key] = "Invalid type: supplied type is "+
                        std::string(SuppliedType.name())+" while required type is " + SuppliedType.name();
                }
            }
        }
        for(int i = 0; i < CalledArguments.PositionalCount();i++)
        {
            if(i >= ArgDef.PositionalCount())
            {
                break;   
            }
            auto SuppliedType = CalledArguments[i].GetTypeID();
            auto DefinedType = ArgDef[i].GetTypeID();
            if(DefinedType != SuppliedType)
            {
                OutErrors.PositionalErrors[i] = "Invalid type: supplied type is "+
                    std::string(SuppliedType.name())+" while required type is " + SuppliedType.name();
            }
        }
    }
    void MQLEvaluator::p_AddDiagnostics(Filter_Component_Func const& Func,ArgumentList const& ParentArgList,ArgumentList const& ErrorArgList,ArgumentErrors const& Errors,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        for(auto const& Error : Errors.GenericErrors)
        {
            p_AddDiagnostic(OutDiagnostics,Func.FilterName,Error);
        }
        for(auto const& Key : ErrorArgList.GetKeys())
        {
            if(Errors.ValidKeys.find(Key) == Errors.ValidKeys.end())
            {
                p_AddKeyError(OutDiagnostics,Func,Key,"No key-argument named \""+Key+"\"");
            }
        }
        for(auto const& Arg : Errors.PositionalErrors)
        {
            if(Arg.first >= ErrorArgList.PositionalCount())
            {
                assert(false);
                continue;   
            }
            if(!ParentArgList.Empty() && ErrorArgList[Arg.first].IsSymbol())
            {
                continue;
            }
            p_AddDiagnostic(OutDiagnostics,Func,Arg.first,Arg.second);
        }
        for(auto const& Arg : Errors.KeyErrors)
        {
            if(!ErrorArgList.HasNamedVariable(Arg.first))
            {
                assert(false);
                continue;
            }
            MQL_Filter const& ArgVar = ErrorArgList.GetRawVariable(Arg.first);
            if(!ParentArgList.Empty() && ArgVar.IsSymbol())
            {
                continue;
            }
            p_AddDiagnostic(OutDiagnostics,Func,Arg.first,Arg.second);
        }
    }
    void MQLEvaluator::p_VerifyFilterComponent(MQL_Module& AssociatedModule,Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        ArgumentList ParentArgList;
        p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToVerify,OutDiagnostics);
    }
    void MQLEvaluator::p_VerifyFilter(MQL_Module& AssociatedModule,Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        ArgumentList ParentArgList;
        p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToVerify.Component,OutDiagnostics);
    }
    bool MQLEvaluator::p_EvaluateImport(MQL_Module& AssociatedModule,Import& ImportStatement,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        std::filesystem::path ModuleDirectory = AssociatedModule.ModulePath.parent_path();
        std::vector<std::filesystem::path> SearchDirectories;
        SearchDirectories.push_back(ModuleDirectory);
        SearchDirectories.insert(SearchDirectories.end(),m_ExtraModuleSearchPaths.begin(),m_ExtraModuleSearchPaths.end());
        std::filesystem::path PotentialDirectoryPath;
        for(auto const& Part : ImportStatement.ImportPath.Parts)
        {
            PotentialDirectoryPath /= Part.Value;
        }
        std::filesystem::path PotentialFilePath = PotentialDirectoryPath;
        PotentialFilePath.replace_extension(".slpspec");
        bool LibraryImported = false;
        try
        {
            for(auto const& Dir : SearchDirectories)
            {
                if(std::filesystem::exists(Dir/PotentialDirectoryPath))
                {
                    std::filesystem::directory_iterator DirIt (Dir/PotentialDirectoryPath);
                    for(auto const& Entry : DirIt)
                    {
                        if(Entry.path().extension() == ".slpspec")
                        {
                            ModuleID NewModuleID = LoadModule(Entry.path());
                            std::shared_ptr<MQL_Module>& NewModule = m_LoadedModules[NewModuleID];
                            if(ImportStatement.Binding.ImportName.Value == "")
                            {
                                AssociatedModule.ModuleScope.OverlayScope(NewModule);
                            }
                            else
                            {
                                AssociatedModule.ModuleScope.BindScope(ImportStatement.Binding.ImportName.Value,NewModule);
                            }
                            LibraryImported = true;
                            if(NewModule->LoadErrors.size() != 0)
                            {
                                MBLSP::Diagnostic NewDiagnostic;
                                NewDiagnostic.message = "Error importing module: Errors in imported module";
                                NewDiagnostic.range.start.line = ImportStatement.ImportPath.Parts.front().Position.Line;
                                NewDiagnostic.range.start.character = ImportStatement.ImportPath.Parts.front().Position.ByteOffset;
                                NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ImportStatement.ImportPath);
                                OutDiagnostics.push_back(NewDiagnostic);
                                ReturnValue = false;
                            }
                        }
                    }
                }
                else if(std::filesystem::exists(Dir/PotentialFilePath))
                {
                    ModuleID NewModuleID = LoadModule(Dir/PotentialFilePath);
                    std::shared_ptr<MQL_Module>& NewModule = m_LoadedModules[NewModuleID];
                    if(ImportStatement.Binding.ImportName.Value == "")
                    {
                        AssociatedModule.ModuleScope.OverlayScope(NewModule);
                    }
                    else
                    {
                        AssociatedModule.ModuleScope.BindScope(ImportStatement.Binding.ImportName.Value,NewModule);
                    }
                    LibraryImported = true;
                    if(NewModule->LoadErrors.size() != 0)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Error importing module: Errors in imported module";
                        NewDiagnostic.range.start.line = ImportStatement.ImportPath.Parts.front().Position.Line;
                        NewDiagnostic.range.start.character = ImportStatement.ImportPath.Parts.front().Position.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ImportStatement.ImportPath);
                        OutDiagnostics.push_back(NewDiagnostic);
                        ReturnValue = false;
                    }
                    break;
                }
            }
            if(!LibraryImported)
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Error importing module: Module not found";
                NewDiagnostic.range.start.line = ImportStatement.ImportPath.Parts.front().Position.Line;
                NewDiagnostic.range.start.character = ImportStatement.ImportPath.Parts.front().Position.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ImportStatement.ImportPath);
                OutDiagnostics.push_back(NewDiagnostic);
                ReturnValue = false;
            }
        }
        catch(std::exception const& e)
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Error importing module: "+std::string(e.what());
            NewDiagnostic.range.start.line = ImportStatement.ImportPath.Parts.front().Position.Line;
            NewDiagnostic.range.start.character = ImportStatement.ImportPath.Parts.front().Position.ByteOffset;
            NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ImportStatement.ImportPath);
            OutDiagnostics.push_back(NewDiagnostic);
            ReturnValue = false;
        }
        return ReturnValue;
    }
    bool MQLEvaluator::VerifyVariableDeclaration(MQL_Module& AssociatedModule,Statement& DeclarationToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool UpdateState)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        //no redeclaration allowed, should allow redeclaration if of the same type
        auto const& VariableBase = DeclarationToVerify.GetType<VariableDeclaration>().GetBase();
        bool VariableExists = false;
        if(AssociatedModule.ModuleScope.HasVariable(VariableBase.Name))
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.range.start.line = VariableBase.NamePosition.Line;
            NewDiagnostic.range.start.character = VariableBase.NamePosition.ByteOffset;
            NewDiagnostic.range.end = NewDiagnostic.range.start+VariableBase.Name.size();
            NewDiagnostic.message = "Variable with name \""+VariableBase.Name+"\" already exists";
            Diagnostics.push_back(std::move(NewDiagnostic));
            VariableExists = true;
        }


        //struct TestType
        //{
        //    typedef std::string BASE;
        //};
        //struct Test2Type : public  TestType
        //{
        //    typedef int BASE;
        //};
        //struct Test3Type : public  Test2Type
        //{
        //    typedef float BASE;
        //};

        //typedef Statement::Base::i_BaseType<VariableDeclaration>::type TB;
        //static_assert(std::is_same_v<TB,void>);
       

        //typedef typename VariableDeclaration::BASE ligma; 
        //typedef std::void_t<typename VariableDeclaration::BASE> a;

        //static_assert(std::is_same_v<SBase,Statement_Base>);
        //static_assert(std::is_same_v<SResolved,void>);
        //static_assert(std::is_same_v<Test2,void>);
        //static_assert(std::is_same_v<Test2,VariableDeclaration_Base>);
        //static_assert(std::is_base_of_v<Statement_Base,VariableDeclaration_Filter>);

      
        //typedef MBCC::BaseType<VariableDeclaration>::type test;
        //typedef MBCC::BaseType<VariableDeclaration2>::type test3;
        //typedef MBCC::BaseType<Statement>::type test2;

        //
        //constexpr bool PossibleType = Statement::PossibleType_v<VariableDeclaration_Filter>;
        //static_assert(PossibleType);
       

        typedef MBCC::BaseType<VariableDeclaration>::type Test; 
        typedef Statement::TypeBaseParent_t<VariableDeclaration> Test2;

        //static_assert(!std::is_same_v<Test,void>,"Bruh bruh bruh, what the fuck is the difference for VariableDeclaration");
        //typedef MBCC::BaseType<Statement>::type bruh; 
        
        if(DeclarationToVerify.IsType<VariableDeclaration_Filter>())
        {
            auto const& FilterVariable = DeclarationToVerify.GetType<VariableDeclaration_Filter>();
            std::shared_ptr<MQL_FilterDefinition> NewDefinition = std::make_shared<MQL_FilterDefinition>();
            ArgumentList ParentArglist;
            NewDefinition->AssociatedModule = AssociatedModule.ID;
            NewDefinition->Arguments = p_ConvertArgList(AssociatedModule,ParentArglist,FilterVariable.Arguments,OutDiagnostics);
            NewDefinition->Component = std::make_shared<MQL_Filter>(p_ConvertFilterComponent(AssociatedModule,NewDefinition->Arguments,FilterVariable.Component,OutDiagnostics));
            //p_VerifyFilterComponent(AssociatedModule,FilterVariable.Component,Diagnostics);
            //check bindings
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                TemporaryVariable.Data = std::move(NewDefinition);
                AssociatedModule.ModuleScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_GameInfoPredicate>())
        {
            auto& PredicateVariable = DeclarationToVerify.GetType<VariableDeclaration_GameInfoPredicate>();
            p_VerifyGameInfoPredicate(AssociatedModule,PredicateVariable.Predicate,false,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_Variable_GameInfoPredicate Value = MQL_Variable_GameInfoPredicate();
                Value.IsPlayerAssignment = false;
                TemporaryVariable.Data = std::move(Value);
                AssociatedModule.ModuleScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_PlayerSelection>())
        {
            auto& PlayerSelectionVariable = DeclarationToVerify.GetType<VariableDeclaration_PlayerSelection>();
            p_VerifyGameInfoPredicate(AssociatedModule,PlayerSelectionVariable.Predicate,true,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_Variable_GameInfoPredicate Value = MQL_Variable_GameInfoPredicate();
                Value.IsPlayerAssignment = true;
                TemporaryVariable.Data = std::move(Value);
                AssociatedModule.ModuleScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_GameList>())
        {
            auto& GameListVariable = DeclarationToVerify.GetType<VariableDeclaration_GameList>();
            p_VerifyGameSelection(AssociatedModule,GameListVariable.Selection,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_LazyGameList Value = MQL_LazyGameList();
                TemporaryVariable.Data = std::move(Value);
                AssociatedModule.ModuleScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsEmpty())
        {
            assert(false && "Trying to verify empty VariableDeclaration");
        }
        else
        {
            assert(false && "VerifyVariableDeclaration doesn't cover all cases");
    
        }
        if(Diagnostics.size() > 0)
        {
            ReturnValue = false;
            OutDiagnostics.insert(OutDiagnostics.end(),std::make_move_iterator(Diagnostics.begin()),std::make_move_iterator(Diagnostics.end()));
            //OutDiagnostics = std::move(Diagnostics);
        }
        return(ReturnValue);
    }
    void MQLEvaluator::p_VerifyPlayerAssignment(MQL_Module& AssociatedModule,PlayerAssignment& AssignmentToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(AssignmentToVerify.AffectedPlayer != "")
        {
            if(!(AssignmentToVerify.AffectedPlayer == "Player1" || AssignmentToVerify.AffectedPlayer == "Player2"))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Only Player1/Player2 is allowed in player assignment";
                NewDiagnostic.range.start.line = AssignmentToVerify.PlayerPosition.Line;
                NewDiagnostic.range.start.character = AssignmentToVerify.PlayerPosition.ByteOffset;
                NewDiagnostic.range.end  = NewDiagnostic.range.start + AssignmentToVerify.AffectedPlayer.size();

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }  
            p_VerifyGameInfoPredicate(AssociatedModule,AssignmentToVerify.PlayerCondition,true,OutDiagnostics);
        } 
    }
    void MQLEvaluator::p_VerifyGameSelection(MQL_Module& AssociatedModule,GameSelection& SelectionToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        p_VerifyPlayerAssignment(AssociatedModule,SelectionToVerify.Assignment,OutDiagnostics);
        p_VerifyGameInfoPredicate(AssociatedModule,SelectionToVerify.GameCondition,false,OutDiagnostics);
    }
    void MQLEvaluator::p_VerifyResult(MQL_Module& AssociatedModule,Result& ResultToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(ResultToVerify.IsType<Result_Tabulate>())
        {
            for(auto const& Column : ResultToVerify.GetType<Result_Tabulate>().Columns)
            {
                ArgumentList ArgList;
                std::type_index OutType = typeid(nullptr);
                p_ConvertMetricComponent(AssociatedModule,ArgList,Column.Metric,OutDiagnostics,OutType);
            }
        }
    }
    bool MQLEvaluator::VerifySelection(MQL_Module& AssociatedModule,Selection& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        for(auto const& Set : SpecToVerify.Games.Using.GameSets)
        {
            if(!m_DBAdapter->GameSetExists(Set.Value))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Can't find game set with name \""+Set.Value+"\"";
                NewDiagnostic.range.start.line = Set.Position.Line;
                NewDiagnostic.range.start.character = Set.Position.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Set.Value.size();
                OutDiagnostics.push_back(std::move(NewDiagnostic));
            }
        }
        p_VerifyPlayerAssignment(AssociatedModule,SpecToVerify.Games.Assignment,Diagnostics);
        p_VerifyGameInfoPredicate(AssociatedModule,SpecToVerify.Games.GameCondition,false,Diagnostics);
        p_VerifyFilter(AssociatedModule,SpecToVerify.SituationFilter,Diagnostics);
        p_VerifyResult(AssociatedModule,SpecToVerify.Output,OutDiagnostics);
        if(Diagnostics.size() > 0)
        {
            ReturnValue = false;
            for(auto& Diagnostic : Diagnostics)
            {
                OutDiagnostics.push_back(std::move(Diagnostic));
            }
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::VerifyStatement(MQL_Module& AssociatedModule,Statement& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        if(SpecToVerify.IsType<Selection>())
        {
            ReturnValue = VerifySelection(AssociatedModule,SpecToVerify.GetType<Selection>(),OutDiagnostics);
        }
        else if(SpecToVerify.IsType<VariableDeclaration>())
        {
            ReturnValue = VerifyVariableDeclaration(AssociatedModule,SpecToVerify,OutDiagnostics);
        }
        else if(SpecToVerify.IsType<Import>())
        {
            ReturnValue = p_EvaluateImport(AssociatedModule,SpecToVerify.GetType<Import>(),OutDiagnostics);
        }
        else if(SpecToVerify.IsEmpty())
        {
            assert(false && "Trying to verify empty Statement");
        }
        else
        {
            assert(false && "VerifyStatement doesn't cover all cases");
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::VerifyModule(MQL_Module& AssociatedModule,Module& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        //ensures that the top scope isn't modified after the verification,
        //but still makes variables visible to other parts of the verification
        //TODO avoid copying...
        MQL_Module TempModule = MQL_Module(AssociatedModule);
        for(auto& Statement : SpecToVerify.Statements)
        {
            if(Statement.IsType<VariableDeclaration>())
            {
                ReturnValue = VerifyVariableDeclaration(TempModule,Statement,OutDiagnostics,true) && ReturnValue;
            }
            else
            {
                ReturnValue = VerifyStatement(TempModule,Statement,OutDiagnostics) && ReturnValue;
            }
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::p_EvaluateGameSelection(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;
        int TermOffset = 0;
        if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Direct>())
        {
            auto const& DirectData = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Direct>();
            int PlayerIndex = 0;
            auto const& Attribute = PredicateToEvaluate.Attribute.Parts;
            if(PredicateToEvaluate.Attribute.Parts[0].Value == "Player2")
            {
                PlayerIndex = 1;
            }
            SlippiGamePlayerInfo const& CurrentPlayer = GameInfo.PlayerInfo[InAssignment[PlayerIndex]];
            if(Attribute[0].Value == "Player1" || Attribute[0].Value == "Player2")
            {
                if(Attribute[1].Value == "Character")
                {
                    //ReturnValue = CurrentPlayer.Character == PredicateToEvaluate.Value;   
                    ReturnValue = h_Comp(CurrentPlayer.Character,DirectData.Comparison,DirectData.Value);
                } 
                else if(Attribute[1].Value == "Code")
                {
                    //ReturnValue = CurrentPlayer.Code == PredicateToEvaluate.Value;   
                    ReturnValue = h_Comp(CurrentPlayer.Code,DirectData.Comparison,DirectData.Value);
                }
                else if(Attribute[1].Value == "Tag")
                {
                    //ReturnValue = CurrentPlayer.Tag == PredicateToEvaluate.Value;   
                    ReturnValue = h_Comp(CurrentPlayer.Tag,DirectData.Comparison,DirectData.Value);
                }
                else
                {
                    assert(false && "Only Tag,Character,Code is recognized as player attribute");   
                }
            }
            else
            {
                if(Attribute[0].Value == "Stage")
                {
                    ReturnValue = GameInfo.Stage == DirectData.Value;
                }
                else if(Attribute[0].Value == "Date")
                {
                    if(DirectData.Comparison == "=" || DirectData.Comparison == "!=")
                    {
                        ReturnValue = GameInfo.Date >= DirectData.DateLowerBound && GameInfo.Date <= DirectData.DateHigherBound;
                        if(DirectData.Comparison == "!=")
                        {
                            ReturnValue = !ReturnValue;   
                        }
                    }
                    else
                    {
                        bool IsGreater = DirectData.Comparison.find('>') != DirectData.Comparison.npos;
                        bool IsEqual = DirectData.Comparison.find('=') != DirectData.Comparison.npos;
                        if( (IsGreater && !IsEqual) || (!IsGreater && IsEqual))
                        {
                            ReturnValue = h_Comp(GameInfo.Date,DirectData.Comparison,DirectData.DateHigherBound);
                        }
                        else
                        {
                            ReturnValue = h_Comp(GameInfo.Date,DirectData.Comparison,DirectData.DateLowerBound);
                        }
                    }
                    //ReturnValue = h_Comp(GameInfo.Date,PredicateToEvaluate.Operator,
                }
            }
        }
        else if(PredicateToEvaluate.Attribute.Parts.size() != 0)
        {
            MQL_Variable& Variable = AssociatedModule.ModuleScope.GetVariable(PredicateToEvaluate.Attribute);
            if(std::holds_alternative<MQL_LazyGameList>(Variable.Data))
            {
                MQL_LazyGameList& GameList = std::get<MQL_LazyGameList>(Variable.Data);
                if(!GameList.Evaluated)
                {
                    GameList.Games = p_RetrieveSpecGames(AssociatedModule,GameList.GamesToRetrieve);
                    std::sort(GameList.Games.begin(),GameList.Games.end(),[](MeleeGame const& lhs,MeleeGame const& rhs)
                            {
                                return(lhs.Metadata.GamePath < rhs.Metadata.GamePath);
                            });
                    GameList.Evaluated = true;
                }
                auto LowestIt = std::lower_bound(GameList.Games.begin(),GameList.Games.end(),GameInfo,
                        [](MeleeGame const& lhs,SlippiGameInfo const& rhs)
                        {
                            return(lhs.Metadata.GamePath < rhs.AbsolutePath);
                        });
                ReturnValue = LowestIt != GameList.Games.end() && LowestIt->Metadata.GamePath == GameInfo.AbsolutePath;
            }
            else if(std::holds_alternative<MQL_Variable_GameInfoPredicate>(Variable.Data))
            {
                MQL_Variable_GameInfoPredicate const& DereferencedPredicate = std::get<MQL_Variable_GameInfoPredicate>(Variable.Data);
                ReturnValue = p_EvaluateGameSelection(AssociatedModule,GameInfo,InAssignment,DereferencedPredicate.Predicate);
            }
        }
        else
        {
            //assert(false && "p_EvaluateGameSelection doesn't cover all cases for GameInfoPredicate.Data");
        }
        if(PredicateToEvaluate.Operator == "||")
        {
            ReturnValue = false;
        }
        for(auto const& ExtraTerm : PredicateToEvaluate.ExtraTerms)
        {
            if(PredicateToEvaluate.Operator == "||")
            {
                if(!ReturnValue)
                {
                    ReturnValue = p_EvaluateGameSelection(AssociatedModule,GameInfo,InAssignment,ExtraTerm);           
                }
            }
            else if(PredicateToEvaluate.Operator == "&&")
            {
                if(!p_EvaluateGameSelection(AssociatedModule,GameInfo,InAssignment,ExtraTerm))
                {
                    ReturnValue = false;   
                    break;
                }
            }
            else
            {
                assert(false && "Only valid operators are && and ||");   
            }
        }
        if(PredicateToEvaluate.Negated)
        {
            ReturnValue = !ReturnValue;
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::p_SatisfiesPlayerAssignment(MQL_Module& AssociatedModule,SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;

        if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Direct>())
        {
            auto const& DirectData = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Direct>();
            auto const& Attribute = PredicateToEvaluate.Attribute.Parts;
            if(Attribute[0].Value == "Character")
            {
                ReturnValue = PlayerInfo.Character == DirectData.Value;
            }
            else if(Attribute[0].Value == "Tag")
            {
                ReturnValue = PlayerInfo.Tag == DirectData.Value;
            }
            else if(Attribute[0].Value == "Code")
            {
                ReturnValue = PlayerInfo.Code == DirectData.Value;
            }
            else
            {
                assert(false && "Only Tag,Character,Code is recognized as player attribute");   
            }
        }
        else if(PredicateToEvaluate.Attribute.Parts.size() != 0)
        {
            GameInfoPredicate const& DereferencedPredicate =std::get<MQL_Variable_GameInfoPredicate>(
                    AssociatedModule.ModuleScope.GetVariable(PredicateToEvaluate.Attribute).Data).Predicate;
            ReturnValue = p_SatisfiesPlayerAssignment(AssociatedModule,PlayerInfo,DereferencedPredicate);
        }
        else
        {
            //assert(false && "p_SatisfiesPlayerAssignment doesn't cover all cases for PredicateToEvaluate.Data");
        }
        if(PredicateToEvaluate.Operator == "||")
        {
            ReturnValue = false;   
        }
        for(auto const& ExtraTerm : PredicateToEvaluate.ExtraTerms)
        {
            //errenous until parsing is fixed
            if(PredicateToEvaluate.Operator == "||")
            {
                if(!ReturnValue)
                {
                    ReturnValue = p_SatisfiesPlayerAssignment(AssociatedModule,PlayerInfo,ExtraTerm);
                }
            }
            else if(PredicateToEvaluate.Operator == "&&")
            {
                if(!p_SatisfiesPlayerAssignment(AssociatedModule,PlayerInfo,ExtraTerm))
                {
                    ReturnValue = false;   
                    break;
                }
            }
            else
            {
                assert(false && "Only valid operators are && and ||");   
            }
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::p_GetPlayerAssignments(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4])
    {
        //TODO Slow implementation, partly because of frame structure
        //Starts with determining which player is 1, then 2, etc
        //currently only has support to truly specify player1
        bool PlayerWasAssigned = false;
        for(int i = 0; i < 4;i++)
        {
            OutAssignemnts[i] = i;
        }
        //assigns player 1
        for(int i = 0; i < 4;i++)
        {
            if(p_SatisfiesPlayerAssignment(AssociatedModule,GameInfo.PlayerInfo[i],AssignemntToApply.PlayerCondition))
            {
                std::swap(OutAssignemnts[0],OutAssignemnts[i]);
                PlayerWasAssigned = true;
                break;
            }
        }
        //assigns player 2
        if(GameInfo.PlayerInfo[OutAssignemnts[1]].Character == "")
        {
            for(int i = 0; i < 4;i++)
            {
                if(i == OutAssignemnts[0])
                {
                    continue;
                }
                if(GameInfo.PlayerInfo[i].Character != "")
                {
                    std::swap(OutAssignemnts[i],OutAssignemnts[1]);   
                }
            }
        }
        return(PlayerWasAssigned);
    }
    void MQLEvaluator::p_ApplyAssignment(MeleeGame& GameToModify,char InAssignments[4])
    {
        for(auto& Frames : GameToModify.Frames)
        {
            //TODO inefficient
            PlayerFrameInfo FrameCopies[4] = {Frames.PlayerInfo[0],Frames.PlayerInfo[1],Frames.PlayerInfo[2],Frames.PlayerInfo[3]};
            for(int i = 0; i < 4;i++)
            {
                Frames.PlayerInfo[i] = FrameCopies[InAssignments[i]];
            }
        }
    }
    void MQLEvaluator::SetDBAdapter(MeleeGameDBAdapter* NewAdapter)
    {
        m_DBAdapter = std::move(NewAdapter);    
    }
    void MQLEvaluator::InitializeServers(std::vector<ServerInitilizationData> const& ServersToInitialize)
    {
        for(auto const& Server : ServersToInitialize)
        {
            SpecServer NewServer(Server.ExecutableName,Server.ExecutableArguments);
            int Index = m_SpecServers.size();
            std::vector<std::string> Filters = NewServer.GetExportedFilters();
            for(auto const& Filter : Filters)
            {
                m_FilterToServer[Filter] = Index;   
            }
            m_SpecServers.push_back(std::move(NewServer));
        }
    }
    void MQLEvaluator::SetRecorder(MeleeGameRecorder* NewRecorder)
    {
        m_Recorder = std::move(NewRecorder);       
    }
    void h_SwapPlayers(MeleeGame& GameToModify)
    {
        std::swap(GameToModify.Players[0],GameToModify.Players[1]);
        for(auto& Frame : GameToModify.Frames)
        {
            std::swap(Frame.PlayerInfo[0],Frame.PlayerInfo[1]);
        }
    }
    std::vector<MeleeGame> MQLEvaluator::p_RetrieveSpecGames( MQL_Module& AssociatedModule,GameSelection const& SpecToEvalaute)
    {
        std::vector<MeleeGame> ReturnValue;
        //TODO improve this, use SQL query so unneccesary game info doesn't need to be parsed
        std::vector<std::string> GameSets;
        for(auto const& Sets : SpecToEvalaute.Using.GameSets)
        {
            GameSets.push_back(Sets.Value);
        }
        std::vector<SlippiGameInfo> Candidates = m_DBAdapter->RetrieveGames("",GameSets);
        for(auto& Candidate : Candidates)
        {
            bool IsSwapped = false;
            char AssignmentIndexes[4] = {0,1,2,3};
            if(p_GetPlayerAssignments(AssociatedModule,Candidate,SpecToEvalaute.Assignment,AssignmentIndexes))
            {
                if(p_EvaluateGameSelection(AssociatedModule,Candidate,AssignmentIndexes,SpecToEvalaute.GameCondition))
                {
                    std::ifstream  GameData(Candidate.AbsolutePath,std::ios::in|std::ios::binary);
                    if(GameData.is_open())
                    {
                        MBUtility::MBFileInputStream InStream(&GameData);
                        MeleeGame GameToAdd;
                        MBError ParseResult = MeleeGame::ParseSlippiGame(InStream,GameToAdd);
                        GameToAdd.Metadata.GamePath = Candidate.AbsolutePath;
                        if(ParseResult)
                        {
                            p_ApplyAssignment(GameToAdd,AssignmentIndexes);   
                            ReturnValue.emplace_back(std::move(GameToAdd));
                        }
                    }
                }
            }
        }
        return(ReturnValue);
    }
    GameIntervall h_GetNegatedIntervall(GameIntervall InputIntevall,GameIntervall NegatedIntervall,std::vector<GameIntervall>& TotalIntervalls)
    {
        GameIntervall ReturnValue = InputIntevall;
        if(InputIntevall.FirstFrame < NegatedIntervall.FirstFrame && InputIntevall.LastFrame > NegatedIntervall.LastFrame)
        {
            GameIntervall IntervallToInsert = InputIntevall;
            IntervallToInsert.LastFrame = NegatedIntervall.FirstFrame;
            TotalIntervalls.push_back(IntervallToInsert);
            ReturnValue.FirstFrame = NegatedIntervall.LastFrame;
            ReturnValue.LastFrame = InputIntevall.LastFrame;
        }
        else
        {
            if(InputIntevall.FirstFrame < NegatedIntervall.FirstFrame)
            {
                ReturnValue.LastFrame = NegatedIntervall.FirstFrame;
            }
            else
            {
                ReturnValue.FirstFrame = NegatedIntervall.LastFrame;   
            }
        }
        if(ReturnValue.FirstFrame > ReturnValue.LastFrame)
        {
            ReturnValue.FirstFrame = ReturnValue.LastFrame;
        }
        return(ReturnValue);
    }
    //does a copy, maybe a bit to expensive
    std::vector<GameIntervall> h_GetNegatedIntervalls(std::vector<GameIntervall> InputIntervalls,
            std::vector<GameIntervall> const& NegatedIntervalls)
    {
        std::vector<GameIntervall>  ReturnValue;
        size_t InputIndex = 0;
        size_t NegatedIndex = 0;
        int InputFrame = 0;
        int OutputFrame = 0;
        while(InputIndex < InputIntervalls.size() && NegatedIndex < NegatedIntervalls.size())
        {
            auto& InputIntervall = InputIntervalls[InputIndex];
            auto const& NegatedIntervall = NegatedIntervalls[NegatedIndex];
            if(InputIntervall.In(NegatedIntervall))
            {
                InputIntervall = h_GetNegatedIntervall(InputIntervall,NegatedIntervall,ReturnValue);
                NegatedIndex++;
            }
            else
            {
               if(InputIntervall < NegatedIntervall)
               {
                    ReturnValue.push_back(InputIntervall);
                    InputIndex++;
               }    
               else
               {
                   NegatedIndex++;
               }
            }
        }
        while(InputIndex < InputIntervalls.size())
        {
            if(InputIntervalls[InputIndex].FirstFrame != InputIntervalls[InputIndex].LastFrame)
            {
                ReturnValue.push_back(InputIntervalls[InputIndex]);
            }
            InputIndex++;
        }
        return(ReturnValue);
    }
    void h_NormalizeIntervalls(std::vector<GameIntervall>& IntervallsToNormalize)
    {
        std::vector<GameIntervall> Result;
        if (IntervallsToNormalize.size() == 0)
        {
            return;
        }
        Result.push_back(IntervallsToNormalize.front());
        for (size_t i = 1; i < IntervallsToNormalize.size(); i++)
        {
            if (IntervallsToNormalize[i].FirstFrame <= Result.back().LastFrame)
            {
                Result.back().LastFrame = IntervallsToNormalize[i].LastFrame;
            }
            else
            {
                Result.push_back(IntervallsToNormalize[i]);
            }
        }
        IntervallsToNormalize = std::move(Result);
    }
    //std::vector<GameIntervall> SpecEvaluator::p_EvaluateGameIntervalls(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,Filter_Component const& FilterToUse,
    //        std::vector<GameIntervall> const& InputIntervalls,MeleeGame const& GameToFilter)
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    if(FilterToUse.FilterName.Parts.size() != 0)
    //    {
    //        //temporery until filters are properly vectorized
    //        if(FilterToUse.FilterName.Parts.size() == 1)
    //        {
    //            if(auto BuiltinFilter = m_BuiltinFilters.find(FilterToUse.FilterName.Parts[0].Value); 
    //                    BuiltinFilter != m_BuiltinFilters.end())
    //            {
    //                for(auto const& Intervall : InputIntervalls)
    //                {
    //                    ArgumentList Arguments(FilterToUse.ArgumentList);
    //                    Arguments.SetParentArgList(&ParentArgList);
    //                    auto NewIntervalls = BuiltinFilter->second(GameToFilter,Arguments,Intervall);
    //                    ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
    //                }
    //            }
    //            else if(auto ServerIndex = m_FilterToServer.find(FilterToUse.FilterName.Parts[0].Value);
    //                    ServerIndex != m_FilterToServer.end())
    //            {
    //                for(auto const& Intervall : InputIntervalls)
    //                {
    //                    auto NewIntervalls = m_SpecServers[ServerIndex->second].ExecuteFilter(FilterToUse.FilterName.Parts[0].Value,GameToFilter,Intervall);
    //                    ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
    //                }
    //            }
    //        }
    //        if(ReturnValue.size() == 0 && AssociatedModule.ModuleScope.HasVariable(FilterToUse.FilterName))
    //        {
    //            MQL_FilterDefinition const& DerferencedFilter = std::get<MQL_FilterDefinition>(
    //                    AssociatedModule.ModuleScope.GetVariable(FilterToUse.FilterName).Data);
    //            ArgumentList FilterArgument(FilterToUse.ArgumentList);
    //            FilterArgument.SetParentArgList(&ParentArgList);
    //            ArgumentList ParentEnvironment(DerferencedFilter.Arguments,FilterArgument);
    //            auto NewIntervalls = p_EvaluateGameIntervalls(AssociatedModule,ParentEnvironment,DerferencedFilter.Component,InputIntervalls,GameToFilter);
    //            ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
    //        }
    //        std::sort(ReturnValue.begin(),ReturnValue.end());
    //    }
    //    else if(FilterToUse.Operator == "|")
    //    {
    //        ReturnValue =  InputIntervalls;
    //    }
    //    //special case becuase the filter is always present, but doesn't neccesarially contain anything
    //    if(FilterToUse.FilterName.Parts.size()  == 0 && FilterToUse.ExtraTerms.size() == 0)
    //    {
    //        ReturnValue = InputIntervalls;
    //        return(ReturnValue);
    //    }
    //    //only present if not term
    //    for(auto const& ExtraFilter : FilterToUse.ExtraTerms)
    //    {
    //        if(FilterToUse.Operator == "&")
    //        {
    //            std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(AssociatedModule,
    //                    ParentArgList,ExtraFilter,InputIntervalls,GameToFilter); 
    //            std::vector<GameIntervall> NewReturnValue;
    //            NewReturnValue.resize(NewSituations.size()+ReturnValue.size());
    //            std::merge(NewSituations.begin(),NewSituations.end(),ReturnValue.begin(),ReturnValue.end(),NewReturnValue.begin());
    //            ReturnValue = std::move(NewReturnValue);
    //        }       
    //        else if(FilterToUse.Operator == "|")
    //        {
    //            std::vector<GameIntervall> NewIntervalls;
    //            NewIntervalls = p_EvaluateGameIntervalls(AssociatedModule,ParentArgList,ExtraFilter,ReturnValue,GameToFilter);
    //            std::swap(ReturnValue,NewIntervalls);
    //        }
    //        else if(FilterToUse.Operator == "+")
    //        {
    //               
    //        }
    //        else
    //        {
    //            assert(false  && "Trying to evaluate filter with extra terms and no valid operator");
    //        }
    //    }
    //    if(FilterToUse.Negated)
    //    {
    //        ReturnValue = h_GetNegatedIntervalls(InputIntervalls,ReturnValue);
    //    }
    //    return(ReturnValue);
    //}
    std::vector<GameIntervall> MQLEvaluator::p_EvaluateSubIntervalls(
            MQL_Module& AssociatedModule,
            MeleeGame const& InputGame,
            std::vector<GameIntervall> const& InputIntervalls,
            ArgumentList const& ArgList,
            MQL_Filter const& Filter)
    {
        std::vector<GameIntervall> ReturnValue;
        auto Parent = ArgList.GetParentArgList();
        if(Parent == nullptr)
        {
            ArgumentList TempList;
            return p_EvaluateGameIntervalls(AssociatedModule,InputGame,InputIntervalls,TempList,Filter);
        }
        else
        {
            return p_EvaluateGameIntervalls(AssociatedModule,InputGame,InputIntervalls,*Parent,Filter);
        }
        return ReturnValue;
    }
    std::vector<GameIntervall> MQLEvaluator::p_EvaluateGameIntervalls(
            MQL_Module& AssociatedModule,
            MeleeGame const& InputGame,
            std::vector<GameIntervall> const& InputIntervalls,
            ArgumentList const& ArgList,
            MQL_Filter const& Filter)
    {
        std::vector<GameIntervall> ReturnValue;
        bool Negated = false;
        if(Filter.IsType<MQL_IntervallExtractor>())
        {
            auto const& IntervallExtractor = Filter.GetType<MQL_IntervallExtractor>();
            Negated = IntervallExtractor.Negated;
            ArgumentList Arguments(IntervallExtractor.Args);
            Arguments.SetParentArgList(&ArgList);
            CallContext Context(AssociatedModule,*this);
            ReturnValue = IntervallExtractor.Filter.Filter(InputGame,Arguments,InputIntervalls,Context);
        }
        else if(Filter.IsType<MQL_FilterReference>())
        {
            auto const& FilterRef = Filter.GetType<MQL_FilterReference>();
            std::shared_ptr<MQL_FilterDefinition> DerferencedFilter = FilterRef.Filter;
            Negated = FilterRef.Negated;
            assert(m_LoadedModules.find(DerferencedFilter->AssociatedModule) != m_LoadedModules.end());
            auto FilterModule = m_LoadedModules[DerferencedFilter->AssociatedModule];
            DerferencedFilter->Arguments.SetParentArgList(&ArgList);
            auto NewIntervalls = p_EvaluateGameIntervalls(*FilterModule,InputGame,InputIntervalls,DerferencedFilter->Arguments,*DerferencedFilter->Component);
            DerferencedFilter->Arguments.SetParentArgList(nullptr);
            ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
        }
        else if(Filter.IsType<MQL_FilterCombiner>())
        {
            auto const& Combiner = Filter.GetType<MQL_FilterCombiner>();
            Negated = Combiner.Negated;
            if(Combiner.Type == OperatorType::Add)
            {
                ReturnValue = {};
                for(auto const& SubFilter : Combiner.Operands)
                {
                    std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(AssociatedModule,InputGame,InputIntervalls,ArgList,SubFilter);
                    std::vector<GameIntervall> NewReturnValue;
                    NewReturnValue.resize(NewSituations.size()+ReturnValue.size());
                    std::merge(NewSituations.begin(),NewSituations.end(),ReturnValue.begin(),ReturnValue.end(),NewReturnValue.begin());
                    ReturnValue = std::move(NewReturnValue);
                }
            }
            else if(Combiner.Type == OperatorType::Pipe)
            {
                ReturnValue = InputIntervalls;
                std::vector<GameIntervall> NewIntervalls;
                for(auto const& SubFilter : Combiner.Operands)
                {
                    NewIntervalls = p_EvaluateGameIntervalls(AssociatedModule,InputGame,ReturnValue,ArgList,SubFilter);
                    std::swap(ReturnValue,NewIntervalls);
                    if(ReturnValue.size() == 0)
                    {
                        break;   
                    }
                }
            }
            else
            {
                assert(false && "Filter combiner doesnt cover all operator types");   
            }
        }
        else if(Filter.IsType<MQL_MetricCombiner>())
        {
            auto const& Combiner = Filter.GetType<MQL_MetricCombiner>();
            Negated = Combiner.Negated;
            auto Results = EvaluateMetric(InputGame,InputIntervalls,ArgList,Filter);
            for(size_t i = 0; i < Results.size();i++)
            {
                if(std::get<bool>(Results[i].Data))
                {
                    ReturnValue.push_back(InputIntervalls[i]);
                }
            }
        }
        else
        {
            assert(false && "Execute Filter doesn't cover all cases");   
        }
        std::sort(ReturnValue.begin(),ReturnValue.end());
        if(Negated)
        {
            ReturnValue = h_GetNegatedIntervalls(InputIntervalls,ReturnValue);
            //probably redundant
            std::sort(ReturnValue.begin(),ReturnValue.end());
        }
        #ifndef NDBEG
        for(auto const& Intervall : ReturnValue)
        {
            assert(Intervall.LastFrame <= InputGame.Frames.size() -1);
        }
        #endif
        return ReturnValue;
    }



    template<typename LhsType,typename RhsType=LhsType,typename FuncType>
    inline std::vector<MQL_MetricVariable> h_CombineValues(FuncType Func,std::vector<MQL_MetricVariable> const& Lhs,std::vector<MQL_MetricVariable> const& Rhs)
    {
        assert(Lhs.size() != 0 && Rhs.size() != 0);
        std::vector<MQL_MetricVariable> ReturnValue;
        size_t ResultSize = std::max(Lhs.size(),Rhs.size());
        size_t LhsIndex = 0;
        size_t RhsIndex = 0;
        size_t IterationCount = 0;
        while(IterationCount < ResultSize)
        {
            ReturnValue.push_back(Func(std::get<LhsType>(Lhs[LhsIndex].Data),std::get<RhsType>(Rhs[RhsIndex].Data)));
            LhsIndex += 1;
            RhsIndex += 1;
            LhsIndex = LhsIndex % Lhs.size();
            RhsIndex = RhsIndex % Rhs.size();
            IterationCount += 1;
        }
        return ReturnValue;
    }

    class i_plus { public: template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs+Rhs;}};
    class i_equal {public: template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs==Rhs;}};
    class i_less { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs<Rhs;}};
    class i_less_eq { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs<=Rhs;}};
    class i_greater { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs>Rhs;}};
    class i_greater_eq { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs>=Rhs;}};
    class i_minus { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs-Rhs;}};
    class i_times { public:template<typename L,typename R> auto operator() (L const& Lhs,R const& Rhs) const {return Lhs*Rhs;}};
    
    template<typename LhsType,typename RhsType>
    std::vector<MQL_MetricVariable> h_EvaluateArithmeticExpression(OperatorType Op,std::vector<MQL_MetricVariable> const& Lhs,std::vector<MQL_MetricVariable> const& Rhs)
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        if(Op == OperatorType::Plus)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_plus(),Lhs,Rhs);
        }
        else if(Op == OperatorType::eq)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_equal(),Lhs,Rhs);
        }
        else if(Op == OperatorType::le)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_less(),Lhs,Rhs);
        }
        else if(Op == OperatorType::leq)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_less_eq(),Lhs,Rhs);
        }
        else if(Op == OperatorType::ge)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_greater(),Lhs,Rhs);
        }
        else if(Op == OperatorType::geq)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_greater_eq(),Lhs,Rhs);
        }
        else if(Op == OperatorType::Minus)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_minus(),Lhs,Rhs);
        }
        else if(Op == OperatorType::Times)
        {
            ReturnValue = h_CombineValues<LhsType,RhsType>(i_times(),Lhs,Rhs);
        }
        else
        {
            assert(false && "MetricCombiner for string doesnt cover all operator types");   
        }

        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::EvaluateMetric(
            MeleeGame const& InputGame,
            std::vector<GameIntervall> const& InputIntervalls,
            ArgumentList const& ArgList,
            MQL_Filter const& Filter)
    {

        std::vector<MQL_MetricVariable> ReturnValue;
        if(InputIntervalls.size() == 0)
        {
            return ReturnValue;
        }
        if(Filter.IsType<MQL_Filter_Literal>())
        {
            auto const& Literal = Filter.GetType<MQL_Filter_Literal>();
            if(Literal.Value.IsType<Literal_String>())
            {
                ReturnValue = {Literal.Value.GetType<Literal_String>().Value};   
            }
            else if(Literal.Value.IsType<Literal_Number>())
            {
                ReturnValue = {Literal.Value.GetType<Literal_Number>().Value};
            }
            else if(Literal.Value.IsType<Literal_Symbol>())
            {
                auto const& Value = ArgList[Literal.Value.GetType<Literal_Symbol>().Value];
                assert(Value.IsLiteral());
                auto const& Literal = Value.GetType<MQL_Filter_Literal>().Value;
                assert(!Literal.IsType<Literal_Symbol>());
                if(Literal.IsType<Literal_String>())
                {
                    ReturnValue = {Literal.GetType<Literal_String>().Value};
                }
                else if(Literal.IsType<Literal_Number>())
                {
                    ReturnValue = {Literal.GetType<Literal_Number>().Value};
                }
                else
                {
                    assert(false && "EvaluateMetric doesn't cover all cases for possible literal values");   
                }
            }
        }
        else if(Filter.IsType<MQL_Metric>())
        {
            auto const& Metric = Filter.GetType<MQL_Metric>();
            assert(Metric.Metric.Func != nullptr);
            ArgumentList Args (Metric.Args);
            Args.SetParentArgList(&ArgList);
            ReturnValue = Metric.Metric.Func(InputGame,Args,InputIntervalls);
        }
        else if(Filter.IsType<MQL_MetricCombiner>())
        {
            auto const& Combiner = Filter.GetType<MQL_MetricCombiner>();
            assert(Combiner.Operands.size() == 2);
            std::vector<MQL_MetricVariable> Lhs = EvaluateMetric(InputGame,InputIntervalls,ArgList,Combiner.Operands[0]);
            std::vector<MQL_MetricVariable> Rhs = EvaluateMetric(InputGame,InputIntervalls,ArgList,Combiner.Operands[1]);
            if(Combiner.LhsType == typeid(std::string))
            {
                if(Combiner.Type == OperatorType::Plus)
                {
                    ReturnValue = h_CombineValues<std::string>(std::plus<std::string>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::eq)
                {
                    ReturnValue = h_CombineValues<std::string>(std::equal_to<std::string>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::le)
                {
                    ReturnValue = h_CombineValues<std::string>(std::less<std::string>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::leq)
                {
                    ReturnValue = h_CombineValues<std::string>(std::less_equal<std::string>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::ge)
                {
                    ReturnValue = h_CombineValues<std::string>(std::greater<std::string>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::geq)
                {
                    ReturnValue = h_CombineValues<std::string>(std::greater_equal<std::string>(),Lhs,Rhs);
                }
                else
                {
                    assert(false && "MetricCombiner for string doesnt cover all operator types");   
                }
            }
            else if(Combiner.LhsType == typeid(float))
            {
                if(Combiner.RhsType == typeid(float))
                {
                    ReturnValue = h_EvaluateArithmeticExpression<float,float>(Combiner.Type,Lhs,Rhs);   
                }
                else if(Combiner.RhsType == typeid(int))
                {
                    ReturnValue = h_EvaluateArithmeticExpression<float,int>(Combiner.Type,Lhs,Rhs);   
                }
            }
            else if(Combiner.LhsType == typeid(int))
            {
                if(Combiner.RhsType == typeid(float))
                {
                    ReturnValue = h_EvaluateArithmeticExpression<int,float>(Combiner.Type,Lhs,Rhs);   
                }
                else if(Combiner.RhsType == typeid(int))
                {
                    ReturnValue = h_EvaluateArithmeticExpression<int,int>(Combiner.Type,Lhs,Rhs);   
                }
            }
            else
            {
                assert(false && "MetricCombiner should only be able to combine strings and floats");
            }
        }
        else
        {
            assert(false && "p_EvaluateMetric doesn't cover all cases");   
        }
        return ReturnValue;
    }
    void MQLEvaluator::EvaluateSelection(MQL_Module& AssociatedModule,Selection& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(m_DBAdapter == nullptr)
        {
            throw std::runtime_error("DBAdapter need to be set in order to evaluate SlippiSpec");  
        } 
        if(m_Recorder == nullptr)
        {
            throw std::runtime_error("Recorder need to be set in order to evaluate SlippiSpec");   
        }
        if(!VerifySelection(AssociatedModule,SpecToEvaluate,OutDiagnostics))
        {
            return;   
        }
        std::vector<MeleeGame> GamesToInspect = p_RetrieveSpecGames(AssociatedModule,SpecToEvaluate.Games);
        std::vector<std::vector<GameIntervall>> GameIntervalls;
        GameIntervalls.reserve(GamesToInspect.size());

        std::vector<RecordingInfo> GamesToRecord;
        ArgumentList TempList;
        MQL_Filter Filter = p_ConvertFilterComponent(AssociatedModule,TempList,SpecToEvaluate.SituationFilter.Component,OutDiagnostics);
        assert(OutDiagnostics.size() == 0);
        for(auto& Game : GamesToInspect)
        {
            //assumes are sorted
            auto Intervalls = 
                p_EvaluateGameIntervalls(AssociatedModule,Game,{GameIntervall(0,Game.Frames.size()-1)},TempList,Filter);
            h_NormalizeIntervalls(Intervalls);
            if(Intervalls.size() != 0)
            {
                RecordingInfo NewRecording;
                NewRecording.GamePath = Game.Metadata.GamePath;
                NewRecording.IntervallsToRecord = std::move(Intervalls);
                NewRecording.GameData = std::move(Game);
                GamesToRecord.push_back(std::move(NewRecording));
            }
        }
        if(SpecToEvaluate.Output.IsType<Result_Record>())
        {
            m_Recorder->RecordGames(GamesToRecord,SpecToEvaluate.Output.GetType<Result_Record>().OutFile);
        }
        else if(SpecToEvaluate.Output.IsType<Result_Tabulate>())
        {
            p_EvaluateTabulate(AssociatedModule,GamesToRecord,SpecToEvaluate.Output.GetType<Result_Tabulate>());
        }
    }
    std::string MQLEvaluator::p_MetricToName(Filter_Component const& FilterToConvert)
    {
        std::string ReturnValue = "";
        if(FilterToConvert.IsType<Filter_OperatorList>())
        {
            return p_MetricToName(FilterToConvert.GetType<Filter_OperatorList>().Components[0]);   
        }
        else if(FilterToConvert.IsType<Filter_Component_Func>())
        {
            return FilterToConvert.GetType<Filter_Component_Func>().FilterName.Parts.back().Value;
        }
        else
        {
            return "-";   
        }


        return ReturnValue;
    }
    void MQLEvaluator::p_EvaluateTabulate(MQL_Module& AssociatedModule,std::vector<RecordingInfo> const& FilterResult,Result_Tabulate const& TabulateInfo)
    {
        std::unique_ptr<MBUtility::MBOctetOutputStream> OutStream = std::make_unique<MBUtility::TerminalOutput>();
        if(TabulateInfo.OutFile.Value != "")
        {
            OutStream = std::make_unique<MBUtility::MBFileOutputStream>(TabulateInfo.OutFile.Value);
        }
        std::vector<MQL_Filter> Columns;
        for(auto const& Column : TabulateInfo.Columns)
        {
            std::vector<MBLSP::Diagnostic> Diagnostics;
            ArgumentList ArgList;
            std::type_index Type = typeid(nullptr);
            MQL_Filter NewFilter = p_ConvertMetricComponent(AssociatedModule,ArgList,Column.Metric,Diagnostics,Type);
            assert(Diagnostics.size() == 0 && "Diagnostics in p_EvaluateTabulate should be zero");
            if(Columns.size() != 0)
            {
                *OutStream<<",";   
            }
            if(Column.Name.Value != "")
            {
                *OutStream<<Column.Name.Value;
            }
            else
            {
                *OutStream<<p_MetricToName(Column.Metric);
            }
            Columns.push_back(std::move(NewFilter));
        }
        *OutStream<<"\n";
        ArgumentList ArgList;
        for(auto const& Game : FilterResult)
        {
            std::vector<std::vector<MQL_MetricVariable>> Values;
            for(auto const& Metric : Columns)
            {
                Values.push_back(EvaluateMetric(Game.GameData,Game.IntervallsToRecord,ArgList,Metric));
            }
            for(size_t i = 0; i < Game.IntervallsToRecord.size();i++)
            {
                for(size_t j = 0; j < Values.size();j++)
                {
                    if(j != 0)
                    {
                        *OutStream<<",";
                    }
                    size_t Index = std::min(i,Values[j].size()-1);
                    *OutStream<<Values[j][Index];
                }
                *OutStream<<"\n";
            }
        }
        OutStream->Flush();
    }
    void MQLEvaluator::EvaluateStatement(MQL_Module& AssociatedModule,Statement& StatementToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(StatementToEvaluate.IsType<Selection>())
        {
            EvaluateSelection(AssociatedModule,StatementToEvaluate.GetType<Selection>(),OutDiagnostics);
        }
        else if(StatementToEvaluate.IsType<VariableDeclaration>())
        {
            EvaluateVariableDeclaration(AssociatedModule,StatementToEvaluate,OutDiagnostics);
        }
        else if(StatementToEvaluate.IsType<Import>())
        {
            EvaluateImport(AssociatedModule,StatementToEvaluate.GetType<Import>(),OutDiagnostics);
        }
        else if(StatementToEvaluate.IsEmpty())
        {
            throw std::runtime_error("Cannot evaluate empty statement");
        }
        else 
        {
            assert(false && "EvaluateStatement doesn't cover all cases");
        }
    }
    void MQLEvaluator::EvaluateImport(MQL_Module& AssociatedModule,Import& ImportToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(!p_EvaluateImport(AssociatedModule,ImportToEvaluate,OutDiagnostics))
        {
            return;
        }
    }
    ModuleID MQLEvaluator::LoadModule(std::filesystem::path const& ModulePath)
    {
        ModuleID ReturnValue = -1;
        std::string CanonicalModulePath = MBUnicode::PathToUTF8(std::filesystem::canonical(ModulePath));
        if(auto ModIt = m_LoadedModulePaths.find(CanonicalModulePath); ModIt != m_LoadedModulePaths.end())
        {
            return(ModIt->second);
        }
        MQL_Module NewModule;
        NewModule.ModulePath = ModulePath;
        ReturnValue = m_CurrentModuleID;
        NewModule.ID = m_CurrentModuleID;
        m_CurrentModuleID++;
        m_LoadedModulePaths[CanonicalModulePath] = ReturnValue;
        if(std::filesystem::exists(CanonicalModulePath))
        {
            try
            {
                std::string ModuleContent = MBUtility::ReadWholeFile(CanonicalModulePath);
                m_Tokenizer.SetText(std::move(ModuleContent));
                FillModule(NewModule.Contents,m_Tokenizer);
                EvaluateModule(NewModule,NewModule.Contents,NewModule.LoadErrors);
            }
            catch(MBCC::ParsingException const& e)
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = e.what();
                NewDiagnostic.range.start.character = e.Position.ByteOffset;
                NewDiagnostic.range.start.line = e.Position.Line;
                NewDiagnostic.range.end = NewDiagnostic.range.start + 3;
                NewModule.LoadErrors.push_back(NewDiagnostic);
            }
            catch(std::exception const& e)
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Error evaluating module: "+std::string(e.what());
                NewDiagnostic.range.start.character = 0;
                NewDiagnostic.range.start.line = 0;
                NewDiagnostic.range.end = NewDiagnostic.range.start + 5;
                NewModule.LoadErrors.push_back(NewDiagnostic);
            }
        }
        else
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Path doesn't exist";
            NewDiagnostic.range.start.character = 0;
            NewDiagnostic.range.start.line = 0;
            NewDiagnostic.range.end = NewDiagnostic.range.start + 5;
            NewModule.LoadErrors.push_back(NewDiagnostic);
        }
        m_LoadedModules[ReturnValue] = std::make_shared<MQL_Module>(std::move(NewModule));
        return ReturnValue;
    }
    ModuleID MQLEvaluator::LoadEmptyModule()
    {
        ModuleID NewModuleID = m_CurrentModuleID;
        m_LoadedModules[NewModuleID] = std::make_shared<MQL_Module>();
        m_LoadedModules[NewModuleID]->ID = NewModuleID;
        m_CurrentModuleID++;
        return(NewModuleID);
    }
    MQL_Module& MQLEvaluator::GetModule(ModuleID ID)
    {
        auto It = m_LoadedModules.find(ID);
        if(It == m_LoadedModules.end())
        {
            throw std::runtime_error("Cannot find module with "+std::to_string(ID));   
        }
        return(*It->second);
    }
    void MQLEvaluator::EvaluateVariableDeclaration(MQL_Module& AssociatedModule,Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(!VerifyVariableDeclaration(AssociatedModule,SpecToEvaluate,OutDiagnostics))
        {
            return;
        }
        if(SpecToEvaluate.IsType<VariableDeclaration_Filter>())
        {
            MQL_Variable NewVariable;
            auto const& FilterDefinition = SpecToEvaluate.GetType<VariableDeclaration_Filter>();
            std::shared_ptr<MQL_FilterDefinition> NewVariableData = std::make_shared<MQL_FilterDefinition>();
            ArgumentList ParentArgList;
            for(auto const& Arg : FilterDefinition.Arguments.Arguments)
            {
            }
            NewVariableData->AssociatedModule = AssociatedModule.ID;
            NewVariableData->Arguments = p_ConvertArgList(AssociatedModule,ParentArgList,FilterDefinition.Arguments,OutDiagnostics);
            NewVariableData->Component = std::make_shared<MQL_Filter>(p_ConvertFilterComponent(AssociatedModule,NewVariableData->Arguments,FilterDefinition.Component,OutDiagnostics));
            NewVariable.Data = std::move(NewVariableData);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration>().GetBase().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameList>())
        {
            MQL_Variable NewVariable;
            MQL_LazyGameList Value;
            Value.GamesToRetrieve = SpecToEvaluate.GetType<VariableDeclaration_GameList>().Selection;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration>().GetBase().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameInfoPredicate>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = false;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_GameInfoPredicate>().Predicate;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration>().GetBase().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_PlayerSelection>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = true;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_PlayerSelection>().Predicate;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration>().GetBase().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsEmpty())
        {
            assert(false && "Trying to execute empty variable declaration");   
        }
        else 
        {
            assert(false && "EvaluateVariableDeclaration doesn't cover all cases");   
        }
    }
    void MQLEvaluator::EvaluateModule(MQL_Module& AssociatedModule,Module& ModuleToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(m_DBAdapter == nullptr)
        {
            throw std::runtime_error("DBAdapter need to be set in order to evaluate SlippiSpec");  
        } 
        if(m_Recorder == nullptr)
        {
            throw std::runtime_error("Recorder need to be set in order to evaluate SlippiSpec");   
        }
        if(!VerifyModule(AssociatedModule,ModuleToEvaluate,OutDiagnostics))
        {
            return;
        }
        for(auto& Statement : ModuleToEvaluate.Statements)
        {
            EvaluateStatement(AssociatedModule,Statement,OutDiagnostics);
        }
    }
    struct i_PunishInfo
	{
		float TotalRecievedPercent = 0;
		GameIntervall PunishIntervall;
	};
    //temp name for refactor
	std::vector<i_PunishInfo> h_ExtractPunishes2(MeleeGame const& GameToInspect, int PunisherIndex, int PunisheeIndex, float PercentThreshold)
	{
		std::vector<i_PunishInfo> ReturnValue;

		float PunisheeLastPercent = 0;
		float PunisherLastPercent = 0;
		float TotalPunishPercent = 0;
		
		int PunishBeginFrame = 0;
        int FrameIndex = 0;
		for (FrameInfo const& Frame : GameToInspect.Frames)
		{
			if (Frame.PlayerInfo[PunisheeIndex].Percent == 0 && PunisheeLastPercent > 0 || Frame.PlayerInfo[PunisherIndex].Percent == 0 && PunisherLastPercent > 0)
			{
				PunisherLastPercent = Frame.PlayerInfo[PunisherIndex].Percent;
				PunisheeLastPercent = Frame.PlayerInfo[PunisheeIndex].Percent;
				if (TotalPunishPercent >= PercentThreshold)
				{
					i_PunishInfo NewInfo;
					NewInfo.TotalRecievedPercent = TotalPunishPercent;
					NewInfo.PunishIntervall.FirstFrame = PunishBeginFrame;
					NewInfo.PunishIntervall.LastFrame = FrameIndex;
					ReturnValue.push_back(NewInfo);
				}
				TotalPunishPercent = 0;
			}
			if (PunisherLastPercent < Frame.PlayerInfo[PunisherIndex].Percent)
			{
				PunisherLastPercent = Frame.PlayerInfo[PunisherIndex].Percent;
				if (TotalPunishPercent >= PercentThreshold)
				{
					i_PunishInfo NewInfo;
					NewInfo.TotalRecievedPercent = TotalPunishPercent;
					NewInfo.PunishIntervall.FirstFrame = PunishBeginFrame;
					NewInfo.PunishIntervall.LastFrame = FrameIndex;
					ReturnValue.push_back(NewInfo);
				}
				TotalPunishPercent = 0;
			}
			if (PunisheeLastPercent < Frame.PlayerInfo[PunisheeIndex].Percent)
			{
				if (TotalPunishPercent == 0)
				{
					PunishBeginFrame = FrameIndex;
				}
				TotalPunishPercent += Frame.PlayerInfo[PunisheeIndex].Percent - PunisheeLastPercent;
				PunisheeLastPercent = Frame.PlayerInfo[PunisheeIndex].Percent;
			}
            FrameIndex++;
		}
		return(ReturnValue);
	}
    //std::vector<GameIntervall> h_GetBiggestPunishes(std::vector<i_PunishInfo> const& Punishes,int Count)
    //{
    //}
    std::vector<GameIntervall> MQLEvaluator::BiggestPunishes FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        float PercentThreshold = 40;
        int PunisherIndex = GetPlayerIndex(ExtraArguments);
        int PunisheeIndex = PunisherIndex == 1 ? 0 : 1;
        float ExtractCount = -1;
        if( ExtraArguments.HasNamedVariable("Percent"))
        {
            PercentThreshold = h_ParseFloat(ExtraArguments.GetNamedVariableString("Percent"));
        }
        if(ExtraArguments.HasNamedVariable("Count"))
        {
            ExtractCount = h_ParseExpandInteger(ExtraArguments.GetNamedVariableString("Count"));
        }
        std::vector<i_PunishInfo> Punishes = h_ExtractPunishes2(GameToInspect,PunisherIndex,PunisheeIndex,PercentThreshold);
        std::sort(Punishes.begin(),Punishes.end(),
                [](i_PunishInfo const& lhs,i_PunishInfo const& rhs){return(lhs.TotalRecievedPercent >= rhs.TotalRecievedPercent);});
        if(Punishes.size() > ExtractCount && ExtractCount != -1)
        {
            Punishes.resize(ExtractCount);
        }
        for(auto const& Punish : Punishes)
        {
            ReturnValue.push_back(Punish.PunishIntervall);   
        }
        return(ReturnValue);
    }
    bool MQLEvaluator::p_IsInteger(MQL_Filter const& Value)
    {
        if(!Value.IsLiteral())
        {
            return false;
        }
        auto const& Literal = Value.GetType<MQL_Filter_Literal>().Value;
        if(Literal.IsType<Literal_Number>())
        {
            return true;
        }
        else if(Literal.IsType<Literal_String>())
        {
            auto const& String = Literal.GetType<Literal_String>().Value;
            for(int i = 0; i < String.size();i++)
            {
                char CurrentChar = String[i];
                if(CurrentChar >= '0' && CurrentChar <= '9')
                {
                }
                else if(i == 0 && CurrentChar == '-' && String.size() > 1)
                {
                }
                else
                {
                    return false;   
                }
            }
            return true;
        }
        return false;
    }
    int MQLEvaluator::p_GetInteger(MQL_Filter const& Value)
    {
        int ReturnValue = 0;
        auto const& Literal = Value.GetType<MQL_Filter_Literal>();
        if(Literal.Value.IsType<Literal_Number>())
        {
            return Literal.Value.GetType<Literal_Number>().Value;   
        }
        else if(Literal.Value.IsType<Literal_String>())
        {
            return std::stoi(Literal.Value.GetType<Literal_String>().Value);
        }
        else
        {
            throw std::runtime_error("Cannot convert non string value to an integer");   
        }
        return ReturnValue;
    }
    bool MQLEvaluator::p_IsFloat(MQL_Filter const& Value)
    {
        if(!Value.IsLiteral())
        {
            return false;
        }
        auto const& Literal = Value.GetType<MQL_Filter_Literal>().Value;
        if(Literal.IsType<Literal_String>())
        {
            auto const& String = Literal.GetType<Literal_String>().Value;
            bool HasDot = false;
            for(int i = 0; i < String.size();i++)
            {
                char CurrentChar = String[i];
                if(CurrentChar >= '0' && CurrentChar <= '9')
                {
                }
                else if(CurrentChar == '.' && !HasDot)
                {
                    HasDot = true;
                }
                else if(i == 0 && CurrentChar == '-' && String.size() > 1)
                {
                }
                else
                {
                    return false;   
                }
            }
            return true;
        }
        return false;
    }
    float MQLEvaluator::p_GetFloat(MQL_Filter const& Value)
    {
        return std::stof(p_GetString(Value));
    }
    bool MQLEvaluator::p_IsString(MQL_Filter const& Value)
    {
        return Value.IsLiteral() && Value.GetType<MQL_Filter_Literal>().Value.IsType<Literal_String>();
    }
    std::string MQLEvaluator::p_GetString(MQL_Filter const& Value)
    {
        return Value.GetType<MQL_Filter_Literal>().Value.GetType<Literal_String>().Value;
    }
    void MQLEvaluator::PlayerArgChecker(ArgumentErrors& OutErrors,ArgumentList const& Args)
    {
        OutErrors.ValidKeys.insert("Player");
        try
        {
            GetPlayerIndex(Args);
        }
        catch(std::exception const& e)
        {
            OutErrors.KeyErrors["Player"] = "Invalid player specification, must be a integer as a string or literal between 0 and 3";
        }
    }
    void MQLEvaluator::MoveArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        if(Args.PositionalCount() != 1)
        {
            OutError.GenericErrors.push_back("Exactly one positional argument is with the move name");
        }
        else
        {
            MQL_Filter const& MoveValue = Args[0];
            if(!MoveValue.IsLiteral())
            {
                OutError.GenericErrors.push_back("First argument is not of string type");
                return;
            }
            auto const& Literal = MoveValue.GetType<MQL_Filter_Literal>();
            if(!Literal.Value.IsType<Literal_String>())
            {
                OutError.GenericErrors.push_back("First argument is not of string type");
                return;
            }
            MBAttackID Move = StringToMBAttackID(Literal.Value.GetType<Literal_String>().Value);
            if(Move == MBAttackID::None)
            {
                OutError.PositionalErrors[0] = "Invalid move name \""+Literal.Value.GetType<Literal_String>().Value+"\"";
            }
        }
    }
    void MQLEvaluator::ShieldArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        OutError.ValidKeys.insert("Shielding");
        OutError.ValidKeys.insert("Shieldstun");
    }
    void MQLEvaluator::ExpandArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        OutError.ValidKeys.insert({"Left","Right"});
        for(auto const& Key : {"Left","Right"})
        {
            if(Args.HasNamedVariable(Key))
            {
                if(!p_IsInteger(Args[Key]))
                {
                    OutError.KeyErrors[Key] = Key+std::string(" must be an integer or a string convertible to a integer"); 
                }
            }
        }
    }
    void MQLEvaluator::PlayerFlagArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        for(int i = 0; i < Args.PositionalCount();i++)
        {
            auto const& Value = Args[i];
            if(!p_IsString(Value))
            {
                OutError.PositionalErrors[i] = "Positional arguments must be of string type";
            }
            auto const& Argument = p_GetString(Value);
            if( !(Argument == "Airborne" || Argument == "Hitlag" || Argument == "FastFall"))
            {
                OutError.PositionalErrors[i] = "Invalid player flag";
            }
        }
    }
    void MQLEvaluator::ActionStateArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        if(Args.PositionalCount() != 1)
        {
            OutError.GenericErrors.push_back("Exactly one positional argument required with action state name");
        }
        else
        {
            MQL_Filter const& MoveValue = Args[0];
            if(!p_IsString(MoveValue))
            {
                OutError.GenericErrors.push_back("Argument is not of string type");
                return;
            }
            MBActionState Move = StringToMBActionState(p_GetString(MoveValue));
            if(Move == MBActionState::None)
            {
                OutError.PositionalErrors[0] = "Invalid action state name \""+p_GetString(MoveValue)+"\"";
            }
        }
    }
    void MQLEvaluator::UntilArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        OutError.ValidKeys.insert({"State","Flag","Pred","Skip"});
        if(!Args.HasNamedVariable("State") && 
                !Args.HasNamedVariable("Flag") && !Args.HasNamedVariable("Pred"))
        {
            OutError.GenericErrors.push_back("Until requires either 'State','Flag' or 'Pred' key argument");
        }
        if(Args.HasNamedVariable("State"))
        {
            MBActionState StateToCheck = MBActionState::None;
            auto const& StateValue = Args["State"];
            if(!p_IsString(StateValue))
            {
                OutError.KeyErrors["State"] = "State argument must be of string type";
            }
            else
            {
                StateToCheck = StringToMBActionState(p_GetString(StateValue));
                if(StateToCheck == MBActionState::None)
                {
                    OutError.KeyErrors["State"] = "Invalid state name";
                }
            }
        }
        if(Args.HasNamedVariable("Flag"))
        {
            auto const& FlagValue = Args["Flag"];
            if(!p_IsString(FlagValue))
            {
                OutError.KeyErrors["Flag"] = "Flag argument must be of string type";
            }
            else
            {
                auto const& Value = p_GetString(FlagValue);
                if( !(Value == "Airborne" || Value == "NoAirborne"))
                {
                    OutError.KeyErrors["Flag"] = "Invalid flag value";
                }
            }
        }
        if(Args.HasNamedVariable("Skip"))
        {
            auto const& SkipVar = Args["Skip"];
            if(!p_IsInteger(SkipVar))
            {
                OutError.KeyErrors["Skip"] = "Skip value must be an integer or string convertible to integer";
            }
            else
            {
                int Skip = p_GetInteger(SkipVar);
                if(Skip < 0)
                {
                    OutError.KeyErrors["Skip"] = "Skip value cannot be negative";
                }
            }
        }
        //overrides other options
        if(Args.HasNamedVariable("Pred"))
        {
            auto const& PredValue = Args["Pred"];
            if(!(PredValue.IsFilter()  || PredValue.IsMetric()))
            {
                OutError.KeyErrors["Pred"] = "Pred argument must be a filter";
            }
        }
    }
    void MQLEvaluator::HasArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        if(Args.PositionalCount() != 1)
        {
            OutError.GenericErrors.push_back("Has requries exactly one argument: the filter to apply");   
            return;
        }
        auto const& FilterValue = Args[0];
        if(!(FilterValue.IsFilter() || FilterValue.IsMetric()))
        {
            OutError.PositionalErrors[0] = "Argument must be a filter";   
        }
    }
    void MQLEvaluator::PunishArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        OutError.ValidKeys.insert({"Count","Percent"});
        if(Args.HasNamedVariable("Count"))
        {
            auto const& CountValue = Args["Count"];
            if(!p_IsInteger(CountValue))
            {
                OutError.KeyErrors["Count"] = "Count must be of integer type";
            }
            else
            {
                int Count = p_GetInteger(CountValue);   
                if(Count < 0)
                {
                    OutError.KeyErrors["Count"] = "Count cannot be negative";
                }
            }
        }
        if(Args.HasNamedVariable("Percent"))
        {
            auto const& PercentValue = Args["Percent"];
            if(!p_IsFloat(PercentValue))
            {
                OutError.KeyErrors["Percent"] = "Percent must be a float or string convertible to float";
            }
            else
            {
                int Percent = p_GetFloat(PercentValue);   
                if(Percent < 0)
                {
                    OutError.KeyErrors["Percent"] = "Percent cannot be negative";
                }
            }
        }
    }
    void MQLEvaluator::CorneredArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        OutError.ValidKeys.insert({"Margin","StageMargin","OffstageMargin","Distance"});
        for(auto const& Key : {"Margin","StageMargin","OffstageMargin","Distance"})
        {
            if(Args.HasNamedVariable(Key))
            {
                auto const& KeyValue = Args[Key];
                if(!p_IsFloat(KeyValue))
                {
                    OutError.KeyErrors[Key] = Key + std::string(" must be a float or a string convertible to a float");   
                    continue;
                }
            }
        }
    }
    void MQLEvaluator::OtherArgChecker(ArgumentErrors& OutError,ArgumentList const& Args)
    {
        if(Args.PositionalCount() != 1)
        {
            OutError.GenericErrors.push_back("Other requires exactly 1 positional option");   
            return;
        }
        auto const& PosOption = Args[0];
        if(!PosOption.IsLiteral())
        {
            OutError.PositionalErrors[0] = "Must be of integer type";
        }
        else
        {
            auto const& Literal = PosOption.GetType<MQL_Filter_Literal>().Value;
            if(!Literal.IsType<Literal_Number>())
            {
                OutError.PositionalErrors[0] = "Must be of integer type";
            }
        }
    }
    int MQLEvaluator::GetPlayerIndex(ArgumentList const& ExtraArguments)
    {
        int ReturnValue = 0;
        if(ExtraArguments.HasNamedVariable("Player"))
        {
            std::string PlayerString = ExtraArguments.GetNamedVariableString("Player");
            if(PlayerString == "0")
            {

            }
            else if(PlayerString == "1")
            {
                ReturnValue = 1;
            }
            else
            {
                throw std::runtime_error("PlayerIndex can only be 1 or 0");   
            }
        }
        return(ReturnValue);
    }
    //std::vector<GameIntervall> MQLEvaluator::HasMove(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    if(ExtraArguments.PositionalCount() == 0)
    //    {
    //        throw std::runtime_error("HasMove requires the move to search for as the first positional argument");
    //    }
    //    MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
    //    int PlayerIndex = GetPlayerIndex(ExtraArguments);
    //    for(int i = IntervallToInspect.FirstFrame; i <= IntervallToInspect.LastFrame;i++)
    //    {
    //        if(GameToInspect.Frames[i].PlayerInfo[PlayerIndex].ActiveAttack == Attack)
    //        {
    //            ReturnValue.push_back(IntervallToInspect);
    //            break;
    //        }
    //    }
    //    return(ReturnValue);
    //}
    std::vector<GameIntervall> MQLEvaluator::Move FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("Move requries exactly 1 positional option, the name of the move to inspect");
        }
        MBAttackID Move = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,
                [&](FrameInfo const& Frame)
                {
                    return(Frame.PlayerInfo[PlayerIndex].ActiveAttack == Move);
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> MQLEvaluator::InShield FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        bool CheckShielding = ExtraArguments.HasNamedVariable("Shielding");
        bool CheckShieldstun = ExtraArguments.HasNamedVariable("Shieldstun");
        if(!CheckShielding && !CheckShieldstun)
        {
            CheckShielding = true;   
            CheckShieldstun = true;   
        }
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,
                [&](FrameInfo const& Frame)
                {
                    bool ReturnValue = false;
                    if(CheckShielding)
                    {
                        ReturnValue = ReturnValue || Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::Shielding;
                    }
                    if(CheckShieldstun)
                    {
                        ReturnValue = ReturnValue || Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::ShieldStun;
                    }
                    return(ReturnValue);
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> MQLEvaluator::Until FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        if(!ExtraArguments.HasNamedVariable("State") && 
                !ExtraArguments.HasNamedVariable("Flag") && !ExtraArguments.HasNamedVariable("Pred"))
        {
            throw std::runtime_error("Until requires requires either 'State' key argument or 'Flag' key argument");
        }
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionState StateToCheck = MBActionState::None;
        if(ExtraArguments.HasNamedVariable("State"))
        {
            StateToCheck = StringToMBActionState(ExtraArguments.GetNamedVariableString("State"));
        }
        //only airborne as of now
        bool CheckAirborne = false;
        bool Airborne = false;
        if(ExtraArguments.HasNamedVariable("Flag"))
        {
            std::string FlagValue = ExtraArguments.GetNamedVariableString("Flag");
            CheckAirborne = true;
            if(FlagValue == "Airborne")
            {
                Airborne = true;
            }
            else if(FlagValue  == "NoAirborne")
            {
                Airborne = false;
            }
            else
            {
                throw std::runtime_error("Unknown state flag \""+FlagValue+"\"");   
            }
        }
        int Skip = -1;
        if(ExtraArguments.HasNamedVariable("Skip"))
        {
            Skip = h_ParseExpandInteger(ExtraArguments.GetNamedVariableString("Skip"));
            if(Skip < 0)
            {
                throw std::runtime_error("Cannot skip by negative number");   
            }
        }
        //overrides other options
        if(ExtraArguments.HasNamedVariable("Pred"))
        {
            MQL_Filter const& Predicate = ExtraArguments["Pred"];
            if(Predicate.IsMetric())
            {
                for(auto const& InputIntervall : IntervallsToInspect)
                {
                    GameIntervall Intervall;
                    Intervall.FirstFrame = Skip == -1 ? InputIntervall.FirstFrame : InputIntervall.FirstFrame+Skip;
                    Intervall.LastFrame = InputIntervall.LastFrame;
                    if(Intervall.FirstFrame > Intervall.LastFrame)
                    {
                        continue;   
                    }
                    //parent of this arglist is the metric supplied as argument scope
                    auto MetricResults = Context.GetEvaluator().EvaluateMetric(GameToInspect,h_SplitIntervall(Intervall),
                            ExtraArguments.GetParentArgList() == nullptr ? ExtraArguments : *ExtraArguments.GetParentArgList() ,Predicate);
                    int IntervallEnd = Intervall.FirstFrame;
                    for(auto const& Result : MetricResults)
                    {
                        assert(std::holds_alternative<bool>(Result.Data));
                        if(std::get<bool>(Result.Data))
                        {
                            break;   
                        }
                        IntervallEnd++;
                    }
                    Intervall.LastFrame = std::min(size_t(IntervallEnd),GameToInspect.Frames.size()-1);
                    ReturnValue.push_back(Intervall);
                }
            }
            else if(Predicate.IsFilter())
            {
                for(auto const& InputIntervall : IntervallsToInspect)
                {
                    GameIntervall Intervall;
                    Intervall.FirstFrame = Skip == -1 ? InputIntervall.FirstFrame : InputIntervall.FirstFrame+Skip;
                    Intervall.LastFrame = InputIntervall.LastFrame;
                    if(Intervall.FirstFrame > Intervall.LastFrame)
                    {
                        continue;   
                    }
                    auto FilterResult = Context.GetEvaluator().p_EvaluateSubIntervalls(Context.GetModule(),GameToInspect,{Intervall},ExtraArguments,Predicate);
                    if(FilterResult.size() > 0)
                    {
                        Intervall.LastFrame = FilterResult.front().FirstFrame;   
                    }
                    ReturnValue.push_back(Intervall);
                }
            }
            return ReturnValue;
        }
        for(auto const& Intervall : IntervallsToInspect)
        {
            GameIntervall IntervallToInsert = Intervall;
            for(int i = Intervall.FirstFrame + ( Skip != -1 ? Skip : 1); i < Intervall.LastFrame;i++)
            {
                bool IsEnd = true;
                if(StateToCheck != MBActionState::None)
                {
                    IsEnd = GameToInspect.Frames[i].PlayerInfo[PlayerIndex].ActionState == StateToCheck;
                }
                if(CheckAirborne)
                {
                    IsEnd = IsEnd && GameToInspect.Frames[i].PlayerInfo[PlayerIndex].StateFlags.Airborne == Airborne;
                }
                if(IsEnd)
                {
                    IntervallToInsert.LastFrame = i;
                    break;
                }
            }
            ReturnValue.push_back(IntervallToInsert);
        }
        return(ReturnValue);
    }
    std::vector<GameIntervall> MQLEvaluator::ActionState FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("ActionState requires exactly 1 positional argument, the name of the action state");
        }
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionState StateToCheck = StringToMBActionState(ExtraArguments.GetPositionalArgumentString(0));
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,
                [&](FrameInfo const& Frame){return(Frame.PlayerInfo[PlayerIndex].ActionState == StateToCheck);});
        return(ReturnValue);
    }
    //std::vector<GameIntervall> MQLEvaluator::HasState FILTER_ARGLIST
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    auto Intervalls = ActionState(GameToInspect,ExtraArguments,IntervallToInspect);
    //    if(Intervalls.size() != 0)
    //    {
    //        return {IntervallToInspect};   
    //    }
    //    return ReturnValue;
    //}
    std::vector<GameIntervall> MQLEvaluator::Expand FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        int LeftExpand = 0;
        int RightExpand = 0;
        if(ExtraArguments.PositionalCount() > 0)
        {
            int TotalExpandSize = h_ParseExpandInteger(ExtraArguments.GetPositionalArgumentString(0));
            LeftExpand = -TotalExpandSize;
            RightExpand = TotalExpandSize;
        }
        if(ExtraArguments.HasNamedVariable("Right"))
        {
            RightExpand = h_ParseExpandInteger(ExtraArguments.GetNamedVariableString("Right"));
        }
        if(ExtraArguments.HasNamedVariable("Left"))
        {
            LeftExpand = -h_ParseExpandInteger(ExtraArguments.GetNamedVariableString("Left"));   
        }
        //automatically inverted
        for(auto const& Intervall : IntervallsToInspect)
        {
            auto IntervallToInspect = Intervall;
            if(LeftExpand == 1)
            {
                IntervallToInspect.FirstFrame = 0;
            }
            else
            {
                IntervallToInspect.FirstFrame += LeftExpand;
            }
            if(RightExpand == -1)
            {
                IntervallToInspect.LastFrame = GameToInspect.Frames.size()-1;
            }
            else
            {
                IntervallToInspect.LastFrame += RightExpand;
            }
            if(IntervallToInspect.FirstFrame < 0)
            {
                IntervallToInspect.FirstFrame = 0;   
            }
            if(IntervallToInspect.LastFrame >= GameToInspect.Frames.size())
            {
                IntervallToInspect.LastFrame = GameToInspect.Frames.size()-1;
            }
            if(IntervallToInspect.FirstFrame < IntervallToInspect.LastFrame)
            {
                ReturnValue.push_back(IntervallToInspect);
            }
        }
        return(ReturnValue);
           
    }
    std::vector<GameIntervall> MQLEvaluator::HitBy FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        int OpponentIndex = PlayerIndex == 0 ? 1 : 0;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("HitBy requires exactly 1 positional argument, the name of the hit move");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        assert(Attack != MBAttackID::None);
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,[&]
                (FrameInfo const& Frame)
                {
                    return((Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::ShieldStun || Frame.PlayerInfo[PlayerIndex].StateFlags.InHitlag)
                            && Frame.PlayerInfo[OpponentIndex].ActiveAttack == Attack);
                });
        return(ReturnValue);
    }
    //std::vector<GameIntervall> MQLEvaluator::HasHitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    int PlayerIndex = GetPlayerIndex(ExtraArguments);
    //    int OpponentIndex = PlayerIndex == 0 ? 1 : 0;
    //    if(ExtraArguments.PositionalCount() != 1)
    //    {
    //        throw std::runtime_error("HasHitBy requires exactly 1 positional argument, the name of the hit move");
    //    }
    //    MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
    //    for(int i = IntervallToInspect.FirstFrame; i < IntervallToInspect.LastFrame;i++)
    //    {
    //        FrameInfo const& Frame = GameToInspect.Frames[i];
    //        if((Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::ShieldStun || Frame.PlayerInfo[PlayerIndex].StateFlags.InHitlag)
    //                        && Frame.PlayerInfo[OpponentIndex].ActiveAttack == Attack)
    //        {
    //            ReturnValue = {IntervallToInspect};
    //            break;
    //        }
    //    }
    //    return(ReturnValue);
    //}
   
    struct i_StageBoundaryInfo
    {
        float OffstageLeft = 0;
        float OffstageRight = 0;
    };
    std::pair<StageID,i_StageBoundaryInfo> i_StageBoundaries[] = { 
        {StageID::Battlefield,{-75,75}},
        {StageID::DreamLandN64,{-80,80}},
        {StageID::YoshisStory,{-60,60}},
        {StageID::FountainOfDreams,{-70,70}},
        {StageID::FinalDestination,{-90,90}},
        {StageID::PokemonStadium,{-90,90}},
    };
    i_StageBoundaryInfo const& i_GetStageBoundaryInfo(StageID IDToSearch)
    {
        for(auto const& Info : i_StageBoundaries)
        {
            if(Info.first == IDToSearch)
            {
                return(Info.second);   
            }
        }
        throw std::runtime_error("Error finding boundary info for stage with name \""+StageIDToString(IDToSearch)+"\"");
    };
    std::vector<GameIntervall> MQLEvaluator::Offstage FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        i_StageBoundaryInfo const& StageInfo = i_GetStageBoundaryInfo(GameToInspect.Stage);
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,[&](FrameInfo const& Frame)
                {
                    return(Frame.PlayerInfo[PlayerIndex].PlayerPosition.x < StageInfo.OffstageLeft || Frame.PlayerInfo[PlayerIndex].PlayerPosition.x > StageInfo.OffstageRight);
                });
        return(ReturnValue);
    }
    //std::vector<GameIntervall> MQLEvaluator::Length(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    if(ExtraArguments.PositionalCount() == 0)
    //    {
    //        throw std::runtime_error("Error in length filter: requires the minimum length of the intervall as the first positiional argument");
    //    }
    //    int Length = h_ParseExpandInteger(ExtraArguments.GetPositionalArgumentString(0));
    //    if(IntervallToInspect.LastFrame-IntervallToInspect.FirstFrame >= Length)
    //    {
    //        ReturnValue = {IntervallToInspect};
    //    }
    //    return(ReturnValue);
    //}
    inline bool h_InIntervall(float xleft,float xright,float x)
    {
        return x <= xright && x >= xleft;
    } 
    std::vector<GameIntervall> MQLEvaluator::Cornered FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        int OpponentIndex = PlayerIndex == 0 ? 1 : 0;
        float OpponentDistance = 8;
        i_StageBoundaryInfo const& StageInfo = i_GetStageBoundaryInfo(GameToInspect.Stage);
        float XStageMargin = 15;
        float XOffstageMargin = 5;
        if(ExtraArguments.HasNamedVariable("Margin"))
        {
            float MarginValue = h_ParseFloat(ExtraArguments.GetNamedVariableString("Margin")); 
            XStageMargin = MarginValue;
            XOffstageMargin = MarginValue;
        }
        else
        {
            if(ExtraArguments.HasNamedVariable("StageMargin"))
            {
                XStageMargin = h_ParseFloat(ExtraArguments.GetNamedVariableString("StageMargin"));
            }
            if(ExtraArguments.HasNamedVariable("OffstageMargin"))
            {
                XOffstageMargin = h_ParseFloat(ExtraArguments.GetNamedVariableString("OffstageMargin"));
            }
        }
        if(ExtraArguments.HasNamedVariable("Distance"))
        {
            OpponentDistance = h_ParseFloat(ExtraArguments.GetNamedVariableString("Distance")); 
        }
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,[&](FrameInfo const& Frame)
                {
                    float CurrentDistance = std::hypot(Frame.PlayerInfo[PlayerIndex].PlayerPosition.x-Frame.PlayerInfo[OpponentIndex].PlayerPosition.x,
                                Frame.PlayerInfo[PlayerIndex].PlayerPosition.y-Frame.PlayerInfo[OpponentIndex].PlayerPosition.y);
                    if(CurrentDistance > OpponentDistance)
                    {
                        return false;
                    }
                    bool InRightIntervall = h_InIntervall(StageInfo.OffstageRight-XStageMargin,StageInfo.OffstageRight+XOffstageMargin,Frame.PlayerInfo[PlayerIndex].PlayerPosition.x);
                    if(InRightIntervall && Frame.PlayerInfo[PlayerIndex].PlayerPosition.x > Frame.PlayerInfo[OpponentIndex].PlayerPosition.x)
                    {
                        return(true);
                    }
                    bool InLeftIntervall = h_InIntervall(StageInfo.OffstageLeft-XOffstageMargin,StageInfo.OffstageLeft+XStageMargin,Frame.PlayerInfo[PlayerIndex].PlayerPosition.x);
                    if(InLeftIntervall && Frame.PlayerInfo[PlayerIndex].PlayerPosition.x < Frame.PlayerInfo[OpponentIndex].PlayerPosition.x)
                    {
                        return(true);
                    }
                    return false;
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> MQLEvaluator::Has FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("Has filter requires exactly 1 argument: the filter to apply");   
        }
        MQL_Filter const& Filter = ExtraArguments[0];
        if(!(Filter.IsFilter() || Filter.IsMetric()))
        {
            throw std::runtime_error("First argument either has to be a intervall extractor, or metric");
        }
        std::vector<GameIntervall> IntervallVector = {GameIntervall()};
        ReturnValue.reserve(IntervallsToInspect.size());
        for(auto const& Intervall : IntervallsToInspect)
        {
            IntervallVector[0] = Intervall;
            if(Filter.IsFilter())
            {
                auto SubsetIntervalls = Context.GetEvaluator().p_EvaluateSubIntervalls(Context.GetModule(),GameToInspect,IntervallVector,ExtraArguments,Filter);
                if(SubsetIntervalls.size() > 0)
                {
                    ReturnValue.push_back(Intervall);   
                }
            }
            else
            {
                auto MetricResults = Context.GetEvaluator().EvaluateMetric(GameToInspect,h_SplitIntervall(Intervall),ExtraArguments,Filter);
                for(auto const& Result : MetricResults)
                {
                    assert(std::holds_alternative<bool>(Result.Data));
                    if(std::get<bool>(Result.Data))
                    {
                        ReturnValue.push_back(Intervall);
                        break;
                    }
                }
            }
        }
        return ReturnValue;
    }
    std::vector<GameIntervall> MQLEvaluator::Normalize FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue = IntervallsToInspect;
        h_NormalizeIntervalls(ReturnValue);
        return ReturnValue;
    }

    bool h_ChangeIsPlayerInduced(MBActionState PreviousState,MBActionState NewState)
    {
        bool ReturnValue = false;
        if(NewState == MBActionState::Airdodge ||
                NewState == MBActionState::Airdodge || 
                NewState == MBActionState::LedgeAttack ||
                NewState == MBActionState::LedgeJump ||
                NewState == MBActionState::LedgeRegular ||
                NewState == MBActionState::LedgeRoll || 
                NewState == MBActionState::Roll || 
                NewState == MBActionState::SpotDodge || 
                NewState == MBActionState::Jump || 
                NewState == MBActionState::DoubleJump || 
                NewState == MBActionState::Attacking)
        {
            ReturnValue = true;   
        }
        else if( (PreviousState == MBActionState::None || PreviousState == MBActionState::Null) && (NewState == MBActionState::Walking || NewState == MBActionState::Running || 
                    NewState == MBActionState::Dashing))
        {
            ReturnValue = true;
        }
        return ReturnValue;
    }
    
    std::vector<MQL_MetricVariable> MQLEvaluator::Delay METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            int Delay = 0;
            if(Intervall.LastFrame == Intervall.FirstFrame)
            {
                ReturnValue.push_back(int(-1));
                continue;
            }
            MBActionState PreviousState = GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].ActionState;
            int ActionIndex = -1;
            for(int i = Intervall.FirstFrame+1; i <= Intervall.LastFrame;i++)
            {
                auto const& CurrentInfo = GameToInspect.Frames[i].PlayerInfo[PlayerIndex];
                if(h_ChangeIsPlayerInduced(PreviousState,CurrentInfo.ActionState))
                {
                    ActionIndex = i-1;
                    break;
                }
                PreviousState = GameToInspect.Frames[i].PlayerInfo[PlayerIndex].ActionState;
            }
            if(ActionIndex == -1)
            {
                ReturnValue.push_back(int(-1));   
                continue;
            }
            for(int i = ActionIndex; i >= Intervall.FirstFrame;i--)
            {
                if(!FrameIsActionable(GameToInspect.Frames[i].PlayerInfo[PlayerIndex]))
                {
                    break;
                }
                Delay += 1;
            }
            ReturnValue.push_back(Delay);
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::Begin METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(int(Intervall.FirstFrame));   
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::End METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(int(Intervall.LastFrame));   
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::Percent METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].Percent);   
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::MoveName METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(MBAttackIDToString(GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].ActiveAttack));
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::PercentDiff METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(GameToInspect.Frames[Intervall.LastFrame].PlayerInfo[PlayerIndex].Percent-GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].Percent);
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::Length METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(Intervall.LastFrame-Intervall.FirstFrame);
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::ActionableFrames METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].ActionableFrames);
        }
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::Other METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        ReturnValue.push_back( (ExtraArguments[0].GetType<MQL_Filter_Literal>().Value.GetType<Literal_Number>().Value + 1)%2);
        return ReturnValue;
    }
    std::vector<MQL_MetricVariable> MQLEvaluator::File METRIC_ARGLIST
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(GameToInspect.Metadata.GamePath);
        }
        return ReturnValue;
    }
    std::vector<GameIntervall> MQLEvaluator::PlayerFlags FILTER_ARGLIST
    {
        std::vector<GameIntervall> ReturnValue;
        //ReturnValue = {IntervallToInspect};
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionStateFlags FlagsToCompare;
        for(int i = 0; i < ExtraArguments.PositionalCount();i++)
        {
            auto const& Argument = ExtraArguments.GetPositionalArgumentString(i);
            if(Argument == "Airborne")
            {
                FlagsToCompare.Airborne = true;
            }
            else if(Argument == "Hitlag")
            {
                FlagsToCompare.InHitlag = true;
            }
            else if(Argument == "FastFall")
            {
                FlagsToCompare.FastFalling = true;   
            }
            else
            {
                throw std::runtime_error("Unrecognized player flag: \""+Argument+"\"");   
            }
        }
        ReturnValue = ExtractSequences(GameToInspect,IntervallsToInspect,[&](FrameInfo const& Frame)
                {
                    bool ReturnValue = true;
                    if(FlagsToCompare.Airborne)
                    {
                        ReturnValue = ReturnValue && Frame.PlayerInfo[PlayerIndex].StateFlags.Airborne;
                    } 
                    if(FlagsToCompare.FastFalling)
                    {
                        ReturnValue = ReturnValue && Frame.PlayerInfo[PlayerIndex].StateFlags.FastFalling;
                    }
                    if(FlagsToCompare.InHitlag)
                    {
                        ReturnValue = ReturnValue && Frame.PlayerInfo[PlayerIndex].StateFlags.InHitlag;
                    }
                    return(ReturnValue);
                });
        return(ReturnValue);
    }
    //std::vector<GameIntervall> MQLEvaluator::HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    //{
    //    std::vector<GameIntervall> ReturnValue;
    //    ReturnValue = {IntervallToInspect};
    //    return(ReturnValue);
    //}
}
