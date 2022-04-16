#pragma once
#include "MBSlippiTypes.h"
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

		DashAttack
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
	};
	//Multiple may be valid and applicable to multiple states
	enum class MBActionStateModifiers : uint64_t
	{
		Hitlag = 1<<0,
		FastFalling = 1<<1,
		Airborne = 1<<2,
	};
	MBAttackID StateToMBAttackID(Event_PostFrameUpdate const& AssociatedState);
	MBActionState StateToMBActionState(Event_PostFrameUpdate const& AssociatedState);
	uint64_t StateToMBStateModifierMap(Event_PostFrameUpdate const& AssociatedState);

	std::string MBAttackIDToString(MBAttackID IDToConvert);
	std::string MBActionStateToString(MBActionState StateToConvert);

	MBAttackID StringToMBAttackID(std::string const& StringToConvert);
	MBActionState StringToMBActionState(std::string const& StringToConvert);
}