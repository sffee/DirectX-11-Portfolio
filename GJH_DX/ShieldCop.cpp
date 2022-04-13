#include "ShieldCop.h"
#include "InstanceObject.h"
#include "Player.h"
#include "MonsterBullet.h"
#include <GJHGameEngineRandom.h>
#include "AStarManager.h"

ShieldCop::ShieldCop()
{

}

ShieldCop::~ShieldCop()
{
}

void ShieldCop::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	MonsterBase::Start(_Renderer, _RenderScale);

	m_MoveSpeed = 200.f;

	CreateAnimation(L"Idle", L"ShieldCop_Idle", 0.1f, true);
	CreateAnimation(L"Walk", L"ShieldCop_Walk", 0.1f, true);
	CreateAnimation(L"Run", L"ShieldCop_Run", 0.1f, true);
	CreateAnimation(L"Turn", L"ShieldCop_Turn", 0.05f, false);
	CreateAnimation(L"Aim", L"ShieldCop_Aim", 10000.f, false);
	CreateAnimation(L"AimCoolTime", L"ShieldCop_Idle", 0.1f, false);
	CreateAnimation(L"Bash", L"ShieldCop_Bash", 0.1f, false);
	CreateAnimation(L"KnockBack", L"ShieldCop_KnockBack", 0.4f, false);
	CreateAnimation(L"HurtFly", L"ShieldCop_HurtFly", 0.08f, false);
	CreateAnimation(L"HurtGround", L"ShieldCop_HurtGround", 0.08f, false);

	State.CreateState(L"Idle", &ShieldCop::Idle_Init, &ShieldCop::Idle_State, nullptr, this);
	State.CreateState(L"Walk", &ShieldCop::Walk_Init, &ShieldCop::Walk_State, nullptr, this);
	State.CreateState(L"Run", &ShieldCop::Run_Init, &ShieldCop::Run_State, nullptr, this);
	State.CreateState(L"Turn", &ShieldCop::Turn_Init, &ShieldCop::Turn_State, nullptr, this);
	State.CreateState(L"Aim", &ShieldCop::Aim_Init, &ShieldCop::Aim_State, &ShieldCop::Aim_Leave, this);
	State.CreateState(L"AimCoolTime", &ShieldCop::AimCoolTime_Init, &ShieldCop::AimCoolTime_State, nullptr, this);
	State.CreateState(L"Bash", &ShieldCop::Bash_Init, &ShieldCop::Bash_State, nullptr, this);
	State.CreateState(L"KnockBack", &ShieldCop::KnockBack_Init, &ShieldCop::KnockBack_State, nullptr, this);
	State.CreateState(L"HurtFly", &ShieldCop::HurtFly_Init, &ShieldCop::HurtFly_State, nullptr, this);
	State.CreateState(L"HurtGround", &ShieldCop::HurtGround_Init, &ShieldCop::HurtGround_State, nullptr, this);
	State.CreateState(L"DoorIn", &ShieldCop::DoorIn_Init, &ShieldCop::DoorIn_State, nullptr, this);
	State.CreateState(L"DoorOut", &ShieldCop::DoorOut_Init, &ShieldCop::DoorOut_State, nullptr, this);
	State.CreateState(L"PatrolTurn", &ShieldCop::PatrolTurn_Init, &ShieldCop::PatrolTurn_State, nullptr, this);
	State.CreateState(L"Patrol", &ShieldCop::Patrol_Init, &ShieldCop::Patrol_State, nullptr, this);

	CreateCollision((int)CollisionLayer::MonsterShield, COLTYPE::AABB, &ShieldCop::ColEnter, &ShieldCop::ColStay, &ShieldCop::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });

	CreateAttackCollision((int)CollisionLayer::MonsterShieldAttack, COLTYPE::AABB, & ShieldCop::ColEnter, & ShieldCop::ColStay, & ShieldCop::ColExit, * this);
	m_AttackCollision->SetLocalPosition({ 30.f, 30.f });
	m_AttackCollision->SetLocalScale({ 40.f, 30.f });
	m_AttackCollision->Disable();

	State.ChangeState(L"Idle");

	m_FireCooltime = 0.f;

	CreateBullet();
}

