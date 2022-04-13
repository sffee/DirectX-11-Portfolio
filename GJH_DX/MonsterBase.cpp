#include "MonsterBase.h"
#include "AStarManager.h"
#include "Player.h"
#include "GJHWorldComponent.h"
#include "define.h"

int MonsterBase::m_MonsterCount = 0;

MonsterBase::MonsterBase() :
	m_RenderColor(1.f),
	m_IsDamage(false),
	m_DamageMoveTime(0.f),
	m_ChangeDirection(false),
	m_OnStairway(false),
	m_OnDoor(false),
	m_PlayDetectEffect(false)
{

}

MonsterBase::~MonsterBase()
{

}

void MonsterBase::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	ObjectBase::Start(_Renderer, _RenderScale);
	m_RenderColor = 1.f;
}

void MonsterBase::Update()
{
	ObjectBase::Update();
	DetectEffect();
	AStarUpdate();
	MovePosUpdate();
	KnockBackUpdate();
	CheckDeathDoorPrevPos();
}

void MonsterBase::DetectEffect()
{
	if (m_IsDetect == false
		|| m_PlayDetectEffect == true)
	{
		return;
	}

	m_PlayDetectEffect = true;
	CreateDetectEffect();
}

void MonsterBase::Damage(float _Angle)
{
	m_SlashImpactAngle = _Angle;
	m_SlashImpactCreate = false;
	m_CheckPlatform = true;

	KnockBack(_Angle);
	CreateSlashSuccess();
}

void MonsterBase::CreateSlashSuccess()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Attack";
	Data.TextureName = L"Player_SlashSuccess";
	Data.Pos = float4(-5.f * (float)m_Dir, -3.f);
	Data.Dir = m_Dir;
	Data.UseTimeScale = false;
	Data.AnimationFrameTime = 0.06f;
	Data.ActorType = ACTORTYPE::EFFECT;
	
	CreateInstanceObject(Data, true);
}

void MonsterBase::CreateSlashImpact(float _Angle)
{
	InstanceObjectData Data;
	Data.AnimationName = L"Attack";
	Data.TextureName = L"Player_SlashImpact";
	Data.Dir = GetOppositeDirection();
	Data.Pos = m_Trans->GetWorldPosition();
	Data.DirVecMovePos = -30.f * (float)m_Dir;
	Data.UseTimeScale = false;
	Data.Angle = _Angle;
	Data.AnimationFrameTime = 0.07f;
	Data.ActorType = ACTORTYPE::EFFECT;

	CreateInstanceObject(Data, false);
}

void MonsterBase::CreateDetectEffect()
{
	InstanceObjectData Data;
	Data.AnimationName = L"Detect";
	Data.TextureName = L"Monster_Player_Detect";
	Data.Dir = m_Dir;
	Data.Pos = float4(0.f, m_Render->GetWorldScale().y) - float4(0.f, 10.f);
	Data.AnimationFrameTime = 0.2f;
	Data.LifeTime = 2.f;
	Data.ActorType = ACTORTYPE::EFFECT;

	CreateInstanceObject(Data, true);
}

void MonsterBase::KnockBack(float _Angle, float _MoveSpeed)
{
	m_IsDamage = true;

	m_MoveSpeed = _MoveSpeed;
	MoveSpeedUniform();
	m_UseTimeScale = false;
	m_DamageMoveTime = 0.03f;

	m_CheckMoveColSizeX = 0;

	if (_Angle != 0.f)
	{
		if (m_Dir == DIRECTION::LEFT)
		{
			_Angle += 180.f;
		}

		m_KnockBackVec = float4::GlobalDirToDeg(_Angle);
	}

	float YJumpPower = 0.f;
	if (0.2f <= m_KnockBackVec.y)
	{
		YJumpPower = m_KnockBackVec.y;
		m_IsDamageFly = true;
		ObjectBase::Jump(YJumpPower * -400.f, 900.f);
	}
	else
	{
		m_IsDamageFly = false;
	}
}

void MonsterBase::KnockBackUpdate(float _MoveSpeed)
{
	if (m_IsDamage == true)
	{
		m_EnableGravity = true;

		m_DamageMoveTime -= DELTATIME(1.f, false);
		if (m_DamageMoveTime <= 0.f)
		{
			CreateSlashImpact(m_SlashImpactAngle);

			m_MoveSpeed = _MoveSpeed;
			m_UseTimeScale = true;
			m_IsDamage = false;
			if (0.2f <= m_KnockBackVec.y)
			{
				MoveSpeedDownStart(0.005f, 1.f, 0.3f);
			}
			else
			{
				MoveSpeedDownStart(0.01f, 1.f);
			}
		}
	}

	if (m_IsDamageFly == true)
	{
		if (m_KnockBackVec != float4::ZERO)
		{
			ObjectBase::Move(float4(m_KnockBackVec.x * DELTATIME(50.f), 0.f), true);
		}

		int RenderSize = (int)(abs(m_Render->GetWorldScale().y - 1.f));
		if (CheckColMapColor(0, RenderSize + 3) == GJHGameEngineVector::RED)
		{
			m_Gravity = 10.f;
		}

		if (CheckColMapColor(0, 0) == GJHGameEngineVector::BLUE)
		{
			while (CheckColMapColor(0, 0) == GJHGameEngineVector::BLUE)
			{
				m_Trans->SetWorldMove(float4::UP * DELTATIME(10.f, false));
			}

			m_IsDamageFly = false;
			m_EnableGravity = false;
			MoveSpeedStop();
		}

		if (CheckColMapColor(0, -1) == GJHGameEngineVector::RED)
		{
			m_IsDamageFly = false;
			MoveSpeedStop();
		}
	}

	if (m_Death == true)
	{
		if (CheckColWall(2) == true)
		{
			m_KnockBackVec *= 0.3f;
			ChangeDirection();
		}
	}
}

