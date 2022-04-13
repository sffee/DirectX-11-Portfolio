#include "HeadHunter2.h"
#include "LaserLine.h"
#include "Player.h"
#include "KnifeDashLine.h"
#include "SpectrumCom.h"
#include <GJHSceneChangeRenderer.h>
#include <GJHClientGameEngine.h>

void HeadHunter2::Idle_Init()
{
	m_Time = 0.f;
	
	MoveSpeedStop();

	ChangeAnimation(L"Idle");
}

void HeadHunter2::Idle_State()
{
	LookPlayer();

	m_Time += DELTATIME();
	if (0.f <= m_Time)
	{
		if (m_NextPattern != "")
		{
			State.ChangeState(m_NextPattern);
			m_NextPattern = L"";
			return;
		}

		if (Escape())
		{
			return;
		}

		RandomPattern();
	}
}

void HeadHunter2::TakeOutRifle_Init()
{
	SOUNDPLAY(L"HeadHunter_TakeGun.wav");

	ChangeAnimation(L"TakeOutRifle");
}

void HeadHunter2::TakeOutRifle_State()
{
	if (AnimationEnd())
	{
		m_AimLaserCount = 0;
		State.ChangeState(L"Aim_Rifle");
	}
}

void HeadHunter2::Aim_Rifle_Init()
{
	m_Time = 0.f;
	m_AimFire = false;

	ChangeAnimation(L"Aim_Rifle");
	SetAnimationFrameIndex(9);

	m_LaserLineList[m_LaserLineIndex]->SetMode(LaserLine::MODE::NONE);
	m_LaserLineList[m_LaserLineIndex]->Attack(m_Dir);
	Aim();
}

void HeadHunter2::Aim_Rifle_State()
{
	if (m_AimFire == false)
	{
		Aim();

		m_Time += DELTATIME();
		if (0.4f <= m_Time)
		{
			m_AimFire = true;
			m_Time = 0.f;
		}
	}
	else
	{
		m_Time += DELTATIME();
		if (0.2f <= m_Time)
		{
			m_LaserLineList[m_LaserLineIndex]->LaserFire();
			m_LaserLineList[m_LaserLineIndex]->LaserSetting();
			m_LaserLineList[m_LaserLineIndex]->LaserFinish();

			AddLaserLineIndex();

			if (m_AimLaserCount < 2)
			{
				m_AimLaserCount++;
				State.ChangeState(L"Aim_Rifle_Cooltime");
			}
			else
			{
				State.ChangeState(L"PutBackRifle");
			}
		}
	}
}

void HeadHunter2::Aim_Rifle_Cooltime_Init()
{
	m_Time = 0.f;
}

void HeadHunter2::Aim_Rifle_Cooltime_State()
{
	if (Escape())
	{
		return;
	}

	m_Time += DELTATIME();
	if (0.3f <= m_Time)
	{
		State.ChangeState(L"Aim_Rifle");
	}
}

void HeadHunter2::PutBackRifle_Init()
{
	ChangeAnimation(L"PutBackRifle");
}

