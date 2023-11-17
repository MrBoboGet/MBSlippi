#include "MBMeleeID.h"
#include "MBSlippiTypes.h"
#include <stdint.h>

#include <assert.h>

#include "MBSlippiParsing.h"
namespace MBSlippi
{
    //MBActionStateFlags StringToStateFlags(std::string const& StringToConvert)
    //{
    //    MBActionStateFlags ReturnValue;

    //    return(ReturnValue);
    //}
    bool StateIsActionable(MBActionState StateToInspect)
    {
        bool ReturnValue = false;       
        ReturnValue = (StateToInspect == MBActionState::None ||
               StateToInspect == MBActionState::Running ||
               StateToInspect == MBActionState::Null ||
               StateToInspect == MBActionState::Tumbling ||
               StateToInspect == MBActionState::Dashing ||
               StateToInspect == MBActionState::Shielding ||
               StateToInspect == MBActionState::Walking);
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_Fox(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Fox);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 341 && IDNumber <= 346)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 347 &&  IDNumber <= 352)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber >= 353 &&  IDNumber <= 359)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber >= 360 &&  IDNumber <= 369)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_Falco(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Falco);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 341 && IDNumber <= 346)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 347 &&  IDNumber <= 352)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber >= 353 &&  IDNumber <= 359)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber >= 360 &&  IDNumber <= 369)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_Peach(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Peach);
        //TODO add support for peach float states. Probably as a event flag rather than unique state here    
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 365 && IDNumber <= 368)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 354 &&  IDNumber <= 360)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber == 369 ||  IDNumber == 370)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber == 352 ||  IDNumber == 353)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_Marth(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Marth);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 341 && IDNumber <= 348)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 349 &&  IDNumber <= 366)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber == 367 || IDNumber == 368)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber >= 369 &&  IDNumber <= 372)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    
    inline MBAttackID StateToMBAttackID_JigglyPuff(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Jigglypuff);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 346 && IDNumber <= 362)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 363 &&  IDNumber <= 364)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber >= 365 &&  IDNumber <= 368)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber >= 369 &&  IDNumber <= 372)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_Falcon(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Captain);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if(IDNumber >= 347 && IDNumber <= 348)
        {
            ReturnValue = MBAttackID::NeutralB; 
        }
        else if(IDNumber >= 349 &&  IDNumber <= 352)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if(IDNumber >= 353 &&  IDNumber <= 356)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if(IDNumber >= 357 &&  IDNumber <= 363)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);    
    }
    inline MBAttackID StateToMBAttackID_Shiek(Event_PostFrameUpdate const& AssociatedState)
    {
        assert(AssociatedState.InternalCharID == InternalCharacterID::Sheik);
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t IDNumber = uint64_t(AssociatedState.ActionStateID);
        if (IDNumber >= 341 && IDNumber <= 348)
        {
            ReturnValue = MBAttackID::NeutralB;
        }
        else if (IDNumber >= 349 && IDNumber <= 354)
        {
            ReturnValue = MBAttackID::SideB;
        }
        else if (IDNumber >= 355 && IDNumber <= 360)
        {
            ReturnValue = MBAttackID::UpB;
        }
        else if (IDNumber >= 361 && IDNumber <= 364)
        {
            ReturnValue = MBAttackID::DownB;
        }
        return(ReturnValue);
    }
    inline MBAttackID StateToMBAttackID_None(Event_PostFrameUpdate const& AssociatedState)
    {
        return(MBAttackID::None);
    }
    

    /*
    enum class InternalCharacterID : uint8_t
    {
        Mario            =    0x00, 
        Fox                  =    0x01,
        Captain         =    0x02,
        Donkey          =    0x03,
        Kirby            =    0x04,
        Bowser            =    0x05,
        Link            =    0x06,
        Sheik            =    0x07,
        Ness            =    0x08,
        Peach            =    0x09,
        Popo            =    0x0A,
        Nana            =    0x0B,
        Pikachu            =    0x0C,
        Samus            =    0x0D,
        Yoshi            =    0x0E,
        Jigglypuff        =    0x0F,
        Mewtwo            =    0x10,
        Luigi            =    0x11,
        Marth            =    0x12,
        Zelda            =    0x13,
        YoungLink       =    0x14,
        DrMario         =    0x15,
        Falco            =    0x16,
        Pichu            =    0x17,
        MrGameWatch     =    0x18,
        Ganondorf        =    0x19,
        Roy                =    0x1A,
        MasterHand      =    0x1B,
        WireframeMale   =    0x1D,
        WireframeFemale =    0x1E,
        GigaBowser      =    0x1F,
        Sandbag            =    0x20, 
    };
    */


    static MBAttackID(*__InternalCharacterIDAttackIDFunctionMap[33])(Event_PostFrameUpdate const& AssociatedState) = {
        StateToMBAttackID_None,
        StateToMBAttackID_Fox,
        StateToMBAttackID_Falcon,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_Shiek,
        StateToMBAttackID_None,
        StateToMBAttackID_Peach,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_JigglyPuff,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_Marth,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_Falco,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
        StateToMBAttackID_None,
    };
    



    MBAttackID StateToMBAttackID(Event_PostFrameUpdate const& AssociatedState)
    {
        //__Map[0] = StateToMBAttackID;
        MBAttackID ReturnValue = MBAttackID::None;
        uint64_t ActionStateNumber = uint64_t(AssociatedState.ActionStateID);
        if (ActionStateNumber >= 65 && ActionStateNumber <= 69)
        {
            MBAttackID  Options[] = { MBAttackID::Nair,MBAttackID::Fair,MBAttackID::Bair,MBAttackID::Uair,MBAttackID::Dair };
            ReturnValue = Options[ActionStateNumber - 65];
        }
        else if(ActionStateNumber == 50)
        {
            ReturnValue = MBAttackID::DashAttack;
        }
        else if (ActionStateNumber >= 51 && ActionStateNumber <= 55)
        {
            ReturnValue = MBAttackID::ForwardTilt;
        }
        else if (ActionStateNumber == 56)
        {
            ReturnValue = MBAttackID::UpTilt;
        }
        else if
            (
                AssociatedState.ActionStateID == ActionState::Attack11 ||
                AssociatedState.ActionStateID == ActionState::Attack12 ||
                AssociatedState.ActionStateID == ActionState::Attack13 ||
                AssociatedState.ActionStateID == ActionState::Attack100Start ||
                AssociatedState.ActionStateID == ActionState::Attack100Loop ||
                AssociatedState.ActionStateID == ActionState::Attack100End
                )
        {
            ReturnValue = MBAttackID::Jab;
        }
        else if(
                AssociatedState.ActionStateID == ActionState::Catch        ||
                AssociatedState.ActionStateID == ActionState::CatchPull    ||
                AssociatedState.ActionStateID == ActionState::CatchDash    ||
                AssociatedState.ActionStateID == ActionState::CatchDashPull||
                AssociatedState.ActionStateID == ActionState::CatchWait    ||
                AssociatedState.ActionStateID == ActionState::CatchAttack  ||
                AssociatedState.ActionStateID == ActionState::CatchCut   
               )
        {
            ReturnValue = MBAttackID::Grab;
        }
        else if(AssociatedState.ActionStateID == ActionState::ThrowB)
        {
            ReturnValue = MBAttackID::BackThrow;
        }
        else if(AssociatedState.ActionStateID == ActionState::ThrowF)
        {
            ReturnValue = MBAttackID::ForwardThrow;
        }
        else if(AssociatedState.ActionStateID == ActionState::ThrowHi)
        {
            ReturnValue = MBAttackID::UpThrow;
        }
        else if(AssociatedState.ActionStateID == ActionState::ThrowLw)
        {
            ReturnValue = MBAttackID::DownThrow;
        }
        else if (ActionStateNumber == 57)
        {
            ReturnValue = MBAttackID::DownTilt;
        }
        else if (ActionStateNumber >= 58 && ActionStateNumber <= 62)
        {
            ReturnValue = MBAttackID::ForwardSmash;
        }
        else if (ActionStateNumber == 63)
        {
            ReturnValue = MBAttackID::UpSmash;
        }
        else if (ActionStateNumber == 64)
        {
            ReturnValue = MBAttackID::DownSmash;
        }
        //TODO take the effort to see of this can be optimized with an intervall
        else if(
                AssociatedState.ActionStateID == ActionState::LightThrowF ||
                AssociatedState.ActionStateID == ActionState::LightThrowB ||
                AssociatedState.ActionStateID == ActionState::LightThrowHi ||
                AssociatedState.ActionStateID == ActionState::LightThrowLw ||
                AssociatedState.ActionStateID == ActionState::LightThrowDash ||
                AssociatedState.ActionStateID == ActionState::LightThrowDrop ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirF ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirB ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirHi ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirLw ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowF ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowB ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowHi ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowLw ||
                AssociatedState.ActionStateID == ActionState::LightThrowF4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowB4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowHi4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowLw4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirF4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirB4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirHi4 ||
                AssociatedState.ActionStateID == ActionState::LightThrowAirLw4 ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowF4 ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowB4 ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowHi4 ||
                AssociatedState.ActionStateID == ActionState::HeavyThrowLw4
               )

        {
            ReturnValue = MBAttackID::ItemThrow;
        }
        else
        {
            ReturnValue = __InternalCharacterIDAttackIDFunctionMap[uint16_t(AssociatedState.InternalCharID)](AssociatedState);
        }
        //TODO specials
        return(ReturnValue);
    }
    MBActionState StateToMBActionState(Event_PostFrameUpdate const& AssociatedState)
    {
        MBActionState ReturnValue = MBActionState::None;
        if (AssociatedState.ActionStateID == ActionState::GuardSetOff)
        {
            ReturnValue = MBActionState::ShieldStun;
        }
        else if (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::ShieldActive))
        {
            ReturnValue = MBActionState::Shielding;
        }
        else if (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::InHitstun))
        {
            ReturnValue = MBActionState::HitStun;
        }
        else if (AssociatedState.ActionStateID == ActionState::WalkSlow || AssociatedState.ActionStateID == ActionState::WalkMiddle || AssociatedState.ActionStateID == ActionState::WalkFast)
        {
            ReturnValue = MBActionState::Walking;
        }
        else if (AssociatedState.ActionStateID == ActionState::Run || AssociatedState.ActionStateID == ActionState::RunDirect)
        {
            ReturnValue = MBActionState::Running;
        }
        else if (AssociatedState.ActionStateID == ActionState::Dash)
        {
            ReturnValue = MBActionState::Dashing;
        }
        else if (
                    AssociatedState.ActionStateID == ActionState::Fall || 
                    AssociatedState.ActionStateID == ActionState::FallB ||
                    AssociatedState.ActionStateID == ActionState::FallF ||
                    AssociatedState.ActionStateID == ActionState::FallAerial ||
                    AssociatedState.ActionStateID == ActionState::FallAerialF ||
                    AssociatedState.ActionStateID == ActionState::FallAerialB
                )
        {
            ReturnValue = MBActionState::Tumbling;
        }
        else if (AssociatedState.ActionStateID == ActionState::DamageFall)
        {
            ReturnValue = MBActionState::Tumbling;
        }
        else if(AssociatedState.ActionStateID == ActionState::CliffCatch || AssociatedState.ActionStateID == ActionState::CliffWait)
        {
            ReturnValue = MBActionState::LedgeHang;
        }
        else if(
                AssociatedState.ActionStateID == ActionState::CliffClimbSlow ||
                AssociatedState.ActionStateID == ActionState::CliffClimbQuick)
        {
            ReturnValue = MBActionState::LedgeRegular;   
        }
        else if(
                AssociatedState.ActionStateID == ActionState::CliffAttackSlow  ||
                AssociatedState.ActionStateID == ActionState::CliffAttackQuick)
        {
            ReturnValue = MBActionState::LedgeAttack;   
        }
        else if(
                AssociatedState.ActionStateID == ActionState::CliffEscapeSlow  ||
                AssociatedState.ActionStateID == ActionState::CliffEscapeQuick)
        {
            ReturnValue = MBActionState::LedgeRoll;
        }
        else if(
                AssociatedState.ActionStateID == ActionState::CliffJumpSlow1   ||
                AssociatedState.ActionStateID == ActionState::CliffJumpSlow2   ||
                AssociatedState.ActionStateID == ActionState::CliffJumpQuick1  ||
                AssociatedState.ActionStateID == ActionState::CliffJumpQuick2
                )
        {
            ReturnValue = MBActionState::LedgeJump;
        }
        else if(
            AssociatedState.ActionStateID == ActionState::Escape
            )
        {
            ReturnValue = MBActionState::SpotDodge;
        }
        else if(
            AssociatedState.ActionStateID == ActionState::EscapeAir
            )
        {
            ReturnValue = MBActionState::Airdodge;
        }
        else if(
            AssociatedState.ActionStateID == ActionState::EscapeB || AssociatedState.ActionStateID == ActionState::EscapeF
            )
        {
            ReturnValue = MBActionState::Roll;
        }
        else if(
            AssociatedState.ActionStateID == ActionState::DownBoundD || AssociatedState.ActionStateID == ActionState::DownBoundU
            )
        {
            ReturnValue = MBActionState::MissedTech;
        }
        else if(AssociatedState.ActionStateID == ActionState::Passive)
        {
            ReturnValue = MBActionState::TechInPlace;
        }
        else if(AssociatedState.ActionStateID == ActionState::PassiveStandF || AssociatedState.ActionStateID == ActionState::PassiveStandB)
        {
            ReturnValue = MBActionState::TechRoll;
        }
        else if(AssociatedState.ActionStateID == ActionState::JumpF || AssociatedState.ActionStateID == ActionState::JumpB)
        {
            ReturnValue = MBActionState::Jump;
        }
        else if(AssociatedState.ActionStateID == ActionState::JumpAerialB || AssociatedState.ActionStateID == ActionState::JumpAerialF)
        {
            ReturnValue = MBActionState::DoubleJump;
        }
        else if(
            AssociatedState.ActionStateID == ActionState::DeadDown ||
            AssociatedState.ActionStateID == ActionState::DeadLeft  ||              
            AssociatedState.ActionStateID == ActionState::DeadRight   ||            
            AssociatedState.ActionStateID == ActionState::DeadUp        ||          
            AssociatedState.ActionStateID == ActionState::DeadUpStar      ||        
            AssociatedState.ActionStateID == ActionState::DeadUpStarIce     ||      
            AssociatedState.ActionStateID == ActionState::DeadUpFallHitCamera ||    
            AssociatedState.ActionStateID == ActionState::DeadUpFallIce         ||  
            AssociatedState.ActionStateID == ActionState::DeadUpFallHitCameraIce)
        {
            ReturnValue = MBActionState::Dead;
        }
        else if(
                AssociatedState.ActionStateID == ActionState::CatchWait ||
                AssociatedState.ActionStateID == ActionState::CapturePulledHi ||
                AssociatedState.ActionStateID == ActionState::CaptureWaitHi   ||
                AssociatedState.ActionStateID == ActionState::CaptureDamageHi ||
                AssociatedState.ActionStateID == ActionState::CapturePulledLw ||
                AssociatedState.ActionStateID == ActionState::CaptureWaitLw   ||
                AssociatedState.ActionStateID == ActionState::CaptureDamageLw ||
                AssociatedState.ActionStateID == ActionState::CaptureCut      ||
                AssociatedState.ActionStateID == ActionState::CaptureJump     ||
                AssociatedState.ActionStateID == ActionState::CaptureNeck     ||
                AssociatedState.ActionStateID == ActionState::CaptureFoot  
               )
        {
            ReturnValue = MBActionState::Grabbed;
        }
        else if (StateToMBAttackID(AssociatedState) != MBAttackID::None)
        {
            ReturnValue = MBActionState::Attacking;
        }
        return(ReturnValue);
    }
    MBActionStateFlags FrameToMBActionStateFlags(Event_PostFrameUpdate const& AssociatedState)
    {
        MBActionStateFlags ReturnValue;
        ReturnValue.Airborne = AssociatedState.Airborne;
        ReturnValue.InHitlag = (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::InHitlag)) != 0;
        ReturnValue.FastFalling = (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::FastFalling)) != 0;
        return(ReturnValue);
    }

    const char* __MBAttackIDToStringMap[] = 
    {
        "None",
        "SideB",
        "UpB",
        "DownB",
        "NeutralB",
        "Fair",
        "Dair",
        "Nair",
        "Bair",
        "Uair",
        "DownTilt",
        "UpTilt",
        "ForwardTilt",
        "Jab",
        "ForwardSmash",
        "DownSmash",
        "UpSmash",
        "DashAttack",
        //
        "Grab",
        "UpThrow",
        "DownThrow",
        "BackThrow",
        "ForwardThrow",
        "ItemThrow",
    };
    std::string MBAttackIDToString(MBAttackID IDToConvert)
    {
        uint64_t IDIndex = uint64_t(IDToConvert);
        if(IDIndex >= sizeof(__MBAttackIDToStringMap)/sizeof(const char*))
        {
            throw std::runtime_error("Invalid MBAttackID");
        }
        return(__MBAttackIDToStringMap[IDIndex]);
    }

    const char* __MBActionStateToStringMap[] = 
    {
        "None",
        "HitStun",
        "ShieldStun",
        "Shielding",
        "Running",
        "Walking",
        "Falling",
        "Tumbling",
        "Dashing",
        "Attacking", 
        "Grabbed",
        "Dead",
        //
        "LedgeHang",
        "LedgeAttack",
        "LedgeRoll",
        "LedgeJump",
        "LedgeRegular",
        "SpotDodge",
        "Roll",
        "MissedTech",
        "TechInPlace",
        "TechRoll",
        "Airdodge",
        "Jump",
        "DoubleJump",
    };
    std::string MBActionStateToString(MBActionState StateToConvert)
    { 
        uint64_t IDIndex = uint64_t(StateToConvert);
        if(IDIndex >= sizeof(__MBActionStateToStringMap)/sizeof(const char*))
        {
            throw std::runtime_error("Invalid MBAttackID");
        }
        return(__MBActionStateToStringMap[IDIndex]);
    }




  

    //TODO use binary search or something
    MBAttackID StringToMBAttackID(std::string const& StringToConvert)
    {
        MBAttackID ReturnValue = MBAttackID::None;
        for(int i = 0; i < sizeof(__MBAttackIDToStringMap)/sizeof(const char*);i++)
        {
            if(__MBAttackIDToStringMap[i] == StringToConvert)
            {
                ReturnValue = MBAttackID(i);
                break;
            }
        }
        if(ReturnValue == MBAttackID::None)
        {
            throw std::runtime_error("No attack ID with name \""+StringToConvert+"\"");
        }
        return(ReturnValue);
    }
    MBActionState StringToMBActionState(std::string const& StringToConvert)
    {
        MBActionState ReturnValue = MBActionState::None;
        for(int i = 0; i < sizeof(__MBActionStateToStringMap)/sizeof(const char*);i++)
        {
            if(__MBActionStateToStringMap[i] == StringToConvert)
            {
                ReturnValue = MBActionState(i);
                break;
            }
        }
        if(ReturnValue == MBActionState::None)
        {
            throw std::runtime_error("No AtionState with name \""+StringToConvert+"\"");
        }
        return(ReturnValue);
    }
    MBCharacter InternalCharacterIDToMBCharacter(InternalCharacterID IDToConvert)
    {
        MBCharacter ReturnValue = MBCharacter::Null;     
        if (uint64_t(IDToConvert) <= uint64_t(InternalCharacterID::Roy))
        {
            ReturnValue = MBCharacter(IDToConvert);
        }
        return(ReturnValue);
    }

    const char* i_MBCharacterToStringMap[]
    {
        "Mario"        , 
        "Fox"            , 
        "Captain"     , 
        "Donkey"      , 
        "Kirby"        , 
        "Bowser"        , 
        "Link"        , 
        "Sheik"        , 
        "Ness"        , 
        "Peach"        , 
        "Popo"        , 
        "Nana"        , 
        "Pikachu"        , 
        "Samus"        , 
        "Yoshi"        , 
        "Jigglypuff"    , 
        "Mewtwo"        , 
        "Luigi"        , 
        "Marth"        , 
        "Zelda"        , 
        "YoungLink"   , 
        "DrMario"     , 
        "Falco"        , 
        "Pichu"        , 
        "MrGameAndWatch" , 
        "Ganondorf"    , 
        "Roy"            , 
        "Null"
    };
    std::string MBCharacterToString(MBCharacter CharacterToConvert)
    {
        std::string ReturnValue;
        if(uint64_t(CharacterToConvert) > uint64_t(MBCharacter::Null))
        {
            throw std::runtime_error("Invalid MBCharacter value");
        }
        ReturnValue = i_MBCharacterToStringMap[size_t(CharacterToConvert)];
        return(ReturnValue);
    }

    PlayerFrameInfo h_ParsePlayerFrameInfo(Event_PostFrameUpdate const& EventToParse)
    {
        PlayerFrameInfo ReturnValue;
        ReturnValue.ActionState =  StateToMBActionState(EventToParse);
        ReturnValue.StateFlags = FrameToMBActionStateFlags(EventToParse);
        ReturnValue.ActiveAttack = StateToMBAttackID(EventToParse);
        ReturnValue.Percent = EventToParse.Percent;
        ReturnValue.PlayerPosition.x = EventToParse.XPosition;
        ReturnValue.PlayerPosition.y = EventToParse.YPosition;
        //if (ReturnValue.ActionState == MBActionState::Null)
        //{
        //    int Hej = 2;
        //}
        return(ReturnValue);    
    }
    
    FrameInfo h_ParseFrameInfo(std::vector<Event> const& FrameEvents)
    {
        FrameInfo ReturnValue; 
        for(Event const& FrameEvent : FrameEvents)
        {
            if(FrameEvent.GetType() == EventType::PostFrameUpdate)
            {
                Event_PostFrameUpdate const& PlayerFrameUpdate = FrameEvent.GetEventData<Event_PostFrameUpdate>();
                ReturnValue.FrameNumber = PlayerFrameUpdate.FrameNumber;//updated many times but negligable
                if(PlayerFrameUpdate.PlayerIndex >= 4 || PlayerFrameUpdate.PlayerIndex < 0)
                {
                    throw std::runtime_error("Player index out of bounds when parsing player data");
                }
                ReturnValue.PlayerInfo[PlayerFrameUpdate.PlayerIndex] = h_ParsePlayerFrameInfo(PlayerFrameUpdate);
            }
        }          
        return(ReturnValue);
    }
    //BEGIN MeleeGame
    MBError MeleeGame::ParseSlippiGame(MBUtility::MBOctetInputStream& InputStream,MeleeGame& OutResult)
    {
        MeleeGame Result;
        MBError ReturnValue = true;
       
        try
        {
              
            MBParsing::JSONObject TotalFileData = MBParsing::ParseUBJSON(&InputStream,&ReturnValue);
            if (!ReturnValue)
            {
                return(ReturnValue);
            }
            std::string const& raw = TotalFileData["raw"].GetStringData();
            std::unique_ptr<MBUtility::MBBufferInputStream> BufferStream = std::unique_ptr<MBUtility::MBBufferInputStream>(new MBUtility::MBBufferInputStream(raw.data(), raw.size()));
            SlippiEventParser EventParser(std::move(BufferStream));
            EventParser.GetNextEvent();
            Event GameStart = EventParser.GetNextEvent();
            if (GameStart.GetType() != EventType::GameStart)
            {
                ReturnValue = false;
                ReturnValue.ErrorMessage = "Invalid Slippi replay file: First event is not of GameStart type";
                return(ReturnValue);
            }
            Event_GameStart StartEvent = GameStart.GetEventData<Event_GameStart>(); 
            GameInfoBlock StartInfo(StartEvent.GameInfoBlock,sizeof(StartEvent.GameInfoBlock));
            Result.Stage = StartInfo.Stage;
            
            std::map<std::string, MBParsing::JSONObject> const& PlayerArray = TotalFileData["metadata"]["players"].GetMapData();
            size_t PlayerOffset = 0;
            for (auto const& Element : PlayerArray)
            {
                //kanske borde göra en aggregate klass...
                if (Element.second["names"].HasAttribute("code"))
                {
                    Result.Players[PlayerOffset].Code = Element.second["names"]["code"].GetStringData();
                }
                if (Element.second["names"].HasAttribute("tag"))
                {
                    Result.Players[PlayerOffset].Tag = Element.second["names"]["tag"].GetStringData();
                }
                if (Element.second.HasAttribute("characters"))
                {
                    Result.Players[PlayerOffset].Character = InternalCharacterIDToMBCharacter(
                            InternalCharacterID(std::stoi(Element.second["characters"].GetMapData().begin()->first)));
                }
                PlayerOffset++;
            }

            FrameParser LocalFrameParser;
            int ActionableFrameCount[4] = {0,0,0,0};
            while (true)
            {
                Event NewEvent = EventParser.GetNextEvent();
                if (NewEvent.GetType() == EventType::Null)
                {
                    break;
                }
                LocalFrameParser.InsertEvent(std::move(NewEvent));
                if (LocalFrameParser.AvailableFrames() > 0)
                {
                    std::vector<Event> FrameEvents = LocalFrameParser.ExtractFrame();
                    Result.Frames.push_back(h_ParseFrameInfo(FrameEvents));
                    auto& CurrentFrame = Result.Frames.back();
                    for(int i = 0; i < 4; i++)
                    {
                        if(StateIsActionable(CurrentFrame.PlayerInfo[i].ActionState))
                        {
                            ActionableFrameCount[i] += 1;    
                        }
                        else
                        {
                            ActionableFrameCount[i] = 0;   
                        }
                        CurrentFrame.PlayerInfo[i].ActionableFrames = ActionableFrameCount[i];
                    }
                }
            }
        } 
        catch(std::exception const& e)
        {
            ReturnValue = false;
            ReturnValue.ErrorMessage = e.what();
            return(ReturnValue);   
        }
    

        OutResult = std::move(Result);
        return(ReturnValue);       
    }
    //END MeleeGame
}
