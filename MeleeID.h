#pragma once
#include <cstdint>
#include <stdint.h>
#include <string>
namespace MBSlippi
{
	enum class ActionState : uint16_t
	{
		DeadDown			    = 0,    //Standard downward death
		DeadLeft				= 1,    //Standard leftward death
		DeadRight				= 2,    //Standard rightward death
		DeadUp					= 3,    //Upward death used in 1P "Team Kirby", etc.
		DeadUpStar				= 4,    //Standard Star KO
		DeadUpStarIce			= 5,    //Star KO while encased in ice(Freezie)
		Unused1                 = 6,
		Unused2                 = 7,
		DeadUpFallHitCamera	    = 8, //
		DeadUpFallIce			= 9,    //
		DeadUpFallHitCameraIce	= 10,    //
		Rebirth				    = 11, //	"Nothing" state, probably - it is the state Shiek / Zelda is in when their counterpart is the one currently playing.Also state of Nana when Sopo is alive
		Unused3                 = 12,
		RebirthWait			    = 13, //	Waiting on halo
		Wait					= 14,    //Standing state
		WalkSlow				= 15,    //
		WalkMiddle				= 16,    //
		WalkFast				= 17,    //
		Turn					= 18,    //
		TurnRun				    = 19, //
		Dash					= 20,    //
		Run					    = 21, //
		RunDirect				= 22,    //
		RunBrake				= 23,    //
		KneeBend				= 24,    //Pre - jump animation
		JumpF					= 25,    //First jump forward
		JumpB					= 26,    //First jump backward
		JumpAerialF			    = 27, //	Aerial jump forward
		JumpAerialB			    = 28, //	Aerial jump backward
		Fall					= 29,    //Falling straight down
		FallF					= 30,    //Falling with forward DI
		FallB					= 31,    //Falling with backward DI
		FallAerial				= 32,    //Falling after the second jump
		FallAerialF			    = 33, //	Falling after the second jump with forward DI
		FallAerialB			    = 34, //	Falling after the second jump with backward DI
		FallSpecial			    = 35, //	Special fall after UpB or airdodge
		FallSpecialF			= 36,    //Special fall with forward DI
		FallSpecialB			= 37,    //Special fall with backward DI
		DamageFall				= 38,    //Tumbling
		Squat					= 39,    //Going from stand to crouch
		SquatWait				= 40,    //Crouching
		SquatRv				    = 41, //	Going from crouch to stand
		Landing				    = 42, //
		LandingFallSpecial		= 43,    //Landing from special fall
		Attack11				= 44,    //Jab 1
		Attack12				= 45,    //Jab 2
		Attack13				= 46,    //Jab 3
		Attack100Start			= 47,    //Start of a rapid jab
		Attack100Loop			= 48,    //Middle of a rapid jab
		Attack100End			= 49,    //End of a rapid jab
		AttackDash				= 50,    //Dash attack
		AttackS3Hi				= 51,    //High Ftilt
		AttackS3HiS			    = 52, //	High - mid Ftilt
		AttackS3S				= 53,    //Mid Ftilt
		AttackS3LwS			    = 54, //	Low - mid Ftilt
		AttackS3Lw				= 55,    //Low Ftilt
		AttackHi3				= 56,    //Uptilt
		AttackLw3				= 57,    //Downtilt
		AttackS4Hi				= 58,    //High Fsmash
		AttackS4HiS			    = 59, //	High - mid Fsmash
		AttackS4S				= 60,    //Mid Fsmash
		AttackS4LwS			    = 61, //	Low - mid Fsmash
		AttackS4Lw				= 62,    //Low Fsmash
		AttackHi4				= 63,    //Upsmash
		AttackLw4				= 64,    //Downsmash
		AttackAirN				= 65,    //Nair
		AttackAirF				= 66,    //Fair
		AttackAirB				= 67,    //Bair
		AttackAirHi			    = 68, //	Uair
		AttackAirLw			    = 69, //	Dair
		LandingAirN			    = 70, //	Landing during Nair
		LandingAirF			    = 71, //	Landing during Fair
		LandingAirB			    = 72, //	Landing during Bair
		LandingAirHi			= 73,    //Landing during Uair
		LandingAirLw			= 74,    //Landing during Dair
		DamageHi1				= 75,    //
		DamageHi2				= 76,    //
		DamageHi3				= 77,    //
		DamageN1				= 78,    //
		DamageN2				= 79,    //
		DamageN3				= 80,    //
		DamageLw1				= 81,    //
		DamageLw2				= 82,    //
		DamageLw3				= 83,    //
		DamageAir1				= 84,    //
		DamageAir2				= 85,    //
		DamageAir3				= 86,    //
		DamageFlyHi			    = 87, //
		DamageFlyN				= 88,    //
		DamageFlyLw			    = 89, //
		DamageFlyTop			= 90,    //
		DamageFlyRoll			= 91,    //
		LightGet				= 92,    //Picking up an item
		HeavyGet				= 93,    //Picking up a heavy item(barrel)
		LightThrowF			    = 94, //	Throwing items at standard speed	0x80095a30
		LightThrowB			    = 95, //
		LightThrowHi			= 96,    //
		LightThrowLw			= 97,    //
		LightThrowDash			= 98,    //
		LightThrowDrop			= 99,    //
		LightThrowAirF			= 100,    //
		LightThrowAirB			= 101,    //
		LightThrowAirHi		    = 102, //
		LightThrowAirLw		    = 103, //
		HeavyThrowF			    = 104, //
		HeavyThrowB			    = 105, //
		HeavyThrowHi			= 106,    //
		HeavyThrowLw			= 107,    //
		LightThrowF4			= 108,    //Throwing items at Smash speed
		LightThrowB4			= 109,    //
		LightThrowHi4			= 110,    //
		LightThrowLw4			= 111,    //
		LightThrowAirF4		    = 112, //
		LightThrowAirB4		    = 113, //
		LightThrowAirHi4		= 114,    //
		LightThrowAirLw4		= 115,    //
		HeavyThrowF4			= 116,    //
		HeavyThrowB4			= 117,    //
		HeavyThrowHi4			= 118,    //
		HeavyThrowLw4			= 119,    //
		SwordSwing1			    = 120, //	Beam sword swings
		SwordSwing3			    = 121, //
		SwordSwing4			    = 122, //
		SwordSwingDash			= 123,    //
		BatSwing1				= 124,    //Home Run Bat swings
		BatSwing3				= 125,    //
		BatSwing4				= 126,    //
		BatSwingDash			= 127,    //
		ParasolSwing1			= 128,    //Parasol swings
		ParasolSwing3			= 129,    //
		ParasolSwing4			= 130,    //
		ParasolSwingDash		= 131,    //
		HarisenSwing1			= 132,    //Fan swings
		HarisenSwing3			= 133,    //
		HarisenSwing4			= 134,    //
		HarisenSwingDash		= 135,    //
		StarRodSwing1			= 136,    //Star Rod swings
		StarRodSwing3			= 137,    //
		StarRodSwing4			= 138,    //
		StarRodSwingDash		= 139,    //
		LipStickSwing1			= 140,    //Lip's Stick swings	
		LipStickSwing3			= 141,    //
		LipStickSwing4			= 142,    //
		LipStickSwingDash		= 143,    //
		ItemParasolOpen		    = 144, //
		ItemParasolFall		    = 145, //
		ItemParasolFallSpecial	= 146,    //
		ItemParasolDamageFall	= 147,    //
		LGunShoot				= 148,    //Raygun shots
		LGunShootAir			= 149,    //
		LGunShootEmpty			= 150,    //
		LGunShootAirEmpty		= 151,    //
		FireFlowerShoot		    = 152, //
		FireFlowerShootAir		= 153,    //
		ItemScrew				= 154,    //
		ItemScrewAir			= 155,    //
		DamageScrew			    = 156, //
		DamageScrewAir			= 157,    //
		ItemScopeStart			= 158,    //
		ItemScopeRapid			= 159,    //
		ItemScopeFire			= 160,    //
		ItemScopeEnd			= 161,    //
		ItemScopeAirStart		= 162,    //
		ItemScopeAirRapid		= 163,    //
		ItemScopeAirFire		= 164,    //
		ItemScopeAirEnd		    = 165, //
		ItemScopeStartEmpty	    = 166, //
		ItemScopeRapidEmpty	    = 167, //
		ItemScopeFireEmpty		= 168,    //
		ItemScopeEndEmpty		= 169,    //
		ItemScopeAirStartEmpty	= 170,    //
		ItemScopeAirRapidEmpty	= 171,    //
		ItemScopeAirFireEmpty	= 172,    //
		ItemScopeAirEndEmpty	= 173,    //
		LiftWait				= 174,    //
		LiftWalk1				= 175,    //
		LiftWalk2				= 176,    //
		LiftTurn				= 177,    //
		GuardOn				    = 178, //	Shield startup	0x80093a50
		Guard					= 179,    //Holding shield	0x80
		GuardOff				= 180,    //Shield release	0x80
		GuardSetOff			    = 181, //	Shield stun
		GuardReflect			= 182,    //0x80
		DownBoundU				= 183,    //The missed tech bounce, facing up
		DownWaitU				= 184,    //Lying on ground facing up
		DownDamageU			    = 185, //	Getting hit by a low knockback move lying on ground facing up
		DownStandU				= 186,    //Neutral getup
		DownAttackU			    = 187, //	Get up attack from ground face up
		DownFowardU			    = 188, //	Missed tech roll forward
		DownBackU				= 189,    //Missed tech roll backward
		DownSpotU				= 190,    //Does not appear to be used, may have been a stick down option for missed tech at one point
		DownBoundD				= 191,    //The missed tech bounce, facing down
		DownWaitD				= 192,    //Lying on ground facing down	0x80
		DownDamageD			    = 193, //	Getting hit by a low knockback move lying on the ground facing down	0x80
		DownStandD				= 194,    //Neutral getup	0x80
		DownAttackD			    = 195, //	Get up attack from ground face down	0x80
		DownFowardD			    = 196, //	Missed tech roll forward	0x80
		DownBackD				= 197,    //Missed tech roll backward	0x80
		DownSpotD				= 198,    //Does not appear to be used, may have been a stick down option for missed tech at one point	0x80
		Passive				    = 199, //	Neutral tech
		PassiveStandF			= 200,    //Forward tech
		PassiveStandB			= 201,    //Backward tech
		PassiveWall			    = 202, //	Wall tech
		PassiveWallJump		    = 203, //	Walljump tech / plain walljump
		PassiveCeil			    = 204, //	Ceiling tech
		ShieldBreakFly			= 205,    //
		ShieldBreakFall		    = 206, //
		ShieldBreakDownU		= 207,    //
		ShieldBreakDownD		= 208,    //
		ShieldBreakStandU		= 209,    //
		ShieldBreakStandD		= 210,    //
		FuraFura				= 211,    //Shield - break tottering	0x80099010
		Catch					= 212,    //Grab	0x800d8c54
		CatchPull				= 213,    //Successfully grabbing a character - pulling them in	0x80
		CatchDash				= 214,    //0x80
		CatchDashPull			= 215,    //0x80
		CatchWait				= 216,    //Grabbing and holding a character	0x80
		CatchAttack			    = 217, //	Pummel	0x80
		CatchCut				= 218,    //When opponent breaks of a character's grab	0x80
		ThrowF					= 219,    //Forward throw	0x80
		ThrowB					= 220,    //Back throw	0x80
		ThrowHi				    = 221, //	Up throw	0x80
		ThrowLw				    = 222, //	Down throw	0x80
		CapturePulledHi		    = 223, //	0x80
		CaptureWaitHi			= 224,    //0x80
		CaptureDamageHi		    = 225, //	0x80
		CapturePulledLw		    = 226, //	Becoming grabbed	0x80
		CaptureWaitLw			= 227,    //When grabbed	0x80
		CaptureDamageLw		    = 228, //	Pummeled	0x80
		CaptureCut				= 229,    //Grab release	0x80
		CaptureJump			    = 230, //	Jumping mash out of grab	0x80
		CaptureNeck			    = 231, //	Does not appear to be used	0x80
		CaptureFoot			    = 232, //	Does not appear to be used	0x80
		EscapeF				    = 233, //	roll forward	0x80
		EscapeB				    = 234, //	roll backward	0x80
		Escape					= 235,    //spotdodge	0x800998ec
		EscapeAir				= 236,    //airdodge	0x80099a9c
		ReboundStop			    = 237, //
		Rebound				    = 238, //
		ThrownF				    = 239, //	Thrown forward
		ThrownB				    = 240, //	Thrown backward
		ThrownHi				= 241,    //Thrown up
		ThrownLw				= 242,    //Thrown down
		ThrownLwWomen			= 243,    //Thrown down as a female character
		Pass					= 244,    //Drop through platform	0x80
		Ottotto				    = 245, //	Ledge teeter
		OttottoWait			    = 246, //
		FlyReflectWall			= 247,    //Missed walltech
		FlyReflectCeil			= 248,    //Missed ceiling tech
		StopWall				= 249,    //Wall bonk
		StopCeil				= 250,    //Ceiling bonk
		MissFoot				= 251,    //Backward shield slideoff
		CliffCatch				= 252,    //Catching the ledge	0x80
		CliffWait				= 253,    //Hanging on the ledge	0x80
		CliffClimbSlow			= 254,    //Climbing the ledge(100 % +)	0x80
		CliffClimbQuick		    = 255, //	Climbing the ledge(< 100 %)	0x80
		CliffAttackSlow		    = 256, //	Ledge attack(100 % +)	0x80
		CliffAttackQuick		= 257,    //Ledge attack(< 100 %)	0x80
		CliffEscapeSlow		    = 258, //	Ledge roll(100 % +)	0x80
		CliffEscapeQuick		= 259,    //Ledge roll(< 100 %)	0x80
		CliffJumpSlow1			= 260,    //Ledge jump / tournament winner(100 % +)
		CliffJumpSlow2			= 261,    //Ledge jump / tournament winner(100 % +)
		CliffJumpQuick1		    = 262, //	Ledge jump / tournament winner(< 100 %)
		CliffJumpQuick2		    = 263, //	Ledge jump / tournament winner(< 100 %)
		AppealR				    = 264, //	Taunt right	0x80
		AppealL				    = 265, //	Taunt left	0x80
		ShoulderedWait			= 266,    //DK carry
		ShoulderedWalkSlow		= 267,    //
		ShoulderedWalkMiddle	= 268,    //
		ShoulderedWalkFast		= 269,    //
		ShoulderedTurn			= 270,    //
		ThrownFF				= 271,    //DK carry throws
		ThrownFB				= 272,    //
		ThrownFHi				= 273,    //
		ThrownFLw				= 274,    //
		CaptureCaptain			= 275,    //
		CaptureYoshi			= 276,    //
		YoshiEgg				= 277,    //
		CaptureKoopa			= 278,    //
		CaptureDamageKoopa		= 279,    //
		CaptureWaitKoopa		= 280,    //
		ThrownKoopaF			= 281,    //
		ThrownKoopaB			= 282,    //
		CaptureKoopaAir		    = 283, //
		CaptureDamageKoopaAir	= 284,    //
		CaptureWaitKoopaAir	    = 285, //
		ThrownKoopaAirF		    = 286, //
		ThrownKoopaAirB		    = 287, //
		CaptureKirby			= 288,    //
		CaptureWaitKirby		= 289,    //
		ThrownKirbyStar		    = 290, //
		ThrownCopyStar			= 291,    //
		ThrownKirby			    = 292, //
		BarrelWait				= 293,    //
		Bury					= 294,    //Stuck in ground by DK side B or similar
		BuryWait				= 295,    //
		BuryJump				= 296,    //
		DamageSong				= 297,    //Put to sleep by Jigglypuff up B or similar
		DamageSongWait			= 298,    //
		DamageSongRv			= 299,    //
		DamageBind				= 300,    //Hit by Mewtwo's Disable	
		CaptureMewtwo			= 301,    //Does not appear to be used
		CaptureMewtwoAir		= 302,    //Does not appear to be used
		ThrownMewtwo			= 303,    //Hit by Mewtwo's Confusion	
		ThrownMewtwoAir		    = 304, //	Hit by Mewtwo's Confusion	
		WarpStarJump			= 305,    //
		WarpStarFall			= 306,    //
		HammerWait				= 307,    //
		HammerWalk				= 308,    //
		HammerTurn				= 309,    //
		HammerKneeBend			= 310,    //
		HammerFall				= 311,    //
		HammerJump				= 312,    //
		HammerLanding			= 313,    //
		KinokoGiantStart		= 314,    //Super / Poison mushroom states
		KinokoGiantStartAir	    = 315, //
		KinokoGiantEnd			= 316,    //
		KinokoGiantEndAir		= 317,    //
		KinokoSmallStart		= 318,    //
		KinokoSmallStartAir	    = 319, //
		KinokoSmallEnd			= 320,    //
		KinokoSmallEndAir		= 321,    //
		Entry					= 322,    //Warp in at beginning of match.
		EntryStart				= 323,    //
		EntryEnd				= 324,    //
		DamageIce				= 325,    //
		DamageIceJump			= 326,    //
		CaptureMasterhand		= 327,    //
		CapturedamageMasterhand = 328,    //
		CapturewaitMasterhand	= 329,    //
		ThrownMasterhand		= 330,    //
		CaptureKirbyYoshi		= 331,    //
		KirbyYoshiEgg			= 332,    //
		CaptureLeadead			= 333,    //
		CaptureLikelike		    = 334, //
		DownReflect			    = 335, //
		CaptureCrazyhand		= 336,    //
		CapturedamageCrazyhand	= 337,    //
		CapturewaitCrazyhand	= 338,    //
		ThrownCrazyhand		    = 339, //
		BarrelCannonWait		= 340,    //
		Wait1					= 341,    //No general action states at this point or later are used, it's all character-specific action states	
		Wait2					= 342,    //
		Wait3					= 343,    //
		Wait4					= 344,    //
		WaitItem				= 345,    //
		SquatWait1				= 346,    //
		SquatWait2				= 347,    //
		SquatWaitItem			= 348,    //
		GuardDamage			    = 349, //
		EscapeN				    = 350, //
		AttackS4Hold			= 351,    //
		HeavyWalk1				= 352,    //
		HeavyWalk2				= 353,    //
		ItemHammerWait			= 354,    //
		ItemHammerMove			= 355,    //
		ItemBlind				= 356,    //
		DamageElec				= 357,    //
		FuraSleepStart			= 358,    //
		FuraSleepLoop			= 359,    //
		FuraSleepEnd			= 360,    //
		WallDamage				= 361,    //
		CliffWait1				= 362,    //
		CliffWait2				= 363,    //
		SlipDown				= 364,    //
		Slip					= 365,    //
		SlipTurn				= 366,    //
		SlipDash				= 367,    //
		SlipWait				= 368,    //
		SlipStand				= 369,    //
		SlipAttack				= 370,    //
		SlipEscapeF			    = 371, //
		SlipEscapeB			    = 372, //
		AppealS				    = 373, //
		Zitabata				= 374,    //
		CaptureKoopaHit		    = 375, //
		ThrownKoopaEndF		    = 376, //
		ThrownKoopaEndB		    = 377, //
		CaptureKoopaAirHit		= 378,    //
		ThrownKoopaAirEndF		= 379,    //
		ThrownKoopaAirEndB		= 380,    //
		ThrownKirbyDrinkSShot	= 381,    //
		ThrownKirbySpitSShot	= 382,    //
	};
    static const const char* __ActionStateStringMap[] = 
    {        
        "DeadDown",			    
        "DeadLeft",				
        "DeadRight",				
        "DeadUp",					
        "DeadUpStar",				
        "DeadUpStarIce",			
        "Unused1",                 
        "Unused2",                 
        "DeadUpFallHitCamera",	    
        "DeadUpFallIce",			
        "DeadUpFallHitCameraIce",	
        "Rebirth",				    
        "Unused3",                 
        "RebirthWait",			    
        "Wait",					
        "WalkSlow",				
        "WalkMiddle",				
        "WalkFast",				
        "Turn",					
        "TurnRun",				    
        "Dash",					
        "Run",					    
        "RunDirect",				
        "RunBrake",				
        "KneeBend",				
        "JumpF",					
        "JumpB",					
        "JumpAerialF",			    
        "JumpAerialB",			    
        "Fall",					
        "FallF",					
        "FallB",					
        "FallAerial",				
        "FallAerialF",			    
        "FallAerialB",			    
        "FallSpecial",			    
        "FallSpecialF",			
        "FallSpecialB",			
        "DamageFall",				
        "Squat",					
        "SquatWait",				
        "SquatRv",				    
        "Landing",				    
        "LandingFallSpecial",		
        "Attack11",				
        "Attack12",				
        "Attack13",				
        "Attack100Start",			
        "Attack100Loop",			
        "Attack100End",			
        "AttackDash",				
        "AttackS3Hi",				
        "AttackS3HiS",			    
        "AttackS3S",				
        "AttackS3LwS",			    
        "AttackS3Lw",				
        "AttackHi3",				
        "AttackLw3",				
        "AttackS4Hi",				
        "AttackS4HiS",			    
        "AttackS4S",				
        "AttackS4LwS",			    
        "AttackS4Lw",				
        "AttackHi4",				
        "AttackLw4",				
        "AttackAirN",				
        "AttackAirF",				
        "AttackAirB",				
        "AttackAirHi",			    
        "AttackAirLw",			    
        "LandingAirN",			    
        "LandingAirF",			    
        "LandingAirB",			    
        "LandingAirHi",			
        "LandingAirLw",			
        "DamageHi1",				
        "DamageHi2",				
        "DamageHi3",				
        "DamageN1",				
        "DamageN2",				
        "DamageN3",				
        "DamageLw1",				
        "DamageLw2",				
        "DamageLw3",				
        "DamageAir1",				
        "DamageAir2",				
        "DamageAir3",				
        "DamageFlyHi",			    
        "DamageFlyN",				
        "DamageFlyLw",			    
        "DamageFlyTop",			
        "DamageFlyRoll",			
        "LightGet",				
        "HeavyGet",				
        "LightThrowF",			    
        "LightThrowB",			    
        "LightThrowHi",			
        "LightThrowLw",			
        "LightThrowDash",			
        "LightThrowDrop",			
        "LightThrowAirF",			
        "LightThrowAirB",			
        "LightThrowAirHi",		    
        "LightThrowAirLw",		    
        "HeavyThrowF",			    
        "HeavyThrowB",			    
        "HeavyThrowHi",			
        "HeavyThrowLw",			
        "LightThrowF4",			
        "LightThrowB4",			
        "LightThrowHi4",			
        "LightThrowLw4",			
        "LightThrowAirF4",		    
        "LightThrowAirB4",		    
        "LightThrowAirHi4",		
        "LightThrowAirLw4",		
        "HeavyThrowF4",			
        "HeavyThrowB4",			
        "HeavyThrowHi4",			
        "HeavyThrowLw4",			
        "SwordSwing1",			    
        "SwordSwing3",			    
        "SwordSwing4",			    
        "SwordSwingDash",			
        "BatSwing1",				
        "BatSwing3",				
        "BatSwing4",				
        "BatSwingDash",			
        "ParasolSwing1",			
        "ParasolSwing3",			
        "ParasolSwing4",			
        "ParasolSwingDash",		
        "HarisenSwing1",			
        "HarisenSwing3",			
        "HarisenSwing4",			
        "HarisenSwingDash",		
        "StarRodSwing1",			
        "StarRodSwing3",			
        "StarRodSwing4",			
        "StarRodSwingDash",		
        "LipStickSwing1",			
        "LipStickSwing3",			
        "LipStickSwing4",			
        "LipStickSwingDash",		
        "ItemParasolOpen",		    
        "ItemParasolFall",		    
        "ItemParasolFallSpecial",	
        "ItemParasolDamageFall",	
        "LGunShoot",				
        "LGunShootAir",			
        "LGunShootEmpty",			
        "LGunShootAirEmpty",		
        "FireFlowerShoot",		    
        "FireFlowerShootAir",		
        "ItemScrew",				
        "ItemScrewAir",			
        "DamageScrew",			    
        "DamageScrewAir",			
        "ItemScopeStart",			
        "ItemScopeRapid",			
        "ItemScopeFire",			
        "ItemScopeEnd",			
        "ItemScopeAirStart",		
        "ItemScopeAirRapid",		
        "ItemScopeAirFire",		
        "ItemScopeAirEnd",		    
        "ItemScopeStartEmpty",	    
        "ItemScopeRapidEmpty",	    
        "ItemScopeFireEmpty",		
        "ItemScopeEndEmpty",		
        "ItemScopeAirStartEmpty",	
        "ItemScopeAirRapidEmpty",	
        "ItemScopeAirFireEmpty",	
        "ItemScopeAirEndEmpty",	
        "LiftWait",				
        "LiftWalk1",				
        "LiftWalk2",				
        "LiftTurn",				
        "GuardOn",				    
        "Guard",					
        "GuardOff",				
        "GuardSetOff",			    
        "GuardReflect",			
        "DownBoundU",				
        "DownWaitU",				
        "DownDamageU",			    
        "DownStandU",				
        "DownAttackU",			    
        "DownFowardU",			    
        "DownBackU",				
        "DownSpotU",				
        "DownBoundD",				
        "DownWaitD",				
        "DownDamageD",			    
        "DownStandD",				
        "DownAttackD",			    
        "DownFowardD",			    
        "DownBackD",				
        "DownSpotD",				
        "Passive",				    
        "PassiveStandF",			
        "PassiveStandB",			
        "PassiveWall",			    
        "PassiveWallJump",		    
        "PassiveCeil",			    
        "ShieldBreakFly",			
        "ShieldBreakFall",		    
        "ShieldBreakDownU",		
        "ShieldBreakDownD",		
        "ShieldBreakStandU",		
        "ShieldBreakStandD",		
        "FuraFura",				
        "Catch",					
        "CatchPull",				
        "CatchDash",				
        "CatchDashPull",			
        "CatchWait",				
        "CatchAttack",			    
        "CatchCut",				
        "ThrowF",					
        "ThrowB",					
        "ThrowHi",				    
        "ThrowLw",				    
        "CapturePulledHi",		    
        "CaptureWaitHi",			
        "CaptureDamageHi",		    
        "CapturePulledLw",		    
        "CaptureWaitLw",			
        "CaptureDamageLw",		    
        "CaptureCut",				
        "CaptureJump",			    
        "CaptureNeck",			    
        "CaptureFoot",			    
        "EscapeF",				    
        "EscapeB",				    
        "Escape",					
        "EscapeAir",				
        "ReboundStop",			    
        "Rebound",				    
        "ThrownF",				    
        "ThrownB",				    
        "ThrownHi",				
        "ThrownLw",				
        "ThrownLwWomen",			
        "Pass",					
        "Ottotto",				    
        "OttottoWait",			    
        "FlyReflectWall",			
        "FlyReflectCeil",			
        "StopWall",				
        "StopCeil",				
        "MissFoot",				
        "CliffCatch",				
        "CliffWait",				
        "CliffClimbSlow",			
        "CliffClimbQuick",		    
        "CliffAttackSlow",		    
        "CliffAttackQuick",		
        "CliffEscapeSlow",		    
        "CliffEscapeQuick",		
        "CliffJumpSlow1",			
        "CliffJumpSlow2",			
        "CliffJumpQuick1",		    
        "CliffJumpQuick2",		    
        "AppealR",				    
        "AppealL",				    
        "ShoulderedWait",			
        "ShoulderedWalkSlow",		
        "ShoulderedWalkMiddle",	
        "ShoulderedWalkFast",		
        "ShoulderedTurn",			
        "ThrownFF",				
        "ThrownFB",				
        "ThrownFHi",				
        "ThrownFLw",				
        "CaptureCaptain",			
        "CaptureYoshi",			
        "YoshiEgg",				
        "CaptureKoopa",			
        "CaptureDamageKoopa",		
        "CaptureWaitKoopa",		
        "ThrownKoopaF",			
        "ThrownKoopaB",			
        "CaptureKoopaAir",		    
        "CaptureDamageKoopaAir",	
        "CaptureWaitKoopaAir",	    
        "ThrownKoopaAirF",		    
        "ThrownKoopaAirB",		    
        "CaptureKirby",			
        "CaptureWaitKirby",		
        "ThrownKirbyStar",		    
        "ThrownCopyStar",			
        "ThrownKirby",			    
        "BarrelWait",				
        "Bury",					
        "BuryWait",				
        "BuryJump",				
        "DamageSong",				
        "DamageSongWait",			
        "DamageSongRv",			
        "DamageBind",				
        "CaptureMewtwo",			
        "CaptureMewtwoAir",		
        "ThrownMewtwo",			
        "ThrownMewtwoAir",		    
        "WarpStarJump",			
        "WarpStarFall",			
        "HammerWait",				
        "HammerWalk",				
        "HammerTurn",				
        "HammerKneeBend",			
        "HammerFall",				
        "HammerJump",				
        "HammerLanding",			
        "KinokoGiantStart",		
        "KinokoGiantStartAir",	    
        "KinokoGiantEnd",			
        "KinokoGiantEndAir",		
        "KinokoSmallStart",		
        "KinokoSmallStartAir",	    
        "KinokoSmallEnd",			
        "KinokoSmallEndAir",		
        "Entry",					
        "EntryStart",				
        "EntryEnd",				
        "DamageIce",				
        "DamageIceJump",			
        "CaptureMasterhand",		
        "CapturedamageMasterhand", 
        "CapturewaitMasterhand",	
        "ThrownMasterhand",		
        "CaptureKirbyYoshi",		
        "KirbyYoshiEgg",			
        "CaptureLeadead",			
        "CaptureLikelike",		    
        "DownReflect",			    
        "CaptureCrazyhand",		
        "CapturedamageCrazyhand",	
        "CapturewaitCrazyhand",	
        "ThrownCrazyhand",		    
        "BarrelCannonWait",		
        "Wait1",					
        "Wait2",					
        "Wait3",					
        "Wait4",					
        "WaitItem",				
        "SquatWait1",				
        "SquatWait2",				
        "SquatWaitItem",			
        "GuardDamage",			    
        "EscapeN",				    
        "AttackS4Hold",			
        "HeavyWalk1",				
        "HeavyWalk2",				
        "ItemHammerWait",			
        "ItemHammerMove",			
        "ItemBlind",				
        "DamageElec",				
        "FuraSleepStart",			
        "FuraSleepLoop",			
        "FuraSleepEnd",			
        "WallDamage",				
        "CliffWait1",				
        "CliffWait2",				
        "SlipDown",				
        "Slip",					
        "SlipTurn",				
        "SlipDash",				
        "SlipWait",				
        "SlipStand",				
        "SlipAttack",				
        "SlipEscapeF",			    
        "SlipEscapeB",			    
        "AppealS",				    
        "Zitabata",				
        "CaptureKoopaHit",		    
        "ThrownKoopaEndF",		    
        "ThrownKoopaEndB",		    
        "CaptureKoopaAirHit",		
        "ThrownKoopaAirEndF",		
        "ThrownKoopaAirEndB",		
        "ThrownKirbyDrinkSShot",	
        "ThrownKirbySpitSShot",	
    };
	inline std::string ActionStateToString(ActionState StateToConvert)
	{
		if(uint64_t(StateToConvert) >= 383)
		{
			throw std::runtime_error("Invalid ActionState to convert to string");
		}
		return(__ActionStateStringMap[uint64_t(StateToConvert)]);
	}


