#include "LaserLine.h"
#include "Laser.h"

LaserLine::LaserLine()
{

}

LaserLine::~LaserLine()
{

}

void LaserLine::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	m_ApplyGravity = false;
	m_IsRenderUpdate = false;
	m_CheckPlatform = false;

	CreateAnimation(L"Attack", L"Bullet", 0.1f, true);
	ChangeAnimation(L"Attack");
	m_Animation->SetAnimationFrameTime(0, 0.01f);

	m_Render->SetMesh(L"RayCenter");

	m_Render->SetWorldScaleX(1000.f);
	m_Render->SetRenderEnable(false);

	m_Angle = 0.f;

	m_Laser = CreateActor<Laser>(ACTORTYPE::EFFECT, L"HeadHunter_Laser", { 0.f, 0.f }, 1.f)->FindComponent<Laser>();

	m_IsEnd = true;
	m_Mode = MODE::NONE;
}

void LaserLine::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		ObjectBase::Update();

		if (m_IsEnd == true)
		{
			return;
		}

		m_RenderYSize -= DELTATIME(m_RenderSizeDeltaTime);
		if (m_RenderYSize <= 0.f)
		{
			if (m_Mode == MODE::AUTOFIRE)
			{
				LaserSetting();
				LaserFire();
				LaserFinish();
			}

			m_IsEnd = true;
			m_Render->SetRenderEnable(false);
		}
		else
		{
			m_Render->SetWorldScaleY(m_RenderYSize);
		}
	}
}

void LaserLine::Attack(DIRECTION _Dir, float _RenderSizeDeltaTime)
{
	SOUNDPLAY(L"HeadHunter_LockOn.wav");

	m_RenderYSize = 1.5f;
	m_Animation->Reset();
	m_Render->SetRenderEnable(true);
	m_Render->SetWorldScaleY(m_RenderYSize);
	m_RenderSizeDeltaTime = _RenderSizeDeltaTime;
	m_IsEnd = false;

	SetDir(_Dir);
}

void LaserLine::SetDir(DIRECTION _Dir)
{
	m_Trans->SetWorldScaleX((float)_Dir);

	SetDirection(_Dir);
}

void LaserLine::Setting(float4 _Pos, float _Angle)
{
	m_Angle = _Angle;
	m_Trans->SetWorldPosition(_Pos);
	m_Render->SetWorldRotationZ(_Angle + 0.3f);
}

void LaserLine::LaserSetting()
{
	m_Laser->Setting(m_Trans->GetWorldPosition(), m_Angle);
}

void LaserLine::LaserFire()
{
	m_Laser->Fire(m_Dir);
}

void LaserLine::LaserFinish()
{
	m_Laser->Finish();
}