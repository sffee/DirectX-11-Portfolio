#include "Grunt.h"
#include <GJHGameEngineRandom.h>
#include "AStarManager.h"

Grunt::Grunt() :
	m_CreateSlashEffect(false)
{

}

Grunt::~Grunt()
{

}

void Grunt::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	MonsterBase::Start(_Renderer, _RenderScale);

	m_MoveSpeed = 200.f;

	CreateAnimation(L"Idle", L"Grunt_Idle", 0.1f, true);
	CreateAnimation(L"Walk", L"Grunt_Walk", 0.1f, true);
	CreateAnimation(L"Run", L"Grunt_Run", 0.1f, true);
	CreateAnimation(L"Turn", L"Grunt_Turn", 0.05f, false);
	CreateAnimation(L"Attack", L"Grunt_Attack", 0.1f, false);
	CreateAnimation(L"HurtFly", L"Grunt_HurtFly", 0.08f, false);
	CreateAnimation(L"HurtGround", L"Grunt_HurtGround", 0.08f, false);

	State.CreateState(L"Idle", &Grunt::Idle_Init, &Grunt::Idle_State, nullptr, this);
	State.CreateState(L"Walk", &Grunt::Walk_Init, &Grunt::Walk_State, nullptr, this);
	State.CreateState(L"Run", &Grunt::Run_Init, &Grunt::Run_State, nullptr, this);
	State.CreateState(L"Turn", &Grunt::Turn_Init, &Grunt::Turn_State, nullptr, this);
	State.CreateState(L"Attack", &Grunt::Attack_Init, &Grunt::Attack_State, &Grunt::Attack_Leave, this);
	State.CreateState(L"HurtFly", &Grunt::HurtFly_Init, &Grunt::HurtFly_State, nullptr, this);
	State.CreateState(L"HurtGround", &Grunt::HurtGround_Init, &Grunt::HurtGround_State, nullptr, this);
	State.CreateState(L"DoorIn", &Grunt::DoorIn_Init, &Grunt::DoorIn_State, nullptr, this);
	State.CreateState(L"DoorOut", &Grunt::DoorOut_Init, &Grunt::DoorOut_State, nullptr, this);
	State.CreateState(L"PatrolTurn", &Grunt::PatrolTurn_Init, &Grunt::PatrolTurn_State, nullptr, this);
	State.CreateState(L"Patrol", &Grunt::Patrol_Init, &Grunt::Patrol_State, nullptr, this);

	CreateCollision((int)CollisionLayer::Monster, COLTYPE::AABB, &Grunt::ColEnter, &Grunt::ColStay, &Grunt::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });

	CreateAttackCollision((int)CollisionLayer::MonsterAttack, COLTYPE::AABB, &Grunt::ColEnter, &Grunt::ColStay, &Grunt::ColExit, *this);
	m_AttackCollision->SetLocalPosition({ 20.f, 20.f });
	m_AttackCollision->SetLocalScale({ 40.f, 30.f });
	m_AttackCollision->Disable();

	State.ChangeState(L"Idle");
}

void Grunt::Update()
{
	if (m_Record->m_RewindStart == false
		&& m_Death == false)
	{
		DetectRay(200, 100.f);

		State.Update();
		MonsterBase::Update();
	}
}

void Grunt::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
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

void Grunt::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Grunt::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Grunt::RewindEnd()
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

void Grunt::SceneChangeStart()
{
	m_MonsterCount++;
}

void Grunt::CreateSlashEffect()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Attack";
	Data.TextureName = L"Grunt_Slash";
	Data.Dir = m_Dir;
	Data.UseTimeScale = true;
	Data.AnimationFrameTime = 0.1f;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_SlashActor = CreateInstanceObject(Data, true);
}

void Grunt::Idle_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedStop();

	m_Time = GJHGameEngineRandom::RandomFloat(1.f, 5.f);

	ChangeAnimation(L"Idle");
}

void Grunt::Idle_State()
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

void Grunt::Walk_Init()
{
	MoveSpeedUniform();

	ChangeAnimation(L"Walk");
}

void Grunt::Walk_State()
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

void Grunt::Run_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedUniform();

	ChangeAnimation(L"Run");
}

void Grunt::Run_State()
{
	if (XDistanceToPlayer(80.f) <= 40.f)
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

void Grunt::Attack_Init()
{
	MoveSpeedStop();

	m_AttackCollision->Disable();
	m_AttackCollision->ChangeDirection();

	m_CreateSlashEffect = false;

	ChangeAnimation(L"Attack");

	SOUNDPLAY(L"Punch.wav");
}

void Grunt::Attack_State()
{
	if (GetAnimationFrameIndex() == 4)
	{
		m_AttackCollision->Enable();
	}

	if (GetAnimationFrameIndex() == 6)
	{
		m_AttackCollision->Disable();
	}

	if (m_CreateSlashEffect == false
		&& 3 <= GetAnimationFrameIndex())
	{
		CreateSlashEffect();
		m_CreateSlashEffect = true;
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void Grunt::Attack_Leave()
{
	m_AttackCollision->Disable();
}

void Grunt::Turn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Grunt::Turn_State()
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

void Grunt::HurtFly_Init()
{
	m_Ray->PauseDebugRender();

	if (m_SlashActor != nullptr)
	{
		m_SlashActor->Death();
	}

	m_Collision->Disable();

	ChangeAnimation(L"HurtFly");
}

void Grunt::HurtFly_State()
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

void Grunt::HurtGround_Init()
{
	m_Ray->PauseDebugRender();

	if (m_SlashActor != nullptr)
	{
		m_SlashActor->Death();
	}

	m_Collision->Disable();

	ChangeAnimation(L"HurtGround");
}

void Grunt::HurtGround_State()
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

void Grunt::DoorIn_Init()
{
	MoveSpeedStop();

	m_Collision->Disable();
	m_RenderColor = 1.f;

	ChangeAnimation(L"Idle");
}

void Grunt::DoorIn_State()
{
	m_RenderColor -= DELTATIME(2.f);
	m_Render->SetColor(m_RenderColor);
	if (m_RenderColor <= 0.f)
	{
		m_RenderColor = 0.f;
		State.ChangeState(L"DoorOut");
	}
}

void Grunt::DoorOut_Init()
{
	m_Trans->SetWorldPosition(m_DoorTeleportPos);

	m_RenderColor = 0.f;
}

void Grunt::DoorOut_State()
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

void Grunt::PatrolTurn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Grunt::PatrolTurn_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Patrol");
	}
}

void Grunt::Patrol_Init()
{
	m_MoveSpeed = 50.f;

	MoveSpeedUniform();
	m_PatrolTime = GJHGameEngineRandom::RandomFloat(1.f, 3.f);
	ChangeAnimation(L"Walk");
}

void Grunt::Patrol_State()
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