#include "SlippiSpec.h"
#include "MBMeleeID.h"
#include "SlippiSpecParser.h"
#include <memory>
#include <MBUtility/Merge.h>
#include <assert.h>
#include <sstream>


#include "SlippiGQPStructs.h"

#include <MBLSP/SemanticTokens.h>
namespace MBSlippi
{

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
            ReturnValue = lhs >= rhs;   
        }
        else
        {
            assert(false && "Invalid operator encountered");   
        }

        return(ReturnValue);
    }
    uint64_t h_GetPosixTime(std::string const& DateToVerify)
    {
        uint64_t ReturnValue = 0;
		std::tm StoredTime{};
		std::istringstream Stream(DateToVerify);
        int DashCount = std::count(DateToVerify.begin(),DateToVerify.end(),'-');
        if(DashCount == 0)
        {
            Stream >> std::get_time(&StoredTime, "%Y");
        }
        else if(DashCount == 1)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m");
        }
        else if(DashCount == 2)
        {
            Stream >> std::get_time(&StoredTime, "%Y-%m-%d");
        }
        else
        {
            assert(false && "Calling h_GetPosixTime with invalid date string");
        }
        StoredTime.tm_isdst = -1;
        ReturnValue = uint64_t(std::mktime(&StoredTime));
        return(ReturnValue);       
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
    void SpecEvaluator::p_VerifyGameInfoPredicate_Direct(GameInfoPredicate_Direct& PredicateToVerify,
            bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        std::vector<AttributeComponent> const& Attribute = PredicateToVerify.Attribute;
        if(Attribute.size() != 0)
        {
            int AttributeOffset = 0;
            if(!IsPlayerAssignment)
            {
                if(Attribute[0].Name == "Date" || Attribute[0].Name == "Stage")
                {
                    if(Attribute.size() > 1)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = Attribute[0].Name +" doesn't have any members";
                        NewDiagnostic.range.start.line = Attribute[0].NamePosition.Line;
                        NewDiagnostic.range.start.character = Attribute[0].NamePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Name.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                        return;
                    }
                    if(IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Only player attributes can be a part of a player specification";
                        NewDiagnostic.range.start.line = Attribute[0].NamePosition.Line;
                        NewDiagnostic.range.start.character = Attribute[0].NamePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Name.size();
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                        return;
                    }
                }
                if(Attribute[0].Name == "Date")
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
                        PredicateToVerify.DateValue = h_GetPosixTime(PredicateToVerify.Value);
                    }
                    return;
                }
                else if(Attribute[0].Name == "Stage")
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
                else if(!(Attribute[0].Name == "Player1" || Attribute[0].Name == "Player2"))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Invalid attribute: \""+Attribute[0].Name+"\"";
                    NewDiagnostic.range.start.line = Attribute[0].NamePosition.Line;
                    NewDiagnostic.range.start.character = Attribute[0].NamePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Name.size();

                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    return;
                }
                AttributeOffset += 1;
            }
            if(AttributeOffset >= Attribute.size())
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Empy attribute is invalid";
                NewDiagnostic.range.start.line = Attribute[0].NamePosition.Line;
                NewDiagnostic.range.start.character = Attribute[0].NamePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Name.size();

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
            //Guaranteed to be player attributes at this point
            if(!(Attribute[AttributeOffset].Name == "Character" || Attribute[AttributeOffset].Name == "Tag" || Attribute[AttributeOffset].Name == "Code"))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Invalid player member: \""+Attribute[AttributeOffset].Name+"\"";
                NewDiagnostic.range.start.line = Attribute[AttributeOffset].NamePosition.Line;
                NewDiagnostic.range.start.character = Attribute[AttributeOffset].NamePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[AttributeOffset].Name.size();

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
        }
    }
    void SpecEvaluator::p_VerifyGameInfoPredicate(GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(PredicateToVerify.Data.IsType<GameInfoPredicate_Direct>())
        {
            p_VerifyGameInfoPredicate_Direct(PredicateToVerify.Data.GetType<GameInfoPredicate_Direct>(),IsPlayerAssignment,OutDiagnostics);
        }
        else if(PredicateToVerify.Data.IsType<GameInfoPredicate_Variable>())
        {
            auto const& VariablePredicate = PredicateToVerify.Data.GetType<GameInfoPredicate_Variable>();
            if(!m_TopContext.GlobalScope.HasVariable(VariablePredicate.VariableName))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "No variable named \"" + VariablePredicate.VariableName + "\" in global scope";
                NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
            else
            {
                auto const& Variable = m_TopContext.GlobalScope.GetVariable(VariablePredicate.VariableName);
                if(std::holds_alternative<MQL_LazyGameList>(Variable.Data))
                {
                       
                }
                else if(!std::holds_alternative<MQL_Variable_GameInfoPredicate>(Variable.Data))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Variable isn't of type GameInfoPredicate";
                    NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                    NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
                else
                {
                    auto const& PredicateData = std::get<MQL_Variable_GameInfoPredicate>(Variable.Data);
                    if(IsPlayerAssignment && !PredicateData.IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Variable isn't of type GameInfoPredicate";
                        NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                        NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
                        OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    }
                    else  if(!IsPlayerAssignment && PredicateData.IsPlayerAssignment)
                    {
                        MBLSP::Diagnostic NewDiagnostic;
                        NewDiagnostic.message = "Variable isn't of type PlayerAssignment";
                        NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                        NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                        NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
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
        if(FilterToVerify.Data.IsType<Filter_Component_Variable>())
        {
            auto const& VariablePredicate = FilterToVerify.Data.GetType<Filter_Component_Variable>();
            if(!m_TopContext.GlobalScope.HasVariable(VariablePredicate.VariableName))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "No variable named \"" + VariablePredicate.VariableName + "\" in global scope";
                NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
            else
            {
                auto const& FilterVariable = m_TopContext.GlobalScope.GetVariable(VariablePredicate.VariableName);   
                if(!std::holds_alternative<Filter_Component>(FilterVariable.Data))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Variable isn't of type Filter";
                    NewDiagnostic.range.start.line = VariablePredicate.VariablePosition.Line;
                    NewDiagnostic.range.start.character = VariablePredicate.VariablePosition.ByteOffset;
                    NewDiagnostic.range.end = NewDiagnostic.range.start + VariablePredicate.VariableName.size()+1;
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
            }
        }
        else if(FilterToVerify.Data.IsType<Filter_Component_Function>())
        {
            auto const& FilterValue = FilterToVerify.Data.GetType<Filter_Component_Function>();
            if(m_BuiltinFilters.find(FilterValue.FilterName) == m_BuiltinFilters.end() &&
                    m_FilterToServer.find(FilterValue.FilterName) == m_FilterToServer.end())
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Can't find filter with name \""+FilterValue.FilterName+"\"";
                NewDiagnostic.range.start.line = FilterValue.NamePosition.Line;
                NewDiagnostic.range.start.character = FilterValue.NamePosition.ByteOffset;
                NewDiagnostic.range.end = NewDiagnostic.range.start + FilterValue.FilterName.size();
                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            }
        }
        else if(FilterToVerify.Data.IsEmpty())
        {
            //Do nothing
        }
        else
        {
            assert(false && "VerifyFilterComponent doesn't cover all cases");
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
    bool SpecEvaluator::p_EvaluateGameSelection(SlippiGameInfo const& GameInfo,bool IsSwapped,GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;
        if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Direct>())
        {
            auto const& DirectData = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Direct>();
            SlippiGamePlayerInfo const* Player1Pointer = &GameInfo.PlayerInfo[0];
            SlippiGamePlayerInfo const* Player2Pointer = &GameInfo.PlayerInfo[1];
            if(IsSwapped)
            {
                std::swap(Player1Pointer,Player1Pointer);   
            }
            SlippiGamePlayerInfo  const*  PlayerToEvaluatePointer = Player1Pointer;
            if(DirectData.Attribute[0].Name == "Player2")
            {
                PlayerToEvaluatePointer = Player2Pointer;
            }
            SlippiGamePlayerInfo const& CurrentPlayer = *PlayerToEvaluatePointer;
            if(DirectData.Attribute[0].Name == "Player1" || DirectData.Attribute[0].Name == "Player2")
            {
                if(DirectData.Attribute[1].Name == "Character")
                {
                    //ReturnValue = CurrentPlayer.Character == PredicateToEvaluate.Value;   
                    ReturnValue = h_Comp(CurrentPlayer.Character,DirectData.Comparison,DirectData.Value);
                } 
                else if(DirectData.Attribute[1].Name == "Code")
                {
                    //ReturnValue = CurrentPlayer.Code == PredicateToEvaluate.Value;   
                    ReturnValue = h_Comp(CurrentPlayer.Code,DirectData.Comparison,DirectData.Value);
                }
                else if(DirectData.Attribute[1].Name == "Tag")
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
                if(DirectData.Attribute[0].Name == "Stage")
                {
                    ReturnValue = GameInfo.Stage == DirectData.Value;
                }
                else if(DirectData.Attribute[0].Name == "Date")
                {
                    ReturnValue = h_Comp(GameInfo.Date,DirectData.Comparison,DirectData.DateValue);
                    //ReturnValue = h_Comp(GameInfo.Date,PredicateToEvaluate.Operator,
                }
            }
        }
        else if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Variable>())
        {
            auto const& VariableData = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Variable>();
            MQL_Variable& Variable = m_TopContext.GlobalScope.GetVariable(VariableData.VariableName);
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
                            return(lhs.Metadata.GamePath < rhs.RelativePath);
                        });
                ReturnValue = LowestIt != GameList.Games.end() && LowestIt->Metadata.GamePath == GameInfo.RelativePath;
            }
            else if(std::holds_alternative<MQL_Variable_GameInfoPredicate>(Variable.Data))
            {
                MQL_Variable_GameInfoPredicate const& DereferencedPredicate = std::get<MQL_Variable_GameInfoPredicate>(Variable.Data);
                ReturnValue = p_EvaluateGameSelection(GameInfo,IsSwapped,DereferencedPredicate.Predicate);
            }
        }
        else if(PredicateToEvaluate.Data.IsEmpty())
        {
            //do nothing   
        }
        else
        {
            assert(false && "p_EvaluateGameSelection doesn't cover all cases for GameInfoPredicate.Data");
        }
        for(auto const& ExtraTerm : PredicateToEvaluate.ExtraTerms)
        {
            if(ExtraTerm.Operator == "||")
            {
                if(!ReturnValue)
                {
                    ReturnValue = p_EvaluateGameSelection(GameInfo,IsSwapped,ExtraTerm);           
                }
            }
            else if(ExtraTerm.Operator == "&&")
            {
                if(!p_EvaluateGameSelection(GameInfo,IsSwapped,ExtraTerm))
                {
                    ReturnValue = false;   
                    break;
                }
            }
            else if(ExtraTerm.Operator == "")
            {
                ReturnValue = p_EvaluateGameSelection(GameInfo,IsSwapped,ExtraTerm);
            }
            else
            {
                assert(false && "Only valid operators are && and ||");   
            }
        }
        return(ReturnValue);
    }
    bool SpecEvaluator::p_SatisfiesPlayerAssignment(SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;

        if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Direct>())
        {
            auto const& DirectData = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Direct>();
            if(DirectData.Attribute[0].Name == "Character")
            {
                ReturnValue = PlayerInfo.Character == DirectData.Value;
            }
            else if(DirectData.Attribute[0].Name == "Tag")
            {
                ReturnValue = PlayerInfo.Tag == DirectData.Value;
            }
            else if(DirectData.Attribute[0].Name == "Code")
            {
                ReturnValue = PlayerInfo.Code == DirectData.Value;
            }
            else
            {
                assert(false && "Only Tag,Character,Code is recognized as player attribute");   
            }
        }
        else if(PredicateToEvaluate.Data.IsType<GameInfoPredicate_Variable>())
        {
            auto const& VariableData  = PredicateToEvaluate.Data.GetType<GameInfoPredicate_Variable>();
            GameInfoPredicate const& DereferencedPredicate =std::get<MQL_Variable_GameInfoPredicate>(
                    m_TopContext.GlobalScope.GetVariable(VariableData.VariableName).Data).Predicate;
            ReturnValue = p_SatisfiesPlayerAssignment(PlayerInfo,DereferencedPredicate);
        }
        else if(PredicateToEvaluate.Data.IsEmpty())
        {
            //do nothjing   
        }
        else
        {
            assert(false && "p_SatisfiesPlayerAssignment doesn't cover all cases for PredicateToEvaluate.Data");
        }
        for(auto const& ExtraTerm : PredicateToEvaluate.ExtraTerms)
        {
            if(ExtraTerm.Operator == "||")
            {
                if(!ReturnValue)
                {
                    ReturnValue = p_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm);
                }
            }
            else if(ExtraTerm.Operator == "&&")
            {
                if(!p_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm))
                {
                    ReturnValue = false;   
                    break;
                }
            }
            else if(ExtraTerm.Operator == "")
            {
                ReturnValue = p_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm);
            }
            else
            {
                assert(false && "Only valid operators are && and ||");   
            }
        }
        return(ReturnValue);
    }
    bool SpecEvaluator::p_IsPlayersSwapped(SlippiGameInfo const& GameInfo, GameInfoPredicate const& PredicateToEvaluate,bool& IsSwapped)
    {
        bool ReturnValue = false;        
        if(PredicateToEvaluate.Data.IsEmpty())
        {
            return(true);   
        }
        if(p_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[1],PredicateToEvaluate))
        {
           IsSwapped = true;   
           ReturnValue = true;
        } 
        else
        {
            ReturnValue = p_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[0],PredicateToEvaluate);   
            if(ReturnValue)
            {
               IsSwapped = false;    
            }
        }
        return(ReturnValue);
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
        //TODO improve this could with SQL querry so unneccesary game info doesn't need to be parsed
        std::vector<SlippiGameInfo> Candidates = m_DBAdapter->RetrieveGames("");
        for(auto& Candidate : Candidates)
        {
            bool IsSwapped = false;
            if(p_IsPlayersSwapped(Candidate,SpecToEvalaute.Assignment.PlayerCondition,IsSwapped))
            {
                if(p_EvaluateGameSelection(Candidate,IsSwapped,SpecToEvalaute.GameCondition))
                {
                    std::ifstream  GameData(Candidate.RelativePath,std::ios::in|std::ios::binary);
                    if(GameData.is_open())
                    {
                        MBUtility::MBFileInputStream InStream(&GameData);
                        MeleeGame GameToAdd;
                        MBError ParseResult = MeleeGame::ParseSlippiGame(InStream,GameToAdd);
                        GameToAdd.Metadata.GamePath = Candidate.RelativePath;
                        if(ParseResult)
                        {
                            if(IsSwapped)
                            {
                                h_SwapPlayers(GameToAdd);
                            }   
                            ReturnValue.emplace_back(std::move(GameToAdd));
                        }
                    }
                }
            }
        }
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::p_EvaluateGameIntervalls(Filter_Component const& FilterToUse,
            GameIntervall CurrentIntervall,MeleeGame const& GameToFilter)
    {
        std::vector<GameIntervall> ReturnValue;


        if(FilterToUse.Data.IsType<Filter_Component_Function>())
        {
            auto const& FunctionData = FilterToUse.Data.GetType<Filter_Component_Function>();
            if(auto BuiltinFilter = m_BuiltinFilters.find(FunctionData.FilterName); BuiltinFilter != m_BuiltinFilters.end())
            {
                ReturnValue = BuiltinFilter->second(GameToFilter,FilterToUse.ArgumentList,CurrentIntervall);
            }
            else if(auto ServerIndex = m_FilterToServer.find(FunctionData.FilterName); ServerIndex != m_FilterToServer.end())
            {
                ReturnValue = m_SpecServers[ServerIndex->second].ExecuteFilter(FunctionData.FilterName,GameToFilter,CurrentIntervall);
            }
            else
            {
                assert(false && "No server or builtin filter found, evaluating spec that shouldn't have been verified");
            }
        }
        else if(FilterToUse.Data.IsType<Filter_Component_Variable>())
        {
            auto const& VariableData = FilterToUse.Data.GetType<Filter_Component_Variable>();
            Filter_Component const& DerferencedFilter = std::get<Filter_Component>(
                m_TopContext.GlobalScope.GetVariable(VariableData.VariableName).Data);
            ReturnValue = p_EvaluateGameIntervalls(DerferencedFilter,CurrentIntervall,GameToFilter);
        }
        else if(FilterToUse.Data.IsEmpty() && FilterToUse.ExtraTerms.size() == 0)
        {
            ReturnValue.push_back(CurrentIntervall);
            return(ReturnValue);
        }
        else
        {
            assert(false && "p_EvaluateGameIntervalls doesn't cover all cases for FilterToUse.Data");   
        }
        for(auto const& ExtraFilter : FilterToUse.ExtraTerms)
        {
            if(ExtraFilter.Operator == "&")
            {
                std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(ExtraFilter,CurrentIntervall,GameToFilter); 
                std::vector<GameIntervall> NewReturnValue;
                NewReturnValue.resize(NewSituations.size()+ReturnValue.size());
                std::merge(NewSituations.begin(),NewSituations.end(),ReturnValue.begin(),ReturnValue.end(),NewReturnValue.begin());
                ReturnValue = std::move(NewReturnValue);
            }       
            else if(ExtraFilter.Operator == "|")
            {
                std::vector<std::vector<GameIntervall>> NewIntervalls;
                for(auto const& Intervall : ReturnValue)
                {
                    NewIntervalls.push_back(p_EvaluateGameIntervalls(ExtraFilter,Intervall,GameToFilter));
                } 
                ReturnValue = MBUtility::Merge<GameIntervall>(NewIntervalls.begin(),NewIntervalls.end());
            }
            else if(ExtraFilter.Operator == "")
            {
                ReturnValue = p_EvaluateGameIntervalls(ExtraFilter,CurrentIntervall,GameToFilter);
            }
            else
            {
                assert(false && "Only | and & are valid operators for filters");   
            }

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
        for(auto const& Game : GamesToInspect)
        {
            //assumes are sorted
            auto Intervalls = p_EvaluateGameIntervalls(SpecToEvaluate.SituationFilter.Component,GameIntervall(0,Game.Frames.size()-1),Game);
            if(Intervalls.size() != 0)
            {
                RecordingInfo NewRecording;
                NewRecording.GamePath = Game.Metadata.GamePath;
                NewRecording.IntervallsToRecord = std::move(Intervalls);
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
        float PunisherIndex = 0;
        float PunisheeIndex = 1;
        float ExtractCount = 10;
        std::vector<i_PunishInfo> Punishes = h_ExtractPunishes2(GameToInspect,PunisherIndex,PunisheeIndex,20);
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
    int h_ParseExpandInteger(std::string const& StringToParse)
    {
        int ReturnValue = 0;
        try
        {
            ReturnValue = std::stoi(StringToParse);
        }   
        catch(std::exception const& e)
        {
            throw std::runtime_error("Error parsing expand integer: "+std::string(e.what()));
        }
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
        else if(auto LeftIt = ExtraArguments.KeyArguments.find("Left"); LeftIt != ExtraArguments.KeyArguments.end())
        {
            LeftExpand = -h_ParseExpandInteger(LeftIt->second);   
        }
        IntervallToInspect.FirstFrame += LeftExpand;
        IntervallToInspect.LastFrame += RightExpand;
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
    std::vector<GameIntervall> SpecEvaluator::SpecialState(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        ReturnValue = {IntervallToInspect};
        return(ReturnValue);
    }
    std::vector<GameIntervall> SpecEvaluator::HasProjectile(MeleeGame const& GameToInspect,ArgumentList const& ExtraArguments,GameIntervall IntervallToInspect)
    {
        std::vector<GameIntervall> ReturnValue;
        ReturnValue = {IntervallToInspect};
        return(ReturnValue);
    }
}
