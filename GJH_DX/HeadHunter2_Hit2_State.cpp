#include "HeadHunter2.h"
#include "Player.h"
#include "LaserLine.h"

void HeadHunter2::Hit2_TeleportIn_Init()
{
	m_EnableGravity = false;
	SetDirection(DIRECTION::LEFT);

	m_Trans->SetWorldPosition(Hit2TeleportPos[m_TeleportCount++]);

	ChangeAnimation(L"TeleportIn");

	PlayAppearSound();
}

void HeadHunter2::Hit2_TeleportIn_State()
{
	if (AnimationEnd())
	{
		float4 Pos = m_Trans->GetWorldPosition();
		Pos += float4(0.f, 28.f);
		Pos += float4(3.f, -20.f);

		m_LaserLineList[m_LaserLineIndex]->Attack(m_Dir, 5.f);
		m_LaserLineList[m_LaserLineIndex]->Setting(Pos, -90.f);
		m_LaserLineList[m_LaserLineIndex]->SetMode(LaserLine::MODE::AUTOFIRE);

		AddLaserLineIndex();

		State.ChangeState(L"Hit2_TeleportOut");
	}
}

void HeadHunter2::Hit2_TeleportOut_Init()
{
	ChangeAnimation(L"TeleportOut");
}

void HeadHunter2::Hit2_TeleportOut_State()
{
	if (AnimationEnd())
	{
		if (Hit2TeleportPos.size() - 1 < m_TeleportCount)
		{
			State.ChangeState(L"Hit2_Left_TeleportIn_Sweep");
		}
		else
		{
			State.ChangeState(L"Hit2_TeleportIn");
		}
	}
}

void HeadHunter2::Hit2_Left_TeleportIn_Sweep_Init()
{
	m_TeleportCount = 0;

	m_Trans->SetWorldPosition(float4(90.f, -120.f));

	ChangeAnimation(L"TeleportIn_Sweep");

	PlayAppearSound();
}

void HeadHunter2::Hit2_Left_TeleportIn_Sweep_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit2_Left_Sweep");
	}
}

void HeadHunter2::Hit2_Left_Sweep_Init()
{
	m_Trans->SetWorldMove(float4(10.f, 0.f));

	m_SweepAngle = -60.f;
	m_Time = 0.f;
	m_Hit2LaserFire = false;

	float4 DirVec = float4::GlobalDirToDeg(m_SweepAngle);
	float4 Pos = m_Trans->GetWorldPosition();
	Pos += float4(-20.f, 28.f);
	Pos += DirVec * 20.f;

	m_LaserLineList[m_LaserLineIndex]->Attack(DIRECTION::LEFT, 6.f);
	m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
	m_LaserLineList[m_LaserLineIndex]->SetMode(LaserLine::MODE::NONE);

	ChangeAnimation(L"Sweep");

	SOUNDPLAY(L"HeadHunter_LaserRotation.wav");
}

void HeadHunter2::Hit2_Left_Sweep_State()
{
	m_Time += DELTATIME();
	if (0.25f <= m_Time)
	{
		if (m_Hit2LaserFire == false)
		{
			m_Hit2LaserFire = true;
			m_LaserLineList[m_LaserLineIndex]->LaserFire();
		}

		float4 DirVec = float4::GlobalDirToDeg(m_SweepAngle) * float4(-1.f, 1.f);

		float4 Pos = m_Trans->GetWorldPosition();
		Pos += float4(0.f, 28.f);
		Pos += DirVec * 20.f;

		m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
		m_LaserLineList[m_LaserLineIndex]->LaserSetting();

		if (m_SweepAngle <= -180.f)
		{
			m_LaserLineList[m_LaserLineIndex]->LaserFinish();

			if (AnimationEnd())
			{
				AddLaserLineIndex();

				State.ChangeState(L"Hit2_Left_TeleportOut_Sweep");
			}
		}

		m_SweepAngle -= DELTATIME(250.f);
		if (m_SweepAngle <= -180.f)
		{
			m_SweepAngle = -180.f;
		}
	}
}

void HeadHunter2::Hit2_Left_TeleportOut_Sweep_Init()
{
	m_Trans->SetWorldMove(float4(5.f, 2.f));

	ChangeAnimation(L"TeleportOut_Sweep");

	PlayAppearSound();
}

void HeadHunter2::Hit2_Left_TeleportOut_Sweep_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit2_Right_TeleportIn_Sweep");
	}
}

