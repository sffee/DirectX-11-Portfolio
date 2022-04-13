#include "Grenade.h"
#include <GJHGameEngineRandom.h>
#include "InstanceObject.h"
#include "SpectrumCom.h"

Grenade::Grenade()
{

}

Grenade::~Grenade()
{

}

void Grenade::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	m_CheckPlatform = false;

	CreateAnimation(L"Attack", L"Grenade", 0.1f, true);
	ChangeAnimation(L"Attack");

	CreateCollision((int)CollisionLayer::HeadHunterGrenadeExplosion, COLTYPE::AABB, &Grenade::ColEnter, &Grenade::ColStay, &Grenade::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 0.f });
	m_Collision->SetLocalScale({ 120.f, 120.f });
	m_Collision->Disable();

	m_Render->SetRenderEnable(false);

	m_IsEnd = true;
	m_BoomStart = false;

	m_Spectrum = CreateComponent<SpectrumCom>(m_Render, m_RenderScale, GetActor(), 0.f, 0.3f, 0.4f, float4(1.f, 0.f, 0.f, 1.f), false);
	m_Spectrum->End();

	State.CreateState(L"Start", &Grenade::Start_Init, &Grenade::Start_State, nullptr, this);
	State.CreateState(L"Bounce", &Grenade::Bounce_Init, &Grenade::Bounce_State, nullptr, this);
	State.CreateState(L"ExpandRange", &Grenade::ExpandRange_Init, &Grenade::ExpandRange_State, nullptr, this);
	State.CreateState(L"BlinkRange", &Grenade::BlinkRange_Init, &Grenade::BlinkRange_State, nullptr, this);
	State.CreateState(L"Explosion", &Grenade::Explosion_Init, &Grenade::Explosion_State, nullptr, this);

	CreateRangeEffect();
	CreateExplosion();
}

void Grenade::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		m_GravityTemp = m_Gravity;

		ObjectBase::Update();

		if (m_IsEnd == true)
		{
			return;
		}

		State.Update();
	}
}

void Grenade::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Grenade::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Grenade::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Grenade::CreateRangeEffect()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Range";
	Data.AnimationFrameTime = 0.06f;
	Data.TextureName = L"Grenade_Range";
	Data.Dir = m_Dir;
	Data.AnimationLoop = true;
	Data.IsDeath = false;
	Data.IsRenderUpdate = false;
	Data.IsRecord = true;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_RangeEffect = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
	m_RangeEffect->FindComponent<GJHActorTransform>()->SetWorldPositionZ(1.f);
	m_RangeEffectRender = m_RangeEffect->GetActor()->FindComponent<GJHSpriteRenderer>();
	m_RangeEffectRender->SetRenderEnable(false);
	m_RangeEffectRender->SetLocalScale({ 0.f, 0.f });
}

void Grenade::CreateExplosion()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Explosion";
	Data.AnimationFrameTime = 0.06f;
	Data.TextureName = L"Explosion";
	Data.Dir = DIRECTION::RIGHT;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.AnimationEndHide = true;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_ExplosionList.resize(25);
	for (int i = 0; i < 25; ++i)
	{
		m_ExplosionList[i] = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
		m_ExplosionList[i]->FindComponent<GJHSpriteRenderer>()->SetRenderEnable(false);
	}
}

void Grenade::Explosion()
{
	float4 MyPos = m_Trans->GetWorldPosition();

	for (int i = 0; i < 25; ++i)
	{
		float4 Pos = MyPos + float4(GJHGameEngineRandom::RandomFloat(-50.f, 50.f), GJHGameEngineRandom::RandomFloat(-50.f, 50.f));
		m_ExplosionList[i]->Start(Pos);
	}

	SOUNDPLAY(L"HeadHunter_Explosion.wav");
}

