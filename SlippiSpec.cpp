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
#include <cmath>
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
    void SpecEvaluator::p_VerifyGameInfoPredicate_Direct(Identifier const& Idf,GameInfoPredicate_Direct& PredicateToVerify,
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
    void SpecEvaluator::p_VerifyGameInfoPredicate(GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(PredicateToVerify.Data.IsType<GameInfoPredicate_Direct>())
        {
            p_VerifyGameInfoPredicate_Direct(PredicateToVerify.Attribute,PredicateToVerify.Data.GetType<GameInfoPredicate_Direct>(),IsPlayerAssignment,OutDiagnostics);
        }
        else if(PredicateToVerify.Attribute.Parts.size() != 0)
        {
            auto const& Attribute = PredicateToVerify.Attribute.Parts;
            if(!m_TopContext.GlobalScope.HasVariable(PredicateToVerify.Attribute))
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
                auto const& Variable = m_TopContext.GlobalScope.GetVariable(PredicateToVerify.Attribute);
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
            p_VerifyGameInfoPredicate(SubPredicate,IsPlayerAssignment,OutDiagnostics);
        }
    }
    void SpecEvaluator::p_VerifyFilterComponent(Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(FilterToVerify.FilterName.Parts.size() != 0)
        {
            if(m_TopContext.GlobalScope.HasVariable(FilterToVerify.FilterName))
            {
                auto const& Variable = m_TopContext.GlobalScope.GetVariable(FilterToVerify.FilterName);   
                if(!std::holds_alternative<Filter_Component>(Variable.Data))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Variable isn't of type Filter";
                    NewDiagnostic.range.start.line = FilterToVerify.NamePosition.Line;
                    NewDiagnostic.range.start.character = FilterToVerify.NamePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(FilterToVerify.FilterName);
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
            }
            else if( FilterToVerify.FilterName.Parts.size() > 1 && (m_BuiltinFilters.find(FilterToVerify.FilterName.Parts[0].Value) == m_BuiltinFilters.end() &&
                    m_FilterToServer.find(FilterToVerify.FilterName.Parts[0].Value) == m_FilterToServer.end()))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Can't find filter with name \""+IdentifierToString(FilterToVerify.FilterName)+"\"";
                NewDiagnostic.range.start.line = FilterToVerify.NamePosition.Line;
                NewDiagnostic.range.start.character = FilterToVerify.NamePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + IdentifierLength(FilterToVerify.FilterName);
                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
        }
        for(auto const& Filter : FilterToVerify.ExtraTerms)
        {
            p_VerifyFilterComponent(Filter,OutDiagnostics);   
        }
    }
    void SpecEvaluator::p_VerifyFilter(Filter const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        p_VerifyFilterComponent(FilterToVerify.Component,OutDiagnostics);
    }
    bool SpecEvaluator::VerifyVariableDeclaration(Statement& DeclarationToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics,bool UpdateState)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        //no redeclaration allowed, should allow redeclaration if of the same type
        auto const& VariableBase = DeclarationToVerify.GetType<VariableDeclaration_Base>();
        bool VariableExists = false;
        if(m_TopContext.GlobalScope.HasVariable(VariableBase.Name))
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
            p_VerifyFilterComponent(FilterVariable.Component,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                TemporaryVariable.Data = Filter_Component();
                m_TopContext.GlobalScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_GameInfoPredicate>())
        {
            auto& PredicateVariable = DeclarationToVerify.GetType<VariableDeclaration_GameInfoPredicate>();
            p_VerifyGameInfoPredicate(PredicateVariable.Predicate,false,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_Variable_GameInfoPredicate Value = MQL_Variable_GameInfoPredicate();
                Value.IsPlayerAssignment = false;
                TemporaryVariable.Data = std::move(Value);
                m_TopContext.GlobalScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_PlayerSelection>())
        {
            auto& PlayerSelectionVariable = DeclarationToVerify.GetType<VariableDeclaration_PlayerSelection>();
            p_VerifyGameInfoPredicate(PlayerSelectionVariable.Predicate,true,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_Variable_GameInfoPredicate Value = MQL_Variable_GameInfoPredicate();
                Value.IsPlayerAssignment = true;
                TemporaryVariable.Data = std::move(Value);
                m_TopContext.GlobalScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
            }
        }
        else if(DeclarationToVerify.IsType<VariableDeclaration_GameList>())
        {
            auto& GameListVariable = DeclarationToVerify.GetType<VariableDeclaration_GameList>();
            p_VerifyGameSelection(GameListVariable.Selection,Diagnostics);
            if(UpdateState && !VariableExists)
            {
                MQL_Variable TemporaryVariable;
                MQL_LazyGameList Value = MQL_LazyGameList();
                TemporaryVariable.Data = std::move(Value);
                m_TopContext.GlobalScope.AddVariable(VariableBase.Name,std::move(TemporaryVariable));
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
    void SpecEvaluator::p_VerifyPlayerAssignment(PlayerAssignment& AssignmentToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
            p_VerifyGameInfoPredicate(AssignmentToVerify.PlayerCondition,true,OutDiagnostics);
        } 
    }
    void SpecEvaluator::p_VerifyGameSelection(GameSelection& SelectionToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        p_VerifyPlayerAssignment(SelectionToVerify.Assignment,OutDiagnostics);
        p_VerifyGameInfoPredicate(SelectionToVerify.GameCondition,false,OutDiagnostics);
    }
    bool SpecEvaluator::VerifySelection(Selection& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        p_VerifyPlayerAssignment(SpecToVerify.Games.Assignment,Diagnostics);
        p_VerifyGameInfoPredicate(SpecToVerify.Games.GameCondition,false,Diagnostics);
        p_VerifyFilter(SpecToVerify.SituationFilter,Diagnostics);
        if(Diagnostics.size() > 0)
        {
            ReturnValue = false;
            OutDiagnostics = std::move(Diagnostics);
        }
        return(ReturnValue);
    }
    bool SpecEvaluator::VerifyStatement(Statement& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        if(SpecToVerify.IsType<Selection>())
        {
            ReturnValue = VerifySelection(SpecToVerify.GetType<Selection>(),OutDiagnostics);
        }
        else if(SpecToVerify.IsType<VariableDeclaration_Base>())
        {
            ReturnValue = VerifyVariableDeclaration(SpecToVerify,OutDiagnostics);
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
    bool SpecEvaluator::VerifyModule(Module& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        //ensures that the top scope isn't modified after the verification,
        //but still makes variables visible to other parts of the verification
        MQL_Scope TopScope = MQL_Scope();
        std::swap(TopScope,m_TopContext.GlobalScope);
        m_TopContext.GlobalScope.SetParentScope(&TopScope);
        for(auto& Statement : SpecToVerify.Statements)
        {
            if(Statement.IsType<VariableDeclaration_Base>())
            {
                ReturnValue = ReturnValue && VerifyVariableDeclaration(Statement,OutDiagnostics,true);
            }
            else
            {
                ReturnValue = ReturnValue && VerifyStatement(Statement,OutDiagnostics);
            }
        }
        std::swap(TopScope,m_TopContext.GlobalScope);
        return(ReturnValue);
    }
    bool SpecEvaluator::p_EvaluateGameSelection(SlippiGameInfo const& GameInfo,char InAssignment[4],GameInfoPredicate const& PredicateToEvaluate)
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
            MQL_Variable& Variable = m_TopContext.GlobalScope.GetVariable(PredicateToEvaluate.Attribute);
            if(std::holds_alternative<MQL_LazyGameList>(Variable.Data))
            {
                MQL_LazyGameList& GameList = std::get<MQL_LazyGameList>(Variable.Data);
                if(!GameList.Evaluated)
                {
                    GameList.Games = p_RetrieveSpecGames(GameList.GamesToRetrieve);
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
                ReturnValue = p_EvaluateGameSelection(GameInfo,InAssignment,DereferencedPredicate.Predicate);
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
                    ReturnValue = p_EvaluateGameSelection(GameInfo,InAssignment,ExtraTerm);           
                }
            }
            else if(PredicateToEvaluate.Operator == "&&")
            {
                if(!p_EvaluateGameSelection(GameInfo,InAssignment,ExtraTerm))
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
    bool SpecEvaluator::p_SatisfiesPlayerAssignment(SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate)
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
                    m_TopContext.GlobalScope.GetVariable(PredicateToEvaluate.Attribute).Data).Predicate;
            ReturnValue = p_SatisfiesPlayerAssignment(PlayerInfo,DereferencedPredicate);
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
                    ReturnValue = p_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm);
                }
            }
            else if(PredicateToEvaluate.Operator == "&&")
            {
                if(!p_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm))
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
    bool SpecEvaluator::p_GetPlayerAssignments(SlippiGameInfo const& GameInfo,PlayerAssignment const& AssignemntToApply,char OutAssignemnts[4])
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
            if(p_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[i],AssignemntToApply.PlayerCondition))
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
    std::vector<MeleeGame> SpecEvaluator::p_RetrieveSpecGames(GameSelection const& SpecToEvalaute)
    {
        std::vector<MeleeGame> ReturnValue;
        //TODO improve this, use SQL query so unneccesary game info doesn't need to be parsed
        std::vector<SlippiGameInfo> Candidates = m_DBAdapter->RetrieveGames("");
        for(auto& Candidate : Candidates)
        {
            bool IsSwapped = false;
            char AssignmentIndexes[4] = {0,1,2,3};
            if(p_GetPlayerAssignments(Candidate,SpecToEvalaute.Assignment,AssignmentIndexes))
            {
                if(p_EvaluateGameSelection(Candidate,AssignmentIndexes,SpecToEvalaute.GameCondition))
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
    std::vector<GameIntervall> SpecEvaluator::p_EvaluateGameIntervalls(Filter_Component const& FilterToUse,
            std::vector<GameIntervall> const& InputIntervalls,MeleeGame const& GameToFilter)
    {
        std::vector<GameIntervall> ReturnValue;
        if(FilterToUse.FilterName.Parts.size() != 0)
        {
            //temporery until filters are properly vectorized
            if(FilterToUse.FilterName.Parts.size() == 1)
            {
                if(auto BuiltinFilter = m_BuiltinFilters.find(FilterToUse.FilterName.Parts[0].Value); BuiltinFilter != m_BuiltinFilters.end())
                {
                    for(auto const& Intervall : InputIntervalls)
                    {
                        auto NewIntervalls = BuiltinFilter->second(GameToFilter,FilterToUse.ArgumentList,Intervall);
                        ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
                    }
                }
                else if(auto ServerIndex = m_FilterToServer.find(FilterToUse.FilterName.Parts[0].Value); ServerIndex != m_FilterToServer.end())
                {
                    for(auto const& Intervall : InputIntervalls)
                    {
                        auto NewIntervalls = m_SpecServers[ServerIndex->second].ExecuteFilter(FilterToUse.FilterName.Parts[0].Value,GameToFilter,Intervall);
                        ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
                    }
                }
            }
            else if(m_TopContext.GlobalScope.HasVariable(FilterToUse.FilterName))
            {
                Filter_Component const& DerferencedFilter = std::get<Filter_Component>(
                        m_TopContext.GlobalScope.GetVariable(FilterToUse.FilterName).Data);
                auto NewIntervalls = p_EvaluateGameIntervalls(DerferencedFilter,InputIntervalls,GameToFilter);
                ReturnValue.insert(ReturnValue.end(),NewIntervalls.begin(),NewIntervalls.end());
            }
            else
            {
                //must be a variable
                Filter_Component const& DerferencedFilter = std::get<Filter_Component>(
                    m_TopContext.GlobalScope.GetVariable(FilterToUse.FilterName).Data);
                ReturnValue = p_EvaluateGameIntervalls(DerferencedFilter,InputIntervalls,GameToFilter);
                //assert(false && "No server or builtin filter found, evaluating spec that shouldn't have been verified");
            }
            std::sort(ReturnValue.begin(),ReturnValue.end());
        }
        else
        {
            ReturnValue =  InputIntervalls;
        }
        //special case becuase the filter is always present, but doesn't neccesarially contain anything
        if(FilterToUse.FilterName.Parts.size()  == 0 && FilterToUse.ExtraTerms.size() == 0)
        {
            return(ReturnValue);
        }
        //only present if not term
        for(auto const& ExtraFilter : FilterToUse.ExtraTerms)
        {
            if(FilterToUse.Operator == "&")
            {
                std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(ExtraFilter,InputIntervalls,GameToFilter); 
                std::vector<GameIntervall> NewReturnValue;
                NewReturnValue.resize(NewSituations.size()+ReturnValue.size());
                std::merge(NewSituations.begin(),NewSituations.end(),ReturnValue.begin(),ReturnValue.end(),NewReturnValue.begin());
                ReturnValue = std::move(NewReturnValue);
            }       
            else if(FilterToUse.Operator == "|")
            {
                std::vector<GameIntervall> NewIntervalls;
                NewIntervalls = p_EvaluateGameIntervalls(ExtraFilter,ReturnValue,GameToFilter);
                std::swap(ReturnValue,NewIntervalls);
            }
            else
            {
                assert(false  && "Trying to evaluate filter with extra terms and no valid operator");
            }
        }
        if(FilterToUse.Negated)
        {
            ReturnValue = h_GetNegatedIntervalls(InputIntervalls,ReturnValue);
        }
        return(ReturnValue);
    }
    void SpecEvaluator::EvaluateSelection(Selection& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(m_DBAdapter == nullptr)
        {
            throw std::runtime_error("DBAdapter need to be set in order to evaluate SlippiSpec");  
        } 
        if(m_Recorder == nullptr)
        {
            throw std::runtime_error("Recorder need to be set in order to evaluate SlippiSpec");   
        }
        if(!VerifySelection(SpecToEvaluate,OutDiagnostics))
        {
            return;   
        }
        std::vector<MeleeGame> GamesToInspect = p_RetrieveSpecGames(SpecToEvaluate.Games);
        std::vector<std::vector<GameIntervall>> GameIntervalls;
        GameIntervalls.reserve(GamesToInspect.size());

        std::vector<RecordingInfo> GamesToRecord;
        for(auto& Game : GamesToInspect)
        {
            //assumes are sorted
            auto Intervalls = p_EvaluateGameIntervalls(SpecToEvaluate.SituationFilter.Component,{GameIntervall(0,Game.Frames.size()-1)},Game);
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
    void SpecEvaluator::EvaluateStatement(Statement& StatementToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(StatementToEvaluate.IsType<Selection>())
        {
            EvaluateSelection(StatementToEvaluate.GetType<Selection>(),OutDiagnostics);
        }
        else if(StatementToEvaluate.IsType<VariableDeclaration_Base>())
        {
            EvaluateVariableDeclaration(StatementToEvaluate,OutDiagnostics);
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
    void SpecEvaluator::EvaluateVariableDeclaration(Statement& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(!VerifyVariableDeclaration(SpecToEvaluate,OutDiagnostics))
        {
            return;
        }
        if(SpecToEvaluate.IsType<VariableDeclaration_Filter>())
        {
            MQL_Variable NewVariable;
            NewVariable.Data = SpecToEvaluate.GetType<VariableDeclaration_Filter>().Component;
            m_TopContext.GlobalScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameList>())
        {
            MQL_Variable NewVariable;
            MQL_LazyGameList Value;
            Value.GamesToRetrieve = SpecToEvaluate.GetType<VariableDeclaration_GameList>().Selection;
            NewVariable.Data = std::move(Value);
            m_TopContext.GlobalScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_GameInfoPredicate>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = false;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_GameInfoPredicate>().Predicate;
            NewVariable.Data = std::move(Value);
            m_TopContext.GlobalScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
        }
        else if(SpecToEvaluate.IsType<VariableDeclaration_PlayerSelection>())
        {
            MQL_Variable NewVariable;
            MQL_Variable_GameInfoPredicate Value;
            Value.IsPlayerAssignment = true;
            Value.Predicate = SpecToEvaluate.GetType<VariableDeclaration_PlayerSelection>().Predicate;
            NewVariable.Data = std::move(Value);
            m_TopContext.GlobalScope.AddVariable(SpecToEvaluate.GetType<VariableDeclaration_Base>().Name, std::move(NewVariable));
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
    void SpecEvaluator::EvaluateModule(Module& ModuleToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(m_DBAdapter == nullptr)
        {
            throw std::runtime_error("DBAdapter need to be set in order to evaluate SlippiSpec");  
        } 
        if(m_Recorder == nullptr)
        {
            throw std::runtime_error("Recorder need to be set in order to evaluate SlippiSpec");   
        }
        if(!VerifyModule(ModuleToEvaluate,OutDiagnostics))
        {
            return;
        }
        for(auto& Statement : ModuleToEvaluate.Statements)
        {
            EvaluateStatement(Statement,OutDiagnostics);
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
        if(auto const& PercentIt = ExtraArguments.KeyArguments.find("Percent"); PercentIt != ExtraArguments.KeyArguments.end())
        {
            PercentThreshold = h_ParseFloat(PercentIt->second);
        }
        if(auto const& CountIt = ExtraArguments.KeyArguments.find("Count"); CountIt != ExtraArguments.KeyArguments.end())
        {
            ExtractCount = h_ParseExpandInteger(CountIt->second);
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
        if(auto const& PlayerIndexIt = ExtraArguments.KeyArguments.find("Player"); PlayerIndexIt != ExtraArguments.KeyArguments.end())
        {
            if(PlayerIndexIt->second == "0")
            {

            }
            else if(PlayerIndexIt->second == "1")
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
        if(ExtraArguments.PositionalArguments.size() == 0)
        {
            throw std::runtime_error("HasMove requires the move to search for as the first positional argument");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.PositionalArguments[0]);
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
        if(ExtraArguments.PositionalArguments.size() != 1)
        {
            throw std::runtime_error("Move requries exactly 1 positional option, the name of the move to inspect");
        }
        MBAttackID Move = StringToMBAttackID(ExtraArguments.PositionalArguments[0]);
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
        bool CheckShielding = true;
        bool CheckShieldstun = true;
        if(auto ShieldingIt = ExtraArguments.KeyArguments.find("Shielding"),ShieldstunIt = ExtraArguments.KeyArguments.find("Shieldstun");
                ShieldingIt != ExtraArguments.KeyArguments.end() || ShieldstunIt != ExtraArguments.KeyArguments.end())
        {
            CheckShieldstun = ShieldstunIt != ExtraArguments.KeyArguments.end();
            CheckShielding = ShieldingIt != ExtraArguments.KeyArguments.end();
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
        if(ExtraArguments.KeyArguments.find("State") == ExtraArguments.KeyArguments.end() && 
                ExtraArguments.KeyArguments.find("Flag") == ExtraArguments.KeyArguments.end())
        {
            throw std::runtime_error("Until requires requires either 'State' key argument or 'Flag' key argument");
        }
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionState StateToCheck = MBActionState::None;
        if(auto const& StateIt = ExtraArguments.KeyArguments.find("State"); StateIt != ExtraArguments.KeyArguments.end())
        {
            StateToCheck = StringToMBActionState(StateIt->second);
        }
        //only airborne as of now
        bool CheckAirborne = false;
        bool Airborne = false;
        if(auto const& FlagIt = ExtraArguments.KeyArguments.find("Flag"); FlagIt != ExtraArguments.KeyArguments.end())
        {
            CheckAirborne = true;
            if(FlagIt->second== "Airborne")
            {
                Airborne = true;
            }
            else if(FlagIt->second  == "NoAirborne")
            {
                Airborne = false;
            }
            else
            {
                throw std::runtime_error("Unknown state flag \""+FlagIt->second+"\"");   
            }
        }
        int Skip = -1;
        if(auto const& SkipIt = ExtraArguments.KeyArguments.find("Skip"); SkipIt != ExtraArguments.KeyArguments.end())
        {
            Skip = h_ParseExpandInteger(SkipIt->second);
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
        if(ExtraArguments.PositionalArguments.size() != 1)
        {
            throw std::runtime_error("ActionState requires exactly 1 positional argument, the name of the action state");
        }
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionState StateToCheck = StringToMBActionState(ExtraArguments.PositionalArguments[0]);
        ReturnValue = ExtractSequences(GameToInspect,IntervallToInspect,
                [&](FrameInfo const& Frame){return(Frame.PlayerInfo[PlayerIndex].ActionState == StateToCheck);});
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::Expand(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        int LeftExpand = 0;
        int RightExpand = 0;
        if(ExtraArguments.PositionalArguments.size() > 0)
        {
            int TotalExpandSize = h_ParseExpandInteger(ExtraArguments.PositionalArguments[0]);
            LeftExpand = -TotalExpandSize;
            RightExpand = TotalExpandSize;
        }
        if(auto RightIt = ExtraArguments.KeyArguments.find("Right"); RightIt != ExtraArguments.KeyArguments.end())
        {
            RightExpand = h_ParseExpandInteger(RightIt->second);
        }
        if(auto LeftIt = ExtraArguments.KeyArguments.find("Left"); LeftIt != ExtraArguments.KeyArguments.end())
        {
            LeftExpand = -h_ParseExpandInteger(LeftIt->second);   
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
        if(ExtraArguments.PositionalArguments.size() != 1)
        {
            throw std::runtime_error("HitBy requires exactly 1 positional argument, the name of the hit move");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.PositionalArguments[0]);
        if(auto ContextIt = ExtraArguments.KeyArguments.find("Context"); ContextIt != ExtraArguments.KeyArguments.end())
        {
            Context = std::stoi(ContextIt->second);
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
        if(ExtraArguments.PositionalArguments.size() != 1)
        {
            throw std::runtime_error("HasHitBy requires exactly 1 positional argument, the name of the hit move");
        }
        MBAttackID Attack = StringToMBAttackID(ExtraArguments.PositionalArguments[0]);
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
        if(ExtraArguments.PositionalArguments.size() == 0)
        {
            throw std::runtime_error("Error in length filter: requires the minimum length of the intervall as the first positiional argument");
        }
        int Length = h_ParseExpandInteger(ExtraArguments.PositionalArguments[0]);
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
        if(auto const& Margin = ExtraArguments.KeyArguments.find("Margin"); Margin != ExtraArguments.KeyArguments.end())
        {
            float MarginValue = h_ParseFloat(Margin->second); 
            XStageMargin = MarginValue;
            XOffstageMargin = MarginValue;
        }
        else
        {
            if(auto const& LeftMargin = ExtraArguments.KeyArguments.find("StageMargin"); LeftMargin != ExtraArguments.KeyArguments.end())
            {
                XStageMargin = h_ParseFloat(LeftMargin->second);
            }
            if(auto const& RightMargin = ExtraArguments.KeyArguments.find("OffstageMargin"); RightMargin != ExtraArguments.KeyArguments.end())
            {
                XOffstageMargin = h_ParseFloat(RightMargin->second);
            }
        }
        if(auto const& Distance = ExtraArguments.KeyArguments.find("Distance"); Distance != ExtraArguments.KeyArguments.end())
        {
            OpponentDistance = h_ParseFloat(Distance->second); 
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
    std::vector<GameIntervall> SpecEvaluator::PlayerFlags(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        //ReturnValue = {IntervallToInspect};
        int PlayerIndex = GetPlayerIndex(ExtraArguments);
        MBActionStateFlags FlagsToCompare;
        for(auto const& Argument : ExtraArguments.PositionalArguments)
        {
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
