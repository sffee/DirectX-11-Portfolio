#include "Player.h"
#include <GJHStateUpdater.h>
#include <GJHRenderer.h>
#include <GJHGameEngineWindow.h>
#include "PlaySceneWindow.h"
#include <GJHGameEngineScene.h>
#include <GJHGameEngineRandom.h>
#include "PlayerSlash.h"
#include "SpectrumCom.h"
#include <GJHSpecialTileMapRenderer.h>
#include "Door.h"
#include "InstanceObject.h"
#include "UIManager.h"
#include <TypeComponent.h>
#include <GJHUvDistortion.h>
#include "MonsterBase.h"

float Player::MOVESPEED = 250.f;
float Player::ATTACK_MOVESPEED = 250.f;
float Player::ROLL_MOVESPEED = 350.f;
float Player::FLIP_MOVESPEED = 400.f;
float Player::m_BatteryGauge;
float Player::m_TimeGauge;
bool Player::m_InifinityBattery = false;
bool Player::m_IsNotDie = false;

Player::Player() :
	m_ChangeDirEnable(true),
	m_KnockBack(false)
{

}

Player::~Player()
{

}

void Player::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);

	m_MoveSpeed = MOVESPEED;
	
	CreateAnimation(L"Idle", L"Player_Idle", 0.1f, true);
	CreateAnimation(L"MoveStart", L"Player_MoveStart", 0.04f);
	CreateAnimation(L"Move", L"Player_Move", 0.08f, true);
	CreateAnimation(L"MoveEnd", L"Player_MoveEnd", 0.05f);
	CreateAnimation(L"Jump", L"Player_Jump", 0.1f);
	CreateAnimation(L"Fall", L"Player_Fall", 0.1f, true);
	CreateAnimation(L"SitStart", L"Player_SitStart", 0.04f);
	CreateAnimation(L"SitLoop", L"Player_Sit", 0.1f, true);
	CreateAnimation(L"SitEnd", L"Player_SitEnd", 0.04f);
	CreateAnimation(L"Attack", L"Player_Attack", 0.025f);
	CreateAnimation(L"Dash", L"Player_Dash", 0.1f);
	CreateAnimation(L"HurtFlyStart", L"Player_HurtFlyStart", 0.06f);
	CreateAnimation(L"HurtFly", L"Player_HurtFly", 0.06f, true);
	CreateAnimation(L"HurtLand", L"Player_HurtLand", 0.06f);
	CreateAnimation(L"DoorKick", L"Player_DoorKick", 0.05f);
	CreateAnimation(L"Roll", L"Player_Roll", 0.04f);
	CreateAnimation(L"WakeUp", L"Player_WakeUp", 0.05f);
	CreateAnimation(L"WallGrab", L"Player_WallGrab", 0.1f, true);
	CreateAnimation(L"WallSlide", L"Player_WallSlide", 0.1f, true);
	CreateAnimation(L"Flip", L"Player_Flip", 0.025f, false);

	State.CreateState(L"Idle", &Player::Idle_Init, &Player::Idle_State, nullptr, this);
	State.CreateState(L"MoveStart", &Player::MoveStart_Init, &Player::MoveStart_State, nullptr, this);
	State.CreateState(L"Move", &Player::Move_Init, &Player::Move_State, nullptr, this);
	State.CreateState(L"MoveEnd", &Player::MoveEnd_Init, &Player::MoveEnd_State, nullptr, this);
	State.CreateState(L"Jump", &Player::Jump_Init, &Player::Jump_State, &Player::Jump_Leave, this);
	State.CreateState(L"Fall", &Player::Fall_Init, &Player::Fall_State, nullptr, this);
	State.CreateState(L"SitStart", &Player::SitStart_Init, &Player::SitStart_State, nullptr, this);
	State.CreateState(L"SitLoop", &Player::SitLoop_Init, &Player::SitLoop_State, nullptr, this);
	State.CreateState(L"SitEnd", &Player::SitEnd_Init, &Player::SitEnd_State, nullptr, this);
	State.CreateState(L"Roll", &Player::Roll_Init, &Player::Roll_State, &Player::Roll_Leave, this);
	State.CreateState(L"Attack", &Player::Attack_Init, &Player::Attack_State, &Player::Attack_Leave, this);
	State.CreateState(L"WallGrab", &Player::WallGrab_Init, &Player::WallGrab_State, &Player::WallGrab_Leave, this);
	State.CreateState(L"DoorKick", &Player::DoorKick_Init, &Player::DoorKick_State, &Player::DoorKick_Leave, this);
	State.CreateState(L"HurtFlyStart", &Player::HurtFlyStart_Init, &Player::HurtFlyStart_State, nullptr, this);
	State.CreateState(L"HurtFly", &Player::HurtFly_Init, &Player::HurtFly_State, nullptr, this);
	State.CreateState(L"HurtLand", &Player::HurtLand_Init, &Player::HurtLand_State, nullptr, this);
	State.CreateState(L"Flip", &Player::Flip_Init, &Player::Flip_State, &Player::Flip_Leave, this);
	State.CreateState(L"WakeUp", &Player::WakeUp_Init, &Player::WakeUp_State, &Player::WakeUp_Leave, this);
	State.CreateState(L"Shield_HurtFlyStart", &Player::Shield_HurtFlyStart_Init, &Player::Shield_HurtFlyStart_State, nullptr, this);
	State.CreateState(L"Shield_HurtLand", &Player::Shield_HurtLand_Init, &Player::Shield_HurtLand_State, nullptr, this);
	State.CreateState(L"Shield_WakeUp", &Player::Shield_WakeUp_Init, &Player::Shield_WakeUp_State, &Player::WakeUp_Leave, this);

	State.ChangeState(L"Idle");

	m_SlashEffect = CreateEffect<PlayerSlash>(L"Player_Slash", { 0.f, 0.f, m_Trans->GetLocalPosition().z - 1.f }, m_RenderScale, true);
	m_SlashEffect->GetActor()->FindComponent<TypeComponent>()->SetActorType(ACTORTYPE::PLAYER);

	m_Spectrum = CreateComponent<SpectrumCom>(m_Render, m_RenderScale, GetActor(), 0.05f, 0.4f, 0.4f, float4(0.f, 4.f, 4.f, 1.f), false);
	m_Spectrum->SetActorType(ACTORTYPE::PLAYER);
	m_Spectrum->SetIsPlayer(true);

	CreateCollision((int)CollisionLayer::Player, COLTYPE::AABB, &Player::ColEnter, &Player::ColStay, &Player::ColExit, *this);
	m_Collision->SetLocalPosition({ 0.f, 20.f });
	m_Collision->SetLocalScale({ 20.f, 40.f });
	m_Collision->SetDebugRenderColor({ 0.f, 0.f, 1.f, 1.f });

	m_FakeCollision = CreateComponent<GJHCollisionComponent>((int)CollisionLayer::PlayerFake, COLTYPE::AABB);
	m_FakeCollision->AddEnterEvent(&Player::FakeColEnter, *this);
	m_FakeCollision->AddStayEvent(&Player::FakeColStay, *this);
	m_FakeCollision->AddExitEvent(&Player::FakeColExit, *this);
	m_FakeCollision->SetLocalPosition({ 0.f, 20.f });
	m_FakeCollision->SetLocalScale({ 20.f, 40.f });
	m_FakeCollision->SetDebugRenderColor({ 0.f, 0.f, 0.f, 0.f });

	CreateExplosion();
	CreateDustCloud();
	CreateJumpCloud();
	CreateLandCloud();

	m_Attack = false;

	m_FadeInValue = 1.f;
	m_FadeInStart = false;
	m_InifinityBattery = false;
	m_DustCloudIndex = 0;
}

