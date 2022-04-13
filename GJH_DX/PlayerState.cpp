#include "Player.h"
#include <GJHGameEngineInput.h>
#include "PlayerSlash.h"
#include "SpectrumCom.h"
#include <GJHGameEngineWindow.h>

void Player::Idle_Init()
{
	m_IsJump = false;
	m_MoveSpeed = MOVESPEED;
	MoveSpeedStop();

	ChangeAnimation(L"Idle");
}

void Player::Idle_State()
{
	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		if (CheckColWall(5) == false)
		{
			State.ChangeState(L"MoveStart");
		}
	}

	if (KEYDOWN(L"Jump"))
	{
		m_XMoveRatio = 0.f;
		State.ChangeState(L"Jump");
	}

	if (KEYPRESS(L"Sit"))
	{
		State.ChangeState(L"SitStart");
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}
}

void Player::MoveStart_Init()
{
	m_DustCloudCount = 0;
	m_Time = 0.f;

	m_CheckMoveColSizeX = 2;

	ChangeAnimation(L"MoveStart");
}

void Player::MoveStart_State()
{
	if (m_DustCloudCount < 7)
	{
		m_Time += DELTATIME();
		if (0.04f <= m_Time)
		{
			m_Time = 0.f;
			DustCloud();
			m_DustCloudCount++;
		}
	}

	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		if (CheckColWall(3) == true)
		{
			State.ChangeState(L"MoveEnd");
		}
	}

	if (KEYFREE(L"LMove")
		&& KEYFREE(L"RMove"))
	{
		State.ChangeState(L"MoveEnd");
	}

	if (KEYDOWN(L"Jump"))
	{
		State.ChangeState(L"Jump");
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (KEYDOWN(L"Sit")
		|| KEYDOWN(L"Space"))
	{
		State.ChangeState(L"Roll");
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"Move");
	}
}

void Player::Move_Init()
{
	m_Attack = false;

	m_Time = 0.f;

	m_CheckMoveColSizeX = 2;

	MoveSpeedUpStart(0.3f);

	SOUNDPLAY(L"Player_PreRun.wav");

	ChangeAnimation(L"Move");
}

void Player::Move_State()
{
	if (m_DustCloudCount < 7)
	{
		m_Time += DELTATIME();
		if (0.04f <= m_Time)
		{
			m_Time = 0.f;
			DustCloud();
			m_DustCloudCount++;
		}
	}

	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		if (CheckColWall(3) == true)
		{
			State.ChangeState(L"MoveEnd");
		}
	}

	if (KEYDOWN(L"Jump"))
	{
		State.ChangeState(L"Jump");
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (KEYDOWN(L"Sit")
		|| KEYDOWN(L"Space"))
	{
		State.ChangeState(L"Roll");
	}

	if (KEYFREE(L"LMove")
		&& KEYFREE(L"RMove"))
	{
		State.ChangeState(L"MoveEnd");
	}
}

void Player::MoveEnd_Init()
{
	if (m_Attack == true)
	{
		MoveSpeedStop();
		m_Attack = false;
	}
	else
	{
		MoveSpeedDownStart(0.06f);
	}

	m_CheckMoveColSizeX = 2;

	ChangeAnimation(L"MoveEnd");
}

void Player::MoveEnd_State()
{
	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		if (CheckColWall(4) == false)
		{
			State.ChangeState(L"MoveStart");
		}
	}

	if (KEYDOWN(L"Jump"))
	{
		Jump(-300.f, 600.f);
		State.ChangeState(L"Jump");
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"Idle");
	}
}

void Player::Jump_Init()
{
	JumpCloud();

	m_CheckMoveColSizeX = 0;

	m_IsJump = true;
	m_IsFalling = false;

	if (m_KnockBack == false)
	{
		Jump(-300.f, 600.f);
	}

	ChangeAnimation(L"Jump");

	SOUNDPLAY(L"Player_Jump.wav");
}

