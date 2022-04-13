#include "SpectrumCom.h"
#include "Spectrum.h"

SpectrumCom::SpectrumCom() :
	m_CreateTime(0),
	m_IsStart(false),
	m_RenderScale(),
	m_IsPlayer(false),
	m_ParentActor(nullptr)
{

}

SpectrumCom::~SpectrumCom()
{

}

void SpectrumCom::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, float _CreateTime, float _LifeTime, float _MaxAlpha, float4 _Color, bool _OnlyColor)
{
	m_ParentRender = _Renderer;
	m_RenderScale = _RenderScale;
	m_ParentActor = _ParentActor;
	m_CreateTime = _CreateTime;
	m_LifeTime = _LifeTime;
	m_MaxAlpha = _MaxAlpha;
	m_Color = _Color;
	m_OnlyColor = _OnlyColor;
	m_TransformZ = 1.f;
	m_ActorType = ACTORTYPE::EFFECT;

	m_Time = 0.f;
}

void SpectrumCom::Update()
{
	if (m_IsStart == false
		|| GJHRecordComponent::m_RewindOn == true)
	{
		return;
	}

	m_Time += DELTATIME();
	if (m_CreateTime < m_Time)
	{
		m_Time = 0.f;

		float4 ParentRenderScale = m_ParentRender->GetWorldScale();
		int ParentDir = (0 < ParentRenderScale.x) ? 1 : -1;

		CreateActor<Spectrum>(m_ActorType, m_ParentRender->GetTexture(), m_ParentActor->FindComponent<GJHActorTransform>()->GetWorldPosition(), m_RenderScale, m_ParentActor, ParentDir, m_LifeTime, m_MaxAlpha, m_Color, m_OnlyColor, m_TransformZ, m_IsPlayer);
	}
}

void SpectrumCom::Start()
{
	m_IsStart = true;
	m_Time = 0.f;
}

void SpectrumCom::End()
{
	m_IsStart = false;
}