void Player::Update()
{
	if (m_Record->m_RewindStart == false)
	{
		CheckDirection();
		CheckFalling();
		CheckGlobalKey();

		Platform();
		State.Update();

		CamMove();

		ObjectBase::Update(true, m_CamTrans->GetWorldPosition());

		SlowTimeUpdate();

		//GUIUpdate();
		UIUpdate();
	}
	else
	{
		m_RewindAddSpeed += DELTATIME(m_RewindAddDeltaTime, false);
		if (1.f <= m_RewindAddSpeed)
		{
			m_RewindAddSpeed = 1.f;
		}

		m_RewindSpeed += m_RewindAddSpeed;
		if (6.f <= m_RewindSpeed)
		{
			m_RewindSpeed = 6.f;
		}

		GJHRecordComponent::SubRewindEndTime(DELTATIME(m_RewindSpeed, false));
	}
}

void Player::CheckGlobalKey()
{
	if (KEYPRESS(L"SlowMotion")
		&& m_IsSlowMotion == false
		&& 1.f <= m_BatteryGauge)
	{
		GJHGameEngineTime::SetTimeScale(0.2f);
		GJHGameEngineSound::SetPitch(0.2f);
		m_FadeInStart = true;
		m_IsSlowMotion = true;

		SOUNDPLAY(L"SlowMotion", L"SlowMotion_Start.wav", 0, 1.f, false);

		m_Render->SetColor({ 0.f, 4.f, 4.f, 1.f });
		m_Spectrum->SetTransformZ(m_Trans->GetWorldPosition().z - 1.f);
		m_Spectrum->SetColor({ 0.f, 8.f, 8.f, 2.f });
		m_Spectrum->SetCreateTime(0.015f);
		m_Spectrum->SetLifeTime(0.15f);
		m_Spectrum->SetIsPlayer(false);

		m_Spectrum->Start();
	}
	else if ((KEYPRESS(L"SlowMotion")
		&& m_IsSlowMotion == true
		&& m_BatteryGauge == 0.f)
		||
		(KEYFREE(L"SlowMotion")
			&& m_IsSlowMotion == true)
		)
	{
		GJHGameEngineTime::SetTimeScale(1.f);
		GJHGameEngineSound::SetPitch(1.f);
		m_IsSlowMotion = false;

		SOUNDSTOP(L"SlowMotion");
		SOUNDPLAY(L"SlowMotion_End.wav", 0, 2.f, false);

		m_Render->SetColor({ 1.f, 1.f, 1.f, 1.f });
		m_Spectrum->SetTransformZ(m_Trans->GetWorldPosition().z + 1.f);
		m_Spectrum->SetColor({ 0.f, 4.f, 4.f, 1.f });
		m_Spectrum->SetCreateTime(0.05f);
		m_Spectrum->SetLifeTime(0.4f);
		m_Spectrum->SetMaxAlpha(0.4f);
		m_Spectrum->SetIsPlayer(true);

		m_Spectrum->End();
	}

	if (KEYDOWN(L"F1"))
	{
		Rewind();
	}

	if (KEYDOWN(L"F2"))
	{
		m_Collision->ToggleDebugRender();
	}

	if (KEYDOWN(L"F3"))
	{
		m_Ray->ToggleDebugRender();
	}

	if (KEYDOWN(L"F4"))
	{
		GJHSpecialTileMapRenderer::ToggleDebugRender();
	}

	if (KEYDOWN(L"F5"))
	{
		m_InifinityBattery = !m_InifinityBattery;
	}

	if (KEYDOWN(L"F6"))
	{
		m_IsNotDie = !m_IsNotDie;
	}

	if (KEYDOWN(L"F12"))
	{
		MonsterBase::SetMonsterCount(0);
	}
}