void Player::Jump_State()
{
	{
		float4 RenderSize = m_Render->GetTexture()->GetSize();

		float4 UpPos = float4(18.f * (int)m_Dir, RenderSize.y);
		float4 DownPos = float4(UpPos.x, 0);

		if (CheckColMapColor(UpPos.ix(), UpPos.iy()) == GJHGameEngineVector::GREEN
			|| CheckColMapColor(DownPos.ix(), DownPos.iy()) == GJHGameEngineVector::GREEN)
		{
			m_IsJump = false;
			m_IsFalling = false;
			State.ChangeState(L"WallGrab");

			return;
		}
	}

	if (m_KnockBack == false
		&& KEYFREE(L"Jump"))
	{
		m_Gravity = -100.f;
		State.ChangeState(L"Fall");
		return;
	}

	if (m_KnockBack == true)
	{
		m_KnockBackMoveBlockTime -= DELTATIME();
	}

	if (m_KnockBack == false
		|| (m_KnockBack == true && m_KnockBackMoveBlockTime <= 0.f))
	{
		if (KEYPRESS(L"LMove")
			|| KEYPRESS(L"RMove"))
		{
			MoveSpeedUpStart(0.3f);
		}

		if (KEYFREE(L"LMove")
			&& KEYFREE(L"RMove"))
		{
			MoveSpeedDownStart(0.01f);
		}

		if (KEYDOWN(L"Attack"))
		{
			State.ChangeState(L"Attack");
		}
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"Fall");
	}

	if (m_IsJump == false)
	{
		if (KEYPRESS(L"LMove")
			|| KEYPRESS(L"RMove"))
		{
			State.ChangeState(L"Move");
		}
		else
		{
			State.ChangeState(L"MoveEnd");
		}
	}
}

void Player::Jump_Leave()
{
	m_KnockBack = false;
}

void Player::Fall_Init()
{
	m_DustCloudCount = 0;

	m_CheckMoveColSizeX = 0;

	m_IsJump = true;
	m_IsFalling = true;

	ChangeAnimation(L"Fall");
}

void Player::Fall_State()
{
	{
		float4 RenderSize = m_Render->GetTexture()->GetSize();

		float4 UpPos = float4(18.f * (int)m_Dir, RenderSize.y);
		float4 DownPos = float4(UpPos.x, 0);

		if (CheckColMapColor(UpPos.ix(), UpPos.iy()) == GJHGameEngineVector::GREEN
			|| CheckColMapColor(DownPos.ix(), DownPos.iy()) == GJHGameEngineVector::GREEN)
		{
			m_IsJump = false;
			m_IsFalling = false;
			m_Attack = false;
			State.ChangeState(L"WallGrab");

			return;
		}
	}

	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		MoveSpeedUpStart(0.3f);
	}

	if (KEYFREE(L"LMove")
		&& KEYFREE(L"RMove"))
	{
		MoveSpeedDownStart();
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (m_IsJump == false)
	{
		LandCloud();
		m_IsJump = false;

		if (KEYPRESS(L"LMove")
			|| KEYPRESS(L"RMove"))
		{
			m_Attack = false;
			SOUNDPLAY(L"Player_Land.wav");
			State.ChangeState(L"Move");
		}
		else
		{
			SOUNDPLAY(L"Player_Land.wav");
			State.ChangeState(L"MoveEnd");
		}
	}
}

void Player::SitStart_Init()
{
	ChangeAnimation(L"SitStart");
}

void Player::SitStart_State()
{
	if (KEYDOWN(L"Sit")
		|| KEYDOWN(L"Space"))
	{
		State.ChangeState(L"Roll");
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"SitLoop");
	}
}

void Player::SitLoop_Init()
{
	ChangeAnimation(L"SitLoop");
}

void Player::SitLoop_State()
{
	if (KEYPRESS(L"LMove") ||
		KEYPRESS(L"RMove"))
	{
		State.ChangeState(L"Roll");
	}

	if (KEYPRESS(L"Sit"))
	{
		if (m_OnPlatform == true)
		{
			m_OnPlatform = false;
			m_IgnorePlatform = true;
			Jump(50.f, 600.f, false);
			m_XMoveRatio = 0.f;
			State.ChangeState(L"Fall");
		}
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}
	
	if (KEYFREE(L"Sit"))
	{
		State.ChangeState(L"SitEnd");
	}
}

void Player::SitEnd_Init()
{
	ChangeAnimation(L"SitEnd");
}

void Player::SitEnd_State()
{
	if (KEYPRESS(L"LMove")
		|| KEYPRESS(L"RMove"))
	{
		if (CheckColWall(5) == false)
		{
			State.ChangeState(L"MoveStart");
		}
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"Idle");
	}
}

void Player::Roll_Init()
{
	m_Collision->Disable();

	m_Spectrum->Start();
	if (m_IsSlowMotion == false)
	{
		m_Spectrum->SetCreateTime(0.03f);
		m_Spectrum->SetMaxAlpha(0.25f);
	}

	ChangeAnimation(L"Roll");

	m_ChangeDirEnable = false;

	m_MoveSpeed = ROLL_MOVESPEED;
	MoveSpeedUpStart(0.2f, 0.4f);

	m_Time = 0.f;

	SOUNDPLAY(L"Player_Roll.wav");
}

