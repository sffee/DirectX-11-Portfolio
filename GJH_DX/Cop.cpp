#include "Cop.h"
#include "InstanceObject.h"
#include "Player.h"
#include "MonsterBullet.h"
#include <GJHGameEngineRandom.h>
#include "AStarManager.h"

Cop::Cop() 
{

}

Cop::~Cop()
{
}

void Cop::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	MonsterBase::Start(_Renderer, _RenderScale);

	m_MoveSpeed = 200.f;

	CreateAnimation(L"Idle", L"Cop_Idle", 0.1f, true);
	CreateAnimation(L"Walk", L"Cop_Walk", 0.1f, true);
	CreateAnimation(L"Run", L"Cop_Run", 0.1f, true);
	CreateAnimation(L"Turn", L"Cop_Turn", 0.05f, false);
	CreateAnimation(L"Attack", L"Cop_Attack", 0.1f, false);
	CreateAnimation(L"HurtFly", L"Cop_HurtFly", 0.08f, false);
	CreateAnimation(L"HurtGround", L"Cop_HurtGround", 0.08f, false);

	State.CreateState(L"Idle", &Cop::Idle_Init, &Cop::Idle_State, nullptr, this);
	State.CreateState(L"Walk", &Cop::Walk_Init, &Cop::Walk_State, nullptr, this);
	State.CreateState(L"Run", &Cop::Run_Init, &Cop::Run_State, nullptr, this);
	State.CreateState(L"Turn", &Cop::Turn_Init, &Cop::Turn_State, nullptr, this);
	State.CreateState(L"Attack", &Cop::Attack_Init, &Cop::Attack_State, &Cop::Attack_Leave, this);
	State.CreateState(L"HurtFly", &Cop::HurtFly_Init, &Cop::HurtFly_State, nullptr, this);
	State.CreateState(L"HurtGround", &Cop::HurtGround_Init, &Cop::HurtGround_State, nullptr, this);
	State.CreateState(L"DoorIn", &Cop::DoorIn_Init, &Cop::DoorIn_State, nullptr, this);
	State.CreateState(L"DoorOut", &Cop::DoorOut_Init, &Cop::DoorOut_State, nullptr, this);
	State.CreateState(L"PatrolTurn", &Cop::PatrolTurn_Init, &Cop::PatrolTurn_State, nullptr, this);
	State.CreateState(L"Patrol", &Cop::Patrol_Init, &Cop::Patrol_State, nullptr, this);

	CreateCollision((int)CollisionLayer::Monster, COLTYPE::AABB, &Cop::ColEnter, &Cop::ColStay, &Cop::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });

	CreateLeftArm();
	CreateRightArm();
	CreateGun();
	CreateBullet();

	State.ChangeState(L"Idle");

	m_FireCooltime = 0.f;
}

void Cop::Update()
{
	if (m_Record->m_RewindStart == false
		&& m_Death == false)
	{
		DetectRay(200, 100.f);

		State.Update();
		MonsterBase::Update();
	}
}

void Cop::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
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

void Cop::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Cop::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Cop::RewindEnd()
{
	SetDirection(DIRECTION::LEFT);
	m_MoveSpeed = 200.f;
	MoveSpeedStop();

	m_LeftArm->FindComponent<GJHActorTransform>()->SetLocalPosition(float4(7.f, 23.f, -2.f));
	m_LeftArm->SetDirection(DIRECTION::LEFT);

	m_RightArm->FindComponent<GJHActorTransform>()->SetLocalPosition(float4(-3.f, 23.f, 1.f));
	m_RightArm->SetDirection(DIRECTION::LEFT);

	m_Gun->FindComponent<GJHActorTransform>()->SetLocalPosition(float4(4.f, 23.f, -1.f));
	m_Gun->SetDirection(DIRECTION::LEFT);

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

void Cop::SceneChangeStart()
{
	m_MonsterCount++;
}

void Cop::CreateLeftArm()
{
	InstanceObjectData Data;
	Data.AnimationName = L"LeftArm";
	Data.TextureName = L"Cop_LeftArm";
	Data.Pos = float4(7.f, 23.f, -2.f);
	Data.Dir = m_Dir;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.ActorType = ACTORTYPE::MONSTER;

	m_LeftArm = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
	m_LeftArmRender = m_LeftArm->GetActor()->FindComponent<GJHSpriteRenderer>();
	m_LeftArmRender->SetRenderEnable(false);
	m_LeftArmRender->SetMesh(L"Ray");
}

void Cop::CreateRightArm()
{
	InstanceObjectData Data;
	Data.AnimationName = L"RightArm";
	Data.TextureName = L"Cop_RightArm";
	Data.Pos = float4(-3.f, 23.f, 1.f);
	Data.Dir = m_Dir;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.ActorType = ACTORTYPE::MONSTER;

	m_RightArm = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
	m_RightArmRender = m_RightArm->GetActor()->FindComponent<GJHSpriteRenderer>();
	m_RightArmRender->SetRenderEnable(false);
	m_RightArmRender->SetMesh(L"Ray");
}

void Cop::CreateGun()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Gun";
	Data.TextureName = L"Cop_Gun";
	Data.Pos = float4(4.f, 23.f, -1.f);
	Data.Dir = m_Dir;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.ActorType = ACTORTYPE::MONSTER;

	m_Gun = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
	m_GunRender = m_Gun->GetActor()->FindComponent<GJHSpriteRenderer>();
	m_GunRender->SetRenderEnable(false);
	m_GunRender->SetMesh(L"Ray");
}