void Player::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		if (State.CurState == L"DoorKick"
			|| m_IsJump == true)
		{
			m_Trans->SetWorldPosition(m_PrevPos);
			return;
		}

		float4 MyPos = m_Trans->GetWorldPosition();
		float4 DoorPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();

		//DIRECTION DoorDir = (MyPos.x < DoorPos.x) ? DIRECTION::RIGHT : DIRECTION::LEFT;

		if (State.CurState == L"Move")
			//&& DoorDir == m_Dir)
		{
			_Other->GetActor()->FindComponent<Door>()->BindDamageFunc(m_DoorKickFunc);
			State.ChangeState(L"DoorKick");
		}
		
		m_Trans->SetWorldPosition(m_PrevPos);
	}

	if (_Other->GetOrder() == (int)CollisionLayer::MonsterBullet
		|| _Other->GetOrder() == (int)CollisionLayer::MonsterAttack)
	{
		Damage(_Other->GetActor());
	}

	if (_Other->GetOrder() == (int)CollisionLayer::HeadHunterGrenadeExplosion)
	{
		ExplosionDamage(_Other->GetActor());
	}

	if (_Other->GetOrder() == (int)CollisionLayer::MonsterShieldAttack)
	{
		SOUNDPLAY(L"Punch_Hit.wav");

		CamShake(0.25f, 5);
		StopTime(0.f, 0.2f);

		float4 OtherPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();
		DIRECTION Dir = (m_Trans->GetWorldPosition().x - OtherPos.x < 0.f) ? DIRECTION::RIGHT : DIRECTION::LEFT;

		SetDirection(Dir);

		State.ChangeState(L"Shield_HurtFlyStart");
	}
}