void Player::Roll_State()
{
	m_Time += DELTATIME();
	if (0.02f <= m_Time)
	{
		m_Time = 0.f;
		DustCloud();
	}

	if (3 < GetAnimationFrameIndex())
	{
		MoveSpeedDownStart(0.02f);
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (AnimationEnd() == true)
	{
		if (KEYPRESS(L"Sit"))
		{
			State.ChangeState(L"SitLoop");
		}
		else if (KEYPRESS(L"LMove")
			|| KEYPRESS(L"RMove"))
		{
			MoveSpeedUpStart(1.f);
			m_DustCloudCount = 7;
			State.ChangeState(L"Move");
		}
		else
		{
			State.ChangeState(L"Idle");
		}
	}
}

void Player::Roll_Leave()
{
	m_Collision->Enable();

	m_MoveSpeed = MOVESPEED;

	m_ChangeDirEnable = true;

	if (m_IsSlowMotion == false)
	{
		m_Spectrum->End();
		m_Spectrum->SetCreateTime(0.05f);
		m_Spectrum->SetMaxAlpha(0.5f);
	}
}

void Player::Attack_Init()
{
	ChangeAnimation(L"Attack");
	MoveSpeedStop();
	m_XMoveRatio = 1.f;

	float4 MousePos = GetMousePos() + (m_CamTrans->GetWorldPosition() - float4(CAMSIZEX * 0.5f, CAMSIZEY * -0.5f));
	float4 Trans = m_Trans->GetWorldPosition();

	m_AttackMoveVec = (MousePos - Trans).GetNormalize();

	if (m_AttackMoveVec.x < 0)
	{
		SetDirection(DIRECTION::LEFT);
	}
	else
	{
		SetDirection(DIRECTION::RIGHT);
	}

	m_AttackMoveVec.x = abs(m_AttackMoveVec.x);
	if (m_Attack == true
		&& 0.f <= m_AttackMoveVec.y)
	{
		m_AttackMoveVec.y *= 0.1f;
	}

	m_ApplyGravity = false;
	m_AttackSlashEffect = false;
	m_ChangeDirEnable = false;
	m_Attack = true;

	m_Spectrum->Start();

}

void Player::Attack_State()
{
	{
		DIRECTION YDir = (0 < m_AttackMoveVec.y) ? DIRECTION::UP : DIRECTION::DOWN;

		if (CheckColUpDown(YDir) == false)
		{
			Move(m_AttackMoveVec * GJHGameEngineTime::FDeltaTime(ATTACK_MOVESPEED));
		}
		else
		{
			Move(float4(m_AttackMoveVec.x * GJHGameEngineTime::FDeltaTime(ATTACK_MOVESPEED), 0.f));
		}

		while (CheckColMapColor(0, 0) == GJHGameEngineVector::RED)
		{
			m_Trans->SetWorldMove(GJHGameEngineVector::UP * DELTATIME(2.f, false));
		}
	}

	if (1 <= GetAnimationFrameIndex()
		&& m_AttackSlashEffect == false)
	{
		m_AttackSlashEffect = true;
		PlaySlashSound();

		float4 MousePos = GetMousePos() + (m_CamTrans->GetWorldPosition() - float4(CAMSIZEX * 0.5f, CAMSIZEY * -0.5f));
		float4 Trans = m_Trans->GetWorldPosition();

		float Angle = float4::CalVectorAngle360((int)m_Dir, MousePos, Trans) * (float)m_Dir;

		m_SlashEffect->AttackStart(m_Dir, m_Trans->GetWorldPosition(), Angle);
	}

	if (AnimationEnd() == true)
	{
		State.ChangeState(L"Fall");
	}
}

void Player::Attack_Leave()	
{
	Jump(0.f, 600.f);
	m_ApplyGravity = true;
	m_ChangeDirEnable = true;

	MoveSpeedDownStart(0.002f, m_AttackMoveVec.x * 0.5f, 0.2f);

	if (m_IsSlowMotion == false)
	{
		m_Spectrum->End();
	}
}

void Player::WallGrab_Init()
{
	if (0 < m_Gravity)
	{
		m_Gravity /= 1.2f;
	}

	m_GravityAddValue /= 1.5f;

	m_IsWallGrab = true;
	m_ChangeDirEnable = false;

	m_Time = 0.f;
	m_WallSlideSoundTime = 1.f;

	MoveSpeedStop();
	m_XMoveRatio = 0.f;

	ChangeAnimation(L"WallGrab");
}

void Player::WallGrab_State()
{
	m_WallSlideSoundTime += DELTATIME(1.f, false);
	if (1.f <= m_WallSlideSoundTime)
	{
		m_WallSlideSoundTime = 0.f;
		SOUNDSTOP(L"WallSlide");
		SOUNDPLAY(L"WallSlide", L"WallSlide.wav");
	}

	m_Time += DELTATIME();
	if (0.02f <= m_Time)
	{
		WallCloud();
		m_Time = 0.f;
	}

	if (KEYDOWN(L"Jump"))
	{
		State.ChangeState(L"Flip");
		return;
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
		return;
	}

	float4 RenderSize = m_Render->GetTexture()->GetSize();
	float4 UpPos = float4(18.f * (int)m_Dir, RenderSize.y);
	float4 FrontPos = float4(UpPos.x, RenderSize.y * 0.5f);
	float4 DownPos = float4(UpPos.x, 0);

	if (CheckColMapColor(UpPos.ix(), UpPos.iy()) != GJHGameEngineVector::GREEN
		&& CheckColMapColor(FrontPos.ix(), FrontPos.iy()) != GJHGameEngineVector::GREEN
		&& CheckColMapColor(DownPos.ix(), DownPos.iy()) != GJHGameEngineVector::GREEN)
	{
		State.ChangeState(L"Fall");
		return;
	}

	float4 Color = CheckColMapColor(0, -1);
	if (0 <= m_Gravity
		&& (Color == GJHGameEngineVector::RED || Color == GJHGameEngineVector::BLUE))
	{
		State.ChangeState(L"MoveEnd");
	}
}

void Player::WallGrab_Leave()
{
	SOUNDSTOP(L"WallSlide");
	m_IsWallGrab = false;
	m_ChangeDirEnable = true;
}

void Player::DoorKick_Init()
{
	m_ChangeDirEnable = false;
	m_IsDoorKick = false;
	MoveSpeedStop();

	ChangeAnimation(L"DoorKick");
}

void Player::DoorKick_State()
{
	if (m_IsDoorKick == false
		&& GetAnimationFrameIndex() == 4)
	{
		m_IsDoorKick = true;
		m_DoorKickFunc(m_Dir);
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
	}
}

void Player::DoorKick_Leave()
{
	m_ChangeDirEnable = true;
}

void Player::HurtFlyStart_Init()
{
	m_Death = true;
	m_ChangeDirEnable = false;
	m_Collision->Disable();

	ChangeAnimation(L"HurtFlyStart");
}

void Player::HurtFlyStart_State()
{
	CheckDeathDoorPrevPos();
	MoveNearNormalPos();

	if (CheckColMapColor(0, 0) == float4::GREEN
		|| CheckColMapColor(0, 0) == float4::RED
		|| CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * -0.5f * (float)m_Dir), 0) == float4::GREEN
		|| CheckColMapColor((int)(m_Render->GetTexture()->GetSizeX() * -0.5f * (float)m_Dir), 0) == float4::RED)
	{
		MoveSpeedStop();
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"HurtFly");
	}
}

