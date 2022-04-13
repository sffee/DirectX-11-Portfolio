#include "ObjectBase.h"
#include <GJHDirectTexture2D.h>
#include <GJHGameEngineScene.h>
#include <GJHGameEngineWindow.h>
#include "StageBase.h"
#include "InstanceObject.h"
#include "Player.h"
#include "GJHWorldComponent.h"
#include <TypeComponent.h>
#include "define.h"

Player* ObjectBase::m_Player;
bool ObjectBase::m_RewindOn = false;
float ObjectBase::m_RewindTime = 0.f;
bool ObjectBase::m_IsSlowMotion = false;

ObjectBase::ObjectBase() :
	m_Render(nullptr),
	m_Collision(nullptr),
	m_AttackCollision(nullptr),
	m_Animation(nullptr),
	m_Dir(DIRECTION::LEFT),
	m_IsJump(false),
	m_IsFalling(false),
	m_OnPlatform(false),
	m_IgnorePlatform(false),
	m_MoveSpeed(350.f),
	m_XMoveRatio(0.f),
	m_MoveTimer(0.f),
	m_MoveTimerVelocity(0.f),
	m_MoveSpeedType(MOVESPEED::STOP),
	m_AddXMoveRatio(0.f),
	m_Gravity(300.f),
	m_GravityAddValue(600.f),
	m_ApplyGravity(true),
	m_EnableGravity(true),
	m_CheckMoveColSizeX(0),
	m_IsWallGrab(false),
	m_UseTimeScale(true),
	m_CheckPlatform(true),
	m_IsDetect(false),
	m_IsRenderUpdate(true),
	m_IsRecord(true),
	m_CheckCelling(true),
	m_CheckWall(true),
	m_IsStopTime(false),
	m_Death(false),
	m_IsGravityCheckMapColor(true)
{

}

ObjectBase::~ObjectBase()
{

}

void ObjectBase::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	m_Render = _Renderer;
	m_Trans = GetActorTrans();
	m_Animation = CreateComponent<GJHSpriteAnimation>();

	m_Record = CreateComponent<GJHRecordComponent>();
	m_Record->SetRewindEndFunc(std::bind(&ObjectBase::RewindEndFunc, this));

	m_Ray = CreateComponent<GJHRayComponent>();

	m_LocalScale = m_Render->GetLocalScale();
	m_RenderScale = _RenderScale;

	m_RayCooltime = 0.f;
}

void ObjectBase::Update(bool _RewindUseCamera, float4 _RewindCameraPos)
{
	Gravity();
	Platform();
	RenderUpdate();
	RecordUpdate(_RewindUseCamera, _RewindCameraPos);
	ChangeMoveRatioToMoveType();
	CheckCeiling();
	StopTimeUpdate();

	if (m_MoveSpeedType != MOVESPEED::STOP)
	{
		Move();
	}
}

void ObjectBase::CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _TextureName, int _StartFrame, int _EndFrame, float _FrameTime, bool _Loop, bool _IsPiece)
{
	m_Animation->CreateAnimation(_AnimationName, _TextureName, _StartFrame, _EndFrame, _FrameTime, _Loop, _IsPiece);
}

void ObjectBase::CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _TextureName, float _FrameTime, bool _Loop, bool _IsPiece)
{
	std::shared_ptr<GJHDirectSprite> Sprite = GJHDirectSprite::Find(_TextureName);

	int SpriteSize = (int)(Sprite->GetPieceSpriteSize() - 1);
	int EndFrame = max(SpriteSize, 0);

	m_Animation->CreateAnimation(_AnimationName, _TextureName, 0, EndFrame, _FrameTime, _Loop, _IsPiece);
}

void ObjectBase::ChangeAnimation(const GJHGameEngineString& _AnimationName)
{
	m_CurAnimationName = _AnimationName;
	m_Animation->ChangeAnimation(_AnimationName);
}