void Player::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		if (State.CurState == L"DoorKick"
			|| m_IsJump == true)
		{
			m_Trans->SetWorldPosition(m_PrevPos);
			return;
		}

		float4 MyPos = m_Trans->GetWorldPosition();
		float4 DoorPos = _Other->GetActor()->FindComponent<GJHActorTransform>()->GetWorldPosition();

		DIRECTION DoorDir = (MyPos.x < DoorPos.x) ? DIRECTION::RIGHT : DIRECTION::LEFT;

		if (State.CurState == L"Move"
			&& DoorDir == m_Dir)
		{
			_Other->GetActor()->FindComponent<Door>()->BindDamageFunc(m_DoorKickFunc);
			State.ChangeState(L"DoorKick");
		}

		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Player::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
}

void Player::FakeColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (m_Collision->GetColCheck() == true)
	{
		return;
	}

	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Player::FakeColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (m_Collision->GetColCheck() == true)
	{
		return;
	}

	if (_Other->GetOrder() == (int)CollisionLayer::Door)
	{
		m_Trans->SetWorldPosition(m_PrevPos);
	}
}

void Player::FakeColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (m_Collision->GetColCheck() == true)
	{
		return;
	}
}

void Player::RewindEnd()
{
	ResetStageTime();
	m_BatteryGauge = 11.f;
	m_TimeGauge = 100.f;

	m_ChangeDirEnable = true;
	m_MoveSpeed = MOVESPEED;
	m_XMoveRatio = 0.f;
	MoveSpeedStop();
	m_Collision->Enable();
	State.ChangeState(L"Idle");

	m_UICam->On();

	GJHTargetPostEffect::FindPostEffect<GJHUvDistortion>(L"UvDistortion")->IsEffectOff();
}

void Player::Move()
{
	float4 MoveSpeed = float4(DELTATIME(m_MoveSpeed), 0.f);

	ObjectBase::Move(MoveSpeed);
}

void Player::Move(const float4& _Value)
{
	ObjectBase::Move(_Value);
}

