#include "MonsterBullet.h"
#include "SlashFlow.h"
#include <GJHGameEngineRandom.h>

MonsterBullet::MonsterBullet()
{

}

MonsterBullet::~MonsterBullet()
{

}

void MonsterBullet::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);
	m_RenderScale = float4(1.f, 0.8f);

	m_ApplyGravity = false;
	m_CheckPlatform = false;
	m_Death = true;

	CreateAnimation(L"Bullet", L"Bullet", 0.01f, false);
	ChangeAnimation(L"Bullet");

	CreateCollision((int)CollisionLayer::MonsterBullet, COLTYPE::AABB, &MonsterBullet::ColEnter, &MonsterBullet::ColStay, &MonsterBullet::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 0.f });
	m_Collision->SetLocalScale({ 20.f, 10.f });
	m_Collision->Disable();

	CreateAttackCollision((int)CollisionLayer::PlayerBullet, COLTYPE::AABB, &MonsterBullet::ColEnter, &MonsterBullet::ColStay, &MonsterBullet::ColExit, *this);
	m_AttackCollision->SetLocalPosition({ 0.f, 0.f });
	m_AttackCollision->SetLocalScale({ 20.f, 10.f });
	m_AttackCollision->Disable();

	m_XMoveRatio = 1.f;
	m_Render->SetRenderEnable(false);

	SetDirection(DIRECTION::RIGHT);
}

void MonsterBullet::Update()
{
	ObjectBase::Update();

	if (m_Death == true)
	{
		return;
	}

	Move(m_MoveVec * DELTATIME(800.f), true);

	float4 Color = CheckColMapColor(0, 0);

	if (Color == float4::RED
		|| Color == float4::GREEN)
	{
		m_Death = true;
		m_Render->SetRenderEnable(false);
		m_Collision->Disable();
	}
}

void MonsterBullet::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Player)
	{
		float4 TargetPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();
		float4 TargetScale = _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetWorldScale();

		m_Death = true;
		m_Render->SetRenderEnable(false);
		m_Collision->Disable();

		float Angle = m_Render->GetLocalRotation().z;

		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition() + _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalPosition();
		CreateEffect<SlashFlow>(L"Player_SlashFlow", float4::ZERO, 1.f, false, Angle, OtherPos, m_Dir);
	}

	if (_Other->GetOrder() == (int)CollisionLayer::PlayerSlash)
	{
		m_MoveVec *= -1.f;
		m_Collision->Disable();
		m_AttackCollision->Enable();
	}

	if (_this->GetOrder() == (int)CollisionLayer::PlayerBullet
		&& _Other->GetOrder() == (int)CollisionLayer::Monster)
	{
		PlayHitSound();

		m_Death = true;
		m_Render->SetRenderEnable(false);
		m_AttackCollision->Disable();

		float Angle = m_Render->GetLocalRotation().z;
		Angle -= 180.f;

		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition() + _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalPosition();
		CreateEffect<SlashFlow>(L"Player_SlashFlow", float4::ZERO, 1.f, false, Angle, OtherPos, m_Dir);
	}

	if (_this->GetOrder() == (int)CollisionLayer::PlayerBullet
		&& _Other->GetOrder() == (int)CollisionLayer::MonsterShield)
	{
		SOUNDPLAY(L"ShieldHit.wav");
		CamShake(0.25f, 5);

		m_Death = true;
		m_Render->SetRenderEnable(false);
		m_AttackCollision->Disable();
	}
}

void MonsterBullet::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void MonsterBullet::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void MonsterBullet::Fire(float4 _Pos, float _Angle)
{
	CamShake(0.1f, 1);

	m_Trans->SetWorldPosition(_Pos);

	m_Render->SetLocalRotationZ(_Angle);
	m_Render->SetRenderEnable(true);

	m_Collision->Enable();
	m_Collision->SetLocalRotation({ 0.f, 0.f, _Angle });
	m_AttackCollision->Disable();
	m_AttackCollision->SetLocalRotation({ 0.f, 0.f, _Angle });

	m_MoveVec = float4::GlobalDirToDeg(_Angle);

	m_Angle = _Angle;
	m_Death = false;
}

void MonsterBullet::PlayHitSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"Death_Generic_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}