std::shared_ptr<GJHGameEngineActor> ObjectBase::CreateInstanceObject(const InstanceObjectData& _Data, bool _AttachParent)
{
	std::shared_ptr<GJHGameEngineActor> Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	Transform->SetWorldPosition(_Data.Pos);

	GJHGameEngineString TextureName = _Data.TextureName + L"_0.png";
	std::shared_ptr<GJHDirectTexture2D> Sprite = GJHDirectTexture2D::Find(TextureName);

	std::shared_ptr<GJHSpriteRenderer> Renderer = Actor->CreateComponent<GJHSpriteRenderer>(_Data.RenderOrder);
	Renderer->TextureSetting(TextureName);
	Renderer->SetWorldScale(_Data.Scale);
	Renderer->SetLocalRotationZ(_Data.Angle);

	if (_AttachParent == true)
	{
		Transform->AttachParent(GetActor()->FindComponent<GJHActorTransform>());
	}

	Actor->CreateComponent<InstanceObject>(_Data, Renderer);
	Actor->CreateComponent<TypeComponent>(_Data.ActorType);

	return Actor;
}

void ObjectBase::AnimationReset()
{
	m_Animation->Reset();
}

void ObjectBase::SetDirection(int _Dir)
{
	m_Dir = (DIRECTION)_Dir;
}

void ObjectBase::SetDirection(DIRECTION _Dir)
{
	m_Dir = _Dir;
}

ObjectBase::DIRECTION ObjectBase::GetDirection()
{
	return m_Dir;
}

ObjectBase::DIRECTION ObjectBase::GetOppositeDirection()
{
	return (m_Dir == DIRECTION::LEFT) ? DIRECTION::RIGHT : DIRECTION::LEFT;
}

void ObjectBase::ChangeDirection()
{
	SetDirection((m_Dir == DIRECTION::LEFT) ? DIRECTION::RIGHT : DIRECTION::LEFT);
}

bool ObjectBase::AnimationEnd()
{
	return m_Animation->AnimationEnd();
}

void ObjectBase::SetAnimationFrameIndex(int _Index)
{
	m_Animation->SetAnimationFrameIndex(_Index);
}

int ObjectBase::GetAnimationFrameIndex()
{
	return m_Animation->GetAnimationFrameIndex();
}

void ObjectBase::Move()
{
	if (CheckColWall(m_CheckMoveColSizeX) == false || m_CheckWall == false)
	{
		m_PrevPos = m_Trans->GetWorldPosition();

		float4 MoveValue = float4(m_MoveSpeed * (float)((int)m_Dir * m_XMoveRatio) * DELTATIME(1.f, m_UseTimeScale) , 0.f);
		m_Trans->SetLocalMove(MoveValue);
	}
}

void ObjectBase::Move(const float4& _Value, bool _SkipWallCheck)
{
	if (CheckColWall(m_CheckMoveColSizeX) == false || _SkipWallCheck == true || m_CheckWall == false)
	{
		m_PrevPos = m_Trans->GetWorldPosition();
		
		float4 MoveValue = float4(_Value.x * (float)((int)m_Dir * m_XMoveRatio), _Value.y);
		m_Trans->SetLocalMove(MoveValue);
	}
}

void ObjectBase::SetShowAnimation(bool _Set)
{
	m_Animation->SetShowAnimation(_Set);
	m_Render->SetRenderEnable(_Set);
}

void ObjectBase::Jump(float _Gravity, float _GravityAddValue, bool _OneUpMove)
{
	m_Gravity = _Gravity;
	m_GravityAddValue = _GravityAddValue;

	m_EnableGravity = true;
	m_IsJump = true;
	m_IsFalling = false;
	m_OnPlatform = false;

	if (_OneUpMove == true)
	{
		m_Trans->SetWorldMove({ 0.f, 1.f });
	}
}