void Player::CamMove()
{
	float4 PlayerPos = m_Trans->GetWorldPosition();

	//float4 MousePos = GetMousePos();
	//MousePos.x = (MousePos.x < 0.f) ? 0.f : MousePos.x;
	//MousePos.x = (CAMSIZEX < MousePos.x) ? CAMSIZEX : MousePos.x;
	//MousePos.y = (0 < MousePos.y) ? 0.f : MousePos.y;
	//MousePos.y = (MousePos.y < -CAMSIZEY) ? -CAMSIZEY : MousePos.y;

	//MousePos.x = (MousePos.x - PlayerPos.x) * 0.1f;
	//MousePos.y = (MousePos.y - PlayerPos.y) * 0.2f;

	float4 CamPos = PlayerPos;// +MousePos;
	float CamPosLeft = CamPos.x - CAMSIZEX * 0.5f;
	float CamPosRight = CamPos.x + CAMSIZEX * 0.5f;
	float CamPosTop = CamPos.y + CAMSIZEY * 0.5f;
	float CamPosBottom = CamPos.y - CAMSIZEY * 0.5f;

	float4 CollImageSize = COLMAPTEXTURE->GetSize();

	if (CAMSIZEX < CollImageSize.x)
	{
		if (CamPosLeft < 0.f)
		{
			CamPos.x = CAMSIZEX * 0.5f;
		}

		if (CollImageSize.x < CamPosRight)
		{
			CamPos.x = CollImageSize.x - CAMSIZEX * 0.5f;
		}
	}
	else
	{
		CamPos.x = CAMSIZEX * 0.5f;
	}

	if (CAMSIZEY < CollImageSize.y)
	{
		if (0.f < CamPosTop)
		{
			CamPos.y = CAMSIZEY * -0.5f;
		}

		if (CollImageSize.y < -CamPosBottom)
		{
			CamPos.y = -CollImageSize.y + CAMSIZEY * 0.5f;
		}
	}
	else
	{
		CamPos.y = CAMSIZEY * -0.5f;
	}

	float4 CamShake;
	if (m_IsCamShake == true)
	{
		float4 RandomPower = float4(GJHGameEngineRandom::RandomFloat((float)-m_CamShakePower, (float)m_CamShakePower), GJHGameEngineRandom::RandomFloat((float)-m_CamShakePower, (float)m_CamShakePower));
		CamShake = RandomPower;

		m_CamShakeTime -= DELTATIME(1.f, false);
		if (m_CamShakeTime < 0.f)
		{
			m_IsCamShake = false;
		}
	}

	CamPos += CamShake;

	m_CamTrans->SetWorldPosition(CamPos);
}

void Player::CheckDirection()
{
	if (m_ChangeDirEnable == false
		|| m_KnockBack == true)
	{
		return;
	}

	if (KEYPRESS(L"LMove"))
	{
		SetDirection(DIRECTION::LEFT);
	}

	if (KEYPRESS(L"RMove"))
	{
		SetDirection(DIRECTION::RIGHT);
	}
}

void Player::CheckFalling()
{
	if (m_ApplyGravity == false
		|| m_OnPlatform == true
		|| m_IsWallGrab == true)
	{
		return;
	}

	if (m_IsJump == false && m_IsFalling == false)
	{
		int YPos = -1;
		bool IsFalling = false;
		bool IsYMove = false;
		while (CheckColMapColor(0, YPos) != GJHGameEngineVector::RED)
		{
			YPos -= 1;
			IsYMove = true;

			if (10 <= abs(YPos))
			{
				YPos = 0;
				IsFalling = true;
				break;
			}
		}

		if (IsFalling == true)
		{
			m_Gravity = 50.f;
			m_GravityAddValue = 600.f;

			m_EnableGravity = true;

			State.ChangeState(L"Fall");
		}
		else
		{
			if (IsYMove == true)
			{
				m_Trans->SetWorldMove({ 0.f, (float)YPos });
			}
		}
	}
}

void Player::Platform()
{
	if (m_IgnorePlatform == true)
	{
		float4 RenderSize = m_Render->GetTexture()->GetSize();

		int LeftDownPosX = static_cast<int>(RenderSize.x * -0.5f);
		int RightDownPosX = static_cast<int>(RenderSize.x * 0.5f);

		m_OnPlatform = false;

		if (CheckColMapColor(LeftDownPosX, 0) != GJHGameEngineVector::BLUE
			&& CheckColMapColor(LeftDownPosX, -1) != GJHGameEngineVector::BLUE
			&& CheckColMapColor(RightDownPosX, 0) != GJHGameEngineVector::BLUE
			&& CheckColMapColor(RightDownPosX, -1) != GJHGameEngineVector::BLUE)
		{
			m_IgnorePlatform = false;
		}
	}
}