void Player::HurtFly_Init()
{
	ChangeAnimation(L"HurtFly");
}

void Player::HurtFly_State()
{
	CheckDeathDoorPrevPos();
	MoveNearNormalPos();

	while (CheckColMapColor(0, -1) == float4::BLUE)
	{
		MoveSpeedStop();
		m_EnableGravity = false;
		m_Trans->SetWorldMove({ float4::UP * DELTATIME(2.f) });
		State.ChangeState(L"HurtLand");
	}

	float4 RenderScale = m_Render->GetLocalScale();
	int RenderSizeX = (int)(RenderScale.x * 0.5f * (float)m_Dir);
	int RenderSizeY = (int)(RenderScale.y * 0.5f);

	if (CheckColMapColor(0, RenderSizeY) == float4::GREEN
		|| CheckColMapColor(0, RenderSizeY) == float4::RED
		|| CheckColMapColor(RenderSizeX, RenderSizeY) == float4::GREEN
		|| CheckColMapColor(RenderSizeX, RenderSizeY) == float4::RED)
	{
		MoveSpeedStop();
	}

	if (m_IsJump == false)
	{
		State.ChangeState(L"HurtLand");
	}
}

void Player::HurtLand_Init()
{
	MoveSpeedStop();
	ChangeAnimation(L"HurtLand");
	m_Animation->SetAnimationFrameTime(5, 0.5f);
}