void ObjectBase::ChangeMoveRatioToMoveType()
{
	switch (m_MoveSpeedType)
	{
	case MOVESPEED::DOWN:
		m_MoveTimer += DELTATIME(m_MoveTimerVelocity, m_UseTimeScale);

		m_AddXMoveRatio += m_MoveTimer;
		m_XMoveRatio -= max(m_AddXMoveRatio, 0.f);

		if (m_XMoveRatio < m_MinXRatio)
		{
			m_XMoveRatio = m_MinXRatio;
		}

		break;
	case MOVESPEED::UP:
		m_MoveTimer += DELTATIME(m_MoveTimerVelocity, m_UseTimeScale);

		m_AddXMoveRatio += m_MoveTimer;
		m_XMoveRatio += min(m_AddXMoveRatio, 1.f);

		if (m_MaxXRatio < m_XMoveRatio)
		{
			m_XMoveRatio = m_MaxXRatio;
		}

		break;
	case MOVESPEED::UNIFORM:
	case MOVESPEED::STOP:
		break;
	}
}

void ObjectBase::MoveSpeedUpStart(float _StartXRatio, float _MoveTimerVelocity, float _MaxXRatio)
{
	if (m_MoveSpeedType == MOVESPEED::UP)
	{
		return;
	}

	m_MoveTimer = 0.f;
	m_MoveTimerVelocity = _MoveTimerVelocity;
	m_AddXMoveRatio = 0.f;
	m_XMoveRatio = _StartXRatio;
	m_MoveSpeedType = MOVESPEED::UP;
	m_MaxXRatio = _MaxXRatio;
}

void ObjectBase::MoveSpeedDownStart(float _MoveTimerVelocity, float _StartXRatio, float _MinXRatio)
{
	if (m_MoveSpeedType == MOVESPEED::DOWN)
	{
		return;
	}

	m_MoveTimer = 0.f;
	m_MoveTimerVelocity = _MoveTimerVelocity;
	m_AddXMoveRatio = 0.f;
	m_MoveSpeedType = MOVESPEED::DOWN;
	m_MinXRatio = _MinXRatio;

	if (_StartXRatio != -1.f)
	{
		m_XMoveRatio = _StartXRatio;
	}
}

void ObjectBase::MoveSpeedUniform()
{
	if (m_MoveSpeedType == MOVESPEED::UNIFORM)
	{
		return;
	}

	m_XMoveRatio = 1.f;
	m_MoveSpeedType = MOVESPEED::UNIFORM;
}

void ObjectBase::MoveSpeedStop()
{
	if (m_MoveSpeedType == MOVESPEED::STOP)
	{
		return;
	}

	m_XMoveRatio = 1.f;
	m_MoveSpeedType = MOVESPEED::STOP;
}

void ObjectBase::Gravity()
{
	if (m_ApplyGravity == false)
	{
		return;
	}

	if (m_EnableGravity == true)
	{
		m_Gravity += m_GravityAddValue * DELTATIME(1.f, m_UseTimeScale);

		if (m_IsWallGrab == true)
		{
			m_Gravity = min(m_Gravity, 150.f);
		}

		float m_GravityForce = m_Gravity * DELTATIME(1.f, m_UseTimeScale);

		float4 MoveSpeed = GJHGameEngineVector::DOWN * m_GravityForce;
		Move(MoveSpeed, true);
	}

	if (m_IsGravityCheckMapColor == false)
	{
		return;
	}

	while (CheckColMapColor(0, 0) == GJHGameEngineVector::RED)
	{
		m_Trans->SetWorldMove(GJHGameEngineVector::UP * DELTATIME(10.f, false));

		m_Gravity = 0;
		m_GravityAddValue = 0;

		m_IgnorePlatform = false;
		m_EnableGravity = false;
		m_IsJump = false;
		m_IsFalling = false;
	}
}