void HeadHunter2::PutBackRifle_State()
{
	if (Escape())
	{
		return;
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void HeadHunter2::PreDash_Init()
{
	float Angle = AngleToPlayer(m_Dir);
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	if (ceil(PlayerPos.y) < ceil(MyPos.y))
	{
		Angle -= 270.f;
	}

	if (20.f <= Angle)
	{
		Angle = 20.f;
	}

	float4 Pos = m_Trans->GetWorldPosition();
	Pos += float4(0.f, 15.f);
	Pos += m_DashMoveVec * float4((float)m_Dir, 1.f) * 20.f;

	m_KnifeDashLine->Attack(m_Dir, 0.5f, 6.f);
	m_KnifeDashLine->Setting(Pos, Angle);

	m_DashMoveVec = float4::GlobalDirToDeg(Angle);
	if (m_Dir == DIRECTION::LEFT)
	{
		m_DashMoveVec.x *= -1.f;
	}

	if ((m_Dir == DIRECTION::LEFT && 0.f < m_DashMoveVec.x)
		|| (m_Dir == DIRECTION::RIGHT && m_DashMoveVec.x <= 0.f))
	{
		m_DashMoveVec.x = (float)m_Dir;
		m_DashMoveVec.y = 0.f;

		Angle = 0.f;
	}

	m_Time = 0.f;

	ChangeAnimation(L"PreDash");

	SOUNDPLAY(L"HeadHunter_TakeKnife.wav");
}

void HeadHunter2::PreDash_State()
{
	m_Time += DELTATIME();
	if (1.f <= m_Time)
	{
		State.ChangeState(L"Dash");
	}
}

void HeadHunter2::Dash_Init()
{
	m_Collision->Disable();
	m_AttackCollision->Enable();

	m_Spectrum->SetColor({ 1.f, 0.f, 0.f, 1.f });
	m_Spectrum->SetCreateTime(0.f);
	m_Spectrum->SetMaxAlpha(1.f);
	m_Spectrum->Start();

	m_EnableGravity = false;

	m_DashMoveVec.x *= (float)m_Dir;

	MoveSpeedStop();
	m_XMoveRatio = 1.f;

	ChangeAnimation(L"Dash");

	SOUNDPLAY(L"HeadHunter_Dash.wav");
}

void HeadHunter2::Dash_State()
{
	Move(m_DashMoveVec * DELTATIME(2000.f), true);

	float4 MyPos = m_Trans->GetWorldPosition();
	bool WallCheck = false;
	if (560.f <= MyPos.x)
	{
		m_Trans->SetWorldPosition(float4(560.f, MyPos.y));
		WallCheck = true;
	}
	else if (MyPos.x <= 80.f)
	{
		m_Trans->SetWorldPosition(float4(80.f, MyPos.y));
		WallCheck = true;
	}

	if (WallCheck == true)
	{
		if (m_DashMoveVec.y <= 0.003f)
		{
			while (CheckColMapColor(0, -1) != GJHGameEngineVector::RED)
			{
				m_Trans->SetWorldMove({ 0.f, -DELTATIME(2.f) });
			}

			State.ChangeState(L"Dash_End_Ground");
		}
		else
		{
			Jump(0.f, 1000.f);
			ChangeDirection();
			State.ChangeState(L"Wall_Idle");
		}
	}
}

void HeadHunter2::Dash_Leave()
{
	m_Collision->Enable();
	m_AttackCollision->Disable();

	m_Spectrum->SetColor({ 1.f, 0.f, 1.f, 1.f });
	m_Spectrum->SetCreateTime(0.01f);
	m_Spectrum->SetMaxAlpha(0.2f);
	m_Spectrum->End();
}

void HeadHunter2::Dash_End_Ground_Init()
{
	ChangeAnimation(L"Dash_End_Ground");
}

void HeadHunter2::Dash_End_Ground_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void HeadHunter2::TakeOutGun_Init()
{
	m_Trans->SetWorldMove({ 19.5f * (float)m_Dir, 0.f });
	m_Collision->SetWorldMove({ 19.5f * (float)m_Dir, 0.f });

	m_GrenadeCount = 0;

	ChangeAnimation(L"TakeOutGun");

	SOUNDPLAY(L"HeadHunter_TakeGun.wav");
}

void HeadHunter2::TakeOutGun_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Shoot");
	}
}

void HeadHunter2::Shoot_Init()
{
	CreateGrenade();
	m_GrenadeCount++;

	ChangeAnimation(L"Shoot");

	SOUNDPLAY(L"HeadHunter_BombShot.wav");
}

void HeadHunter2::Shoot_State()
{
	if (AnimationEnd())
	{
		if (2 < m_GrenadeCount)
		{
			State.ChangeState(L"PutBackGun");
			return;
		}

		State.ChangeState(L"Shoot_Cooltime");
	}
}

void HeadHunter2::Shoot_Cooltime_Init()
{
	m_Time = 0.f;
}