void ShieldCop::Update()
{
	if (m_Record->m_RewindStart == false
		&& m_Death == false)
	{
		m_FireCooltime -= DELTATIME();
		DetectRay(200, 100.f);

		State.Update();
		MonsterBase::Update();

	}
}

void ShieldCop::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::PlayerSlash)
	{
		float4 OtherScale = _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalScale();
		DIRECTION OtherDir = (OtherScale.x < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;

		if (OtherDir == m_Dir)
		{
			LookOpposite(_Other->GetActor());
			Damage(_Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalRotation().z);
			m_AttackCollision->Disable();

			if (m_IsDamageFly == true)
			{
				State.ChangeState(L"HurtFly");
			}
			else
			{
				State.ChangeState(L"HurtGround");
			}
		}
		else
		{
			State.ChangeState(L"KnockBack");
		}
	}

	if (_Other->GetOrder() == (int)CollisionLayer::PlayerBullet)
	{
		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();
		float4 MyPos = m_Trans->GetWorldPosition();
		DIRECTION OtherDir = (OtherPos.x - MyPos.x < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;

		if (OtherDir == m_Dir)
		{
			State.ChangeState(L"KnockBack");
		}
		else
		{
			CamShake(0.25f, 5);
			StopTime(0.f, 0.2f);

			ChangeDirection();

			Damage(0.f);
			m_AttackCollision->Disable();

			State.ChangeState(L"HurtGround");
		}
	}

	if (_Other->GetOrder() == (int)CollisionLayer::DoorAttack)
	{
		LookOpposite(_Other->GetActor());

		Damage(0.f);
		m_AttackCollision->Disable();

		State.ChangeState(L"HurtGround");
	}

	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void ShieldCop::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void ShieldCop::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void ShieldCop::RewindEnd()
{
	SetDirection(DIRECTION::LEFT);

	if (m_Death == true)
	{
		m_Death = false;
		m_MonsterCount++;
	}

	m_IsDetect = false;
	m_PlayDetectEffect = false;
	m_Collision->Enable();

	m_Ray->ResumeDebugRender();

	State.ChangeState(L"Idle");
}

void ShieldCop::SceneChangeStart()
{
	m_MonsterCount++;
}

void ShieldCop::CreateGunFireEffect(float _Angle)
{
	InstanceObjectData Data;
	Data.AnimationName = L"Fire";
	Data.TextureName = L"GunFire3";
	Data.Pos = m_Trans->GetWorldPosition() + float4(0.f, m_Render->GetLocalScale().y * 0.5f);
	Data.DirVecMovePos = 13.f * (float)m_Dir;
	Data.Dir = m_Dir;
	Data.Angle = _Angle;
	Data.AnimationFrameTime = 0.08f;
	Data.MeshName = L"Ray";
	Data.ActorType = ACTORTYPE::EFFECT;

	CreateInstanceObject(Data, false);
}

void ShieldCop::CreateBullet()
{
	m_BulletList.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_BulletList[i] = CreateActor<MonsterBullet>(ACTORTYPE::BULLET, L"Bullet", { 0.f, 0.f }, 1.f)->FindComponent<MonsterBullet>();
	}

	m_BulletIndex = 0;
}

void ShieldCop::Fire()
{
	float Angle = AngleToPlayer(m_Dir);
	Angle = (m_Player->GetActorTrans()->GetWorldPosition().x < m_Trans->GetWorldPosition().x) ? Angle *= -1.f : Angle;
	CreateGunFireEffect(Angle);

	float4 Pos = m_Trans->GetWorldPosition() + float4(0.f, 23.f);
	Angle = (m_Player->GetActorTrans()->GetWorldPosition().x < m_Trans->GetWorldPosition().x) ? Angle - 180.f : Angle;

	m_BulletList[m_BulletIndex++]->Fire(Pos, Angle);
	if (m_BulletList.size() <= m_BulletIndex)
	{
		m_BulletIndex = 0;
	}

	SOUNDPLAY(L"Gun_Fire.wav");
}

void ShieldCop::Idle_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedStop();
	m_AttackCollision->Disable();

	m_Time = GJHGameEngineRandom::RandomFloat(1.f, 5.f);

	ChangeAnimation(L"Idle");
}