	enum class ActionState_Peach : uint16_t
	{
		Float			 = 341, 
		FloatEndForward	 = 342, 
		FloatEndBackward = 343, 
		FloatNair		 = 344, 
		FloatFair		 = 345, 
		FloatBair		 = 346, 
		FloatUair		 = 347, 
		FloatDair		 = 348, 
	};

	enum class AttackID : uint8_t	
	{
		None								= 0 , 	//See sheet Not Attacks for things that do not qualify as attacks
		NonStaling							= 1 , 	//See sheet Non - Staling Attacks for a full list
		Jab1								= 2 , 	//
		Jab2								= 3 , 	//
		Jab3								= 4 , 	//
		RapidJabs							= 5 , 	//
		DashAttack							= 6 , 	//Includes both grounded and aerial Kirby dash attacks
		SideTilt							= 7 , 	//Includes all side tilts(mid, high, etc.)
		UpTilt								= 8 , 	//
		DownTilt							= 9 , 	//
		SideSmash							= 10, 	//Includes all side smashes(mid, high, etc.).Link / Young Link second side smash and all Peach side smashes are included.
		UpSmash								= 11, 	//
		DownSmash							= 12, 	//
		Nair								= 13, 	//Includes Peach FC aerial and G & W aerial
		Fair								= 14, 	//Includes Peach FC aerial
		Bair								= 15, 	//Includes Peach FC aerial and G & W aerial
		Uair								= 16, 	//Includes Peach FC aerial and G & W aerial
		Dair								= 17, 	//Includes Peach FC aerial
		NeutralSpecial						= 18, 	//See sheet Specials for special rulesand clarifications
		SideSpecial							= 19, 	//See sheet Specials for special rulesand clarifications
		UpSpecial							= 20, 	//See sheet Specials for special rulesand clarifications
		DownSpecial							= 21, 	//See sheet Specials for special rulesand clarifications
		
		
		
		
		
