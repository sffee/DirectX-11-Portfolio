#include "HeadHunter2.h"
#include <GJHGameEngineRandom.h>
#include "SpectrumCom.h"
#include "Player.h"
#include "LaserLine.h"
#include "MonsterBullet.h"
#include "KnifeDashLine.h"
#include "Grenade.h"

HeadHunter2::HeadHunter2()
{

}

HeadHunter2::~HeadHunter2()
{

}

void HeadHunter2::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	CreateAnimation(L"Idle", L"HeadHunter_Idle", 0.1f, true);
	CreateAnimation(L"TakeOutRifle", L"HeadHunter_TakeOutRifle", 0.04f, false);
	CreateAnimation(L"Aim_Rifle", L"HeadHunter_Aim_Rifle", 10000.f, false);
	CreateAnimation(L"Aim_Rifle_Cooltime", L"HeadHunter_Aim_Rifle", 10000.f, false);
	CreateAnimation(L"PutBackRifle", L"HeadHunter_PutBackRifle", 0.04f, false);
	CreateAnimation(L"TakeOutGun", L"HeadHunter_TakeOutGun", 0.06f, false);
	CreateAnimation(L"Shoot", L"HeadHunter_Shoot", 0.06f, false);
	CreateAnimation(L"PutBackGun", L"HeadHunter_PutBackGun", 0.06f, false);
	CreateAnimation(L"DodgeRoll", L"HeadHunter_DodgeRoll", 0.06f, false);
	CreateAnimation(L"PreJump", L"HeadHunter_PreJump", 0.09f, false);
	CreateAnimation(L"Jump", L"HeadHunter_Jump", 1.f, false);
	CreateAnimation(L"Wall_Idle", L"HeadHunter_Wall_Idle", 0.05f, false);
	CreateAnimation(L"Wall_Jump", L"HeadHunter_Wall_Jump", 0.1f, false);
	CreateAnimation(L"Wall_Jump_Land", L"HeadHunter_Wall_Jump_Land", 0.06f, false);
	CreateAnimation(L"TeleportIn_Sweep", L"HeadHunter_TeleportIn_Sweep", 0.08f, false);
	CreateAnimation(L"TeleportOut_Sweep", L"HeadHunter_TeleportOut_Sweep", 0.08f, false);
	CreateAnimation(L"Sweep", L"HeadHunter_Sweep", 0.04f, false);
	CreateAnimation(L"PreDash", L"HeadHunter_PreDash", 0.06f, false);
	CreateAnimation(L"Dash", L"HeadHunter_Dash", 0.1f, true);
	CreateAnimation(L"Dash_End_Ground", L"HeadHunter_Dash_End_Ground", 0.06f, false);
	CreateAnimation(L"TeleportIn", L"HeadHunter_TeleportIn", 0.05f, false);
	CreateAnimation(L"TeleportOut", L"HeadHunter_TeleportOut", 0.05f, false);
	CreateAnimation(L"TeleportIn_Ground", L"HeadHunter_TeleportIn_Ground", 0.05f, false);
	CreateAnimation(L"TeleportOut_Ground", L"HeadHunter_TeleportOut_Ground", 0.05f, false);
	CreateAnimation(L"Hurt", L"HeadHunter_Hurt", 0.08f, false);
	CreateAnimation(L"Invisible", L"HeadHunter_Invisible", 0.1f, true);
	CreateAnimation(L"DieFly", L"HeadHunter_DieFly", 0.1f, false);
	CreateAnimation(L"DieLand", L"HeadHunter_DieLand", 0.1f, false);

	State.CreateState(L"Idle", &HeadHunter2::Idle_Init, &HeadHunter2::Idle_State, nullptr, this);
	State.CreateState(L"TakeOutRifle", &HeadHunter2::TakeOutRifle_Init, &HeadHunter2::TakeOutRifle_State, nullptr, this);
	State.CreateState(L"Aim_Rifle", &HeadHunter2::Aim_Rifle_Init, &HeadHunter2::Aim_Rifle_State, nullptr, this);
	State.CreateState(L"Aim_Rifle_Cooltime", &HeadHunter2::Aim_Rifle_Cooltime_Init, &HeadHunter2::Aim_Rifle_Cooltime_State, nullptr, this);
	State.CreateState(L"PutBackRifle", &HeadHunter2::PutBackRifle_Init, &HeadHunter2::PutBackRifle_State, nullptr, this);
	State.CreateState(L"TakeOutGun", &HeadHunter2::TakeOutGun_Init, &HeadHunter2::TakeOutGun_State, nullptr, this);
	State.CreateState(L"Shoot", &HeadHunter2::Shoot_Init, &HeadHunter2::Shoot_State, nullptr, this);
	State.CreateState(L"Shoot_Cooltime", &HeadHunter2::Shoot_Cooltime_Init, &HeadHunter2::Shoot_Cooltime_State, nullptr, this);
	State.CreateState(L"PutBackGun", &HeadHunter2::PutBackGun_Init, &HeadHunter2::PutBackGun_State, nullptr, this);
	State.CreateState(L"PreDash", &HeadHunter2::PreDash_Init, &HeadHunter2::PreDash_State, nullptr, this);
	State.CreateState(L"Dash", &HeadHunter2::Dash_Init, &HeadHunter2::Dash_State, &HeadHunter2::Dash_Leave, this);
	State.CreateState(L"Dash_End_Ground", &HeadHunter2::Dash_End_Ground_Init, &HeadHunter2::Dash_End_Ground_State, nullptr, this);
	State.CreateState(L"DodgeRoll", &HeadHunter2::DodgeRoll_Init, &HeadHunter2::DodgeRoll_State, nullptr, this);
	State.CreateState(L"PreJump", &HeadHunter2::PreJump_Init, &HeadHunter2::PreJump_State, nullptr, this);
	State.CreateState(L"Jump", &HeadHunter2::Jump_Init, &HeadHunter2::Jump_State, nullptr, this);
	State.CreateState(L"Wall_Idle", &HeadHunter2::Wall_Idle_Init, &HeadHunter2::Wall_Idle_State, nullptr, this);
	State.CreateState(L"Wall_Jump", &HeadHunter2::Wall_Jump_Init, &HeadHunter2::Wall_Jump_State, nullptr, this);
	State.CreateState(L"Wall_Jump_Land", &HeadHunter2::Wall_Jump_Land_Init, &HeadHunter2::Wall_Jump_Land_State, nullptr, this);
	State.CreateState(L"Hit1_TeleportIn_Sweep", &HeadHunter2::Hit1_TeleportIn_Sweep_Init, &HeadHunter2::Hit1_TeleportIn_Sweep_State, nullptr, this);
	State.CreateState(L"Hit1_Sweep", &HeadHunter2::Hit1_Sweep_Init, &HeadHunter2::Hit1_Sweep_State, nullptr, this);
	State.CreateState(L"Hit1_Dash", &HeadHunter2::Hit1_Dash_Init, &HeadHunter2::Hit1_Dash_State, &HeadHunter2::Hit1_Dash_Leave, this);
	State.CreateState(L"Hit1_Dash_End_Ground", &HeadHunter2::Hit1_Dash_End_Ground_Init, &HeadHunter2::Hit1_Dash_End_Ground_State, nullptr, this);
	State.CreateState(L"Hit2_TeleportIn", &HeadHunter2::Hit2_TeleportIn_Init, &HeadHunter2::Hit2_TeleportIn_State, nullptr, this);
	State.CreateState(L"Hit2_TeleportOut", &HeadHunter2::Hit2_TeleportOut_Init, &HeadHunter2::Hit2_TeleportOut_State, nullptr, this);
	State.CreateState(L"Hit2_Left_TeleportIn_Sweep", &HeadHunter2::Hit2_Left_TeleportIn_Sweep_Init, &HeadHunter2::Hit2_Left_TeleportIn_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_Left_Sweep", &HeadHunter2::Hit2_Left_Sweep_Init, &HeadHunter2::Hit2_Left_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_Left_TeleportOut_Sweep", &HeadHunter2::Hit2_Left_TeleportOut_Sweep_Init, &HeadHunter2::Hit2_Left_TeleportOut_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_Right_TeleportIn_Sweep", &HeadHunter2::Hit2_Right_TeleportIn_Sweep_Init, &HeadHunter2::Hit2_Right_TeleportIn_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_Right_Sweep", &HeadHunter2::Hit2_Right_Sweep_Init, &HeadHunter2::Hit2_Right_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_Right_TeleportOut_Sweep", &HeadHunter2::Hit2_Right_TeleportOut_Sweep_Init, &HeadHunter2::Hit2_Right_TeleportOut_Sweep_State, nullptr, this);
	State.CreateState(L"Hit2_TeleportIn_Ground", &HeadHunter2::Hit2_TeleportIn_Ground_Init, &HeadHunter2::Hit2_TeleportIn_Ground_State, nullptr, this);
	State.CreateState(L"Hit2_TeleportOut_Ground", &HeadHunter2::Hit2_TeleportOut_Ground_Init, &HeadHunter2::Hit2_TeleportOut_Ground_State, nullptr, this);
	State.CreateState(L"Hit2_Wall_TeleportIn", &HeadHunter2::Hit2_Wall_TeleportIn_Init, &HeadHunter2::Hit2_Wall_TeleportIn_State, nullptr, this);
	State.CreateState(L"Hurt", &HeadHunter2::Hurt_Init, &HeadHunter2::Hurt_State, nullptr, this);
	State.CreateState(L"Hurt_Invisible", &HeadHunter2::Hurt_Invisible_Init, &HeadHunter2::Hurt_Invisible_State, nullptr, this);
	State.CreateState(L"DieFly", &HeadHunter2::DieFly_Init, &HeadHunter2::DieFly_State, nullptr, this);
	State.CreateState(L"DieLand", &HeadHunter2::DieLand_Init, &HeadHunter2::DieLand_State, nullptr, this);

	CreateCollision((int)CollisionLayer::HeadHunter, COLTYPE::AABB, &HeadHunter2::ColEnter, &HeadHunter2::ColStay, &HeadHunter2::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });

	CreateAttackCollision((int)CollisionLayer::MonsterAttack, COLTYPE::AABB, &HeadHunter2::ColEnter, &HeadHunter2::ColStay, &HeadHunter2::ColExit, *this);
	m_AttackCollision->SetLocalPosition({ 20.f, 20.f });
	m_AttackCollision->SetLocalScale({ 40.f, 30.f });
	m_AttackCollision->Disable();

	State.ChangeState(L"Idle");

	m_LaserLineIndex = 0;
	m_GrenadeIndex = 0;
	m_DodgeCooltime = 0.f;
	m_CheckCelling = false;
	m_TeleportCount = 0;
	m_HurtCount = 0;

	m_Spectrum = CreateComponent<SpectrumCom>(m_Render, m_RenderScale, GetActor(), 0.01f, 0.2f, 0.2f, float4(1.f, 0.f, 1.f, 1.f), true);
	
	m_LaserLineList.resize(5);
	for (int i = 0; i < 5; ++i)
	{
		m_LaserLineList[i] = CreateActor<LaserLine>(ACTORTYPE::EFFECT, L"Bullet", { 0.f, 0.f }, 1.f)->FindComponent<LaserLine>();
	}

	m_BulletList.resize(18);
	for (int i = 0; i < 18; ++i)
	{
		m_BulletList[i] = CreateActor<MonsterBullet>(ACTORTYPE::BULLET, L"Bullet", { 0.f, 0.f }, 1.f)->FindComponent<MonsterBullet>();
	}

	m_GrenadeList.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_GrenadeList[i] = CreateActor<Grenade>(ACTORTYPE::EFFECT, L"Grenade", { 0.f, 0.f }, 1.f)->FindComponent<Grenade>();
	}

	m_KnifeDashLine = CreateActor<KnifeDashLine>(ACTORTYPE::EFFECT, L"KnifeDashLine", { 0.f, 0.f }, 1.f)->FindComponent<KnifeDashLine>();

	Hit2TeleportPos.reserve(4);
	Hit2TeleportPos.push_back(float4(100.f, -100.f));
	Hit2TeleportPos.push_back(float4(540.f, -100.f));
	Hit2TeleportPos.push_back(float4(160.f, -100.f));
	Hit2TeleportPos.push_back(float4(480.f, -100.f));

	m_PrevNormalPatternIndex = -1;
}

