#include "SlashFlow.h"
#include <TypeComponent.h>

SlashFlow::SlashFlow() :
	m_Start(false),
	m_ColorType(ColorType::MAZENTA)
{

}

SlashFlow::~SlashFlow()
{

}

void SlashFlow::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, float _Angle, float4 _TargetPos, DIRECTION _Dir)
{
	SetDirection(DIRECTION::RIGHT);
	m_MoveDir = _Dir;
	m_TargetPos = _TargetPos;

	EffectBase::Start(_Renderer, _RenderScale);

	m_ApplyGravity = false;
	m_ParentActor = _ParentActor;

	m_IsRecord = false;
	m_Angle = _Angle;

	CreateAnimation(L"Attack", L"Player_SlashFlow", 0.05f);
	ChangeAnimation(L"Attack");

	GetActor()->FindComponent<TypeComponent>()->SetActorType(ACTORTYPE::PLAYER);
}

void SlashFlow::Update()
{
	float4 MoveVec = float4::GlobalDirToDeg(m_Angle) * (float)m_MoveDir;

	if (m_Start == false)
	{
		float4 ParentPos = m_ParentActor->FindComponent<GJHActorTransform>()->GetWorldPosition();

		float ParentXScale = m_ParentActor->FindComponent<GJHSpriteRenderer>()->GetWorldScale().x;
		DIRECTION Dir = (ParentXScale < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;

		float ParentZRotation = m_ParentActor->FindComponent<GJHSpriteRenderer>()->GetLocalRotation().z;
		if (Dir == DIRECTION::LEFT)
		{
			ParentZRotation += 180.f;
		}

		m_Render->SetColor({ 1.f, 0.f, 1.f, 0.7f });

		m_Trans->SetWorldPosition({ m_TargetPos.x, m_TargetPos.y, 1.f });
		m_Trans->SetWorldMove(MoveVec * -300.f);
		m_Render->SetLocalRotationZ(ParentZRotation);

		m_Start = true;

		m_XMoveRatio = 1.f;
		m_DeathTime = 1.f;
		m_ColorChangeTime = 0.08f;
	}

	Move(MoveVec * DELTATIME(2500.f, false), true);

	ObjectBase::Update();

	m_ColorChangeTime -= DELTATIME(1.f, false);
	if (m_ColorChangeTime < 0.f)
	{
		m_ColorChangeTime = 0.08f;
		
		switch (m_ColorType)
		{
		case ColorType::MAZENTA:
			m_Render->SetColor({ 0.f, 1.f, 1.f, 0.7f });
			m_ColorType = ColorType::CYAN;
			break;
		case ColorType::CYAN:
			m_Render->SetColor({ 1.f, 0.f, 1.f, 0.7f });
			m_ColorType = ColorType::MAZENTA;
			break;
		default:
			break;
		}
	}

	m_DeathTime -= DELTATIME(1.f, false);
	if (m_DeathTime < 0.f)
	{
		GetActor()->Death();
	}
}