		KirbyHat_MarioNeutralSpecial		= 22, 
		KirbyHat_FoxNeutralSpecial			= 23, 
		KirbyHat_CFalconNeutralSpecial		= 24, 
		KirbyHat_DKNeutralSpecial			= 25, 
		KirbyHat_BowserNeutralSpecial		= 26, 
		KirbyHat_LinkNeutralSpecial			= 27, 
		KirbyHat_SheikNeutralSpecial		= 28, 
		KirbyHat_NessNeutralSpecial			= 29, 
		KirbyHat_PeachNeutralSpecial		= 30, 
		KirbyHat_IceClimberNeutralSpecial	= 31, 
		KirbyHat_PikachuNeutralSpecial		= 32, 
		KirbyHat_SamusNeutralSpecial		= 33, 
		KirbyHat_YoshiNeutralSpecial		= 34,     //This appears to be unused
		KirbyHat_JigglypuffNeutralSpecial	= 35, 
		KirbyHat_MewtwoNeutralSpecial		= 36, 
		KirbyHat_LuigiNeutralSpecial		= 37, 
		KirbyHat_MarthNeutralSpecial		= 38, 
		KirbyHat_ZeldaNeutralSpecial		= 39, 
		KirbyHat_YoungLinkNeutralSpecial	= 40, 
		KirbyHat_DocNeutralSpecial			= 41, 
		KirbyHat_FalcoNeutralSpecial		= 42, 
		KirbyHat_PichuNeutralSpecial		= 43, 
		KirbyHat_GameWatchNeutralSpecial	= 44, 
		KirbyHat_GanonNeutralSpecial		= 45, 
		KirbyHat_RoyNeutralSpecial			= 46, 
		Unk1								= 47, 
		Unk2								= 48, 
		Unk3								= 49, 
		GetUpAttack_Back					= 50, 
		GetUpAttack_Front					= 51, 
		Pummel								= 52, 
		ForwardThrow						= 53, 
		BackThrow							= 54, 
		UpThrow								= 55, 
		DownThrow							= 56, 
		CargoForwardThrow					= 57, 
		CargoBackThrow						= 58, 
		CargoUpThrow						= 59, 
		CargoDownThrow						= 60, 
		LedgeGetUpAttack_100				= 61, 
		LedgeGetUpAttack					= 62, 
		BeamSwordJab						= 63, 
		BeamSwordTiltSwing					= 64, 
		BeamSwordSmashSwing					= 65, 
		BeamSwordDashSwing					= 66, 
		HomeRunBatJab						= 67, 
		HomeRunBatTiltSwing					= 68, 
		HomeRunBatSmashSwing				= 69, 
		HomeRunBatDashSwing					= 70, 
		ParasolJab							= 71, 
		ParasolTiltSwing					= 72, 
		ParasolSmashSwing					= 73, 
		ParasolDashSwing					= 74, 
		FanJab								= 75, 
		FanTiltSwing						= 76, 
		FanSmashSwing						= 77, 
		FanDashSwing						= 78, 
		StarRodJab							= 79, 
		StarRodTiltSwing					= 80, 
		StarRodSmashSwing					= 81, 
		StarRodDashSwing					= 82, 
		LipsStickJab						= 83, 
		LipsStickTiltSwing					= 84, 
		LipsStickSmashSwing					= 85, 
		LipsStickDashSwing					= 86, 
		OpenParasol						    = 87,                   //Parasol float hits.Both Peach parasol and item parasol are included
		RayGunShoot							= 88, 
		FireFlowerShoot						= 89, 
		ScrewAttack							= 90, 
		SuperScope_Rapid					= 91, 
		SuperScope_Charged					= 92, 
		Hammer								= 93, 
	};
	static const const char* __AttackIDMap[] =
	{
		"None",
		"NonStaling",
		"Jab1",
		"Jab2",
		"Jab3",
		"RapidJabs",
		"DashAttack",
		"SideTilt",
		"UpTilt",
		"DownTilt",
		"SideSmash",
		"UpSmash",
		"DownSmash",
		"Nair",
		"Fair",
		"Bair",
		"Uair",
		"Dair",
		"NeutralSpecial",
		"SideSpecial",
		"UpSpecial",
		"DownSpecial",
		"KirbyHat_MarioNeutralSpecial",
		"KirbyHat_FoxNeutralSpecial",
		"KirbyHat_CFalconNeutralSpecial",
		"KirbyHat_DKNeutralSpecial",
		"KirbyHat_BowserNeutralSpecial",
		"KirbyHat_LinkNeutralSpecial",
		"KirbyHat_SheikNeutralSpecial",
		"KirbyHat_NessNeutralSpecial",
		"KirbyHat_PeachNeutralSpecial",
		"KirbyHat_IceClimberNeutralSpecia",
		"KirbyHat_PikachuNeutralSpecial",
		"KirbyHat_SamusNeutralSpecial",
		"KirbyHat_YoshiNeutralSpecial",
		"KirbyHat_JigglypuffNeutralSpecia",
		"KirbyHat_MewtwoNeutralSpecial",
		"KirbyHat_LuigiNeutralSpecial",
		"KirbyHat_MarthNeutralSpecial",
		"KirbyHat_ZeldaNeutralSpecial",
		"KirbyHat_YoungLinkNeutralSpecial",
		"KirbyHat_DocNeutralSpecial",
		"KirbyHat_FalcoNeutralSpecial",
		"KirbyHat_PichuNeutralSpecial",
		"KirbyHat_GameWatchNeutralSpecial",
		"KirbyHat_GanonNeutralSpecial",
		"KirbyHat_RoyNeutralSpecial",
		"Unk1",
		"Unk2",
		"Unk3",
		"GetUpAttack_Back",
		"GetUpAttack_Front",
		"Pummel",
		"ForwardThrow",
		"BackThrow",
		"UpThrow",
		"DownThrow",
		"CargoForwardThrow",
		"CargoBackThrow",
		"CargoUpThrow",
		"CargoDownThrow",
		"LedgeGetUpAttack_100",
		"LedgeGetUpAttack",
		"BeamSwordJab",
		"BeamSwordTiltSwing",
		"BeamSwordSmashSwing",
		"BeamSwordDashSwing",
		"HomeRunBatJab",
		"HomeRunBatTiltSwing",
		"HomeRunBatSmashSwing",
		"HomeRunBatDashSwing",
		"ParasolJab",
		"ParasolTiltSwing",
		"ParasolSmashSwing",
		"ParasolDashSwing",
		"FanJab",
		"FanTiltSwing",
		"FanSmashSwing",
		"FanDashSwing",
		"StarRodJab",
		"StarRodTiltSwing",
		"StarRodSmashSwing",
		"StarRodDashSwing",
		"LipsStickJab",
		"LipsStickTiltSwing",
		"LipsStickSmashSwing",
		"LipsStickDashSwing",
		"OpenParasol",
		"RayGunShoot",
		"FireFlowerShoot",
		"ScrewAttack",
		"SuperScope_Rapid",
		"SuperScope_Charged",
		"Hammer",
	};
    inline std::string AttackIDToString(AttackID IDToConvert)
    {
        if(uint64_t(IDToConvert) >= 94)
        {
            throw std::runtime_error("Invalid AttackID to convert to string");   
        }
        return(__AttackIDMap[uint64_t(IDToConvert)]);
    }
	enum ExternalCharacterID : uint8_t
	{
		CaptainFalcon            =  0x00, 
		DonkeyKong               =  0x01,
		Fox	                     =  0x02,
		MrGameWatch              =  0x03,
		Kirby                    =  0x04,
		Bowser                   =  0x05,
		Link                     =  0x06,
		Luigi                    =  0x07,
		Mario                    =  0x08,
		Marth                    =  0x09,
		Mewtwo                   =  0x0A,
		Ness                     =  0x0B,
		Peach                    =  0x0C,
		Pikachu                  =  0x0D,
		IceClimbers              =  0x0E,
		Jigglypuff               =  0x0F,
		Samus                    =  0x10,
		Yoshi                    =  0x11,
		Zelda                    =  0x12,
		Sheik                    =  0x13,
		Falco                    =  0x14,
		YoungLink                =  0x15,
		DrMario                  =  0x16,
		Roy                      =  0x17,
		Pichu                    =  0x18,
		Ganondorf                =  0x19,
		MasterHand	             =  0x1A,
		WireframeMale            =  0x1B,
		WireframeFemale          =  0x1C,
		GigaBowser               =  0x1D,
		CrazyHand                =  0x1E,
		Sandbag                  =  0x1F,
		Popo                     =  0x20,
		UserSelect               =  0x21,
	};
	enum class InternalCharacterID : uint8_t
	{
		Mario	        =    0x00, 
		Fox	          	=    0x01,
		Captain         =    0x02,
		Donkey          =    0x03,
		Kirby	        =    0x04,
		Bowser	        =    0x05,
		Link	        =    0x06,
		Sheik	        =    0x07,
		Ness	        =    0x08,
		Peach	        =    0x09,
		Popo	        =    0x0A,
		Nana	        =    0x0B,
		Pikachu	        =    0x0C,
		Samus	        =    0x0D,
		Yoshi	        =    0x0E,
		Jigglypuff	    =    0x0F,
		Mewtwo	        =    0x10,
		Luigi	        =    0x11,
		Marth	        =    0x12,
		Zelda	        =    0x13,
		YoungLink       =    0x14,
		DrMario         =    0x15,
		Falco	        =    0x16,
		Pichu	        =    0x17,
		MrGameWatch     =    0x18,
		Ganondorf	    =    0x19,
		Roy	            =    0x1A,
		MasterHand      =    0x1B,
		WireframeMale   =    0x1D,
		WireframeFemale =    0x1E,
		GigaBowser      =    0x1F,
		Sandbag	        =    0x20, 
	};
    enum class StageID : uint16_t
    {
	    Dummy                 =     0x000, 
	    TEST                  =     0x001,
	    FountainOfDreams      =     0x002,
	    PokémonStadium        =     0x003,
	    PrincessPeachsCastle  =     0x004,
	    KongoJungle           =     0x005,
	    Brinstar              =     0x006,
	    Corneria              =     0x007,
	    YoshisStory           =     0x008,
	    Onett                 =     0x009,
	    MuteCity              =     0x00A,
	    RainbowCruise         =     0x00B,
	    JungleJapes           =     0x00C,
	    GreatBay              =     0x00D,
	    HyruleTemple          =     0x00E,
	    BrinstarDepths        =     0x00F,
	    YoshisIsland          =     0x010,
	    GreenGreens           =     0x011,
	    Fourside              =     0x012,
	    MushroomKingdom1      =     0x013,
	    MushroomKingdom2      =     0x014,
	    Venom                 =     0x016,
	    PokeFloats            =     0x017,
	    BigBlue               =     0x018,
	    IcicleMountain        =     0x019,
	    Icetop	              =     0x01A,
	    FlatZone              =     0x01B,
	    DreamLandN64          =     0x01C,
	    YoshisIslandN64       =     0x01D,
        KongoJungleN64        =     0x01E,
	    Battlefield           =     0x01F,
	    FinalDestination      =     0x020,
    };

