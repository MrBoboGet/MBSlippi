#pragma once
#include "MBSlippiTypes.h"
#include <MBUtility/MBErrorHandling.h>
#include <MBUtility/MBInterfaces.h>
#include "MeleeID.h"
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

        Null
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
        Null
	};
	
    enum class MBCharacter
    {
            
        Mario	    , 
        Fox	        , 
        Captain     , 
        Donkey      , 
        Kirby	    , 
        Bowser	    , 
        Link	    , 
        Sheik	    , 
        Ness	    , 
        Peach	    , 
        Popo	    , 
        Nana	    , 
        Pikachu	    , 
        Samus	    , 
        Yoshi	    , 
        Jigglypuff	, 
        Mewtwo	    , 
        Luigi	    , 
        Marth	    , 
        Zelda	    , 
        YoungLink   , 
        DrMario     , 
        Falco	    , 
        Pichu	    , 
        MrGameAndWatch , 
        Ganondorf	, 
        Roy	        , 
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
    struct PlayerFrameInfo
    {
        MBActionState ActionState = MBActionState::None;
        MBActionStateFlags StateFlags;
        MBAttackID ActiveAttack = MBAttackID::None;
        int ActionableFrames = 0;
        float Percent = 0;
    };
    struct FrameInfo
    {
        PlayerFrameInfo PlayerInfo[4];
        int FrameNumber = 0;
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
}
