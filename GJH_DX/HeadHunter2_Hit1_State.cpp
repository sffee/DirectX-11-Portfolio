#include "HeadHunter2.h"
#include "Player.h"
#include "LaserLine.h"
#include "SpectrumCom.h"

void HeadHunter2::Hit1_TeleportIn_Sweep_Init()
{
	m_Trans->SetWorldPosition(float4(295.f, -130.f));
	m_EnableGravity = false;
	
	SetDirection(DIRECTION::RIGHT);

	ChangeAnimation(L"TeleportIn_Sweep");

	PlayAppearSound();
}

void HeadHunter2::Hit1_TeleportIn_Sweep_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit1_Sweep");
	}
}

void HeadHunter2::Hit1_Sweep_Init()
{
	m_Trans->SetWorldPosition(float4(285.f, -130.f));
	m_LaserLineList[m_LaserLineIndex]->SetDir(DIRECTION::RIGHT);
	m_LaserLineList[m_LaserLineIndex]->LaserFire();

	m_SweepAngle = 0.f;
	m_Time = 0.f;

	float4 Pos = m_Trans->GetWorldPosition();
	Pos += float4(0.f, 28.f);

	m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
	m_LaserLineList[m_LaserLineIndex]->LaserSetting();

	ChangeAnimation(L"Sweep");

	SOUNDPLAY(L"HeadHunter_Swipe.wav");
}

void HeadHunter2::Hit1_Sweep_State()
{
	if (-180.f <= m_SweepAngle)
	{
		float4 DirVec = float4::GlobalDirToDeg(m_SweepAngle);

		float4 Pos = m_Trans->GetWorldPosition();
		Pos += float4(0.f, 28.f);
		Pos += DirVec * 20.f;

		m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
		m_LaserLineList[m_LaserLineIndex]->LaserSetting();

		m_SweepAngle -= DELTATIME(250.f);
	}
	else
	{
		m_LaserLineList[m_LaserLineIndex]->LaserFinish();
	}

	if (AnimationEnd())
	{
		m_Time += DELTATIME();
		if (0.1f <= m_Time)
		{
			State.ChangeState(L"Hit1_Dash");
		}
	}
}

void HeadHunter2::Hit1_Dash_Init()
{
	m_Collision->Enable();
	m_AttackCollision->Enable();

	m_Spectrum->SetColor({ 1.f, 0.f, 0.f, 1.f });
	m_Spectrum->SetCreateTime(0.f);
	m_Spectrum->SetMaxAlpha(1.f);
	m_Spectrum->Start();

	ChangeDirection();

	m_Render->SetWorldRotationZ(90.f);

	ChangeAnimation(L"Dash");

	SOUNDPLAY(L"HeadHunter_Dash.wav");
}

void HeadHunter2::Hit1_Dash_State()
{
	Move(float4(0.f, -DELTATIME(2000.f)), false);

	if (CheckColMapColor(0, 0) == float4::RED)
	{
		State.ChangeState(L"Hit1_Dash_End_Ground");
	}
}

void HeadHunter2::Hit1_Dash_Leave()
{
	m_Spectrum->SetColor({ 1.f, 0.f, 1.f, 1.f });
	m_Spectrum->SetCreateTime(0.01f);
	m_Spectrum->SetMaxAlpha(0.2f);
	m_Spectrum->End();

	m_AttackCollision->Disable();
}

void HeadHunter2::Hit1_Dash_End_Ground_Init()
{
	m_Render->SetWorldRotationZ(0.f);

	ChangeAnimation(L"Dash_End_Ground");
}

void HeadHunter2::Hit1_Dash_End_Ground_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}