void Player::SlowTimeUpdate()
{
	if (m_FadeInStart == false)
	{
		return;
	}

	if (m_IsSlowMotion == true)
	{
		m_FadeInValue -= DELTATIME(2.f, false);
		if (m_FadeInValue <= 0.5f)
		{
			m_FadeInValue = 0.5f;
		}
	}
	else
	{
		m_FadeInValue += DELTATIME(2.f, false);
		if (1.f <= m_FadeInValue)
		{
			m_FadeInValue = 1.f;
			m_FadeInStart = false;
		}
	}

	std::list<ActorPtr>& ActorList = GetScene()->GetActorList();

	for (auto& Actor : ActorList)
	{
		std::shared_ptr<TypeComponent> TypeCom = Actor->FindComponent<TypeComponent>();

		if (TypeCom != nullptr)
		{
			ACTORTYPE ActorType = TypeCom->GetActorType();
			if (ActorType != ACTORTYPE::PLAYER
				&& ActorType != ACTORTYPE::MONSTER
				&& ActorType != ACTORTYPE::UI
				&& ActorType != ACTORTYPE::BULLET)
			{
				std::shared_ptr<GJHSpriteRenderer> Render = Actor->FindComponent<GJHSpriteRenderer>();
				if (Render != nullptr)
				{
					Render->SetColor({ m_FadeInValue, m_FadeInValue, m_FadeInValue, Render->GetColor().a });
				}
			}
			else if (ActorType == ACTORTYPE::MONSTER)
			{
				std::shared_ptr<GJHSpriteRenderer> Render = Actor->FindComponent<GJHSpriteRenderer>();
				if (Render != nullptr)
				{
					float Value = (m_FadeInValue == 1.f) ? 1.f : 1.f - ((1.f - m_FadeInValue) * 2.f);
					float RedValue = (m_FadeInValue == 1.f) ? 1.f : 2.f;

					Render->SetColor({ RedValue, Value, Value, Render->GetColor().a });
				}
			}
		}
	}
}

void Player::GUIUpdate()
{
	//PlaySceneWindow::m_CharPos = m_Trans->GetWorldPosition();
	//PlaySceneWindow::m_CamPos = m_CamTrans->GetWorldPosition();
	//PlaySceneWindow::m_MousePos = GetMousePos();

	//float4 MousePos = GetMousePos() + (m_CamTrans->GetWorldPosition() - float4(CAMSIZEX * 0.5f, CAMSIZEY * -0.5f));
	//float4 CharPos = m_Trans->GetWorldPosition();
}

void Player::UIUpdate()
{
	m_TimeGauge -= DELTATIME(1.f, false);
	if (m_TimeGauge <= 0.f)
	{
		m_TimeGauge = 0.f;
	}

	if (m_IsSlowMotion == true)
	{
		if (m_InifinityBattery == true)
		{
			return;
		}

		m_BatteryGauge -= DELTATIME(1.5f, false);
		if (m_BatteryGauge <= 0.f)
		{
			m_BatteryGauge = 0.f;
		}
	}
	else
	{
		m_BatteryGauge += DELTATIME(1.f);
		if (11.f <= m_BatteryGauge)
		{
			m_BatteryGauge = 11.f;
		}
	}
}

void Player::SceneChangeStart()
{
	ObjectBase::m_Player = this;

	m_BatteryGauge = 11.f;
	m_TimeGauge = 100.f;
}

void Player::SetCamShake(float _Time, int _Power)
{
	m_IsCamShake = true;
	m_CamShakeTime = _Time;
	m_CamShakePower = _Power;
}

void Player::KnockBack()
{
	MoveSpeedDownStart(0.05f, 0.f, -0.3f);
	m_KnockBack = true;
	m_KnockBackMoveBlockTime = 0.5f;
	State.ChangeState(L"Jump");
	Jump(-200.f, 600.f);
}

void Player::Damage(GJHGameEngineActor* _Other)
{
	SOUNDPLAY(L"Dragon_Death.wav");
	SOUNDPLAY(L"Punch_Hit.wav");

	CamShake(0.25f, 5);
	StopTime(0.f, 0.2f);

	float4 OtherPos = _Other->FindComponent<GJHActorTransform>()->GetWorldPosition();
	DIRECTION Dir = (m_Trans->GetWorldPosition().x - OtherPos.x < 0.f) ? DIRECTION::RIGHT : DIRECTION::LEFT;

	SetDirection(Dir);

	Jump(-100.f, 600.f);
	m_MoveSpeed = -300.f;
	MoveSpeedUniform();
	m_XMoveRatio = 1.f;
	State.ChangeState(L"HurtFlyStart");
}