void ShieldCop::Idle_State()
{
	m_Time -= DELTATIME();
	if (m_Time <= 0.f)
	{
		if (CheckColWall() == true
			|| AStarManager::GetTile(m_Trans->GetWorldPosition()).Type == AStarManager::TYPE::STAIRWAYINOUT)
		{
			State.ChangeState(L"PatrolTurn");
			return;
		}

		int Random = GJHGameEngineRandom::RandomInt(0, 1);
		if (Random == 0)
		{
			State.ChangeState(L"PatrolTurn");
		}
		else
		{
			State.ChangeState(L"Patrol");
		}
	}

	if (m_IsDetect == true)
	{
		if (m_ChangeDirection == true)
		{
			State.ChangeState(L"Turn");
		}
		else
		{
			State.ChangeState(L"Run");
		}
	}
}

void ShieldCop::Walk_Init()
{
	MoveSpeedUniform();

	ChangeAnimation(L"Walk");
}

void ShieldCop::Walk_State()
{
	if (m_IsDetect == true)
	{
		if (m_ChangeDirection == true)
		{
			State.ChangeState(L"Turn");
		}
		else
		{
			State.ChangeState(L"Run");
		}
	}
}

void ShieldCop::Run_Init()
{
	m_MoveSpeed = 200.f;

	MoveSpeedUniform();

	ChangeAnimation(L"Run");
}

void ShieldCop::Run_State()
{
	if (XDistanceToPlayer(120.f) <= 60.f)
	{
		float4 Distance = DistanceToPlayer();

		if (m_Dir == DIRECTION::LEFT && 0.f < Distance.x)
		{
			State.ChangeState(L"Turn");
			return;
		}
		else if (m_Dir == DIRECTION::RIGHT && Distance.x < 0.f)
		{
			State.ChangeState(L"Turn");
			return;
		}

		State.ChangeState(L"Bash");
		return;
	}

	if (m_OnDoor == true)
	{
		State.ChangeState(L"DoorIn");
		return;
	}

	if (m_ChangeDirection == true)
	{
		State.ChangeState(L"Turn");
		return;
	}

	if (XDistanceToPlayer(150.f) <= 200.f)
	{
		float4 Distance = DistanceToPlayer();

		if (m_Dir == DIRECTION::LEFT && 0.f < Distance.x)
		{
			State.ChangeState(L"Turn");
		}
		else if (m_Dir == DIRECTION::RIGHT && Distance.x < 0.f)
		{
			State.ChangeState(L"Turn");
		}
		else
		{
			State.ChangeState(L"Aim");
		}
	}
}

void ShieldCop::Aim_Init()
{
	MoveSpeedStop();

	ChangeAnimation(L"Aim");
	SetAnimationFrameIndex(8);

	m_FireCooltime = 0.5f;
}

void ShieldCop::Aim_State()
{
	float Angle = AngleToPlayer(m_Dir);
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	float4 DirVec = PlayerPos - MyPos;

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

	AnimationIndex = (18 < AnimationIndex) ? 8 : AnimationIndex;
	SetAnimationFrameIndex(AnimationIndex);

	m_FireCooltime -= DELTATIME();
	if (m_FireCooltime <= 0.f)
	{
		m_FireCooltime = 1.f;
		Fire();
		State.ChangeState(L"AimCoolTime");
	}
}

