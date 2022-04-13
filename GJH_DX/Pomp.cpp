#include "Pomp.h"
#include <GJHRenderer.h>
#include "Player.h"
#include "PlayerSlash.h"
#include "AStarManager.h"
#include <GJHGameEngineRandom.h>

Pomp::Pomp() :
	m_SlashActor(nullptr)
{

}

Pomp::~Pomp()
{

}

void Pomp::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	MonsterBase::Start(_Renderer, _RenderScale);

	m_MoveSpeed = 200.f;
	m_Time = 2.f;

	CreateAnimation(L"Idle", L"Pomp_Idle", 0.1f, true);
	CreateAnimation(L"Walk", L"Pomp_Walk", 0.1f, true);
	CreateAnimation(L"Run", L"Pomp_Run", 0.1f, true);
	CreateAnimation(L"Turn", L"Pomp_Turn", 0.05f, false);
	CreateAnimation(L"Attack", L"Pomp_Attack", 0.1f, false);
	CreateAnimation(L"HurtFly", L"Pomp_HurtFly", 0.08f, false);
	CreateAnimation(L"HurtGround", L"Pomp_HurtGround", 0.08f, false);

	State.CreateState(L"Idle", &Pomp::Idle_Init, &Pomp::Idle_State, nullptr, this);
	State.CreateState(L"Walk", &Pomp::Walk_Init, &Pomp::Walk_State, nullptr, this);
	State.CreateState(L"Run", &Pomp::Run_Init, &Pomp::Run_State, nullptr, this);
	State.CreateState(L"Turn", &Pomp::Turn_Init, &Pomp::Turn_State, nullptr, this);
	State.CreateState(L"Attack", &Pomp::Attack_Init, &Pomp::Attack_State, &Pomp::Attack_Leave, this);
	State.CreateState(L"HurtFly", &Pomp::HurtFly_Init, &Pomp::HurtFly_State, nullptr, this);
	State.CreateState(L"HurtGround", &Pomp::HurtGround_Init, &Pomp::HurtGround_State, &Pomp::HurtGround_Leave, this);
	State.CreateState(L"DoorIn", &Pomp::DoorIn_Init, &Pomp::DoorIn_State, nullptr, this);
	State.CreateState(L"DoorOut", &Pomp::DoorOut_Init, &Pomp::DoorOut_State, nullptr, this);
	State.CreateState(L"PatrolTurn", &Pomp::PatrolTurn_Init, &Pomp::PatrolTurn_State, nullptr, this);
	State.CreateState(L"Patrol", &Pomp::Patrol_Init, &Pomp::Patrol_State, nullptr, this);

	CreateCollision((int)CollisionLayer::Monster, COLTYPE::AABB, &Pomp::ColEnter, &Pomp::ColStay, &Pomp::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });

	CreateAttackCollision((int)CollisionLayer::MonsterAttack, COLTYPE::AABB, &Pomp::ColEnter, &Pomp::ColStay, &Pomp::ColExit, *this);
	m_AttackCollision->SetLocalPosition({ 20.f, 20.f });
	m_AttackCollision->SetLocalScale({ 40.f, 30.f });
	m_AttackCollision->Disable();

	State.ChangeState(L"Idle");

	SetDirection(DIRECTION::RIGHT);
}

void Pomp::Update()
{
	if (m_Record->m_RewindStart == false
		&& m_Death == false)
	{
		DetectRay(200, 100.f);

		State.Update();
		MonsterBase::Update();
	}
}

void Pomp::CreateSlashEffect()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Attack";
	Data.TextureName = L"Pomp_Slash";;
	Data.Dir = m_Dir;
	Data.UseTimeScale = true;
	Data.AnimationFrameTime = 0.06f;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_SlashActor = CreateInstanceObject(Data, true);
}

