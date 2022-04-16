#include "MBMeleeID.h"

namespace MBSlippi
{
	MBAttackID StateToMBAttackID_Fox(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}
	MBAttackID StateToMBAttackID_Peach(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}
	MBAttackID StateToMBAttackID_Marth(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}
	MBAttackID StateToMBAttackID_Falco(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}
	MBAttackID StateToMBAttackID_JigglyPuff(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}
	MBAttackID StateToMBAttackID_Falcon(Event_PostFrameUpdate const& AssociatedState)
	{
		return(MBAttackID::None);
	}


	MBAttackID(*__InternalCharacterIDAttackIDFunctionMap[])(Event_PostFrameUpdate const& AssociatedState) = {
		StateToMBAttackID,

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
		//TODO specials
		return(ReturnValue);
	}
	MBActionState StateToMBActionState(Event_PostFrameUpdate const& AssociatedState)
	{
		MBActionState ReturnValue = MBActionState::None;
		if (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::ShieldActive))
		{
			ReturnValue = MBActionState::Shielding;
		}
		else if (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::InHitstun))
		{
			ReturnValue = MBActionState::HitStun;
		}
		else if (AssociatedState.ActionStateID == ActionState::GuardSetOff)
		{
			ReturnValue = MBActionState::ShieldStun;
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
		else if (StateToMBAttackID(AssociatedState) != MBAttackID::None)
		{
			ReturnValue = MBActionState::Attacking;
		}
		return(ReturnValue);
	}
	uint64_t StateToMBStateModifierMap(Event_PostFrameUpdate const& AssociatedState)
	{
		uint64_t ReturnValue = 0;
		ReturnValue += uint64_t(MBActionStateModifiers::Airborne) *    (AssociatedState.Airborne);
		ReturnValue += uint64_t(MBActionStateModifiers::Hitlag) *      (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::InHitlag));
		ReturnValue += uint64_t(MBActionStateModifiers::FastFalling) * (AssociatedState.StateBitFlags & uint64_t(StateBitFlags::FastFalling));
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
		"DashAttack"
   
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




  


	MBAttackID StringToMBAttackID(std::string const& StringToConvert)
	{
		return(MBAttackID::None);
	}
	MBActionState StringToMBActionState(std::string const& StringToConvert)
	{
		return(MBActionState::Attacking);
	}
}