void Grenade::Attack(DIRECTION _Dir, float4 _Pos, float _Gravity)
{
	MoveSpeedUniform();
	m_MoveSpeed = 500.f;
	SetDirection(_Dir);
	
	m_Trans->SetWorldPosition(_Pos);

	m_Animation->Reset();
	m_Render->SetRenderEnable(true);

	m_ApplyGravity = true;

	m_Time = 0.f;
	m_IsEnd = false;
	m_Bounce = false;
	m_BoomStart = false;
	m_BlinkRangeEffect = false;

	m_BoomStartWaitTime = GJHGameEngineRandom::RandomFloat(0.4f, 0.8f);

	Jump(_Gravity, 600.f);

	m_Spectrum->Start();

	m_RangeEffectScale = 1.f;
	m_RangeEffectRender->SetRenderEnable(true);
	m_RangeEffect->FindComponent<GJHSpriteAnimation>()->SetAnimationStop(true);
	m_RangeEffect->FindComponent<GJHSpriteAnimation>()->SetAnimationFrameIndex(1);
	m_RangeEffectRender->SetLocalScale({ 0.f, 0.f });

	for (int i = 0; i < 25; ++i)
	{
		m_ExplosionList[i]->FindComponent<GJHSpriteRenderer>()->SetRenderEnable(false);
	}

	State.ChangeState(L"Start");
}

void Grenade::Start_Init()
{

}

void Grenade::Start_State()
{
	if (CheckColMapColor(10 * (int)m_Dir, 0) == float4::GREEN
		|| CheckColMapColor(10 * (int)m_Dir, 0) == float4::RED)
	{
		ChangeDirection();
		m_Gravity = 0.f;
		m_MoveSpeed = GJHGameEngineRandom::RandomFloat(40.f, 300.f);

		State.ChangeState(L"Bounce");
		return;
	}

	if (CheckColMapColor(0, -1) == float4::RED)
	{
		m_GravityTemp = abs(m_GravityTemp) * -0.7f;
		Jump(m_GravityTemp, 600.f);

		m_MoveSpeed = GJHGameEngineRandom::RandomFloat(20.f, 50.f);
		State.ChangeState(L"Bounce");
		return;
	}
}

void Grenade::Bounce_Init()
{
	m_Time = 0.f;
}

void Grenade::Bounce_State()
{
	if (CheckColMapColor(0, -1) == float4::RED)
	{
		m_GravityTemp = abs(m_GravityTemp) * -0.7f;
		Jump(m_GravityTemp, 600.f);

		m_MoveSpeed = GJHGameEngineRandom::RandomFloat(20.f, 50.f);
	}

	m_Time += DELTATIME();
	if (m_BoomStartWaitTime <= m_Time)
	{
		MoveSpeedStop();
		m_ApplyGravity = false;

		m_Spectrum->End();

		State.ChangeState(L"ExpandRange");
	}
}

void Grenade::ExpandRange_Init()
{
	m_Time = 0.f;

	SOUNDPLAY(L"HeadHunter_BombBlink.wav");
}

void Grenade::ExpandRange_State()
{
	float Ratio = 1.f - min(m_RangeEffectScale / 150.f, 1.f);
	m_RangeEffectScale += DELTATIME(1500.f * Ratio);
	m_RangeEffectScale = (150.f <= m_RangeEffectScale) ? 150.f : m_RangeEffectScale;

	m_RangeEffectRender->SetLocalScale({ m_RangeEffectScale, m_RangeEffectScale, 0.f });

	m_Time += DELTATIME();
	if (0.5f <= m_Time)
	{
		m_Time = 0.f;

		m_RangeEffect->FindComponent<GJHSpriteAnimation>()->SetAnimationStop(false);
		State.ChangeState(L"BlinkRange");
	}
}

void Grenade::BlinkRange_Init()
{
	m_Time = 0.f;
}

void Grenade::BlinkRange_State()
{
	m_Time += DELTATIME();
	if (0.8f <= m_Time)
	{
		m_Collision->Enable();

		Explosion();

		m_Render->SetRenderEnable(false);
		m_RangeEffectRender->SetRenderEnable(false);

		State.ChangeState(L"Explosion");
	}
}

void Grenade::Explosion_Init()
{
	m_Time = 0.f;
}

void Grenade::Explosion_State()
{
	m_Time += DELTATIME();
	if (0.6f <= m_Time)
	{
		m_Collision->Disable();
		m_IsEnd = true;
	}
}