void ShieldCop::Aim_Leave()
{
}

void ShieldCop::AimCoolTime_Init()
{
	m_Time = 0.f;
}

void ShieldCop::AimCoolTime_State()
{
	float Angle = AngleToPlayer(m_Dir);
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	float4 DirVec = PlayerPos - MyPos;

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

	AnimationIndex = (18 < AnimationIndex) ? 8 : AnimationIndex;
	SetAnimationFrameIndex(AnimationIndex);

	float4 Distance = DistanceToPlayer();

	if (m_Dir == DIRECTION::LEFT && 0.f < Distance.x)
	{
		State.ChangeState(L"Turn");
		return;
	}
	else if (m_Dir == DIRECTION::RIGHT && Distance.x < 0.f)
	{
		State.ChangeState(L"Turn");
		return;
	}

	m_Time += DELTATIME();
	if (1.f <= m_Time)
	{
		if (XDistanceToPlayer(80.f) <= 60.f)
		{
			float4 Distance = DistanceToPlayer();

			if (m_Dir == DIRECTION::LEFT && 0.f < Distance.x)
			{
				State.ChangeState(L"Turn");
				return;
			}
			else if (m_Dir == DIRECTION::RIGHT && Distance.x < 0.f)
			{
				State.ChangeState(L"Turn");
				return;
			}

			State.ChangeState(L"Bash");
			return;
		}

		if (XDistanceToPlayer(150.f) <= 200.f)
		{
			State.ChangeState(L"Aim");
			return;
		}

		State.ChangeState(L"Run");
	}
}

void ShieldCop::Bash_Init()
{
	MoveSpeedStop();

	m_AttackCollision->ChangeDirection();

	ChangeAnimation(L"Bash");

	SOUNDPLAY(L"Punch.wav");
}

void ShieldCop::Bash_State()
{
	if (GetAnimationFrameIndex() == 1)
	{
		m_AttackCollision->Enable();
	}
	else if (4 <= GetAnimationFrameIndex())
	{
		m_AttackCollision->Disable();
	}

	if (AnimationEnd())
	{
		if (XDistanceToPlayer(80.f) <= 60.f)
		{
			float4 Distance = DistanceToPlayer();

			if (m_Dir == DIRECTION::LEFT && 0.f < Distance.x)
			{
				State.ChangeState(L"Turn");
				return;
			}
			else if (m_Dir == DIRECTION::RIGHT && Distance.x < 0.f)
			{
				State.ChangeState(L"Turn");
				return;
			}

			State.ChangeState(L"Bash");
			return;
		}
		else
		{
			State.ChangeState(L"Run");
		}
	}
}

void ShieldCop::KnockBack_Init()
{
	m_MoveSpeed = 600.f;
	MoveSpeedStop();
	MoveSpeedUpStart(-0.5f, 0.005f, 0.f);
	ChangeAnimation(L"KnockBack");
}