void HeadHunter2::Shoot_Cooltime_State()
{
	if (Escape())
	{
		return;
	}

	m_Time += DELTATIME();
	if (0.4f <= m_Time)
	{
		State.ChangeState(L"Shoot");
	}
}

void HeadHunter2::PutBackGun_Init()
{
	ChangeAnimation(L"PutBackGun");
}

void HeadHunter2::PutBackGun_State()
{
	if (AnimationEnd())
	{
		m_Trans->SetWorldMove({ -19.5f * (float)m_Dir, 0.f });
		m_Collision->SetWorldMove({ -19.5f * (float)m_Dir, 0.f });
		State.ChangeState(L"Idle");
	}
}

void HeadHunter2::DodgeRoll_Init()
{
	m_Collision->Disable();
	m_Spectrum->Start();

	m_MoveSpeed = 300.f;
	MoveSpeedDownStart(0.002f, 1.f);

	ChangeAnimation(L"DodgeRoll");

	SOUNDPLAY(L"HeadHunter_Jump.wav");
}

void HeadHunter2::DodgeRoll_State()
{
	if (AnimationEnd())
	{
		m_Collision->Enable();
		m_Spectrum->End();

		m_DodgeCooltime = 3.f;
		State.ChangeState(L"Idle");
	}
}

void HeadHunter2::PreJump_Init()
{
	ChangeAnimation(L"PreJump");
}

void HeadHunter2::PreJump_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Jump");
	}
}

void HeadHunter2::Jump_Init()
{
	m_Spectrum->Start();

	float4 MyPos = m_Trans->GetWorldPosition();
	if (MyPos.x <= 322.f)
	{
		SetDirection(DIRECTION::RIGHT);
	}
	else
	{
		SetDirection(DIRECTION::LEFT);
	}

	MoveSpeedUniform();
	m_MoveSpeed = -300.f;
	Jump(-400.f, 800.f);
	
	ChangeAnimation(L"Jump");

	SOUNDPLAY(L"HeadHunter_Jump.wav");
}

void HeadHunter2::Jump_State()
{
	if (CheckColMapColor((int)(m_Render->GetLocalScale().x * -0.5f), 0) == float4::GREEN)
	{
		State.ChangeState(L"Wall_Idle");
	}
}

void HeadHunter2::Wall_Idle_Init()
{
	m_Spectrum->Start();

	MoveSpeedStop();
	m_EnableGravity = false;
	m_Time = 0.f;

	ChangeAnimation(L"Wall_Idle");
}

void HeadHunter2::Wall_Idle_State()
{
	m_Time += DELTATIME();
	if (0.2f <= m_Time)
	{
		State.ChangeState(L"Wall_Jump");
	}
}

void HeadHunter2::Wall_Jump_Init()
{
	MoveSpeedUniform();
	m_MoveSpeed = 300.f;
	Jump(-400.f, 800.f);

	m_CheckWall = false;

	m_JumpBulletAngle = (m_Dir == DIRECTION::LEFT) ? 180.f : 0.f;
	m_JumpBulletTime = 0.05f;
	m_BulletIndex = 0;

	ChangeAnimation(L"Wall_Jump");
	m_Animation->SetAnimationFrameTime(0, 0.2f);

	SOUNDPLAY(L"HeadHunter_Jump.wav");

	m_SoundPlay = false;
}

void HeadHunter2::Wall_Jump_State()
{
	if (1 <= GetAnimationFrameIndex())
	{
		if (m_SoundPlay == false)
		{
			m_SoundPlay = true;
			SOUNDPLAY(L"HeadHunter_Gatling.wav");
		}

		m_JumpBulletTime += DELTATIME();
		if (0.02f <= m_JumpBulletTime
			&& m_BulletIndex < 18)
		{
			m_JumpBulletTime = 0.f;
			CreateBullet(m_JumpBulletAngle);
			m_JumpBulletAngle += (m_Dir == DIRECTION::LEFT) ? 10.f : -10.f;
		}
	}

	if (m_IsJump == false)
	{
		m_Spectrum->End();

		m_CheckWall = true;
		State.ChangeState(L"Wall_Jump_Land");
	}
}