void ObjectBase::Platform()
{
	if (m_CheckPlatform == false)
	{
		return;
	}

	float4 RenderSize = m_Render->GetTexture()->GetSize();
	
	int LeftDownPosX = static_cast<int>(RenderSize.x * -0.5f);
	int RightDownPosX = static_cast<int>(RenderSize.x * 0.5f);

	if (m_IgnorePlatform == false)
	{
		if (m_IsJump == true && m_IsFalling == false)
		{
			return;
		}

		if ((CheckColMapColor(LeftDownPosX, 0) != GJHGameEngineVector::BLUE && CheckColMapColor(LeftDownPosX, -1) == GJHGameEngineVector::BLUE)
			|| (CheckColMapColor(RightDownPosX, 0) != GJHGameEngineVector::BLUE && CheckColMapColor(RightDownPosX, -1) == GJHGameEngineVector::BLUE))
		{
			m_OnPlatform = true;

			m_EnableGravity = false;
			m_IsJump = false;
			m_IsFalling = false;
		}
		else
		{
			m_OnPlatform = false;
		}

		for (int i = 1; i < 6; ++i)
		{
			if ((CheckColMapColor(LeftDownPosX, 0) == GJHGameEngineVector::BLUE && CheckColMapColor(LeftDownPosX, i) != GJHGameEngineVector::BLUE)
				|| (CheckColMapColor(RightDownPosX, 0) == GJHGameEngineVector::BLUE && CheckColMapColor(RightDownPosX, i) != GJHGameEngineVector::BLUE))
			{
				m_Trans->SetWorldMove(float4(0.f, static_cast<float>(i), 0.f));

				m_OnPlatform = true;

				m_EnableGravity = false;
				m_IsJump = false;
				m_IsFalling = false;
				break;
			}
		}
	}
}

float4 ObjectBase::CheckColMapColor(int _X, int _Y)
{
	float4 Pos = m_Trans->GetWorldPosition();
	Pos.y *= -1.f;

	_Y *= -1;

	float4 ColColor = COLMAPTEXTURE->GetPixel(Pos.ix() + _X, Pos.iy() + _Y);

	return ColColor;
}

bool ObjectBase::CheckColWall(int _XGap, bool _ReverseDir)
{
	float4 RenderSize = m_Render->GetTexture()->GetSize();

	bool IsWall = false;
	
	int Dir = (int)m_Dir;
	if (_ReverseDir == true)
	{
		Dir *= -1;
	}

	if (m_IsJump == true)
	{
		float4 UpPos = float4(0.f, RenderSize.y - 3.f);
		float4 FrontUpPos = float4(RenderSize.x * 0.5f * Dir, RenderSize.y - 3.f);
		float4 FrontPos = float4(FrontUpPos.x, RenderSize.y * 0.5f);

		for(int i = 0; i <= _XGap; ++i)
		{
			int XGap = i * Dir;

			float4 UpColor = CheckColMapColor(UpPos.ix() + XGap, UpPos.iy());
			float4 FrontUpColor = CheckColMapColor(FrontUpPos.ix() + XGap, FrontUpPos.iy());
			float4 FrontColor = CheckColMapColor(FrontPos.ix() + XGap, FrontPos.iy());

			if (UpColor == GJHGameEngineVector::RED
				|| UpColor == GJHGameEngineVector::GREEN
				|| FrontUpColor == GJHGameEngineVector::RED
				|| FrontUpColor == GJHGameEngineVector::GREEN
				|| FrontColor == GJHGameEngineVector::RED
				|| FrontColor == GJHGameEngineVector::GREEN)
			{
				IsWall = true;
				break;
			}
		}
	}
	else
	{
		float4 UpPos = float4(RenderSize.x * 0.5f * Dir, RenderSize.y);
		float4 DownPos = float4(UpPos.x, 0.f);

		for (int i = 0; i <= _XGap; ++i)
		{
			int XGap = i * Dir;

			float4 UpColor = CheckColMapColor(UpPos.ix() + XGap, UpPos.iy());
			float4 DownColor = CheckColMapColor(DownPos.ix() + XGap, DownPos.iy());

			if ((UpColor == GJHGameEngineVector::RED
				|| UpColor == GJHGameEngineVector::GREEN)
				&&
				(DownColor == GJHGameEngineVector::RED
				|| DownColor == GJHGameEngineVector::GREEN))
			{
				IsWall = true;
				break;
			}

			float4 FrontWallColor = CheckColMapColor((int)(RenderSize.x * 0.5f * Dir + XGap), 0);

			if (FrontWallColor == GJHGameEngineVector::CYAN)
			{
				IsWall = true;
				break;
			}
		}
	}

	return IsWall;
}