void MonsterBase::StairwayDownMove()
{
	int YPos = 0;
	bool IsUpDown = true;
	while (CheckColMapColor(0, YPos) != GJHGameEngineVector::RED)
	{
		--YPos;

		if (100 <= abs(YPos))
		{
			YPos = 0;
			IsUpDown = false;
			break;
		}
	}

	if (IsUpDown == true)
	{
		m_Trans->SetWorldMove({ 0.f, (float)YPos });
	}

	float4 RenderSize = m_Render->GetWorldScale();
	float4 WorldPos = m_Trans->GetWorldPosition();
	int LeftXPos = (int)(RenderSize.x * -0.5f);
	int RightXPos = (int)(RenderSize.x * 0.5f);

	while (CheckColMapColor(LeftXPos, 0) == GJHGameEngineVector::RED
		   && CheckColMapColor(RightXPos, 0) == GJHGameEngineVector::RED)
	{
		m_Trans->SetWorldMove(GJHGameEngineVector::UP * DELTATIME(2.f, false));
	}
}

void MonsterBase::AStarUpdate()
{
	if (m_IsDetect == false)
	{
		return;
	}

	float4 PlayerPos = m_Player->GetActorTrans()->GetWorldPosition();
	float4 MyPos = m_Trans->GetWorldPosition();

	if (m_OnStairway == false)
	{
		m_AStarPath = AStarManager::FindPath(MyPos, PlayerPos);

		if (1 < m_AStarPath.size()
			&& AStarManager::GetTile(MyPos).Type == AStarManager::TYPE::STAIRWAYINOUT
			&& m_AStarPath[0].second == (int)AStarManager::TYPE::STAIRWAY)
		{
			m_CheckPlatform = false;
			m_OnStairway = true;
		}
	}
	else
	{
		float4 PlayerTilePos = AStarManager::GetNearNoneTile(PlayerPos) - float4(0.f, TILESIZE);
		float4 MyTilePos = AStarManager::GetNearNoneTile(MyPos);

		if (PlayerTilePos.y < MyTilePos.y
			|| (m_AStarPath.empty() == false
				&& (m_AStarPath[0].second == (int)AStarManager::TYPE::STAIRWAY || m_AStarPath[0].second == (int)AStarManager::TYPE::STAIRWAYINOUT)))
		{
			m_CheckPlatform = false;
			StairwayDownMove();
			MyPos = m_Trans->GetWorldPosition();
		}
		else
		{
			m_CheckPlatform = true;
		}

		m_AStarPath = AStarManager::FindPath(MyPos, PlayerPos);

		if (AStarManager::GetTile(MyPos).Type == AStarManager::TYPE::NONE
			|| AStarManager::GetTile(MyPos).Type == AStarManager::TYPE::STAIRWAYINOUT)
		{
			m_CheckPlatform = true;
			m_OnStairway = false;
		}
	}
}

void MonsterBase::MovePosUpdate()
{
	if (m_IsDetect == false
		|| m_AStarPath.empty())
	{
		return;
	}

	float4 MyPos = m_Trans->GetWorldPosition();

	if (AStarManager::GetTile(MyPos).Type == AStarManager::TYPE::DOOR
		&& m_AStarPath.front().second == (int)AStarManager::TYPE::DOOR)
	{
		float MyTileXPos = AStarManager::GetTile(MyPos).WorldPos.x;
		float DoorDirVecX = MyPos.x - MyTileXPos;
		if (-5.f <= DoorDirVecX && DoorDirVecX <= 5.f)
		{
			m_OnDoor = true;
			m_DoorTeleportPos = m_AStarPath.front().first + float4(0.f, -TILESIZE);
		}
	}
	else
	{
		m_OnDoor = false;
	}

	float4 TargetPos = m_AStarPath.front().first;
	if (m_OnStairway == true
		&& m_AStarPath.front().second == (int)AStarManager::TYPE::STAIRWAYINOUT)
	{
		TargetPos.x += TILESIZE * 1.5f * (float)m_Dir;
	}

	float4 DirVec = TargetPos - MyPos;
	DIRECTION MoveDir = (DirVec.x < 0.f) ? DIRECTION::LEFT : DIRECTION::RIGHT;

	if (DirVec.y <= TILESIZE)
	{
		m_ChangeDirection = (m_Dir != MoveDir) ? true : false;
	}
}