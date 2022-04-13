#include "KnifeDashLine.h"

KnifeDashLine::KnifeDashLine()
{

}

KnifeDashLine::~KnifeDashLine()
{

}

void KnifeDashLine::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	m_ApplyGravity = false;
	m_IsRenderUpdate = false;
	m_CheckPlatform = false;

	CreateAnimation(L"Attack", L"KnifeDashLine", 0.1f, false);
	ChangeAnimation(L"Attack");
	m_Animation->SetAnimationFrameTime(0, 0.01f);

	m_Render->SetMesh(L"RayCenter");

	m_Render->SetWorldScaleX(1000.f);
	m_Render->SetRenderEnable(false);

	m_Angle = 0.f;

	m_IsEnd = true;
}

void KnifeDashLine::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		ObjectBase::Update();

		if (m_IsEnd == true)
		{
			return;
		}
		
		if (m_AddRenderSizeEnd == false)
		{
			m_RenderYSize += DELTATIME(m_RenderSizeDeltaTime);
			if (m_RenderYMaxSize <= m_RenderYSize)
			{
				m_RenderYSize = m_RenderYMaxSize;

				m_Time += DELTATIME();
				if (m_RenderSizeSubWaitTime <= m_Time)
				{
					m_AddRenderSizeEnd = true;
				}
			}

			m_Render->SetWorldScaleY(m_RenderYSize);
		}
		else
		{
			m_RenderYSize -= DELTATIME(m_RenderSizeDeltaTime);
			if (m_RenderYSize <= 0.f)
			{
				m_IsEnd = true;
				m_Render->SetRenderEnable(false);
			}
			else
			{
				m_Render->SetWorldScaleY(m_RenderYSize);
			}
		}
	}
}

void KnifeDashLine::Attack(DIRECTION _Dir, float _RenderSizeSubWaitTime, float _RenderSizeDeltaTime)
{
	m_RenderYSize = 0.f;
	m_RenderYMaxSize = 1.f;
	m_Time = 0.f;
	m_RenderSizeSubWaitTime = _RenderSizeSubWaitTime;
	m_AddRenderSizeEnd = false;

	m_Animation->Reset();
	m_Render->SetRenderEnable(true);
	m_Render->SetWorldScaleY(m_RenderYSize);
	m_RenderSizeDeltaTime = _RenderSizeDeltaTime;
	m_IsEnd = false;

	m_Trans->SetWorldScaleX((float)_Dir);
	SetDirection(_Dir);
}

void KnifeDashLine::Setting(float4 _Pos, float _Angle)
{
	m_Angle = _Angle;
	m_Trans->SetWorldPosition(_Pos);
	m_Render->SetWorldRotationZ(_Angle + 0.3f);
}