void HeadHunter2::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		State.Update();
		ObjectBase::Update();
		
		CalcCooltime();
	}
}

void HeadHunter2::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::PlayerSlash)
	{
		if (m_HurtCount <= 2)
		{
			Look(_Other->GetActor());
			State.ChangeState(L"Hurt");
		}
		else
		{
			LookOpposite(_Other->GetActor());
			State.ChangeState(L"DieFly");
		}
	}
}

void HeadHunter2::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
}

void HeadHunter2::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void HeadHunter2::RewindEnd()
{
	SetDirection(DIRECTION::LEFT);
	m_Collision->Enable();
	m_AttackCollision->Disable();
	m_HurtCount = 0;
	m_Spectrum->End();

	State.ChangeState(L"Idle");
}

void HeadHunter2::LookPlayer()
{
	float4 Distance = DistanceToPlayer();

	if (Distance.x < 0.f)
	{
		SetDirection(DIRECTION::LEFT);
	}
	else
	{
		SetDirection(DIRECTION::RIGHT);
	}
}

void HeadHunter2::Aim()
{
	float Angle = AngleToPlayer(m_Dir);
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	int AnimationIndex = 0;
	if (ceil(PlayerPos.y) < ceil(MyPos.y))
	{
		Angle -= 270.f;

		AnimationIndex = abs((int)(Angle / 9.5f));
	}
	else
	{
		AnimationIndex = 9 + (int)(Angle / 9.5f);
	}

	float4 DirVec = (PlayerPos - MyPos).GetNormalize();
	if ((m_Dir == DIRECTION::LEFT && DirVec.x < 0.f)
		 || (m_Dir == DIRECTION::RIGHT && 0.f <= DirVec.x))
	{
		AnimationIndex = min(18, AnimationIndex);
	}
	else
	{
		AnimationIndex = 9;

		DirVec.x = (float)m_Dir;
		DirVec.y = 0.f;

		Angle = 0.f;
	}

	SetAnimationFrameIndex(AnimationIndex);

	float4 Pos = m_Trans->GetWorldPosition();
	Pos += float4(0.f, 28.f);
	Pos += DirVec * 20.f;

	m_LaserLineList[m_LaserLineIndex]->Setting(Pos, Angle);
}

