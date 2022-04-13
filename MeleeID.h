#include <cstdint>
namespace MBSlippi
{
	enum class ActionState : int16_t
	{
		0000	0000	DeadDown				Standard downward death
		0001	0001	DeadLeft				Standard leftward death
		0002	0002	DeadRight				Standard rightward death
		0003	0003	DeadUp					Upward death used in 1P "Team Kirby", etc.
		0004	0004	DeadUpStar				Standard Star KO
		0005	0005	DeadUpStarIce			Star KO while encased in ice(Freezie)
		0008	0008	DeadUpFallHitCamera
		0009	0009	DeadUpFallIce
		0010	000A	DeadUpFallHitCameraIce
		0011	000B	Rebirth					"Nothing" state, probably - it is the state Shiek / Zelda is in when their counterpart is the one currently playing.Also state of Nana when Sopo is alive
		0013	000D	RebirthWait				Waiting on halo
		0014	000E	Wait					Standing state
		0015	000F	WalkSlow
		0016	0010	WalkMiddle
		0017	0011	WalkFast
		0018	0012	Turn
		0019	0013	TurnRun
		0020	0014	Dash
		0021	0015	Run
		0022	0016	RunDirect
		0023	0017	RunBrake
		0024	0018	KneeBend				Pre - jump animation
		0025	0019	JumpF					First jump forward
		0026	001A	JumpB					First jump backward
		0027	001B	JumpAerialF				Aerial jump forward
		0028	001C	JumpAerialB				Aerial jump backward
		0029	001D	Fall					Falling straight down
		0030	001E	FallF					Falling with forward DI
		0031	001F	FallB					Falling with backward DI
		0032	0020	FallAerial				Falling after the second jump
		0033	0021	FallAerialF				Falling after the second jump with forward DI
		0034	0022	FallAerialB				Falling after the second jump with backward DI
		0035	0023	FallSpecial				Special fall after UpB or airdodge
		0036	0024	FallSpecialF			Special fall with forward DI
		0037	0025	FallSpecialB			Special fall with backward DI
		0038	0026	DamageFall				Tumbling
		0039	0027	Squat					Going from stand to crouch
		0040	0028	SquatWait				Crouching
		0041	0029	SquatRv					Going from crouch to stand
		0042	002A	Landing
		0043	002B	LandingFallSpecial		Landing from special fall
		0044	002C	Attack11				Jab 1
		0045	002D	Attack12				Jab 2
		0046	002E	Attack13				Jab 3
		0047	002F	Attack100Start			Start of a rapid jab
		0048	0030	Attack100Loop			Middle of a rapid jab
		0049	0031	Attack100End			End of a rapid jab
		0050	0032	AttackDash				Dash attack
		0051	0033	AttackS3Hi				High Ftilt
		0052	0034	AttackS3HiS				High - mid Ftilt
		0053	0035	AttackS3S				Mid Ftilt
		0054	0036	AttackS3LwS				Low - mid Ftilt
		0055	0037	AttackS3Lw				Low Ftilt
		0056	0038	AttackHi3				Uptilt
		0057	0039	AttackLw3				Downtilt
		0058	003A	AttackS4Hi				High Fsmash
		0059	003B	AttackS4HiS				High - mid Fsmash
		0060	003C	AttackS4S				Mid Fsmash
		0061	003D	AttackS4LwS				Low - mid Fsmash
		0062	003E	AttackS4Lw				Low Fsmash
		0063	003F	AttackHi4				Upsmash
		0064	0040	AttackLw4				Downsmash
		0065	0041	AttackAirN				Nair
		0066	0042	AttackAirF				Fair
		0067	0043	AttackAirB				Bair
		0068	0044	AttackAirHi				Uair
		0069	0045	AttackAirLw				Dair
		0070	0046	LandingAirN				Landing during Nair
		0071	0047	LandingAirF				Landing during Fair
		0072	0048	LandingAirB				Landing during Bair
		0073	0049	LandingAirHi			Landing during Uair
		0074	004A	LandingAirLw			Landing during Dair
		0075	004B	DamageHi1
		0076	004C	DamageHi2
		0077	004D	DamageHi3
		0078	004E	DamageN1
		0079	004F	DamageN2
		0080	0050	DamageN3
		0081	0051	DamageLw1
		0082	0052	DamageLw2
		0083	0053	DamageLw3
		0084	0054	DamageAir1
		0085	0055	DamageAir2
		0086	0056	DamageAir3
		0087	0057	DamageFlyHi
		0088	0058	DamageFlyN
		0089	0059	DamageFlyLw
		0090	005A	DamageFlyTop
		0091	005B	DamageFlyRoll
		0092	005C	LightGet				Picking up an item
		0093	005D	HeavyGet				Picking up a heavy item(barrel)
		0094	005E	LightThrowF				Throwing items at standard speed	0x80095a30
		0095	005F	LightThrowB
		0096	0060	LightThrowHi
		0097	0061	LightThrowLw
		0098	0062	LightThrowDash
		0099	0063	LightThrowDrop
		0100	0064	LightThrowAirF
		0101	0065	LightThrowAirB
		0102	0066	LightThrowAirHi
		0103	0067	LightThrowAirLw
		0104	0068	HeavyThrowF
		0105	0069	HeavyThrowB
		0106	006A	HeavyThrowHi
		0107	006B	HeavyThrowLw
		0108	006C	LightThrowF4			Throwing items at Smash speed
		0109	006D	LightThrowB4
		0110	006E	LightThrowHi4
		0111	006F	LightThrowLw4
		0112	0070	LightThrowAirF4
		0113	0071	LightThrowAirB4
		0114	0072	LightThrowAirHi4
		0115	0073	LightThrowAirLw4
		0116	0074	HeavyThrowF4
		0117	0075	HeavyThrowB4
		0118	0076	HeavyThrowHi4
		0119	0077	HeavyThrowLw4
		0120	0078	SwordSwing1				Beam sword swings
		0121	0079	SwordSwing3
		0122	007A	SwordSwing4
		0123	007B	SwordSwingDash
		0124	007C	BatSwing1				Home Run Bat swings
		0125	007D	BatSwing3
		0126	007E	BatSwing4
		0127	007F	BatSwingDash
		0128	0080	ParasolSwing1			Parasol swings
		0129	0081	ParasolSwing3
		0130	0082	ParasolSwing4
		0131	0083	ParasolSwingDash
		0132	0084	HarisenSwing1			Fan swings
		0133	0085	HarisenSwing3
		0134	0086	HarisenSwing4
		0135	0087	HarisenSwingDash
		0136	0088	StarRodSwing1			Star Rod swings
		0137	0089	StarRodSwing3
		0138	008A	StarRodSwing4
		0139	008B	StarRodSwingDash
		0140	008C	LipStickSwing1			Lip's Stick swings	
		0141	008D	LipStickSwing3
		0142	008E	LipStickSwing4
		0143	008F	LipStickSwingDash
		0144	0090	ItemParasolOpen
		0145	0091	ItemParasolFall
		0146	0092	ItemParasolFallSpecial
		0147	0093	ItemParasolDamageFall
		0148	0094	LGunShoot				Raygun shots
		0149	0095	LGunShootAir
		0150	0096	LGunShootEmpty
		0151	0097	LGunShootAirEmpty
		0152	0098	FireFlowerShoot
		0153	0099	FireFlowerShootAir
		0154	009A	ItemScrew
		0155	009B	ItemScrewAir
		0156	009C	DamageScrew
		0157	009D	DamageScrewAir
		0158	009E	ItemScopeStart
		0159	009F	ItemScopeRapid
		0160	00A0	ItemScopeFire
		0161	00A1	ItemScopeEnd
		0162	00A2	ItemScopeAirStart
		0163	00A3	ItemScopeAirRapid
		0164	00A4	ItemScopeAirFire
		0165	00A5	ItemScopeAirEnd
		0166	00A6	ItemScopeStartEmpty
		0167	00A7	ItemScopeRapidEmpty
		0168	00A8	ItemScopeFireEmpty
		0169	00A9	ItemScopeEndEmpty
		0170	00AA	ItemScopeAirStartEmpty
		0171	00AB	ItemScopeAirRapidEmpty
		0172	00AC	ItemScopeAirFireEmpty
		0173	00AD	ItemScopeAirEndEmpty
		0174	00AE	LiftWait
		0175	00AF	LiftWalk1
		0176	00B0	LiftWalk2
		0177	00B1	LiftTurn
		0178	00B2	GuardOn					Shield startup	0x80093a50
		0179	00B3	Guard					Holding shield	0x80
		0180	00B4	GuardOff				Shield release	0x80
		0181	00B5	GuardSetOff				Shield stun
		0182	00B6	GuardReflect			0x80
		0183	00B7	DownBoundU				The missed tech bounce, facing up
		0184	00B8	DownWaitU				Lying on ground facing up
		0185	00B9	DownDamageU				Getting hit by a low knockback move lying on ground facing up
		0186	00BA	DownStandU				Neutral getup
		0187	00BB	DownAttackU				Get up attack from ground face up
		0188	00BC	DownFowardU				Missed tech roll forward
		0189	00BD	DownBackU				Missed tech roll backward
		0190	00BE	DownSpotU				Does not appear to be used, may have been a stick down option for missed tech at one point
		0191	00BF	DownBoundD				The missed tech bounce, facing down
		0192	00C0	DownWaitD				Lying on ground facing down	0x80
		0193	00C1	DownDamageD				Getting hit by a low knockback move lying on the ground facing down	0x80
		0194	00C2	DownStandD				Neutral getup	0x80
		0195	00C3	DownAttackD				Get up attack from ground face down	0x80
		0196	00C4	DownFowardD				Missed tech roll forward	0x80
		0197	00C5	DownBackD				Missed tech roll backward	0x80
		0198	00C6	DownSpotD				Does not appear to be used, may have been a stick down option for missed tech at one point	0x80
		0199	00C7	Passive					Neutral tech
		0200	00C8	PassiveStandF			Forward tech
		0201	00C9	PassiveStandB			Backward tech
		0202	00CA	PassiveWall				Wall tech
		0203	00CB	PassiveWallJump			Walljump tech / plain walljump
		0204	00CC	PassiveCeil				Ceiling tech
		0205	00CD	ShieldBreakFly
		0206	00CE	ShieldBreakFall
		0207	00CF	ShieldBreakDownU
		0208	00D0	ShieldBreakDownD
		0209	00D1	ShieldBreakStandU
		0210	00D2	ShieldBreakStandD
		0211	00D3	FuraFura				Shield - break tottering	0x80099010
		0212	00D4	Catch					Grab	0x800d8c54
		0213	00D5	CatchPull				Successfully grabbing a character - pulling them in	0x80
		0214	00D6	CatchDash				0x80
		0215	00D7	CatchDashPull			0x80
		0216	00D8	CatchWait				Grabbing and holding a character	0x80
		0217	00D9	CatchAttack				Pummel	0x80
		0218	00DA	CatchCut				When opponent breaks of a character's grab	0x80
		0219	00DB	ThrowF					Forward throw	0x80
		0220	00DC	ThrowB					Back throw	0x80
		0221	00DD	ThrowHi					Up throw	0x80
		0222	00DE	ThrowLw					Down throw	0x80
		0223	00DF	CapturePulledHi			0x80
		0224	00E0	CaptureWaitHi			0x80
		0225	00E1	CaptureDamageHi			0x80
		0226	00E2	CapturePulledLw			Becoming grabbed	0x80
		0227	00E3	CaptureWaitLw			When grabbed	0x80
		0228	00E4	CaptureDamageLw			Pummeled	0x80
		0229	00E5	CaptureCut				Grab release	0x80
		0230	00E6	CaptureJump				Jumping mash out of grab	0x80
		0231	00E7	CaptureNeck				Does not appear to be used	0x80
		0232	00E8	CaptureFoot				Does not appear to be used	0x80
		0233	00E9	EscapeF					roll forward	0x80
		0234	00EA	EscapeB					roll backward	0x80
		0235	00EB	Escape					spotdodge	0x800998ec
		0236	00EC	EscapeAir				airdodge	0x80099a9c
		0237	00ED	ReboundStop
		0238	00EE	Rebound
		0239	00EF	ThrownF					Thrown forward
		0240	00F0	ThrownB					Thrown backward
		0241	00F1	ThrownHi				Thrown up
		0242	00F2	ThrownLw				Thrown down
		0243	00F3	ThrownLwWomen			Thrown down as a female character
		0244	00F4	Pass					Drop through platform	0x80
		0245	00F5	Ottotto					Ledge teeter
		0246	00F6	OttottoWait
		0247	00F7	FlyReflectWall			Missed walltech
		0248	00F8	FlyReflectCeil			Missed ceiling tech
		0249	00F9	StopWall				Wall bonk
		0250	00FA	StopCeil				Ceiling bonk
		0251	00FB	MissFoot				Backward shield slideoff
		0252	00FC	CliffCatch				Catching the ledge	0x80
		0253	00FD	CliffWait				Hanging on the ledge	0x80
		0254	00FE	CliffClimbSlow			Climbing the ledge(100 % +)	0x80
		0255	00FF	CliffClimbQuick			Climbing the ledge(< 100 %)	0x80
		0256	0100	CliffAttackSlow			Ledge attack(100 % +)	0x80
		0257	0101	CliffAttackQuick		Ledge attack(< 100 %)	0x80
		0258	0102	CliffEscapeSlow			Ledge roll(100 % +)	0x80
		0259	0103	CliffEscapeQuick		Ledge roll(< 100 %)	0x80
		0260	0104	CliffJumpSlow1			Ledge jump / tournament winner(100 % +)
		0261	0105	CliffJumpSlow2			Ledge jump / tournament winner(100 % +)
		0262	0106	CliffJumpQuick1			Ledge jump / tournament winner(< 100 %)
		0263	0107	CliffJumpQuick2			Ledge jump / tournament winner(< 100 %)
		0264	0108	AppealR					Taunt right	0x80
		0265	0109	AppealL					Taunt left	0x80
		0266	010A	ShoulderedWait			DK carry
		0267	010B	ShoulderedWalkSlow
		0268	010C	ShoulderedWalkMiddle
		0269	010D	ShoulderedWalkFast
		0270	010E	ShoulderedTurn
		0271	010F	ThrownFF				DK carry throws
		0272	0110	ThrownFB
		0273	0111	ThrownFHi
		0274	0112	ThrownFLw
		0275	0113	CaptureCaptain
		0276	0114	CaptureYoshi
		0277	0115	YoshiEgg
		0278	0116	CaptureKoopa
		0279	0117	CaptureDamageKoopa
		0280	0118	CaptureWaitKoopa
		0281	0119	ThrownKoopaF
		0282	011A	ThrownKoopaB
		0283	011B	CaptureKoopaAir
		0284	011C	CaptureDamageKoopaAir
		0285	011D	CaptureWaitKoopaAir
		0286	011E	ThrownKoopaAirF
		0287	011F	ThrownKoopaAirB
		0288	0120	CaptureKirby
		0289	0121	CaptureWaitKirby
		0290	0122	ThrownKirbyStar
		0291	0123	ThrownCopyStar
		0292	0124	ThrownKirby
		0293	0125	BarrelWait
		0294	0126	Bury					Stuck in ground by DK side B or similar
		0295	0127	BuryWait
		0296	0128	BuryJump
		0297	0129	DamageSong				Put to sleep by Jigglypuff up B or similar
		0298	012A	DamageSongWait
		0299	012B	DamageSongRv
		0300	012C	DamageBind				Hit by Mewtwo's Disable	
		0301	012D	CaptureMewtwo			Does not appear to be used
		0302	012E	CaptureMewtwoAir		Does not appear to be used
		0303	012F	ThrownMewtwo			Hit by Mewtwo's Confusion	
		0304	0130	ThrownMewtwoAir			Hit by Mewtwo's Confusion	
		0305	0131	WarpStarJump
		0306	0132	WarpStarFall
		0307	0133	HammerWait
		0308	0134	HammerWalk
		0309	0135	HammerTurn
		0310	0136	HammerKneeBend
		0311	0137	HammerFall
		0312	0138	HammerJump
		0313	0139	HammerLanding
		0314	013A	KinokoGiantStart		Super / Poison mushroom states
		0315	013B	KinokoGiantStartAir
		0316	013C	KinokoGiantEnd
		0317	013D	KinokoGiantEndAir
		0318	013E	KinokoSmallStart
		0319	013F	KinokoSmallStartAir
		0320	0140	KinokoSmallEnd
		0321	0141	KinokoSmallEndAir
		0322	0142	Entry					Warp in at beginning of match.
		0323	0143	EntryStart
		0324	0144	EntryEnd
		0325	0145	DamageIce
		0326	0146	DamageIceJump
		0327	0147	CaptureMasterhand
		0328	0148	CapturedamageMasterhand
		0329	0149	CapturewaitMasterhand
		0330	014A	ThrownMasterhand
		0331	014B	CaptureKirbyYoshi
		0332	014C	KirbyYoshiEgg
		0333	014D	CaptureLeadead
		0334	014E	CaptureLikelike
		0335	014F	DownReflect	
		0336	0150	CaptureCrazyhand
		0337	0151	CapturedamageCrazyhand
		0338	0152	CapturewaitCrazyhand
		0339	0153	ThrownCrazyhand
		0340	0154	BarrelCannonWait
		0341	0155	Wait1					No general action states at this point or later are used, it's all character-specific action states	
		0342	0156	Wait2
		0343	0157	Wait3
		0344	0158	Wait4
		0345	0159	WaitItem
		0346	015A	SquatWait1
		0347	015B	SquatWait2
		0348	015C	SquatWaitItem
		0349	015D	GuardDamage
		0350	015E	EscapeN
		0351	015F	AttackS4Hold
		0352	0160	HeavyWalk1
		0353	0161	HeavyWalk2
		0354	0162	ItemHammerWait
		0355	0163	ItemHammerMove
		0356	0164	ItemBlind
		0357	0165	DamageElec
		0358	0166	FuraSleepStart
		0359	0167	FuraSleepLoop
		0360	0168	FuraSleepEnd
		0361	0169	WallDamage
		0362	016A	CliffWait1
		0363	016B	CliffWait2
		0364	016C	SlipDown
		0365	016D	Slip
		0366	016E	SlipTurn
		0367	016F	SlipDash
		0368	0170	SlipWait
		0369	0171	SlipStand
		0370	0172	SlipAttack
		0371	0173	SlipEscapeF
		0372	0174	SlipEscapeB
		0373	0175	AppealS
		0374	0176	Zitabata
		0375	0177	CaptureKoopaHit
		0376	0178	ThrownKoopaEndF
		0377	0179	ThrownKoopaEndB
		0378	017A	CaptureKoopaAirHit
		0379	017B	ThrownKoopaAirEndF
		0380	017C	ThrownKoopaAirEndB
		0381	017D	ThrownKirbyDrinkSShot
		0382	017E	ThrownKirbySpitSShot
}