bool ObjectBase::CheckColUpDown(DIRECTION _Dir)
{
	float4 Pos = m_Trans->GetWorldPosition();
	float4 RenderSize = m_Render->GetTexture()->GetSize();

	if (_Dir == DIRECTION::UP)
	{
		float4 UpPos = float4(0.f, RenderSize.y);
		float4 FrontUpPos = float4(RenderSize.x * 0.5f * (int)m_Dir, UpPos.y);

		if (CheckColMapColor(UpPos.ix(), UpPos.iy()) == GJHGameEngineVector::RED
			|| CheckColMapColor(FrontUpPos.ix(), FrontUpPos.iy()) == GJHGameEngineVector::RED)
		{
			return true;
		}
	}
	else if (_Dir == DIRECTION::DOWN)
	{
		float4 DownPos = float4(0.f, 0.f);
		float4 FrontDownPos = float4(RenderSize.x * 0.5f * (int)m_Dir, DownPos.y);

		if (CheckColMapColor(DownPos.ix(), DownPos.iy()) == GJHGameEngineVector::RED
			|| CheckColMapColor(FrontDownPos.ix(), FrontDownPos.iy()) == GJHGameEngineVector::RED)
		{
			return true;
		}
	}

	return false;
}

void ObjectBase::RenderUpdate()
{
	if (m_IsRenderUpdate == false)
	{
		return;
	}

	float4 RenderSize = m_Render->GetTexture()->GetSize() * m_RenderScale;
	RenderSize.x *= (float)m_Dir;
	m_Render->SetLocalPositionY(RenderSize.y * 0.5f);
	m_Render->SetLocalScale(RenderSize);

	if (m_Ray != nullptr)
	{
		m_Ray->SetLocalPosition({ 0.f, RenderSize.y * 0.5f });
	}
}

void ObjectBase::RecordUpdate(bool _RewindUseCamera, float4 _RewindCameraPos)
{
	if (m_IsRecord == false)
	{
		return;
	}

	GJHRecordComponent::FrameInfo Info;
	Info.Time = GetStageTime();
	Info.ActorPos = m_Trans->GetWorldPosition();
	Info.ActorScale = m_Trans->GetWorldScale();
	Info.AnimationName = m_CurAnimationName;
	Info.AnimationFrameIndex = GetAnimationFrameIndex();
	Info.UseCamera = _RewindUseCamera;
	Info.CamPos = _RewindCameraPos;
	Info.ShowAnimation = m_Animation->GetShowAnimation();
	Info.RenderPos = m_Render->GetWorldPosition();
	Info.RenderScale = m_Render->GetWorldScale();
	Info.RenderRotation = m_Render->GetWorldRotation();
	Info.RenderEnable = m_Render->GetRenderEnable();
	Info.RenderAlpha = m_Render->GetAlpha();

	m_Record->Record(Info);
}

void ObjectBase::RewindEndFunc()
{
	if (m_Record != nullptr)
	{
		m_Record->Clear();
	}

	RewindEnd();
}

void ObjectBase::RewindEnd()
{
}

void ObjectBase::Look(GJHGameEngineActor* _Target)
{
	GJHGameEngineActor* Target = _Target->FindComponent<GJHSpriteRenderer>()->GetActor();

	float XScale = Target->FindComponent<GJHSpriteRenderer>()->GetWorldScale().x;
	DIRECTION Dir = (XScale < 0.f) ? DIRECTION::RIGHT : DIRECTION::LEFT;
	SetDirection(Dir);
}