void HeadHunter2::CalcCooltime()
{
	m_DodgeCooltime -= DELTATIME();
}

void HeadHunter2::AddLaserLineIndex()
{
	m_LaserLineIndex++;
	if (m_LaserLineList.size() - 1 < m_LaserLineIndex)
	{
		m_LaserLineIndex = 0;
	}
}

void HeadHunter2::AddGrenadeCount()
{
	m_GrenadeIndex++;
	if (m_GrenadeList.size() - 1 < m_GrenadeIndex)
	{
		m_GrenadeIndex = 0;
	}
}

void HeadHunter2::CreateBullet(float _Angle)
{
	CreateGunFireEffect(_Angle);

	float4 Pos = m_Trans->GetWorldPosition() + float4(0.f, 23.f);
	Pos += float4::GlobalDirToDeg(_Angle) * 30.f;
	m_BulletList[m_BulletIndex++]->Fire(Pos, _Angle);
}

void HeadHunter2::CreateGunFireEffect(float _Angle)
{
	InstanceObjectData Data;
	Data.AnimationName = L"Fire";
	Data.TextureName = L"GunFire3";
	Data.Pos = m_Trans->GetWorldPosition();
	Data.DirVecMovePos = -40.f;
	Data.Angle = _Angle + 180.f;
	Data.AnimationFrameTime = 0.03f;
	Data.ActorType = ACTORTYPE::EFFECT;

	CreateInstanceObject(Data, false);
}

