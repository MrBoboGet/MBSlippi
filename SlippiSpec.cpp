#include "SlippiSpec.h"
#include "MBMeleeID.h"
#include "MeleeID.h"
#include "SlippiSpecParser.h"
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










    
    void SpecEvaluator::p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
    }
    void SpecEvaluator::p_VerifyGameInfoPredicate_Direct(MQL_Module& AssociatedModule,Identifier const& Idf,GameInfoPredicate_Direct& PredicateToVerify,
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
    void SpecEvaluator::p_VerifyGameInfoPredicate(MQL_Module& AssociatedModule,GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
    MBLSP::Position SpecEvaluator::p_GetBegin(Literal const& Component)
    {
        MBLSP::Position ReturnValue;
        ReturnValue.line = Component.GetBase().ValuePosition.Line;
        ReturnValue.character = Component.GetBase().ValuePosition.ByteOffset;
        return ReturnValue;
    }
    MBLSP::Position SpecEvaluator::p_GetEnd(Literal const& Component)
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
    MBLSP::Position SpecEvaluator::p_GetBegin(Filter_Component const& Component)
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
    MBLSP::Position SpecEvaluator::p_GetEnd(Filter_Component const& Component)
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
    void SpecEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Literal const& ErrorLiteral,std::string_view Message)
    {
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start = p_GetBegin(ErrorLiteral);
        NewDiagnostic.range.end = p_GetEnd(ErrorLiteral);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    void SpecEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Identifier const& ErrorIdentifier,std::string_view Message)
    {
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start.line = ErrorIdentifier.Parts.front().Position.Line;
        NewDiagnostic.range.start.character = ErrorIdentifier.Parts.front().Position.ByteOffset;
        NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(ErrorIdentifier);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    void SpecEvaluator::p_AddDiagnostic(std::vector<MBLSP::Diagnostic>& OutDiagnostics,Filter_OperatorList const& ErrorList,int Begin,int End,std::string_view Message)
    {
        Filter_Component const& BeginError = ErrorList.Components[Begin];
        Filter_Component const& EndError = ErrorList.Components[End-1];
        MBLSP::Diagnostic NewDiagnostic;
        NewDiagnostic.message = Message;
        NewDiagnostic.range.start = p_GetBegin(ErrorList.Components[Begin]);
        NewDiagnostic.range.end = p_GetEnd(ErrorList.Components[End-1]);
        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
    }
    SpecEvaluator::PrecedenceInfo SpecEvaluator::p_GetPrecedenceInfo(std::vector<std::string> const& Operators,int BeginIndex,int EndIndex)
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
    MQL_Filter SpecEvaluator::p_ConvertMetricOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
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
        std::type_index ResultType = typeid(nullptr);
        std::type_index NewType = typeid(nullptr);
        for(auto Index : PartOperator.OperatorPositions)
        {
            ReturnValue.Operands.push_back(p_ConvertMetricOperatorList(AssociatedModule,
                        ParentArgList,FilterToConvert,PrevIndex,Index,OutDiagnostics,NewType));
            if(ResultType == typeid(nullptr))
            {
                ResultType = NewType;
                ReturnValue.OperandTypes = ResultType;
            }
            else if(ResultType != NewType)
            {
                p_AddDiagnostic(OutDiagnostics,FilterToConvert,PrevIndex,Index,
                        "Invalid type of expression: lhs has type "+std::string(ResultType.name())+" and rhs has type "+NewType.name());
            }
            PrevIndex = Index;
        }
        if(ReturnValue.Type == OperatorType::eq ||
           ReturnValue.Type == OperatorType::leq || 
           ReturnValue.Type == OperatorType::le || 
           ReturnValue.Type == OperatorType::ge || 
           ReturnValue.Type == OperatorType::geq
                )
        {
            OutType = typeid(bool);
        }
        else
        {
            OutType = ResultType;
        }
        return ReturnValue;
    }
    MQL_Filter SpecEvaluator::p_ConvertMetricComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
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
                OutType = typeid(float);
            }
            else if(Literal.Value.IsType<Literal_Symbol>())
            {
                Literal_Symbol const& Symbol = Literal.Value.GetType<Literal_Symbol>();
                //auto& Variable = AssociatedModule.ModuleScope.GetVariable(Symbol.Value);
                //what type...
                OutType = typeid(std::string);
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
                    NewMetric.Args = ArgumentList(Func.ArgumentList);
                    NewMetric.Metric = MetricIt->second.Func;
                    OutType = MetricIt->second.ResultType;
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
    MQL_Filter SpecEvaluator::p_ConvertFilterOperatorList(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_OperatorList const& FilterToConvert,int BeginIndex,int EndIndex,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        MQL_FilterCombiner ReturnValue;
        ReturnValue.Negated = FilterToConvert.Negated;
        if(EndIndex-BeginIndex == 1)
        {
            return p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToConvert.Components[BeginIndex],OutDiagnostics);
        }
        PrecedenceInfo OperatorInfo = p_GetPrecedenceInfo(FilterToConvert.Operators,BeginIndex,EndIndex);
        ReturnValue.Type = OperatorInfo.Operator;
        int PrevIndex = BeginIndex;
        if(OperatorInfo.Operator == OperatorType::Add || OperatorInfo.Operator == OperatorType::Pipe)
        {
            for(auto Index : OperatorInfo.OperatorPositions)
            {
                ReturnValue.Operands.push_back(p_ConvertFilterOperatorList(AssociatedModule,ParentArgList,FilterToConvert,PrevIndex,Index,OutDiagnostics));
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
    MQL_Filter SpecEvaluator::p_ConvertFilterComponent(MQL_Module& AssociatedModule,ArgumentList& ParentArgList,
            Filter_Component const& FilterToConvert,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        MQL_Filter ReturnValue;
        if(FilterToConvert.IsType<Filter_Component_Literal>())
        {
            auto& Literal = FilterToConvert.GetType<Filter_Component_Literal>();
            p_AddDiagnostic(OutDiagnostics,Literal.Value,"Expressions in filter component must be a part of a comparison");
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
                    NewValue.Args = FuncFilter.ArgumentList;
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
                    NewValue.Args = ArgumentList(FuncFilter.ArgumentList);
                    NewValue.Filter = It->second;
                    NewValue.Negated = FuncFilter.Negated;
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
                return p_ConvertFilterComponent(AssociatedModule,ParentArgList,OperatorList.Components[0],OutDiagnostics);
            }
            return p_ConvertFilterOperatorList(AssociatedModule,ParentArgList,OperatorList,0,OperatorList.Components.size(),OutDiagnostics);
        }
        return ReturnValue;
    }
    void SpecEvaluator::p_VerifyFilterComponent(MQL_Module& AssociatedModule,Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        ArgumentList ParentArgList;
        p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToVerify,OutDiagnostics);
    }
    void SpecEvaluator::p_VerifyFilter(MQL_Module& AssociatedModule,Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        ArgumentList ParentArgList;
        p_ConvertFilterComponent(AssociatedModule,ParentArgList,FilterToVerify.Component,OutDiagnostics);
    }
    bool SpecEvaluator::p_EvaluateImport(MQL_Module& AssociatedModule,Import& ImportStatement,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
    bool SpecEvaluator::VerifyVariableDeclaration(MQL_Module& AssociatedModule,Statement& DeclarationToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool UpdateState)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        //no redeclaration allowed, should allow redeclaration if of the same type
        auto const& VariableBase = DeclarationToVerify.GetType<VariableDeclaration_Base>();
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
        if(DeclarationToVerify.IsType<VariableDeclaration_Filter>())
        {
            auto const& FilterVariable = DeclarationToVerify.GetType<VariableDeclaration_Filter>();
            p_VerifyFilterComponent(AssociatedModule,FilterVariable.Component,Diagnostics);
            //check bindings
            
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                std::shared_ptr<MQL_FilterDefinition> NewDefinition = std::make_shared<MQL_FilterDefinition>();
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
            OutDiagnostics = std::move(Diagnostics);
        }
        return(ReturnValue);
    }
    void SpecEvaluator::p_VerifyPlayerAssignment(MQL_Module& AssociatedModule,PlayerAssignment& AssignmentToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
    void SpecEvaluator::p_VerifyGameSelection(MQL_Module& AssociatedModule,GameSelection& SelectionToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        p_VerifyPlayerAssignment(AssociatedModule,SelectionToVerify.Assignment,OutDiagnostics);
        p_VerifyGameInfoPredicate(AssociatedModule,SelectionToVerify.GameCondition,false,OutDiagnostics);
    }
    bool SpecEvaluator::VerifySelection(MQL_Module& AssociatedModule,Selection& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
    bool SpecEvaluator::VerifyStatement(MQL_Module& AssociatedModule,Statement& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        if(SpecToVerify.IsType<Selection>())
        {
            ReturnValue = VerifySelection(AssociatedModule,SpecToVerify.GetType<Selection>(),OutDiagnostics);
        }
        else if(SpecToVerify.IsType<VariableDeclaration_Base>())
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
    bool SpecEvaluator::VerifyModule(MQL_Module& AssociatedModule,Module& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        //ensures that the top scope isn't modified after the verification,
        //but still makes variables visible to other parts of the verification
        //TODO avoid copying...
        MQL_Module TempModule = MQL_Module(AssociatedModule);
        for(auto& Statement : SpecToVerify.Statements)
        {
            if(Statement.IsType<VariableDeclaration_Base>())
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
    bool SpecEvaluator::p_EvaluateGameSelection(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate)
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
    bool SpecEvaluator::p_SatisfiesPlayerAssignment(MQL_Module& AssociatedModule,SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate)
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
    bool SpecEvaluator::p_GetPlayerAssignments(MQL_Module& AssociatedModule,SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4])
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
        return(PlayerWasAssigned);
    }
    void SpecEvaluator::p_ApplyAssignment(MeleeGame& GameToModify,char InAssignments[4])
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
    void SpecEvaluator::SetDBAdapter(MeleeGameDBAdapter* NewAdapter)
    {
        m_DBAdapter = std::move(NewAdapter);    
    }
    void SpecEvaluator::InitializeServers(std::vector<ServerInitilizationData> const& ServersToInitialize)
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
    void SpecEvaluator::SetRecorder(MeleeGameRecorder* NewRecorder)
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
    std::vector<MeleeGame> SpecEvaluator::p_RetrieveSpecGames( MQL_Module& AssociatedModule,GameSelection const& SpecToEvalaute)
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
    std::vector<GameIntervall> SpecEvaluator::p_EvaluateGameIntervalls(
            MQL_Module& AssociatedModule,
            MeleeGame const& InputGame,
            std::vector<GameIntervall> const& InputIntervalls,
            ArgumentList& ArgList,
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
            for(auto const& Intervall : InputIntervalls)
            {
                auto NewIntervalls = IntervallExtractor.Filter(InputGame,Arguments,Intervall);
                ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
            }
        }
        else if(Filter.IsType<MQL_FilterReference>())
        {
            auto const& FilterRef = Filter.GetType<MQL_FilterReference>();
            std::shared_ptr<MQL_FilterDefinition> DerferencedFilter = FilterRef.Filter;
            Negated = FilterRef.Negated;
            ArgumentList FilterArgument(FilterRef.Args);
            FilterArgument.SetParentArgList(&ArgList);
            ArgumentList ParentEnvironment(DerferencedFilter->Arguments,FilterArgument);
            auto NewIntervalls = p_EvaluateGameIntervalls(AssociatedModule,InputGame,InputIntervalls,ParentEnvironment,*DerferencedFilter->Component);
            ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
        }
        else if(Filter.IsType<MQL_FilterCombiner>())
        {
            auto const& Combiner = Filter.GetType<MQL_FilterCombiner>();
            Negated = Combiner.Negated;
            if(Combiner.Type == OperatorType::Add)
            {
                ReturnValue = InputIntervalls;
                for(auto const& SubFilter : Combiner.Operands)
                {
                    std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(AssociatedModule,InputGame,ReturnValue,ArgList,SubFilter);
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
            auto Results = p_EvaluateMetric(InputGame,InputIntervalls,ArgList,Filter);
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
        return ReturnValue;
    }



    template<typename ValueType,typename FuncType>
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
            ReturnValue.push_back(Func(std::get<ValueType>(Lhs[LhsIndex].Data),std::get<ValueType>(Rhs[RhsIndex].Data)));
            LhsIndex += 1;
            RhsIndex += 1;
            LhsIndex = LhsIndex % Lhs.size();
            RhsIndex = RhsIndex % Rhs.size();
            IterationCount += 1;
        }
        return ReturnValue;
    }

    std::vector<MQL_MetricVariable> SpecEvaluator::p_EvaluateMetric(
            MeleeGame const& InputGame,
            std::vector<GameIntervall> const& InputIntervalls,
            ArgumentList& ArgList,
            MQL_Filter const& Filter)
    {

        std::vector<MQL_MetricVariable> ReturnValue;
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
                std::string const& SymbolValue = ArgList.GetNamedVariableString(Literal.Value.GetType<Literal_Symbol>().Value);
                ReturnValue = {SymbolValue};
            }
        }
        else if(Filter.IsType<MQL_Metric>())
        {
            auto const& Metric = Filter.GetType<MQL_Metric>();
            assert(Metric.Metric != nullptr);
            ReturnValue = Metric.Metric(InputGame,ArgList,InputIntervalls);
        }
        else if(Filter.IsType<MQL_MetricCombiner>())
        {
            auto const& Combiner = Filter.GetType<MQL_MetricCombiner>();
            assert(Combiner.Operands.size() == 2);
            std::vector<MQL_MetricVariable> Lhs = p_EvaluateMetric(InputGame,InputIntervalls,ArgList,Combiner.Operands[0]);
            std::vector<MQL_MetricVariable> Rhs = p_EvaluateMetric(InputGame,InputIntervalls,ArgList,Combiner.Operands[1]);
            if(Combiner.OperandTypes == typeid(std::string))
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
            else if(Combiner.OperandTypes == typeid(float))
            {
                if(Combiner.Type == OperatorType::Plus)
                {
                    ReturnValue = h_CombineValues<float>(std::plus<float>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::eq)
                {
                    ReturnValue = h_CombineValues<float>(std::equal_to<float>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::le)
                {
                    ReturnValue = h_CombineValues<float>(std::less<float>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::leq)
                {
                    ReturnValue = h_CombineValues<float>(std::less_equal<float>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::ge)
                {
                    ReturnValue = h_CombineValues<float>(std::greater<float>(),Lhs,Rhs);
                }
                else if(Combiner.Type == OperatorType::geq)
                {
                    ReturnValue = h_CombineValues<float>(std::greater_equal<float>(),Lhs,Rhs);
                }
                else
                {
                    assert(false && "MetricCombiner for string doesnt cover all operator types");   
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
    void SpecEvaluator::EvaluateSelection(MQL_Module& AssociatedModule,Selection& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
        m_Recorder->RecordGames(GamesToRecord,SpecToEvaluate.Output.GetType<Result_Record>().OutFile);
    }
    void SpecEvaluator::EvaluateStatement(MQL_Module& AssociatedModule,Statement& StatementToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(StatementToEvaluate.IsType<Selection>())
        {
            EvaluateSelection(AssociatedModule,StatementToEvaluate.GetType<Selection>(),OutDiagnostics);
        }
        else if(StatementToEvaluate.IsType<VariableDeclaration_Base>())
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
    void SpecEvaluator::EvaluateImport(MQL_Module& AssociatedModule,Import& ImportToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(!p_EvaluateImport(AssociatedModule,ImportToEvaluate,OutDiagnostics))
        {
            return;
        }
    }
    ModuleID SpecEvaluator::LoadModule(std::filesystem::path const& ModulePath)
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
        m_CurrentModuleID++;
        m_LoadedModulePaths[CanonicalModulePath] = ReturnValue;
        if(std::filesystem::exists(CanonicalModulePath))
        {
            try
            {
                std::string ModuleContent = MBUtility::ReadWholeFile(CanonicalModulePath);
                m_Tokenizer.SetText(std::move(ModuleContent));
                NewModule.Contents = ParseModule(m_Tokenizer);
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
    ModuleID SpecEvaluator::LoadEmptyModule()
    {
        ModuleID NewModuleID = m_CurrentModuleID;
        m_CurrentModuleID++;
        m_LoadedModules[NewModuleID] = std::make_shared<MQL_Module>();
        return(NewModuleID);
    }
    MQL_Module& SpecEvaluator::GetModule(ModuleID ID)
    {
        auto It = m_LoadedModules.find(ID);
        if(It == m_LoadedModules.end())
        {
            throw std::runtime_error("Cannot find module with "+std::to_string(ID));   
        }
        return(*It->second);
    }
    void SpecEvaluator::EvaluateVariableDeclaration(MQL_Module& AssociatedModule,Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
            NewVariableData->Arguments = FilterDefinition.Arguments;
            ArgumentList List;
            NewVariableData->Component = std::make_shared<MQL_Filter>(p_ConvertFilterComponent(AssociatedModule,List,FilterDefinition.Component,OutDiagnostics));
            NewVariable.Data = std::move(NewVariableData);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameList>())
        {
            MQL_Variable NewVariable;
            MQL_LazyGameList Value;
            Value.GamesToRetrieve = SpecToEvaluate.GetType<VariableDeclaration_GameList>().Selection;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameInfoPredicate>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = false;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_GameInfoPredicate>().Predicate;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_PlayerSelection>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = true;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_PlayerSelection>().Predicate;
            NewVariable.Data = std::move(Value);
            AssociatedModule.ModuleScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
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
    void SpecEvaluator::EvaluateModule(MQL_Module& AssociatedModule,Module& ModuleToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
    std::vector<GameIntervall> SpecEvaluator::BiggestPunishes(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        float PercentThreshold = 40;
        int PunisherIndex = GetPlayerIndex(ExtraArguments);
        int PunisheeIndex = PunisherIndex == 1 ? 0 : 1;
        float ExtractCount = 10;
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
        if(Punishes.size() > ExtractCount)
        {
            Punishes.resize(ExtractCount);
        }
        for(auto const& Punish : Punishes)
        {
            ReturnValue.push_back(Punish.PunishIntervall);   
        }
        return(ReturnValue);
    }
    int SpecEvaluator::GetPlayerIndex(ArgumentList const& ExtraArguments)
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
    std::vector<GameIntervall> SpecEvaluator::HasMove(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() == 0)
        {
            throw std::runtime_error("HasMove requires the move to search for as the first positional argument");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(int i = IntervallToInspect.FirstFrame; i <= IntervallToInspect.LastFrame;i++)
        {
            if(GameToInspect.Frames[i].PlayerInfo[PlayerIndex].ActiveAttack == Attack)
            {
                ReturnValue.push_back(IntervallToInspect);
                break;
            }
        }
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::Move(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("Move requries exactly 1 positional option, the name of the move to inspect");
        }
        MBAttackID Move = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,
                [&](FrameInfo const& Frame)
                {
                    return(Frame.PlayerInfo[PlayerIndex].ActiveAttack == Move);
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::InShield(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
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
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,
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
    std::vector<GameIntervall> SpecEvaluator::Until(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        if(!ExtraArguments.HasNamedVariable("State") && 
                !ExtraArguments.HasNamedVariable("Flag"))
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
        GameIntervall IntervallToInsert = IntervallToInspect;
        for(int i = IntervallToInspect.FirstFrame + ( Skip != -1 ? Skip : 1); i < IntervallToInspect.LastFrame;i++)
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
        ReturnValue = {IntervallToInsert};
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::ActionState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("ActionState requires exactly 1 positional argument, the name of the action state");
        }
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionState StateToCheck = StringToMBActionState(ExtraArguments.GetPositionalArgumentString(0));
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,
                [&](FrameInfo const& Frame){return(Frame.PlayerInfo[PlayerIndex].ActionState == StateToCheck);});
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::HasState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        auto Intervalls = ActionState(GameToInspect,ExtraArguments,IntervallToInspect);
        if(Intervalls.size() != 0)
        {
            return {IntervallToInspect};   
        }
        return ReturnValue;
    }
    std::vector<GameIntervall> SpecEvaluator::Expand(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
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
            IntervallToInspect.LastFrame = GameToInspect.Frames.size();
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
            IntervallToInspect.LastFrame = GameToInspect.Frames.size();
        }
        if(IntervallToInspect.FirstFrame < IntervallToInspect.LastFrame)
        {
            ReturnValue = {IntervallToInspect};
        }
        return(ReturnValue);
           
    }
    std::vector<GameIntervall> SpecEvaluator::HitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        int OpponentIndex = PlayerIndex == 0 ? 1 : 0;
        int Context = 10;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("HitBy requires exactly 1 positional argument, the name of the hit move");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        if(ExtraArguments.HasNamedVariable("Context"))
        {
            Context = std::stoi(ExtraArguments.GetNamedVariableString("Context"));
        }
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,[&]
                (FrameInfo const& Frame)
                {
                    return((Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::ShieldStun || Frame.PlayerInfo[PlayerIndex].StateFlags.InHitlag)
                            && Frame.PlayerInfo[OpponentIndex].ActiveAttack == Attack);
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::HasHitBy(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        int OpponentIndex = PlayerIndex == 0 ? 1 : 0;
        if(ExtraArguments.PositionalCount() != 1)
        {
            throw std::runtime_error("HasHitBy requires exactly 1 positional argument, the name of the hit move");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.GetPositionalArgumentString(0));
        for(int i = IntervallToInspect.FirstFrame; i < IntervallToInspect.LastFrame;i++)
        {
            FrameInfo const& Frame = GameToInspect.Frames[i];
            if((Frame.PlayerInfo[PlayerIndex].ActionState == MBActionState::ShieldStun || Frame.PlayerInfo[PlayerIndex].StateFlags.InHitlag)
                            && Frame.PlayerInfo[OpponentIndex].ActiveAttack == Attack)
            {
                ReturnValue = {IntervallToInspect};
                break;
            }
        }
        return(ReturnValue);
    }
   
    struct i_StageBoundaryInfo
    {
        float OffstageLeft = 0;
        float OffstageRight = 0;
    };
    std::pair<StageID,i_StageBoundaryInfo> i_StageBoundaries[] = { 
        {StageID::Battlefield,{-75,75}},
        {StageID::DreamLandN64,{-80,80}},
        {StageID::YoshisIsland,{-60,60}},
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
    std::vector<GameIntervall> SpecEvaluator::Offstage(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        i_StageBoundaryInfo const& StageInfo = i_GetStageBoundaryInfo(GameToInspect.Stage);
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,[&](FrameInfo const& Frame)
                {
                    return(Frame.PlayerInfo[PlayerIndex].PlayerPosition.x < StageInfo.OffstageLeft || Frame.PlayerInfo[PlayerIndex].PlayerPosition.x > StageInfo.OffstageRight);
                });
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::Length(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        if(ExtraArguments.PositionalCount() == 0)
        {
            throw std::runtime_error("Error in length filter: requires the minimum length of the intervall as the first positiional argument");
        }
        int Length = h_ParseExpandInteger(ExtraArguments.GetPositionalArgumentString(0));
        if(IntervallToInspect.LastFrame-IntervallToInspect.FirstFrame >= Length)
        {
            ReturnValue = {IntervallToInspect};
        }
        return(ReturnValue);
    }
    inline bool h_InIntervall(float xleft,float xright,float x)
    {
        return x <= xright && x >= xleft;
    } 
    std::vector<GameIntervall> SpecEvaluator::Cornered(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
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
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,[&](FrameInfo const& Frame)
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
    std::vector<MQL_MetricVariable> SpecEvaluator::Percent(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,std::vector<GameIntervall> const& IntervallToInspect)
    {
        std::vector<MQL_MetricVariable> ReturnValue;
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        for(auto const& Intervall : IntervallToInspect)
        {
            ReturnValue.push_back(GameToInspect.Frames[Intervall.FirstFrame].PlayerInfo[PlayerIndex].Percent);   
        }
        return ReturnValue;
    }
    std::vector<GameIntervall> SpecEvaluator::PlayerFlags(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
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
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,[&](FrameInfo const& Frame)
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
    std::vector<GameIntervall> SpecEvaluator::HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        ReturnValue = {IntervallToInspect};
        return(ReturnValue);
    }
}