	inline std::string StageIDToString(StageID IDToConvert)
	{
		return("");
	}

	static const const char* __InternalCharacterStringMap[33] =
	{
		"Mario",
		"Fox",
		"Captain"			,
		"Donkey"			,
		"Kirby"				,
		"Bowser"			,
		"Link"				,
		"Sheik"				,
		"Ness"				,
		"Peach"				,
		"Popo"				,
		"Nana"				,
		"Pikachu"			,
		"Samus"				,
		"Yoshi"				,
		"Jigglypuff"		,
		"Mewtwo"			,
		"Luigi"				,
		"Marth"				,
		"Zelda"				,
		"YoungLink"			,
		"DrMario"			,
		"Falco"				,
		"Pichu"				,
		"MrGameWatch"		,
		"Ganondorf"			,
		"Roy"				,
		"MasterHand"		,
		"WireframeMale"		,
		"WireframeFemale"	,
		"GigaBowser"		,
		"Sandbag"           ,
	};
	
	inline std::string CharacterToString(InternalCharacterID IdToConvert)
	{
		if (uint64_t(IdToConvert) >= 32)
		{
			throw std::runtime_error("InternalCharacterID out of range");
		}
		return(__InternalCharacterStringMap[uint64_t(IdToConvert)]);
	}
	static const const char* __ExternalCharacterStringMap[34] =
	{
		"CaptainFalcon"     ,
        "DonkeyKong"        ,
        "Fox"               ,
        "MrGameWatch"       ,
        "Kirby"          ,
        "Bowser" ,
        "Link" ,
        "Luigi" ,
        "Mario" ,
        "Marth" ,
        "Mewtwo" ,
        "Ness" ,
        "Peach" ,
        "Pikachu" ,
        "IceClimbers" ,
        "Jigglypuff" ,
        "Samus" ,
        "Yoshi" ,
        "Zelda" ,
        "Sheik" ,
        "Falco" ,
        "YoungLink" ,
        "DrMario" ,
        "Roy" ,
        "Pichu" ,
        "Ganondorf" ,
        "MasterHand" ,
        "WireframeMale" ,
        "WireframeFemale" ,
        "GigaBowser" ,
        "CrazyHand" ,
        "Sandbag" ,
        "Popo" ,
        "UserSelect",
	};
	inline std::string CharacterToString(ExternalCharacterID IdToConvert)
	{
		if (uint64_t(IdToConvert) >= 33)
		{
			throw std::runtime_error("InternalCharacterID out of range");
		}
		return(__ExternalCharacterStringMap[uint64_t(IdToConvert)]);
	}

	//AttackID ActionStateToAttackID(ActionState StateToConvert);
}