void HeadHunter2::CreateGrenade()
{
	float4 Pos = m_Trans->GetWorldPosition() + float4(5.f * (float)m_Dir, 23.f);
	m_GrenadeList[m_GrenadeCount]->Attack(m_Dir, Pos, GJHGameEngineRandom::RandomFloat(-550.f, -250.f));

	AddGrenadeCount();
}

void HeadHunter2::RandomPattern()
{
	int Pattern = 0;
	do 
	{
		Pattern = GJHGameEngineRandom::RandomInt(0, 2);
	} while (m_PrevNormalPatternIndex == Pattern);

	switch (Pattern)
	{
	case 0:
		State.ChangeState(L"TakeOutRifle");
		break;
	case 1:
		State.ChangeState(L"PreDash");
		break;
	case 2:
		State.ChangeState(L"TakeOutGun");
		break;
	}

	m_PrevNormalPatternIndex = Pattern;
}

bool HeadHunter2::Escape()
{
	if (abs(DistanceToPlayer().x) <= 150.f)
	{
		if (m_DodgeCooltime <= 0.f)
		{
			State.ChangeState(L"DodgeRoll");
			return true;
		}
		else
		{
			State.ChangeState(L"PreJump");
			return true;
		}
	}

	return false;
}

void HeadHunter2::CreateHurtEffect()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Fire";
	Data.TextureName = L"GunFire5";
	Data.AnimationFrameTime = 0.08f;
	Data.ActorType = ACTORTYPE::EFFECT;

	float Angle = 0.f;
	for (int i = 0; i < 4; ++i)
	{
		Data.Pos = m_Trans->GetWorldPosition() + float4(0, -20.f);
		Data.DirVecMovePos = -30.f;
		Data.Angle = Angle + 180.f;
		CreateInstanceObject(Data, false);

		Angle += 60.f;
	}

	Angle = 0.f;
	for (int i = 0; i < 7; ++i)
	{
		Data.Pos = m_Trans->GetWorldPosition() + float4(0, -20.f);
		Data.DirVecMovePos = -50.f;
		Data.Angle = Angle + 180.f;
		CreateInstanceObject(Data, false);

		Angle += 30.f;
	}

	Angle = 20.f;
	for (int i = 0; i < 8; ++i)
	{
		Data.Pos = m_Trans->GetWorldPosition() + float4(0, -20.f);
		Data.DirVecMovePos = -70.f;
		Data.Angle = Angle + 180.f;
		CreateInstanceObject(Data, false);

		Angle += 20.f;
	}

	SOUNDPLAY(L"HeadHunter_Vanish.wav");
}

void HeadHunter2::PlayAppearSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"HeadHunter_Appear_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}

void HeadHunter2::PlayHurtSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"HeadHunter_Hurt_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}