void HeadHunter2::Hit2_Right_TeleportIn_Sweep_Init()
{
	SetDirection(DIRECTION::RIGHT);

	m_Trans->SetWorldPosition(float4(510.f, -120.f));

	ChangeAnimation(L"TeleportIn_Sweep");

	SOUNDPLAY(L"HeadHunter_LaserRotation.wav");
}

void HeadHunter2::Hit2_Right_TeleportIn_Sweep_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit2_Right_Sweep");
	}
}

void HeadHunter2::Hit2_Right_Sweep_Init()
{
	m_Trans->SetWorldMove(float4(-10.f, 0.f));

	m_SweepAngle = -60.f;
	m_Time = 0.f;
	m_Hit2LaserFire = false;

	float4 DirVec = float4::GlobalDirToDeg(m_SweepAngle);
	float4 Pos = m_Trans->GetWorldPosition();
	Pos += float4(0.f, 28.f);
	Pos += DirVec * 20.f;

	m_LaserLineList[m_LaserLineIndex]->Attack(DIRECTION::RIGHT, 6.f);
	m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
	m_LaserLineList[m_LaserLineIndex]->SetMode(LaserLine::MODE::NONE);

	ChangeAnimation(L"Sweep");

	PlayAppearSound();
}

void HeadHunter2::Hit2_Right_Sweep_State()
{
	m_Time += DELTATIME();
	if (0.25f <= m_Time)
	{
		if (m_Hit2LaserFire == false)
		{
			m_Hit2LaserFire = true;
			m_LaserLineList[m_LaserLineIndex]->LaserFire();
		}

		float4 DirVec = float4::GlobalDirToDeg(m_SweepAngle) * float4(1.f, 1.f);

		float4 Pos = m_Trans->GetWorldPosition();
		Pos += float4(0.f, 28.f);
		Pos += DirVec * 20.f;

		m_LaserLineList[m_LaserLineIndex]->Setting(Pos, m_SweepAngle);
		m_LaserLineList[m_LaserLineIndex]->LaserSetting();

		if (m_SweepAngle <= -180.f)
		{
			m_LaserLineList[m_LaserLineIndex]->LaserFinish();

			if (AnimationEnd())
			{
				AddLaserLineIndex();

				State.ChangeState(L"Hit2_Right_TeleportOut_Sweep");
			}
		}

		m_SweepAngle -= DELTATIME(250.f);
		if (m_SweepAngle <= -180.f)
		{
			m_SweepAngle = -180.f;
		}
	}
}

void HeadHunter2::Hit2_Right_TeleportOut_Sweep_Init()
{
	m_Trans->SetWorldMove(float4(-5.f, 2.f));

	ChangeAnimation(L"TeleportOut_Sweep");
}

void HeadHunter2::Hit2_Right_TeleportOut_Sweep_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit2_TeleportIn_Ground");
	}
}

void HeadHunter2::Hit2_TeleportIn_Ground_Init()
{
	m_Trans->SetWorldPosition({ 525.f, -280.f });

	SetDirection(DIRECTION::LEFT);
	ChangeAnimation(L"TeleportIn_Ground");
}

void HeadHunter2::Hit2_TeleportIn_Ground_State()
{
	if (AnimationEnd())
	{
		float4 Pos = m_Trans->GetWorldPosition();
		Pos += float4(-20.f, 23.f);

		m_LaserLineList[m_LaserLineIndex]->Attack(m_Dir, 1.5f);
		m_LaserLineList[m_LaserLineIndex]->Setting(Pos, 0.f);
		m_LaserLineList[m_LaserLineIndex]->SetMode(LaserLine::MODE::AUTOFIRE);

		AddLaserLineIndex();

		State.ChangeState(L"Hit2_TeleportOut_Ground");
	}
}

void HeadHunter2::Hit2_TeleportOut_Ground_Init()
{
	ChangeAnimation(L"TeleportOut_Ground");
}

void HeadHunter2::Hit2_TeleportOut_Ground_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Hit2_Wall_TeleportIn");
	}
}

void HeadHunter2::Hit2_Wall_TeleportIn_Init()
{
	SetDirection(DIRECTION::RIGHT);

	m_Trans->SetWorldPosition({ 77.f, -190.f });

	ChangeAnimation(L"TeleportIn");

	PlayAppearSound();
}

void HeadHunter2::Hit2_Wall_TeleportIn_State()
{
	if (AnimationEnd())
	{
		m_Collision->Enable();
		State.ChangeState(L"Wall_Idle");
	}
}