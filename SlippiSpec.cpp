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
    void SpecEvaluator::p_VerifyGameInfoPredicate(GameInfoPredicate& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        std::vector<AttributeComponent> const& Attribute = PredicateToVerify.Attribute;
        if(Attribute.size() == 0)
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Empy attribute is invalid";

            OutDiagnostics.emplace_back(std::move(NewDiagnostic));
            return;
        }
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
            NewDiagnostic.range.start.line = Attribute[0].NamePosition.Line;
            NewDiagnostic.range.start.character = Attribute[0].NamePosition.ByteOffset;
            NewDiagnostic.range.end = NewDiagnostic.range.start + Attribute[0].Name.size();

            OutDiagnostics.emplace_back(std::move(NewDiagnostic));
        }
        for(auto& SubPredicate : PredicateToVerify.ExtraTerms)
        {
            p_VerifyGameInfoPredicate(SubPredicate,IsPlayerAssignment,OutDiagnostics);
        }
    }
    void SpecEvaluator::p_VerifyFilterComponent(Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(m_BuiltinFilters.find(FilterToVerify.FilterName) == m_BuiltinFilters.end() &&
                m_FilterToServer.find(FilterToVerify.FilterName) == m_FilterToServer.end())
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Can't find filter with name \""+FilterToVerify.FilterName+"\"";
            NewDiagnostic.range.start.line = FilterToVerify.NamePosition.Line;
            NewDiagnostic.range.start.character = FilterToVerify.NamePosition.ByteOffset;
            NewDiagnostic.range.end = NewDiagnostic.range.start + FilterToVerify.FilterName.size();
            OutDiagnostics.emplace_back(std::move(NewDiagnostic));
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
    bool SpecEvaluator::VerifySpec(SlippiSpec& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = true;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        if(SpecToVerify.Assignment.AffectedPlayer != "")
        {
            if(!(SpecToVerify.Assignment.AffectedPlayer == "Player1" || SpecToVerify.Assignment.AffectedPlayer == "Player2"))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Only Player1/Player2 is allowed in player assignment";
                NewDiagnostic.range.start.line = SpecToVerify.Assignment.PlayerPosition.Line;
                NewDiagnostic.range.start.character = SpecToVerify.Assignment.PlayerPosition.ByteOffset;
                NewDiagnostic.range.end  = NewDiagnostic.range.start + SpecToVerify.Assignment.AffectedPlayer.size();

                Diagnostics.emplace_back(std::move(NewDiagnostic));
            }  
            p_VerifyGameInfoPredicate(SpecToVerify.Assignment.PlayerCondition,true,Diagnostics);
        } 
        p_VerifyGameInfoPredicate(SpecToVerify.Games.GameCondition,false,Diagnostics);
        p_VerifyFilter(SpecToVerify.SituationFilter,OutDiagnostics);
        if(Diagnostics.size() > 0)
        {
            ReturnValue = false;
        }
        return(ReturnValue);
    }
    bool SpecEvaluator::p_EvaluateGameSelection(SlippiGameInfo const& GameInfo,bool IsSwapped,GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;
        SlippiGamePlayerInfo const* Player1Pointer = &GameInfo.PlayerInfo[0];
        SlippiGamePlayerInfo const* Player2Pointer = &GameInfo.PlayerInfo[1];
        if(IsSwapped)
        {
            std::swap(Player1Pointer,Player1Pointer);   
        }
        SlippiGamePlayerInfo  const*  PlayerToEvaluatePointer = Player1Pointer;
        if(PredicateToEvaluate.Attribute[0].Name == "Player2")
        {
            PlayerToEvaluatePointer = Player2Pointer;
        }
        SlippiGamePlayerInfo const& CurrentPlayer = *PlayerToEvaluatePointer;
        if(PredicateToEvaluate.Attribute[0].Name == "Player1" || PredicateToEvaluate.Attribute[0].Name == "Player2")
        {
            if(PredicateToEvaluate.Attribute[1].Name == "Character")
            {
                //ReturnValue = CurrentPlayer.Character == PredicateToEvaluate.Value;   
                ReturnValue = h_Comp(CurrentPlayer.Character,PredicateToEvaluate.Comparison,PredicateToEvaluate.Value);
            } 
            else if(PredicateToEvaluate.Attribute[1].Name == "Code")
            {
                //ReturnValue = CurrentPlayer.Code == PredicateToEvaluate.Value;   
                ReturnValue = h_Comp(CurrentPlayer.Code,PredicateToEvaluate.Comparison,PredicateToEvaluate.Value);
            }
            else if(PredicateToEvaluate.Attribute[1].Name == "Tag")
            {
                //ReturnValue = CurrentPlayer.Tag == PredicateToEvaluate.Value;   
                ReturnValue = h_Comp(CurrentPlayer.Tag,PredicateToEvaluate.Comparison,PredicateToEvaluate.Value);
            }
            else
            {
                assert(false && "Only Tag,Character,Code is recognized as player attribute");   
            }
        }
        else
        {
            if(PredicateToEvaluate.Attribute[0].Name == "Stage")
            {
                ReturnValue = GameInfo.Stage == PredicateToEvaluate.Value;
            }
            else if(PredicateToEvaluate.Attribute[0].Name == "Date")
            {
                ReturnValue = h_Comp(GameInfo.Date,PredicateToEvaluate.Comparison,PredicateToEvaluate.DateValue);
                //ReturnValue = h_Comp(GameInfo.Date,PredicateToEvaluate.Operator,
            }
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
            else
            {
                assert(false && "Only valid operators are && and ||");   
            }
        }
        return(ReturnValue);
    }
    bool h_SatisfiesPlayerAssignment(SlippiGamePlayerInfo const& PlayerInfo,GameInfoPredicate const& PredicateToEvaluate)
    {
        bool ReturnValue = true;
        if(PredicateToEvaluate.Attribute[0].Name == "Character")
        {
            ReturnValue = PlayerInfo.Character == PredicateToEvaluate.Value;
        }
        else if(PredicateToEvaluate.Attribute[0].Name == "Tag")
        {
            ReturnValue = PlayerInfo.Tag == PredicateToEvaluate.Value;
        }
        else if(PredicateToEvaluate.Attribute[0].Name == "Code")
        {
            ReturnValue = PlayerInfo.Code == PredicateToEvaluate.Value;
        }
        else
        {
            assert(false && "Only Tag,Character,Code is recognized as player attribute");   
        }
        for(auto const& ExtraTerm : PredicateToEvaluate.ExtraTerms)
        {
            //errenous until parsing is fixed
            if(ExtraTerm.Operator == "||")
            {
                if(!ReturnValue)
                {
                    ReturnValue = h_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm);
                }
            }
            else if(ExtraTerm.Operator == "&&")
            {
                if(!h_SatisfiesPlayerAssignment(PlayerInfo,ExtraTerm))
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
    bool SpecEvaluator::p_IsPlayersSwapped(SlippiGameInfo const& GameInfo, GameInfoPredicate const& PredicateToEvaluate,bool& IsSwapped)
    {
        bool ReturnValue = false;        
        if(h_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[1],PredicateToEvaluate))
        {
           IsSwapped = true;   
        } 
        else
        {
            ReturnValue = h_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[0],PredicateToEvaluate);   
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
    std::vector<MeleeGame> SpecEvaluator::p_RetrieveSpecGames(SlippiSpec const& SpecToEvalaute)
    {
        std::vector<MeleeGame> ReturnValue;
        //TODO improve this could with SQL querry so unneccesary game info doesn't need to be parsed
        std::vector<SlippiGameInfo> Candidates = m_DBAdapter->RetrieveGames("");
        for(auto& Candidate : Candidates)
        {
            bool IsSwapped = false;
            if(p_IsPlayersSwapped(Candidate,SpecToEvalaute.Assignment.PlayerCondition,IsSwapped))
            {
                if(p_EvaluateGameSelection(Candidate,IsSwapped,SpecToEvalaute.Games.GameCondition))
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
        if(FilterToUse.FilterName == "")
        {
            ReturnValue.push_back(CurrentIntervall);
            return(ReturnValue);
        }         
        if(auto BuiltinFilter = m_BuiltinFilters.find(FilterToUse.FilterName); BuiltinFilter != m_BuiltinFilters.end())
        {
            ReturnValue = BuiltinFilter->second(GameToFilter,FilterToUse.ArgumentList,CurrentIntervall);
        }
        else if(auto ServerIndex = m_FilterToServer.find(FilterToUse.FilterName); ServerIndex != m_FilterToServer.end())
        {
            ReturnValue = m_SpecServers[ServerIndex->second].ExecuteFilter(FilterToUse.FilterName,GameToFilter,CurrentIntervall);
        }
        else
        {
            assert(false && "No server or builtin filter found, evaluating spec that shouldn't have been verified");
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
            else
            {
                assert(false && "Only | and & are valid operators for filters");   
            }

        }
        return(ReturnValue);
    }
    void SpecEvaluator::EvaluateSpec(SlippiSpec& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {       
        if(m_DBAdapter == nullptr)
        {
            throw std::runtime_error("DBAdapter need to be set in order to evaluate SlippiSpec");  
        } 
        if(m_Recorder == nullptr)
        {
            throw std::runtime_error("Recorder need to be set in order to evaluate SlippiSpec");   
        }
        if(!VerifySpec(SpecToEvaluate,OutDiagnostics))
        {
            return;   
        }
        std::vector<MeleeGame> GamesToInspect = p_RetrieveSpecGames(SpecToEvaluate);
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
        if(ExtraArguments.PositionalArguments.size() != 0)
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