void Player::ExplosionDamage(GJHGameEngineActor* _Other)
{
	SOUNDPLAY(L"Dragon_Death.wav");

	CamShake(0.25f, 5);
	StopTime(0.f, 0.2f);

	float4 OtherPos = _Other->FindComponent<GJHActorTransform>()->GetWorldPosition();
	DIRECTION Dir = (m_Trans->GetWorldPosition().x - OtherPos.x < 0.f) ? DIRECTION::RIGHT : DIRECTION::LEFT;

	SetDirection(Dir);

	Jump(-200.f, 600.f);
	MoveSpeedStop();
	State.ChangeState(L"HurtFlyStart");
}

void Player::LaserDamage(GJHGameEngineActor* _Other)
{
	SOUNDPLAY(L"Dragon_Death.wav");

	CamShake(0.25f, 5);
	StopTime(0.f, 0.2f);
	Explosion();

	float4 OtherPos = _Other->FindComponent<GJHActorTransform>()->GetWorldPosition();
	DIRECTION Dir = (m_Trans->GetWorldPosition().x - OtherPos.x < 0.f) ? DIRECTION::RIGHT : DIRECTION::LEFT;

	SetDirection(Dir);

	Jump(-200.f, 600.f);
	MoveSpeedStop();
	State.ChangeState(L"HurtFlyStart");
}

void Player::CreateExplosion()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Explosion";
	Data.AnimationFrameTime = 0.06f;
	Data.TextureName = L"Explosion";
	Data.Dir = DIRECTION::RIGHT;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.AnimationEndHide = true;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_ExplosionList.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_ExplosionList[i] = CreateInstanceObject(Data, true)->FindComponent<InstanceObject>();
		m_ExplosionList[i]->FindComponent<GJHSpriteRenderer>()->SetRenderEnable(false);
	}
}

void Player::Explosion()
{
	float4 MyPos = m_Trans->GetWorldPosition();

	for (int i = 0; i < 3; ++i)
	{
		float4 Pos = MyPos + float4(GJHGameEngineRandom::RandomFloat(-20.f, 20.f), GJHGameEngineRandom::RandomFloat(-20.f, 20.f));
		m_ExplosionList[i]->Start(Pos);
	}
}

void Player::CreateDustCloud()
{
	InstanceObjectData Data;
	Data.AnimationName = L"DustCloud";
	Data.AnimationFrameTime = 0.08f;
	Data.TextureName = L"DustCloud";
	Data.Dir = DIRECTION::RIGHT;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.AnimationEndHide = true;
	Data.ApplyGravity = true;
	Data.IsGravityCheckMapColor = false;
	Data.CheckWall = false;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_DustCloudList.resize(20);
	for (int i = 0; i < 20; ++i)
	{
		m_DustCloudList[i] = CreateInstanceObject(Data, false)->FindComponent<InstanceObject>();
		m_DustCloudList[i]->FindComponent<GJHSpriteRenderer>()->SetRenderEnable(false);
		m_DustCloudList[i]->MoveSpeedUniform();
	}
}

void Player::CreateJumpCloud()
{
	InstanceObjectData Data;
	Data.AnimationName = L"JumpCloud";
	Data.AnimationFrameTime = 0.08f;
	Data.TextureName = L"JumpCloud";
	Data.Dir = DIRECTION::RIGHT;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.AnimationEndHide = true;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_JumpCloud = CreateInstanceObject(Data, false)->FindComponent<InstanceObject>();
}

void Player::CreateLandCloud()
{
	InstanceObjectData Data;
	Data.AnimationName = L"LandCloud";
	Data.AnimationFrameTime = 0.06f;
	Data.TextureName = L"LandCloud";
	Data.Dir = DIRECTION::RIGHT;
	Data.IsDeath = false;
	Data.IsRecord = true;
	Data.AnimationEndHide = true;
	Data.ActorType = ACTORTYPE::EFFECT;

	m_LandCloud = CreateInstanceObject(Data, false)->FindComponent<InstanceObject>();
}

