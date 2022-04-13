#pragma once
#include <GJHContentsComponent.h>
#include <GJHCollisionComponent.h>
#include <GJHStateUpdater.h>
#include <GJHSpriteRenderer.h>
#include <GJHSpriteAnimation.h>
#include <GJHRecordComponent.h>
#include <GJHRayComponent.h>
#include <GJHGameEngineScene.h>
#include "StageBase.h"
#include "LogicValue.h"
#include "define.h"
#include <GJHGameEngineSound.h>

#define COLMAPTEXTURE StageBase::m_ColMapTexture

class Player;
class GJHDirectTexture2D;
class ObjectBase : public GJHContentsComponent
{
protected:
	static bool m_IsSlowMotion;

protected:
	enum class DIRECTION
	{
		NONE = 0,
		LEFT = -1,
		RIGHT = 1,
		UP = 1,
		DOWN = -1
	};

	enum class MOVESPEED
	{
		DOWN = -1,
		UNIFORM,
		UP,
		STOP
	};

	struct InstanceObjectData
	{
		float4 Pos;
		float4 Scale;
		DIRECTION Dir;
		float DirVecMovePos;
		float Angle;
		GJHGameEngineString AnimationName;
		GJHGameEngineString TextureName;
		float AnimationFrameTime;
		bool AnimationLoop;
		bool AnimationEndHide;
		bool ApplyGravity;
		bool IsJump;
		float Gravity;
		float GravityAddValue;
		bool UseTimeScale;
		float LifeTime;
		bool IsDeath;
		bool IsRecord;
		bool IsRenderUpdate;
		GJHGameEngineString MeshName;
		ACTORTYPE ActorType;
		bool IsGravityCheckMapColor;
		float MoveSpeed;
		bool CheckWall;
		float4 RenderScale;
		int RenderOrder;

		InstanceObjectData() :
			Pos({ 0.f, 0.f, 0.f }),
			Scale({ 1.f, 1.f, 1.f }),
			Dir(DIRECTION::LEFT),
			DirVecMovePos(0.f),
			AnimationName(),
			TextureName(),
			AnimationFrameTime(0.1f),
			AnimationLoop(false),
			ApplyGravity(false),
			UseTimeScale(true),
			Angle(0.f),
			LifeTime(0.f),
			IsDeath(true),
			IsRecord(false),
			MeshName(),
			IsRenderUpdate(true),
			AnimationEndHide(false),
			ActorType(ACTORTYPE::NONE),
			IsJump(false),
			Gravity(0.f),
			GravityAddValue(0.f),
			IsGravityCheckMapColor(true),
			MoveSpeed(0),
			CheckWall(true),
			RenderScale({ 1.f, 1.f }),
			RenderOrder(0)
		{
		}
	};

public:
	static Player* m_Player;

protected:
	static bool m_RewindOn;
	static float m_RewindTime;

protected:
	float m_StopTime;
	bool m_IsStopTime;

protected:
	float m_Time;

protected:
	std::shared_ptr<GJHSpriteRenderer> m_Render;
	std::shared_ptr<GJHActorTransform> m_Trans;
	std::shared_ptr<GJHCollisionComponent> m_Collision;
	std::shared_ptr<GJHCollisionComponent> m_AttackCollision;
	std::shared_ptr<GJHSpriteAnimation> m_Animation;
	std::shared_ptr<GJHRecordComponent> m_Record;
	std::shared_ptr<GJHRayComponent> m_Ray;
	float4 m_RenderScale;

public:
	bool m_CheckPlatform;

private:
	GJHGameEngineString m_CurAnimationName;

protected:
	int m_CheckMoveColSizeX;
	bool m_UseTimeScale;
	bool m_IsDetect;
	bool m_Death;
	float m_RayCooltime;

protected:
	DIRECTION m_Dir;
	float4 m_LocalScale;
	float4 m_PrevPos;
	float4 m_RenderPosPivot;
	bool m_IsJump;
	bool m_IsFalling;
	bool m_OnPlatform;
	bool m_IgnorePlatform;
	bool m_IsWallGrab;
	bool m_IsRenderUpdate;

protected:
	float m_MoveSpeed;
	float m_XMoveRatio;
	float m_MinXRatio;
	float m_MaxXRatio;
	float m_MoveTimer;
	float m_MoveTimerVelocity;
	float m_AddXMoveRatio;
	MOVESPEED m_MoveSpeedType;

protected:
	float m_Gravity;
	float m_GravityAddValue;
	bool m_ApplyGravity;
	bool m_EnableGravity;
	bool m_IsGravityCheckMapColor;

protected:
	bool m_CheckCelling;
	bool m_CheckWall;

protected:
	bool m_IsRecord;

public:
	ObjectBase();
	~ObjectBase();

public:
	ObjectBase(const ObjectBase& _Other) = delete;
	ObjectBase(const ObjectBase&& _Other) = delete;

public:
	ObjectBase operator=(const ObjectBase& _Other) = delete;
	ObjectBase operator=(const ObjectBase&& _Other) = delete;

protected:
	void CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _TextureName, int _StartFrame, int _EndFrame, float _FrameTime, bool _Loop = false, bool _IsPiece = true);
	void CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _TextureName, float _FrameTime, bool _Loop = false, bool _IsPiece = true);
	void ChangeAnimation(const GJHGameEngineString& _AnimationName);
	