void Player::HurtLand_State()
{
	if (GetAnimationFrameIndex() == 5 && m_IsNotDie == true)
	{
		if (KEYDOWN(L"Attack"))
		{
			State.ChangeState(L"WakeUp");
		}
	}

	if (AnimationEnd() && m_IsNotDie == false)
	{
		Rewind();
	}
}

void Player::Flip_Init()
{
	ChangeDirection();

	m_Spectrum->Start();

	m_ChangeDirEnable = false;

	m_MoveSpeed = FLIP_MOVESPEED;

	m_CheckMoveColSizeX = -1;

	Jump(-220.f, 600.f);
	MoveSpeedUniform();

	ChangeAnimation(L"Flip");

	SOUNDPLAY(L"Player_Roll.wav");
}

void Player::Flip_State()
{
	{
		float4 RenderSize = m_Render->GetTexture()->GetSize();
		float4 UpPos = float4(8.f * (float)m_Dir, RenderSize.y);
		float4 FrontPos = float4(UpPos.x, RenderSize.y * 0.5f);
		float4 DownPos = float4(UpPos.x, 0);

		if (CheckColMapColor(UpPos.ix(), UpPos.iy()) == GJHGameEngineVector::GREEN
			|| CheckColMapColor(FrontPos.ix(), FrontPos.iy()) == GJHGameEngineVector::GREEN
			|| CheckColMapColor(DownPos.ix(), DownPos.iy()) == GJHGameEngineVector::GREEN)
		{
			m_IsJump = false;
			m_IsFalling = false;
			State.ChangeState(L"WallGrab");
			return;
		}
	}

	if (KEYDOWN(L"Attack"))
	{
		State.ChangeState(L"Attack");
	}

	if (AnimationEnd())
	{
		MoveSpeedDownStart(0.05f, 1.f);
		State.ChangeState(L"Fall");
	}
}

void Player::Flip_Leave()
{
	m_CheckMoveColSizeX = 0;
	m_ChangeDirEnable = true;
	m_MoveSpeed = MOVESPEED;
	m_Spectrum->End();
}

void Player::WakeUp_Init()
{
	ChangeAnimation(L"WakeUp");
}

void Player::WakeUp_State()
{
	if (AnimationEnd())
	{
		m_Collision->Enable();

		m_ChangeDirEnable = true;
		m_MoveSpeed = MOVESPEED;
		m_XMoveRatio = 0.f;
		m_Death = false;
		m_IsJump = false;
		m_IsFalling = false;
		State.ChangeState(L"Idle");
	}
}

void Player::WakeUp_Leave()
{
}

void Player::Shield_HurtFlyStart_Init()
{
	m_ChangeDirEnable = false;
	m_MoveSpeed = -600.f;
	MoveSpeedUniform();
	m_XMoveRatio = 1.f;

	ChangeAnimation(L"HurtFlyStart");
}

void Player::Shield_HurtFlyStart_State()
{
	CheckDeathDoorPrevPos();

	float4 RenderScale = m_Render->GetLocalScale();
	int RenderSizeX = (int)(RenderScale.x * 0.5f * (float)m_Dir);
	int RenderSizeY = (int)(RenderScale.y * 0.5f);

	if (CheckColMapColor(0, RenderSizeY) == float4::GREEN
		|| CheckColMapColor(0, RenderSizeY) == float4::RED
		|| CheckColMapColor(RenderSizeX, RenderSizeY) == float4::GREEN
		|| CheckColMapColor(RenderSizeX, RenderSizeY) == float4::RED)
	{
		MoveSpeedStop();
	}

	if (AnimationEnd())
	{
		State.ChangeState(L"Shield_HurtLand");
	}
}

void Player::Shield_HurtLand_Init()
{
	MoveSpeedStop();
	ChangeAnimation(L"HurtLand");
	m_Animation->SetAnimationFrameTime(5, 0.3f);
}

void Player::Shield_HurtLand_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"WakeUp");
	}
}

void Player::Shield_WakeUp_Init()
{
	ChangeAnimation(L"WakeUp");
}

void Player::Shield_WakeUp_State()
{
	if (AnimationEnd())
	{
		m_ChangeDirEnable = true;
		State.ChangeState(L"Idle");
	}
}