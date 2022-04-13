#include "Laser.h"
#include "Player.h"
#include <GJHGameEngineRandom.h>

Laser::Laser()
{

}

Laser::~Laser()
{

}

void Laser::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	m_ApplyGravity = false;
	m_IsRenderUpdate = false;
	m_CheckPlatform = false;
	m_IsFinish = true;

	CreateAnimation(L"Attack", L"HeadHunter_Laser", 0.04f, true);
	ChangeAnimation(L"Attack");

	m_Render->SetMesh(L"RayCenter");

	m_Render->SetRenderEnable(false);

	m_IsEnd = true;
}

void Laser::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		ObjectBase::Update();

		if (m_IsEnd == false)
		{
			if (CheckPlayerCol() == true)
			{
				m_Player->LaserDamage(GetActor());
			}
		}

		if (m_IsEnd == true
			|| m_IsFinish == false)
		{
			return;
		}

		m_RenderYSize -= DELTATIME(50.f);
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

bool Laser::CheckPlayerCol()
{
	bool PlayerColCheck = m_Player->GetActor()->FindComponent<GJHCollisionComponent>()->GetColCheck();
	if (PlayerColCheck == false)
	{
		return false;
	}

	float4 PlayerColPos = m_Player->GetActor()->FindComponent<GJHCollisionComponent>()->GetWorldPosition();
	float4 PlayerColScale = m_Player->GetActor()->FindComponent<GJHCollisionComponent>()->GetWorldScale();

	float Angle = m_Angle;
	if (m_Dir == DIRECTION::LEFT)
	{
		Angle += 180.f;
		Angle *= -1.f;
	}

	if (m_Ray->CheckActorColToAngle(m_Trans->GetWorldPosition(), PlayerColPos, PlayerColScale, Angle, (int)m_Dir) == true)
	{
		return true;
	}

	return false;
}

void Laser::Setting(float4 _Pos, float _Angle)
{
	m_Trans->SetWorldPosition(_Pos);

	m_Render->SetWorldRotationZ(_Angle);

	m_Angle = _Angle;
}

void Laser::Fire(DIRECTION _Dir)
{
	PlayLaserShotSound();

	CamShake(0.15f, 2);

	m_RenderYSize = 13.f;
	m_Render->SetWorldScaleY(m_RenderYSize);

	m_Trans->SetWorldScaleX((float)_Dir);
	SetDirection(_Dir);

	m_Animation->Reset();
	m_Render->SetRenderEnable(true);

	m_IsEnd = false;
	m_IsFinish = false;
}

void Laser::Finish()
{
	m_IsFinish = true;
}

void Laser::PlayLaserShotSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"HeadHunter_LaserShot_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}