protected:
	template <typename T>
	void CreateCollision(int _Order, COLTYPE _Type, void(T::* _EnterFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _StayFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _ExitFunc)(GJHCollisionComponent*, GJHCollisionComponent*), T& _Obj)
	{
		m_Collision = CreateComponent<GJHCollisionComponent>(_Order, _Type);

		if (_EnterFunc != nullptr)
		{
			m_Collision->AddEnterEvent(_EnterFunc, _Obj);
		}

		if (_StayFunc != nullptr)
		{
			m_Collision->AddStayEvent(_StayFunc, _Obj);
		}

		if (_ExitFunc != nullptr)
		{
			m_Collision->AddExitEvent(_ExitFunc, _Obj);
		}
	}

	template <typename T>
	void CreateAttackCollision(int _Order, COLTYPE _Type, void(T::* _EnterFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _StayFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _ExitFunc)(GJHCollisionComponent*, GJHCollisionComponent*), T& _Obj)
	{
		m_AttackCollision = CreateComponent<GJHCollisionComponent>(_Order, _Type);

		if (_EnterFunc != nullptr)
		{
			m_AttackCollision->AddEnterEvent(_EnterFunc, _Obj);
		}

		if (_StayFunc != nullptr)
		{
			m_AttackCollision->AddStayEvent(_StayFunc, _Obj);
		}

		if (_ExitFunc != nullptr)
		{
			m_AttackCollision->AddExitEvent(_ExitFunc, _Obj);
		}
	}
	
protected:
	template<typename T, typename ... REST>
	std::shared_ptr<T> CreateEffect(const GJHGameEngineString& _TexName, const float4& _Position, const float4& _Scale = float4(1.f, 1.f), bool _AttachParent = false, REST ... _Arg)
	{
		std::shared_ptr<GJHGameEngineActor> NewActor = GJHContentsSceneComponent::CreateEffect<T>(_TexName, _Position, _Scale, GetActor(), _Arg...);
		std::shared_ptr<GJHActorTransform> NewActorTransform = NewActor->FindComponent<GJHActorTransform>();

		if (_AttachParent == true)
		{
			NewActorTransform->AttachParent(GetActor()->FindComponent<GJHActorTransform>());
		}
		else
		{
			float4 LocalPos = NewActorTransform->GetLocalPosition();

			NewActorTransform->SetLocalPosition({ LocalPos.x + CAMSIZEX * 0.5f, LocalPos.y + CAMSIZEY * -0.5f, _Position.z });
		}
		
		return NewActor->FindComponent<T>();
	}

	template<typename T, typename ... REST>
	std::shared_ptr<T> CreateEffect(const GJHGameEngineString& _TexName, const float4& _Position, const float _Scale, bool _AttachParent = false, REST ... _Arg)
	{
		return CreateEffect<T>(_TexName, _Position, { _Scale, _Scale }, _AttachParent, _Arg...);
	}

protected:
	std::shared_ptr<GJHGameEngineActor> CreateInstanceObject(const InstanceObjectData& _Data, bool _AttachParent = false);
	
protected:
	bool AnimationEnd();
	void AnimationReset();
	void SetAnimationFrameIndex(int _Index);
	int GetAnimationFrameIndex();
	void SetShowAnimation(bool _Set);

public:
	void SetDirection(int _Dir);
	void SetDirection(DIRECTION _Dir);
	DIRECTION GetDirection();
	DIRECTION GetOppositeDirection();
	void ChangeDirection();

protected:
	void Move();
	void Move(const float4& _Value, bool _SkipWallCheck = false);
	void ChangeMoveRatioToMoveType();
	void Look(GJHGameEngineActor* _Target);
	void LookOpposite(GJHGameEngineActor* _Target);
	void CamShake(float _Time, int _Power);

protected:
	void Gravity();
	void Platform();
	float4 CheckColMapColor(int _X, int _Y);
	bool CheckColWall(int _XGap = 0, bool _ReverseDir = false);
	bool CheckColUpDown(DIRECTION _Dir);
	void CheckCeiling();
	void CheckDeathDoorPrevPos();
	bool CheckDoorCol();

private:
	void StopTimeUpdate();

protected:
	void RenderUpdate();
	void RecordUpdate(bool _RewindUseCamera = false, float4 _RewindCameraPos = float4(0.f, 0.f, 0.f, 0.f));

protected:
	void StopTime(float _SetRatio, float _StopTime);

protected:
	bool ShootRayToPlayer(int _Range, bool _IsConditionCheck = true);
	float AngleToPlayer(DIRECTION _Dir = DIRECTION::RIGHT);
	float XDistanceToPlayer(float _YDistance, int _Range = 500);
	float4 DistanceToPlayer();
	void DetectRay(int _Range, float _YDistance);

protected:
	void MoveNearNormalPos();
	float4 GetMousePos();

public:
	void SetMoveSpeed(float _Value);
	void Jump(float _Gravity, float _GravityAddValue, bool _OneUpMove = true);
	void MoveSpeedUpStart(float _StartXRatio = 0.f, float _MoveTimerVelocity = 0.05f, float _MaxXRatio = 1.f);
	void MoveSpeedDownStart(float _MoveTimerVelocity = 0.05f, float _StartXRatio = -1.f, float _MinXRatio = 0.f);
	void MoveSpeedUniform();
	void MoveSpeedStop();

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update(bool _RewindUseCamera = false, float4 _RewindCameraPos = float4(0.f, 0.f, 0.f, 0.f));
	void RewindEndFunc();
	virtual void RewindEnd();
};