void Player::DustCloud()
{
	int Index = m_DustCloudIndex % 4;
	float Gravity = 0.f;
	float MoveSpeed = 0.f;
	if (Index == 0)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-20.f, -10.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(-20.f, -10.f);
	}
	else if (Index == 1)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-50.f, -40.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(-40.f, -30.f);
	}
	else if (Index == 2)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-30.f, -20.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(-30.f, -20.f);
	}
	else if (Index == 3)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-40.f, -30.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(-50.f, -40.f);
	}

	MoveSpeed *= (float)m_Dir;

	float4 Pos = m_Trans->GetWorldPosition() + float4(0.f, -10.f);
	Pos.z += 1.f;
	m_DustCloudList[m_DustCloudIndex]->Start(Pos);
	m_DustCloudList[m_DustCloudIndex]->Jump(Gravity, 0.f);
	m_DustCloudList[m_DustCloudIndex]->SetMoveSpeed(MoveSpeed);

	m_DustCloudIndex++;
	if (m_DustCloudList.size() - 1 < m_DustCloudIndex)
	{
		m_DustCloudIndex = 0;
	}
}

void Player::JumpCloud()
{
	m_JumpCloud->Start(m_Trans->GetWorldPosition());
}

void Player::LandCloud()
{
	m_LandCloud->Start(m_Trans->GetWorldPosition());
}

void Player::WallCloud()
{
	int Index = m_DustCloudIndex % 4;
	float Gravity = 0.f;
	float MoveSpeed = 0.f;
	if (Index == 0)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-20.f, -10.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(10.f, 20.f);
	}
	else if (Index == 1)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-50.f, -40.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(30.f, 40.f);
	}
	else if (Index == 2)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-30.f, -20.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(20.f, 30.f);
	}
	else if (Index == 3)
	{
		Gravity = GJHGameEngineRandom::RandomFloat(-40.f, -30.f);
		MoveSpeed = GJHGameEngineRandom::RandomFloat(40.f, 50.f);
	}

	float4 Pos = m_Trans->GetWorldPosition() + float4(10.f * (float)m_Dir, 0.f);
	Pos.z += 1.f;
	m_DustCloudList[m_DustCloudIndex]->Start(Pos);
	m_DustCloudList[m_DustCloudIndex]->Jump(Gravity, 0.f);
	m_DustCloudList[m_DustCloudIndex]->SetMoveSpeed(MoveSpeed);

	m_DustCloudIndex++;
	if (m_DustCloudList.size() - 1 < m_DustCloudIndex)
	{
		m_DustCloudIndex = 0;
	}
}

void Player::Rewind(float _RewindAddDeltaTime, bool _UseUvShader)
{
	if (_UseUvShader == true)
	{
		std::shared_ptr<GJHUvDistortion> Distortion = GJHTargetPostEffect::FindPostEffect<GJHUvDistortion>(L"UvDistortion");
		Distortion->SetEffectTarget(m_Cam->GetCameraTarget());
		Distortion->Reset();
		Distortion->IsEffectOn();
	}
	
	SOUNDPLAY(L"Rewind.wav");

	m_Record->SetCamTrans(m_CamTrans);
	m_Record->RewindStart(GetStageTime());
	m_RewindSpeed = 1.f;
	m_RewindAddSpeed = 0.f;
	m_RewindAddDeltaTime = _RewindAddDeltaTime;

	m_UICam->Off();

	if (_UseUvShader == true)
	{
		std::shared_ptr<GJHUvDistortion> Distortion = GJHTargetPostEffect::FindPostEffect<GJHUvDistortion>(L"UvDistortion");
		Distortion->SetEffectTarget(m_Cam->GetCameraTarget());
		Distortion->Reset();
		Distortion->IsEffectOn();
	}
}

void Player::PlaySlashSound()
{
	int Random = GJHGameEngineRandom::RandomInt(1, 3);

	GJHGameEngineString FileName = GJHGameEngineString(L"Slash_") + GJHGameEngineString(Random) + GJHGameEngineString(L".wav");
	SOUNDPLAY(FileName);
}