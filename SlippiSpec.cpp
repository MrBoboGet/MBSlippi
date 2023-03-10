#include "SlippiSpec.h"
#include "SlippiSpecParser.h"
#include <memory>
#include <MBUtility/Merge.h>
namespace MBSlippi
{
    bool h_IsValidDate(std::string const& DateToVerify)
    {
        return(true);       
    }
    bool h_IsValidStage(std::string const& StageToVerify)
    {
        return(true);    
    }
    void SpecEvaluator::p_VerifyAttribute(std::vector<std::string> const& Attribute,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
    }
    void SpecEvaluator::p_VerifyGameInfoPredicate(GameInfoPredicate const& PredicateToVerify,bool IsPlayerAssignment,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        std::vector<std::string> const& Attribute = PredicateToVerify.Attribute;
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
            if(Attribute[0] == "Date" || Attribute[0] == "Stage")
            {
                if(Attribute.size() > 1)
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = Attribute[0] +" doesn't have any members";
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    return;
                }
                if(IsPlayerAssignment)
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Only player attributes can be a part of a player specification";
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                    return;
                }
            }
            if(Attribute[0] == "Date")
            {
                if(!(h_IsValidDate(PredicateToVerify.Value)))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Invalid date for predicate string";
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
            }
            else if(Attribute[0] == "Stage")
            {
                if(!(h_IsValidStage(PredicateToVerify.Value)))
                {
                    MBLSP::Diagnostic NewDiagnostic;
                    NewDiagnostic.message = "Invalid date for predicate string";
                    OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                }
            }
            else if(!(Attribute[0] == "Player1" || Attribute[0] == "Player2"))
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Invalid attribute: \""+Attribute[0]+"\"";

                OutDiagnostics.emplace_back(std::move(NewDiagnostic));
                return;
            }
            AttributeOffset += 1;
        }
        if(AttributeOffset >= Attribute.size())
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Empy attribute is invalid";

            OutDiagnostics.emplace_back(std::move(NewDiagnostic));
        }
        //Guaranteed to be player attributes at this point
        if(Attribute[AttributeOffset] != "Character" || Attribute[AttributeOffset] != "Tag" || Attribute[AttributeOffset] != "Code")
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Invalid player member: \""+Attribute[AttributeOffset]+"\"";

            OutDiagnostics.emplace_back(std::move(NewDiagnostic));
        }
        for(auto const& SubPredicate : PredicateToVerify.ExtraTerms)
        {
            p_VerifyGameInfoPredicate(SubPredicate,IsPlayerAssignment,OutDiagnostics);
        }
    }
    void SpecEvaluator::p_VerifyFilterComponent(Filter_Component const& FilterToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        if(m_BuiltinFilters.find(FilterToVerify.FilterName) == m_BuiltinFilters.end())
        {
            MBLSP::Diagnostic NewDiagnostic;
            NewDiagnostic.message = "Can't find builtin filter with name \""+FilterToVerify.FilterName+"\"";

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
    bool SpecEvaluator::VerifySpec(SlippiSpec const& SpecToVerify,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
    {
        bool ReturnValue = false;
        std::vector<MBLSP::Diagnostic> Diagnostics;
        if(SpecToVerify.Assignment.AffectedPlayer != "")
        {
            if(SpecToVerify.Assignment.AffectedPlayer != "Player1" || SpecToVerify.Assignment.AffectedPlayer != "Player2")
            {
                MBLSP::Diagnostic NewDiagnostic;
                NewDiagnostic.message = "Only Player1/Player2 is allowed in player assignment";

                Diagnostics.emplace_back(std::move(NewDiagnostic));
            }  
            p_VerifyGameInfoPredicate(SpecToVerify.Assignment.PlayerCondition,true,Diagnostics);
        } 
        p_VerifyGameInfoPredicate(SpecToVerify.Games.GameCondition,false,Diagnostics);
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
        if(PredicateToEvaluate.Attribute[0] == "Player2")
        {
            PlayerToEvaluatePointer = Player2Pointer;
        }
        SlippiGamePlayerInfo const& CurrentPlayer = *PlayerToEvaluatePointer;
        if(PredicateToEvaluate.Attribute[0] == "Player1" || PredicateToEvaluate.Attribute[1] == "Player2")
        {
            if(PredicateToEvaluate.Attribute[1] == "Character")
            {
                ReturnValue = CurrentPlayer.Character == PredicateToEvaluate.Value;   
            } 
            else if(PredicateToEvaluate.Attribute[1] == "Code")
            {
                ReturnValue = CurrentPlayer.Code == PredicateToEvaluate.Value;   
            }
            else if(PredicateToEvaluate.Attribute[1] == "Tag")
            {
                ReturnValue = CurrentPlayer.Tag == PredicateToEvaluate.Value;   
            }
            else
            {
                assert(false && "Only Tag,Character,Code is recognized as player attribute");   
            }
        }
        else
        {
            if(PredicateToEvaluate.Attribute[0] == "Stage")
            {
                ReturnValue = GameInfo.Stage == PredicateToEvaluate.Value;
            }
            else if(PredicateToEvaluate.Attribute[0] == "Date")
            {
                assert(false && "Date not implemented yet B)");   
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
        if(PredicateToEvaluate.Attribute[0] == "Character")
        {
            ReturnValue = PlayerInfo.Character == PredicateToEvaluate.Value;
        }
        else if(PredicateToEvaluate.Attribute[0] == "Tag")
        {
            ReturnValue = PlayerInfo.Tag == PredicateToEvaluate.Value;
        }
        else if(PredicateToEvaluate.Attribute[0] == "Code")
        {
            ReturnValue = PlayerInfo.Tag == PredicateToEvaluate.Value;
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
        bool ReturnValue = true;        
        if(h_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[1],PredicateToEvaluate))
        {
           IsSwapped = true;   
        } 
        else
        {
            ReturnValue = h_SatisfiesPlayerAssignment(GameInfo.PlayerInfo[0],PredicateToEvaluate);   
        }
        return(ReturnValue);
    }
    void SpecEvaluator::SetDBAdapter(std::unique_ptr<MeleeGameDBAdapter> NewAdapter)
    {
        m_DBAdapter = std::move(NewAdapter);    
    }
    void SpecEvaluator::SetRecorder(std::unique_ptr<MeleeGameRecorder> NewRecorder)
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
        BuiltinFilterType BuiltinFilter = m_BuiltinFilters[FilterToUse.FilterName];
        ReturnValue = BuiltinFilter(GameToFilter,FilterToUse.ArgumentList,CurrentIntervall);
        for(auto const& ExtraFilter : FilterToUse.ExtraTerms)
        {
            if(FilterToUse.Operator == "&")
            {
                std::vector<GameIntervall> NewSituations = p_EvaluateGameIntervalls(ExtraFilter,CurrentIntervall,GameToFilter); 
                std::vector<GameIntervall> NewReturnValue;
                NewReturnValue.resize(NewSituations.size()+ReturnValue.size());
                std::merge(NewSituations.begin(),NewSituations.end(),ReturnValue.begin(),ReturnValue.end(),NewReturnValue.begin());
                ReturnValue = std::move(NewReturnValue);
            }       
            else if(FilterToUse.Operator == "|")
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
    void SpecEvaluator::EvaluateSpec(SlippiSpec const& SpecToEvaluate,std::vector<MBLSP::Diagnostic>& OutDiagnostics)
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
}