void Pomp::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::PlayerSlash)
	{
		LookOpposite(_Other->GetActor());
		Damage(_Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalRotation().z);

		if (m_IsDamageFly == true)
		{
			State.ChangeState(L"HurtFly");
		}
		else
		{
			State.ChangeState(L"HurtGround");
		}
	}

	if (_Other->GetOrder() == (int)CollisionLayer::PlayerBullet)
	{
		CamShake(0.25f, 5);
		StopTime(0.f, 0.2f);

		ChangeDirection();

		Damage(0.f);

		State.ChangeState(L"HurtGround");
	}

	if (_Other->GetOrder() == (int)CollisionLayer::DoorAttack)
	{
		LookOpposite(_Other->GetActor());

		Damage(0.f);

		State.ChangeState(L"HurtGround");
	}

	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Pomp::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Pomp::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Pomp::RewindEnd()
{
	SetDirection(DIRECTION::LEFT);
	m_MoveSpeed = 200.f;

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

void Pomp::SceneChangeStart()
{
	m_MonsterCount++;
}

void Pomp::Idle_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedStop();

	m_Time = GJHGameEngineRandom::RandomFloat(1.f, 5.f);

	ChangeAnimation(L"Idle");
}

void Pomp::Idle_State()
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

void Pomp::Walk_Init()
{
	MoveSpeedUniform();

	ChangeAnimation(L"Walk");
}

void Pomp::Walk_State()
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

void Pomp::Run_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedUniform();

	ChangeAnimation(L"Run");
}

void Pomp::Run_State()
{
	if (XDistanceToPlayer(40.f) <= 40.f)
	{
		State.ChangeState(L"Attack");
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
	}
}

void Pomp::Attack_Init()
{
	MoveSpeedStop();

	m_AttackCollision->ChangeDirection();

	CreateSlashEffect();

	ChangeAnimation(L"Attack");

	SOUNDPLAY(L"Punch.wav");
}

void Pomp::Attack_State()
{
	if (GetAnimationFrameIndex() == 2)
	{
		m_AttackCollision->Enable();
	}

	if (GetAnimationFrameIndex() == 3)
	{
		m_AttackCollision->Disable();
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void Pomp::Attack_Leave()
{
	m_AttackCollision->Disable();
}

void Pomp::Turn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Pomp::Turn_State()
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

void Pomp::HurtFly_Init()
{
	m_Ray->PauseDebugRender();

	if (m_SlashActor != nullptr)
	{
		m_SlashActor->Death();
	}

	m_Collision->Disable();

	ChangeAnimation(L"HurtFly");
}

void Pomp::HurtFly_State()
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

void Pomp::HurtGround_Init()
{
	m_Ray->PauseDebugRender();

	if (m_SlashActor != nullptr)
	{
		m_SlashActor->Death();
	}

	m_Collision->Disable();

	ChangeAnimation(L"HurtGround");
}

void Pomp::HurtGround_State()
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
			m_Ray->PauseDebugRender();
			m_MonsterCount--;
			m_Death = true;
		}
	}
}

void Pomp::HurtGround_Leave()
{
}

void Pomp::DoorIn_Init()
{
	MoveSpeedStop();

	m_Collision->Disable();
	m_RenderColor = 1.f;

	ChangeAnimation(L"Idle");
}

void Pomp::DoorIn_State()
{
	m_RenderColor -= DELTATIME(2.f);
	m_Render->SetColor(m_RenderColor);
	if (m_RenderColor <= 0.f)
	{
		m_RenderColor = 0.f;
		State.ChangeState(L"DoorOut");
	}
}

void Pomp::DoorOut_Init()
{
	m_Trans->SetWorldPosition(m_DoorTeleportPos);

	m_RenderColor = 0.f;
}

void Pomp::DoorOut_State()
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

void Pomp::PatrolTurn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Pomp::PatrolTurn_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Patrol");
	}
}

void Pomp::Patrol_Init()
{
	m_MoveSpeed = 50.f;

	MoveSpeedUniform();
	m_PatrolTime = GJHGameEngineRandom::RandomFloat(1.f, 3.f);
	ChangeAnimation(L"Walk");
}

void Pomp::Patrol_State()
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