void Cop::CreateBullet()
{
	m_BulletList.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_BulletList[i] = CreateActor<MonsterBullet>(ACTORTYPE::BULLET, L"Bullet", { 0.f, 0.f }, 1.f)->FindComponent<MonsterBullet>();
	}

	m_BulletIndex = 0;
}

void Cop::CreateGunFireEffect(float _Angle)
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

void Cop::Fire()
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

void Cop::Idle_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedStop();

	m_Time = GJHGameEngineRandom::RandomFloat(1.f, 5.f);

	ChangeAnimation(L"Idle");
}

void Cop::Idle_State()
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

void Cop::Walk_Init()
{
	MoveSpeedUniform();

	ChangeAnimation(L"Walk");
}

void Cop::Walk_State()
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

void Cop::Run_Init()
{
	m_MoveSpeed = 200.f;
	MoveSpeedUniform();

	ChangeAnimation(L"Run");
}

void Cop::Run_State()
{
	if (XDistanceToPlayer(150.f) <= 200.f)
	{
		float Angle = AngleToPlayer();
		if (m_Dir == DIRECTION::LEFT)
		{
			Angle -= 180.f;
		}

		float4 PlayerPos = m_Player->FindComponent<GJHActorTransform>()->GetWorldPosition();
		float4 MyPos = m_Trans->GetWorldPosition();

		if (m_Dir == DIRECTION::RIGHT
			&& ceil(PlayerPos.y) < ceil(MyPos.y))
		{
			Angle -= 360.f;
		}

		if (abs(Angle) <= 45.f)
		{
			State.ChangeState(L"Attack");
			return;
		}
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

void Cop::Attack_Init()
{
	if (m_LeftArm->GetDirection() != m_Dir)
	{
		std::shared_ptr<GJHActorTransform> Trans = m_LeftArm->GetActor()->FindComponent<GJHActorTransform>();
		Trans->SetLocalMove(float4(Trans->GetLocalPosition().x * -2.f, 0.f));
	}

	if (m_RightArm->GetDirection() != m_Dir)
	{
		std::shared_ptr<GJHActorTransform> Trans = m_RightArm->GetActor()->FindComponent<GJHActorTransform>();
		Trans->SetLocalMove(float4(Trans->GetLocalPosition().x * -2.f, 0.f));
	}

	if (m_Gun->GetDirection() != m_Dir)
	{
		std::shared_ptr<GJHActorTransform> Trans = m_Gun->GetActor()->FindComponent<GJHActorTransform>();
		Trans->SetLocalMove(float4(Trans->GetLocalPosition().x * -2.f, 0.f));
	}

	m_LeftArm->SetDirection(m_Dir);
	m_RightArm->SetDirection(m_Dir);
	m_Gun->SetDirection(m_Dir);

	m_LeftArmRender->SetRenderEnable(true);
	m_RightArmRender->SetRenderEnable(true);
	m_GunRender->SetRenderEnable(true);

	m_IsFireRotationUp = false;
	m_IsFireRotationDown = false;
	
	MoveSpeedStop();

	ChangeAnimation(L"Attack");

	m_FireCooltime = 0.3f;
}

void Cop::Attack_State()
{
	m_FireCooltime -= DELTATIME();

	if (m_IsFireRotationUp == false && m_IsFireRotationDown == false)
	{
		float Angle = AngleToPlayer(m_Dir);

		float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
		float4 MyPos = m_Trans->GetWorldPosition();

		if (ceil(PlayerPos.y) < ceil(MyPos.y))
		{
			Angle -= 360.f;
		}

		if (abs(Angle) <= 45.f)
		{
			float AngleDir = (Angle < 0.f) ? -1.f : 1.f;
			float Rotation = (25.f <= abs(Angle)) ? 25.f * AngleDir : Angle;
			Rotation = (PlayerPos.x < MyPos.x) ? Rotation * -1.f : Rotation;

			m_LeftArmRender->SetWorldRotationZ(Rotation);
			m_RightArmRender->SetWorldRotationZ(Rotation);
			m_GunRender->SetWorldRotationZ(Rotation);
		}
		else
		{
			State.ChangeState(L"Idle");
			return;
		}

		if (m_FireCooltime <= 0.f)
		{
			m_FireCooltime = 1.f;
			Fire();
			m_FireGunRotationTime = 0.06f;

			m_IsFireRotationUp = true;
			m_IsFireRotationDown = false;
		}
	}

	if (m_IsFireRotationUp == true)
	{
		float Rotation = m_LeftArmRender->GetWorldRotation().z + DELTATIME(180.f) * (float)m_Dir;
		m_LeftArmRender->SetWorldRotationZ(Rotation);
		m_RightArmRender->SetWorldRotationZ(Rotation);
		m_GunRender->SetWorldRotationZ(Rotation);

		m_FireGunRotationTime -= DELTATIME();
		if (m_FireGunRotationTime <= 0.f)
		{
			m_FireGunRotationTime = 0.06f;
			m_IsFireRotationUp = false;
			m_IsFireRotationDown = true;
		}
	}
	else if (m_IsFireRotationDown == true)
	{
		float Rotation = m_LeftArmRender->GetWorldRotation().z - DELTATIME(180.f) * (float)m_Dir;
		m_LeftArmRender->SetWorldRotationZ(Rotation);
		m_RightArmRender->SetWorldRotationZ(Rotation);
		m_GunRender->SetWorldRotationZ(Rotation);

		m_FireGunRotationTime -= DELTATIME();
		if (m_FireGunRotationTime <= 0.f)
		{
			m_IsFireRotationUp = false;
			m_IsFireRotationDown = false;
		}
	}

	float Distance = XDistanceToPlayer(150.f);

	if (200.f <= Distance
		&& m_IsFireRotationUp == false
		&& m_IsFireRotationDown == false)
	{
		State.ChangeState(L"Idle");
	}
}

void Cop::Attack_Leave()
{
	m_LeftArmRender->SetRenderEnable(false);
	m_RightArmRender->SetRenderEnable(false);
	m_GunRender->SetRenderEnable(false);
}

void Cop::Turn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Cop::Turn_State()
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

void Cop::HurtFly_Init()
{
	m_Ray->PauseDebugRender();
	m_Collision->Disable();

	ChangeAnimation(L"HurtFly");
}

void Cop::HurtFly_State()
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

void Cop::HurtGround_Init()
{
	m_Ray->PauseDebugRender();
	m_Collision->Disable();

	ChangeAnimation(L"HurtGround");
}

void Cop::HurtGround_State()
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

void Cop::DoorIn_Init()
{
	MoveSpeedStop();

	m_Collision->Disable();
	m_RenderColor = 1.f;

	ChangeAnimation(L"Idle");
}

void Cop::DoorIn_State()
{
	m_RenderColor -= DELTATIME(2.f);
	m_Render->SetColor(m_RenderColor);
	if (m_RenderColor <= 0.f)
	{
		m_RenderColor = 0.f;
		State.ChangeState(L"DoorOut");
	}
}

void Cop::DoorOut_Init()
{
	m_Trans->SetWorldPosition(m_DoorTeleportPos);

	m_RenderColor = 0.f;
}

void Cop::DoorOut_State()
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

void Cop::PatrolTurn_Init()
{
	MoveSpeedStop();
	ChangeDirection();
	m_ChangeDirection = false;

	ChangeAnimation(L"Turn");
}

void Cop::PatrolTurn_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Patrol");
	}
}

void Cop::Patrol_Init()
{
	m_MoveSpeed = 50.f;

	MoveSpeedUniform();
	m_PatrolTime = GJHGameEngineRandom::RandomFloat(1.f, 3.f);
	ChangeAnimation(L"Walk");
}

void Cop::Patrol_State()
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