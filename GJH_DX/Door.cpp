#include "Door.h"
#include "Player.h"
#include <GJHGameEngineRandom.h>

Door::Door()
{

}

Door::~Door()
{

}

void Door::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	CreateAnimation(L"Iron_Idle", L"Door_Iron_Idle", 0.1f, true);
	CreateAnimation(L"Iron_Damage", L"Door_Iron_Damage", 0.05f);

	State.CreateState(L"Iron_Idle", &Door::Iron_Idle_Init, &Door::Iron_Idle_State, nullptr, this);
	State.CreateState(L"Iron_Damage", &Door::Iron_Damage_Init, &Door::Iron_Damage_State, nullptr, this);

	CreateCollision((int)CollisionLayer::Door, COLTYPE::AABB, &Door::ColEnter, &Door::ColStay, &Door::ColExit, *this);
	m_Collision->SetLocalScale({ 22.f, 63.f });

	CreateAttackCollision((int)CollisionLayer::DoorAttack, COLTYPE::AABB, &Door::ColEnter, &Door::ColStay, &Door::ColExit, *this);
	m_AttackCollision->SetLocalScale({ 40.f, 63.f });
	m_AttackCollision->SetLocalPosition({ 0.f, 30.f });
	m_AttackCollision->Disable();

	State.ChangeState(L"Iron_Idle");

	SetDirection(-1);

	m_ApplyGravity = false;
	m_CheckPlatform = false;

	m_StartPos = m_Trans->GetWorldPosition();
}

void Door::Update()
{
	if (m_Record->m_RewindStart == false
		&& m_Death == false)
	{
		State.Update();
		ObjectBase::Update();
	}
}

void Door::RewindEnd()
{
	m_Collision->Enable();
	SetDirection(-1);
	m_Trans->SetWorldPosition(m_StartPos);
	m_Death = false;

	State.ChangeState(L"Iron_Idle");
}

void Door::SetDirection(int _Dir)
{
	if (_Dir == -1)
	{
		m_Trans->SetWorldMove({ -37.f, 0.f });
	}

	m_Collision->SetLocalPosition({ -19.f * (float)_Dir, 33.f });
	ObjectBase::SetDirection((DIRECTION)_Dir);
}

void Door::Damage(DIRECTION _Dir)
{
	if (_Dir == DIRECTION::RIGHT)
	{
		m_Trans->SetWorldMove({ 37.f * (float)_Dir, 0.f });
		m_Collision->SetLocalPosition({ -19.f * (float)_Dir, 33.f });
		ObjectBase::SetDirection(_Dir);
	}

	m_AttackCollision->ChangeDirection();

	State.ChangeState(L"Iron_Damage");

	SOUNDPLAY(L"DoorOpen.wav", 0, 1.5f);
}

void Door::BindDamageFunc(std::function<void(ObjectBase::DIRECTION)>& _Func)
{
	_Func = std::bind(&Door::Damage, this, std::placeholders::_1);
}

void Door::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::PlayerSlash)
	{
		float4 PlayerPos = m_Player->FindComponent<GJHActorTransform>()->GetWorldPosition();
		float4 MyPos = m_Trans->GetWorldPosition();

		DIRECTION Dir = (PlayerPos.x < MyPos.x) ? DIRECTION::RIGHT : DIRECTION::LEFT;

		Damage(Dir);
	}

	if (_this->GetOrder() == (int)CollisionLayer::DoorAttack
		&& (_Other->GetOrder() == (int)CollisionLayer::Monster || _Other->GetOrder() == (int)CollisionLayer::MonsterShield))
	{
		PlayHitSound();

		CamShake(0.25f, 5);
		StopTime(0.f, 0.2f);
	}
}

void Door::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Door::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Door::Iron_Idle_Init()
{
	m_Collision->Enable();
	ChangeAnimation(L"Iron_Idle");
}

void Door::Iron_Idle_State()
{

}

void Door::Iron_Damage_Init()
{
	m_Collision->Disable();
	m_IsCamShake = false;
	m_Time = 2.f;

	ChangeAnimation(L"Iron_Damage");
}

void Door::Iron_Damage_State()
{
	if (m_IsCamShake == false
		&& GetAnimationFrameIndex() == 2)
	{
		m_AttackCollision->Enable();
		CamShake(0.08f, 7);
		m_IsCamShake = true;
	}

	if (4 == GetAnimationFrameIndex())
	{
		m_AttackCollision->Disable();
	}

	if (AnimationEnd())
	{
		m_Death = true;
	}
}

void Door::PlayHitSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"Death_Generic_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}