void ObjectBase::LookOpposite(GJHGameEngineActor* _Target)
{
	GJHGameEngineActor* Target = _Target->FindComponent<GJHSpriteRenderer>()->GetActor();

	float XScale = Target->FindComponent<GJHSpriteRenderer>()->GetWorldScale().x;
	DIRECTION Dir = (XScale < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;
	SetDirection(Dir);
}

void ObjectBase::CamShake(float _Time, int _Power)
{
	m_Player->SetCamShake(_Time, _Power);
}

void ObjectBase::CheckCeiling()
{
	if (m_CheckCelling == false)
	{
		return;
	}

	if (m_IsJump == false
		&& m_IsWallGrab == false)
	{
		return;
	}

	float4 RenderSize = m_Render->GetTexture()->GetSize();

	//float4 BackUpPos = float4(RenderSize.x * -0.5f * (int)m_Dir, RenderSize.y);
	float4 UpPos = float4(0.f, RenderSize.y);
	//float4 FrontUpPos = float4(RenderSize.x * 0.5f * (int)m_Dir, RenderSize.y);
	float4 UpColor = CheckColMapColor(UpPos.ix(), UpPos.iy());
	while (UpColor == GJHGameEngineVector::RED
		   || UpColor == GJHGameEngineVector::GREEN)
	{
		m_Trans->SetWorldMove(GJHGameEngineVector::DOWN * DELTATIME(30.f, false));
		UpColor = CheckColMapColor(UpPos.ix(), UpPos.iy());
	}
}

bool ObjectBase::ShootRayToPlayer(int _Range, bool _IsConditionCheck)
{
	if (_IsConditionCheck == true)
	{
		m_RayCooltime += DELTATIME(1.f, false);
		if (0.1f <= m_RayCooltime)
		{
			m_RayCooltime = 0.f;
		}
		else
		{
			return false;
		}

		if ((m_Death == true || m_IsDetect == true)
			&& (GJHRayComponent::IsDebugRender() == false))
		{
			return false;
		}
	}

	float4 PlayerRenderScale = m_Player->FindComponent<GJHSpriteRenderer>()->GetWorldScale();
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition() + float4(0.f, PlayerRenderScale.y * 0.5f);

	float4 MyRenderScale = m_Render->GetWorldScale();
	float4 MyPos = m_Trans->GetWorldPosition() + float4(0.f, MyRenderScale.y * 0.5f);

	bool ResultCheckActor = m_Ray->CheckActor(PlayerPos, MyPos, COLMAPTEXTURE, (int)m_Dir, AngleToPlayer(), _Range);

	if (ResultCheckActor == true)
	{
		m_Ray->SetColor(float4::GREEN);
	}
	else
	{
		m_Ray->SetColor(float4::RED);
	}

	return ResultCheckActor;
}

float ObjectBase::AngleToPlayer(DIRECTION _Dir)
{
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	PlayerPos.y = ceil(PlayerPos.y);

	float4 MyPos = m_Trans->GetWorldPosition();
	MyPos.y = ceil(MyPos.y);

	return float4::CalVectorAngle360((int)_Dir, PlayerPos, MyPos);
}

float ObjectBase::XDistanceToPlayer(float _YDistance, int _Range)
{
	if (ShootRayToPlayer(_Range, false) == true)
	{
		float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
		float4 MyPos = m_Trans->GetWorldPosition();

		float4 DirVec = PlayerPos - MyPos;

		if (abs(DirVec.y) <= _YDistance)
		{
			return abs(DirVec.x);
		}
	}

	return 10000.f;
}

float4 ObjectBase::DistanceToPlayer()
{
	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	float4 DirVec = PlayerPos - MyPos;

	return DirVec;
}

void ObjectBase::DetectRay(int _Range, float _YDistance)
{
	if (200.f <= float4::CalLen(DistanceToPlayer()))
	{
		m_Ray->ClearmDebugRenderScaleX();
		return;
	}

	if (m_IsDetect == false
		&& XDistanceToPlayer(_YDistance, _Range) != 10000.f)
	{
		m_IsDetect = true;
	}
}

void ObjectBase::StopTime(float _SetRatio, float _StopTime)
{
	if (m_IsSlowMotion == true)
	{
		return;
	}

	GJHGameEngineTime::SetTimeScale(_SetRatio);
	
	m_StopTime = _StopTime;
	m_IsStopTime = true;
}

void ObjectBase::StopTimeUpdate()
{
	if (m_IsStopTime == false)
	{
		return;
	}

	m_StopTime -= DELTATIME(1.f, false);
	if (m_StopTime <= 0.f)
	{
		if (m_IsSlowMotion == false)
		{
			GJHGameEngineTime::SetTimeScale(1.f);
		}

		m_IsStopTime = false;
	}
}

void ObjectBase::CheckDeathDoorPrevPos()
{
	if (m_Death == false)
	{
		return;
	}

	std::list<std::shared_ptr<GJHGameEngineCollision>>& ColList = m_Trans->GetMasterWorld()->GetCollisionGroup(6);
	for (auto& Col : ColList)
	{
		if (Col->GetColCheck() == false)
		{
			continue;
		}

		float4 RenderPos = Col->GetTransform().GetWorldPosition();
		float4 RenderScale = Col->GetTransform().GetWorldScale();

		float RenderLeftX = RenderPos.x - RenderScale.x * 0.5f;
		float RenderRightX = RenderPos.x + RenderScale.x * 0.5f;
		float RenderTopY = RenderPos.y + RenderScale.y * 0.5f;
		float RenderBottomY = RenderPos.y - RenderScale.y * 0.5f;

		float4 MyPos = m_Trans->GetWorldPosition();
		MyPos.y += m_Render->GetLocalScale().y * 0.5f;

		if (RenderLeftX <= MyPos.x
			&& MyPos.x <= RenderRightX
			&& MyPos.y <= RenderTopY
			&& RenderBottomY <= MyPos.y)
		{
			m_Trans->SetWorldPosition(m_PrevPos);
			break;
		}
	}
}

bool ObjectBase::CheckDoorCol()
{
	std::list<std::shared_ptr<GJHGameEngineCollision>>& ColList = m_Trans->GetMasterWorld()->GetCollisionGroup(6);
	for (auto& Col : ColList)
	{
		if (Col->GetColCheck() == false)
		{
			continue;
		}

		float4 RenderPos = Col->GetTransform().GetWorldPosition();
		float4 RenderScale = Col->GetTransform().GetWorldScale();

		float RenderLeftX = RenderPos.x - RenderScale.x * 0.5f;
		float RenderRightX = RenderPos.x + RenderScale.x * 0.5f;
		float RenderTopY = RenderPos.y + RenderScale.y * 0.5f;
		float RenderBottomY = RenderPos.y - RenderScale.y * 0.5f;

		float4 LocalScale = m_Render->GetLocalScale();

		float4 MyPos = m_Trans->GetWorldPosition();
		MyPos.x += LocalScale.x * 0.5f;
		MyPos.y += LocalScale.y * 0.5f;

		if (RenderLeftX <= MyPos.x
			&& MyPos.x <= RenderRightX
			&& MyPos.y <= RenderTopY
			&& RenderBottomY <= MyPos.y)
		{
			return true;
		}
	}

	return false;
}

void ObjectBase::SetMoveSpeed(float _Value)
{
	m_MoveSpeed = _Value;
}

void ObjectBase::MoveNearNormalPos()
{
	int Left = 0;
	int Right = 0;

	float4 MyPos = m_Trans->GetWorldPosition();

	float4 MovePos;
	while (true)
	{
		float4 LeftColor = CheckColMapColor(Left, 0);
		float4 RightColor = CheckColMapColor(Right, 0);
		if ((LeftColor == float4(0.f, 0.f, 0.f, 1.f) || LeftColor == float4::BLUE)
			&& 0.f < MyPos.x + Left)

		{
			MovePos = float4((float)Left, 0);
			break;
		}
		else if ((RightColor == float4(0.f, 0.f, 0.f, 1.f) || RightColor == float4::BLUE)
				 && MyPos.x + Left < COLMAPTEXTURE->GetSizeX())
		{
			MovePos = float4((float)Right, 0);
			break;
		}

		Left--;
		Right++;
	}

	m_Trans->SetWorldMove(MovePos);
}

float4 ObjectBase::GetMousePos()
{
	float4 MousePos = GJHGameEngineWindow::GetMousePos();
	float4 WindowSize = GJHGameEngineWindow::GetSize();

	MousePos.x /= (WindowSize.x / CAMSIZEX);
	MousePos.y /= (WindowSize.y / CAMSIZEY);

	MousePos.y += m_Render->GetTexture()->GetSizeY() * 0.5f;

	MousePos.y *= -1.f;

	return MousePos;
}