void ShieldCop::KnockBack_State()
{
	if (CheckColWall(2, true) == true)
	{
		m_XMoveRatio = 0.f;
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void ShieldCop::Turn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void ShieldCop::Turn_State()
{
	if (AnimationEnd())
	{
		if (m_ChangeDirection == true)
		{
			State.ChangeState(L"Turn");
		}
		else
		{
			State.ChangeState(L"Run");
		}
	}
}

void ShieldCop::HurtFly_Init()
{
	m_Ray->PauseDebugRender();
	m_Collision->Disable();

	ChangeAnimation(L"HurtFly");
}

void ShieldCop::HurtFly_State()
{
	{
		float4 Color = CheckColMapColor(0, 0);
		if (Color == float4::GREEN
			|| Color == float4::RED
			|| CheckDoorCol() == true)
		{
			m_Trans->SetWorldPosition(m_PrevPos);
			MoveSpeedStop();
			m_KnockBackVec = float4(0.f, 0.f);

			MoveNearNormalPos();
			float4 Move = float4((float)m_Dir * -1.f, 0.f);
			while (CheckDoorCol() == true)
			{
				m_Trans->SetWorldMove(Move);
			}
		}
	}

	{
		float4 Color = CheckColMapColor(0, -1);
		if (Color != float4::RED
			&& Color != float4::BLUE
			&& Color != float4::GREEN
			&& m_Gravity == 0.f)
		{
			Jump(100.f, 600.f);
		}
	}

	if (m_IsDamageFly == false)
	{
		State.ChangeState(L"HurtGround");
	}
}

void ShieldCop::HurtGround_Init()
{
	m_Ray->PauseDebugRender();
	m_Collision->Disable();

	ChangeAnimation(L"HurtGround");
}

void ShieldCop::HurtGround_State()
{
	{
		float4 Color = CheckColMapColor(0, 0);
		if (Color == float4::GREEN
			|| Color == float4::RED
			|| CheckDoorCol() == true)
		{
			MoveSpeedStop();
			m_KnockBackVec = float4(0.f, 0.f);

			MoveNearNormalPos();
			float4 Move = float4((float)m_Dir * -1.f, 0.f);
			while (CheckDoorCol() == true)
			{
				m_Trans->SetWorldMove(Move);
			}
		}
	}

	{
		float4 Color = CheckColMapColor(0, -1);
		if (Color != float4::RED
			&& Color != float4::BLUE
			&& Color != float4::GREEN
			&& m_Gravity == 0.f)
		{
			Jump(100.f, 600.f);
		}

		if (AnimationEnd()
			&& (Color == float4::RED || Color == float4::BLUE))
		{
			m_MonsterCount--;
			m_Death = true;
		}
	}
}

void ShieldCop::DoorIn_Init()
{
	MoveSpeedStop();

	m_Collision->Disable();
	m_RenderColor = 1.f;

	ChangeAnimation(L"Idle");
}

void ShieldCop::DoorIn_State()
{
	m_RenderColor -= DELTATIME(2.f);
	m_Render->SetColor(m_RenderColor);
	if (m_RenderColor <= 0.f)
	{
		m_RenderColor = 0.f;
		State.ChangeState(L"DoorOut");
	}
}

void ShieldCop::DoorOut_Init()
{
	m_Trans->SetWorldPosition(m_DoorTeleportPos);

	m_RenderColor = 0.f;
}

void ShieldCop::DoorOut_State()
{
	m_RenderColor += DELTATIME(2.f);
	m_Render->SetColor(m_RenderColor);
	if (1.f <= m_RenderColor)
	{
		m_RenderColor = 1.f;
		m_Collision->Enable();
		State.ChangeState(L"Idle");
	}
}

void ShieldCop::PatrolTurn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void ShieldCop::PatrolTurn_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Patrol");
	}
}

void ShieldCop::Patrol_Init()
{
	m_MoveSpeed = 50.f;

	MoveSpeedUniform();
	m_PatrolTime = GJHGameEngineRandom::RandomFloat(1.f, 3.f);
	ChangeAnimation(L"Walk");
}

void ShieldCop::Patrol_State()
{
	m_PatrolTime -= DELTATIME();
	if (m_PatrolTime <= 0.f)
	{
		State.ChangeState(L"Idle");
	}

	if (CheckColWall() == true
		|| AStarManager::GetTile(m_Trans->GetWorldPosition()).Type == AStarManager::TYPE::STAIRWAYINOUT
		|| CheckDoorCol() == true)
	{
		State.ChangeState(L"Idle");
	}

	if (m_IsDetect == true)
	{
		if (m_ChangeDirection == true)
		{
			State.ChangeState(L"Turn");
		}
		else
		{
			State.ChangeState(L"Run");
		}
	}
}