void HeadHunter2::Wall_Jump_Land_Init()
{
	MoveSpeedDownStart(0.05f, 1.f);
	m_MoveSpeed = 200.f;

	ChangeAnimation(L"Wall_Jump_Land");
}

void HeadHunter2::Wall_Jump_Land_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void HeadHunter2::Hurt_Init()
{
	m_Collision->Disable();

	m_HurtCount++;
	
	MoveSpeedUniform();
	m_MoveSpeed = -100.f;

	Jump(-250.f, 800.f);

	m_Spectrum->End();

	ChangeAnimation(L"Hurt");
	m_Animation->SetAnimationFrameTime(2, 10000.f);

	PlayHurtSound();
}

void HeadHunter2::Hurt_State()
{
	if (GetAnimationFrameIndex() == 2
		&& CheckColMapColor(0, -1) == float4::RED)
	{
		if (CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * -0.5f * (float)m_Dir), 0) != float4::GREEN)
		{
			MoveSpeedDownStart();
		}
		m_Animation->SetAnimationFrameIndex(3);
	}

	if (CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * -0.5f * (float)m_Dir), 0) == float4::GREEN)
	{
		MoveSpeedStop();
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Hurt_Invisible");
	}
}

void HeadHunter2::Hurt_Invisible_Init()
{
	CreateHurtEffect();
	m_Time = 0.f;

	ChangeAnimation(L"Invisible");
}

void HeadHunter2::Hurt_Invisible_State()
{
	m_Time += DELTATIME();
	if (0.3f <= m_Time)
	{
		if (m_HurtCount == 1)
		{
			State.ChangeState(L"Hit1_TeleportIn_Sweep");
		}
		else if (2 <= m_HurtCount)
		{
			State.ChangeState(L"Hit2_TeleportIn");
		}
	}
}

void HeadHunter2::DieFly_Init()
{
	m_Collision->Disable();

	MoveSpeedUniform();
	m_MoveSpeed = 200.f;

	Jump(-250.f, 800.f);
	
	m_Spectrum->End();

	ChangeAnimation(L"DieFly");

	PlayHurtSound();
	GJHGameEngineSound::FadeOut(L"BGM", 6.f);
	SOUNDPLAY(L"HeadHunter_Explosion.wav");
}

void HeadHunter2::DieFly_State()
{
	GJHGameEngineTime::SetTimeScale(0.4f);

	if (CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * 0.5f * (float)m_Dir), 0) == float4::GREEN)
	{
		MoveSpeedStop();
	}

	if (m_IsJump == false)
	{
		MoveSpeedDownStart(0.02f, 1.f);
		State.ChangeState(L"DieLand");
	}
}

void HeadHunter2::DieLand_Init()
{
	ChangeAnimation(L"DieLand");

	m_Time = 0.f;
	m_SceneEnd = false;

	GJHGameEngineTime::SetTimeScale(0.3f);

	SOUNDPLAY(L"HeadHunter_FloorHit.wav");
}

void HeadHunter2::DieLand_State()
{
	if (CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * 0.5f * (float)m_Dir), 0) == float4::GREEN)
	{
		MoveSpeedStop();
	}

	if (AnimationEnd())
	{
		GJHGameEngineTime::SetTimeScale(1.f);
	}

	if (m_SceneEnd == false)
	{
		m_Time += DELTATIME(1.f, false);
		if (4.f <= m_Time)
		{
			SOUNDPLAY(L"Transition_Start.wav");
			GetScene()->CreateActor()->CreateComponent<GJHSceneChangeRenderer>(2, SceneChangeType::Start);
			m_SceneEnd = true;
			m_Time = 0.f;
		}
	}
	else
	{
		m_Time += DELTATIME(1.f, false);
		if (1.f <= m_Time)
		{
			GJHClientGameEngine::ChangeScene(L"EndingScene");
		}
	}
}