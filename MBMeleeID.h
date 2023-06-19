#pragma once
#include "MBSlippiTypes.h"
#include <MBUtility/MBErrorHandling.h>
#include <MBUtility/MBInterfaces.h>
#include "MeleeID.h"
#include <MBParsing/MBParsing.h>

#include <cmath>
namespace MBSlippi
{
    enum class MBAttackID : uint8_t
    {
        None,
        
        SideB,
        UpB,
        DownB,
        NeutralB,

        Fair,
        Dair,
        Nair,
        Bair,
        Uair,
        
        DownTilt,
        UpTilt,
        ForwardTilt,
        Jab,
        
        ForwardSmash,
        DownSmash,
        UpSmash,

        DashAttack,
        //
        Grab,
        UpThrow,
        DownThrow,
        BackThrow,
        ForwardThrow,
        ItemThrow,

        Null
    };


    enum class MBMoveID
    {
        LRoll,
        RRoll,
        SpotDodge
    };
    //Mutually exclusive
    //OBS! only add stats to the bottom
    enum class MBActionState : uint16_t
    {
        None,
        HitStun,
        ShieldStun,
        Shielding,
        Running,
        Walking,
        Falling,
        Tumbling,
        Dashing,
        Attacking,
        //
        Grabbed,
        Dead,
        //
        LedgeHang,
        LedgeAttack,
        LedgeRoll,
        LedgeJump,
        LedgeRegular,
        SpotDodge,
        Roll,
        MissedTech,
        TechInPlace,
        TechRoll,
        Airdodge,
        Null
    };
    
    enum class MBCharacter
    {
            
        Mario        , 
        Fox            , 
        Captain     , 
        Donkey      , 
        Kirby        , 
        Bowser        , 
        Link        , 
        Sheik        , 
        Ness        , 
        Peach        , 
        Popo        , 
        Nana        , 
        Pikachu        , 
        Samus        , 
        Yoshi        , 
        Jigglypuff    , 
        Mewtwo        , 
        Luigi        , 
        Marth        , 
        Zelda        , 
        YoungLink   , 
        DrMario     , 
        Falco        , 
        Pichu        , 
        MrGameAndWatch , 
        Ganondorf    , 
        Roy            , 
        Null
    };

    struct MBActionStateFlags
    {
        bool InHitlag : 1;
        bool FastFalling : 1;
        bool Airborne : 1;
        MBActionStateFlags()
        {
            InHitlag = false;
            FastFalling = false;
            Airborne = false;
        }
    };
    //MBActionStateFlags StringToStateFlags(std::string const& StringToConvert);
    
    MBAttackID StateToMBAttackID(Event_PostFrameUpdate const& AssociatedState);
    MBActionState StateToMBActionState(Event_PostFrameUpdate const& AssociatedState);
    MBActionStateFlags FrameToMBActionStateFlags(Event_PostFrameUpdate const& AssociatedState);
    //uint64_t StateToMBStateModifierMap(Event_PostFrameUpdate const& AssociatedState);

    MBCharacter InternalCharacterIDToMBCharacter(InternalCharacterID IDToConvert);
    std::string MBCharacterToString(MBCharacter CharacterToConvert); 


    std::string MBAttackIDToString(MBAttackID IDToConvert);
    std::string MBActionStateToString(MBActionState StateToConvert);

    MBAttackID StringToMBAttackID(std::string const& StringToConvert);
    MBActionState StringToMBActionState(std::string const& StringToConvert);
    
    bool StateIsActionable(MBActionState StateToInspect);
    struct PlayerInfo
    {
        std::string Tag;
        std::string Code;
        MBCharacter Character = MBCharacter::Null;
    };

    struct Position
    {
        float x = 0;
        float y = 0;

        float Distance(Position OtherPos)
        {
            return(std::hypot(OtherPos.x-x,OtherPos.y-y));
        }
    };
    struct PlayerFrameInfo
    {
        MBActionState ActionState = MBActionState::None;
        MBActionStateFlags StateFlags;
        MBAttackID ActiveAttack = MBAttackID::None;
        int ActionableFrames = 0;
        float Percent = 0;
        Position PlayerPosition;

        MBParsing::JSONObject ToJSON() const
        {
            MBParsing::JSONObject ReturnValue(MBParsing::JSONObjectType::Aggregate);
            ReturnValue["actionState"] = MBActionStateToString(ActionState);
            ReturnValue["activeAttack"] = MBAttackIDToString(ActiveAttack);
            ReturnValue["inHitlag"] = StateFlags.InHitlag;
            ReturnValue["fastFalling"] = StateFlags.FastFalling;
            ReturnValue["airborne"] = StateFlags.Airborne;
            ReturnValue["actionableFrames"] = ActionableFrames;
            ReturnValue["percent"] = Percent;
            ReturnValue["x"] = PlayerPosition.x;
            ReturnValue["y"] = PlayerPosition.y;
            return(ReturnValue);
        }
    };
    struct FrameInfo
    {
        PlayerFrameInfo PlayerInfo[4];
        int FrameNumber = 0;
        MBParsing::JSONObject ToJSON() const
        {
            MBParsing::JSONObject ReturnValue;
            ReturnValue = std::vector<MBParsing::JSONObject> {PlayerInfo[0].ToJSON(),PlayerInfo[1].ToJSON(),PlayerInfo[2].ToJSON(),PlayerInfo[3].ToJSON()};
            return(ReturnValue);
        }
    };
    struct MeleeGameMetadata
    {
        std::string GamePath;       
    };
    struct MeleeGame
    {
        MeleeGameMetadata Metadata;
        PlayerInfo Players[4];
        StageID Stage;
        std::vector<FrameInfo> Frames;
        static MBError ParseSlippiGame(MBUtility::MBOctetInputStream& InputStream,MeleeGame& OutGame);
    };
    struct SlippiGamePlayerInfo
    {
        std::string Code;
        std::string Tag;
        std::string Character;
        //Color?
    };
    struct SlippiGameInfo
    {
        std::string AbsolutePath;
        uint64_t Date;
        std::string Stage;
        SlippiGamePlayerInfo PlayerInfo[4];
        uint64_t FrameDuration = 0;
    };
}
