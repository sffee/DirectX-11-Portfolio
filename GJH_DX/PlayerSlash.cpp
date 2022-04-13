#include "PlayerSlash.h"
#include "SlashFlow.h"
#include "Player.h"
#include <GJHRenderer.h>
#include <GJHGameEngineScene.h>
#include "InstanceObject.h"
#include <GJHGameEngineRandom.h>

PlayerSlash::PlayerSlash()
{

}

PlayerSlash::~PlayerSlash()
{

}

void PlayerSlash::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor)
{
	EffectBase::Start(_Renderer, _RenderScale);

	m_ApplyGravity = false;

	m_ParentActor = _ParentActor;
	m_ParentRender = m_ParentActor->FindComponent<GJHRenderer>();

	CreateAnimation(L"Attack", L"Player_Slash", 0.05f);
	ChangeAnimation(L"Attack");

	SetAnimationFrameIndex(5);

	CreateCollision((int)CollisionLayer::PlayerSlash, COLTYPE::OBB, &PlayerSlash::ColEnter, &PlayerSlash::ColStay, &PlayerSlash::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 85.f, 30.f });
	m_Collision->SetDebugRenderColor({ 0.f, 0.f, 1.f, 1.f });
	m_Collision->SetDebugRenderEnable(false);
	m_Collision->Enable();
}

void PlayerSlash::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		ObjectBase::Update();

		if (m_IsStopTime == false)
		{
			if (GetAnimationFrameIndex() < 5)
			{
				m_Collision->Enable();
			}
			else
			{
				m_Collision->Disable();
			}
		}
		else
		{
			m_Collision->Disable();
		}
	}
}

void PlayerSlash::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Monster
		|| _Other->GetOrder() == (int)CollisionLayer::HeadHunter)
	{
		PlayHitSound();

		CamShake(0.25f, 5);
		StopTime(0.f, 0.2f);

		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition() + _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalPosition();
		CreateEffect<SlashFlow>(L"Player_SlashFlow", float4::ZERO, 1.f, false, m_Angle, OtherPos, m_Dir);
	}

	if (_Other->GetOrder() == (int)CollisionLayer::MonsterBullet)
	{
		CamShake(0.25f, 3);

		float4 MyPos = m_Trans->GetWorldPosition();
		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();
		OtherPos += float4(10.f * (float)m_Dir, -15.f);
		CreateBulletReplectEffect(OtherPos);

		SOUNDPLAY(L"Slash_Bullet.wav");
	}

	if (_Other->GetOrder() == (int)CollisionLayer::MonsterShield)
	{
		float4 OtherScale = _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalScale();
		DIRECTION OtherDir = (OtherScale.x < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;

		if (OtherDir == m_Dir)
		{
			PlayHitSound();

			CamShake(0.25f, 5);
			StopTime(0.f, 0.2f);

			float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition() + _Other->GetActor()->FindComponent<GJHSpriteRenderer>()->GetLocalPosition();
			CreateEffect<SlashFlow>(L"Player_SlashFlow", float4::ZERO, 1.f, false, m_Angle, OtherPos, m_Dir);
		}
		else
		{
			CamShake(0.25f, 3);
			SOUNDPLAY(L"ShieldHit.wav");

			m_ParentActor->FindComponent<Player>()->KnockBack();
		}
	}
}

void PlayerSlash::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	
}

void PlayerSlash::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	
}

void PlayerSlash::AttackStart(DIRECTION _Dir, const float4& _CharPos, float _Angle)
{
	SetDirection(_Dir);
	m_Trans->SetWorldPosition(_CharPos);
	m_Render->SetLocalRotationZ(_Angle);

	m_Animation->Reset();

	m_Collision->Enable();
	m_Collision->SetLocalRotation({ 0.f, 0.f, _Angle });

	m_Angle = _Angle;
}

void PlayerSlash::RewindEnd()
{
	SetAnimationFrameIndex(5);
}

void PlayerSlash::CreateBulletReplectEffect(float4 _Pos)
{
	InstanceObjectData Data;
	Data.AnimationName = L"GunFire4";
	Data.AnimationFrameTime = 0.08f;
	Data.Pos = _Pos;
	Data.TextureName = L"GunFire4";
	Data.Dir = m_Dir;
	Data.ActorType = ACTORTYPE::PLAYER;
	Data.RenderScale = float4(0.7f, 0.7f);

	CreateInstanceObject(Data, false);
}

void PlayerSlash::PlayHitSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"Death_Generic_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}