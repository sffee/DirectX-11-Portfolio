#include "Spectrum.h"

Spectrum::Spectrum() :
	m_Time(0)
{

}

Spectrum::~Spectrum()
{

}

void Spectrum::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, int _Dir, float _LifeTime, float _MaxAlpha, float4 m_Color, bool _OnlyColor, float m_TransformZ, bool _IsPlayer)
{
	m_Render = _Renderer;
	m_Render->SetColor(m_Color);
	m_Render->SetOnlyColor(_OnlyColor);

	m_RenderScale = _RenderScale;
	m_Dir = static_cast<DIRECTION>(_Dir);
	m_Trans = GetActorTrans();
	m_LifeTime = _LifeTime;
	m_MaxAlpha = _MaxAlpha;
	
	m_ApplyGravity = false;
	
	float4 ParentWorldPos = _ParentActor->FindComponent<GJHActorTransform>()->GetWorldPosition();
	m_Trans->SetWorldPosition(float4(ParentWorldPos.x, ParentWorldPos.y, ParentWorldPos.z + m_TransformZ));

	m_IsPlayer = _IsPlayer;
}

void Spectrum::Update()
{
	ObjectBase::RenderUpdate();

	m_Time += DELTATIME();
	if (m_LifeTime < m_Time)
	{
		GetActor()->Death();
	}

	float Alpha = (1.f - (m_Time / m_LifeTime)) * m_MaxAlpha;
	m_Render->SetAlpha(Alpha);

	if (m_IsPlayer == true)
	{
		if (0.15f <= m_Time)
		{
			float4 Color = m_Render->GetColor();
			Color.x += DELTATIME(20.f);
			Color.y -= DELTATIME(20.f);

			m_Render->SetColor(Color);
		}
	}
}