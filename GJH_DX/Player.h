#pragma once
#include <GJHCamera.h>
#include "Door.h"
#include "ObjectBase.h"
#include <functional>

class GJHSpriteRenderer;
class PlayerSlash;
class SpectrumCom;
class InstanceObject;
class UIManager;
class StageBase;
class Player : public ObjectBase
{
private:
	static float MOVESPEED;
	static float ROLL_MOVESPEED;
	static float ATTACK_MOVESPEED;
	static float FLIP_MOVESPEED;

public:
	static float m_BatteryGauge;
	static float m_TimeGauge;

private:
	static bool m_InifinityBattery;
	static bool m_IsNotDie;

public:
	static bool GetInfinityBattery()
	{
		return m_InifinityBattery;
	}

	static bool GetIsNotDie()
	{
		return m_IsNotDie;
	}

private:
	float m_FadeInValue;
	bool m_FadeInStart;

public:
	std::shared_ptr<GJHCamera> m_Cam;
	std::shared_ptr<GJHCamera> m_UICam;
	std::shared_ptr<GJHActorTransform> m_CamTrans;
	bool m_IsCamShake;
	float m_CamShakeTime;
	int m_CamShakePower;

public:
	void SetCamera(std::shared_ptr<GJHCamera> _Cam, std::shared_ptr<GJHCamera> _UICam)
	{
		m_Cam = _Cam;
		m_UICam = _UICam;
		m_CamTrans = m_Cam->GetActor()->FindComponent<GJHActorTransform>();
	}

	void SetCamShake(float _Time, int _Power);

private:
	GJHStateUpdater<Player> State;

private:
	std::shared_ptr<GJHCollisionComponent> m_FakeCollision;

private:
	float4 m_AttackMoveVec;

private:
	std::shared_ptr<PlayerSlash> m_SlashEffect;
	std::shared_ptr<SpectrumCom> m_Spectrum;

private:
	bool m_ChangeDirEnable;
	bool m_AttackSlashEffect;
	bool m_IsDoorKick;
	bool m_KnockBack;
	float m_KnockBackMoveBlockTime;
	bool m_Attack;
	float m_Time;
	float m_WallSlideSoundTime;
	float m_RewindSpeed;
	float m_RewindAddSpeed;
	float m_RewindAddDeltaTime;
	std::function<void(ObjectBase::DIRECTION)> m_DoorKickFunc;

private:
	std::vector<std::shared_ptr<InstanceObject>> m_ExplosionList;
	std::shared_ptr<InstanceObject> m_JumpCloud;
	std::shared_ptr<InstanceObject> m_LandCloud;

private:
	std::vector<std::shared_ptr<InstanceObject>> m_DustCloudList;
	int m_DustCloudIndex;
	int m_DustCloudCount;
	
public:
	Player();
	~Player();

public:
	Player(const Player& _Other) = delete;
	Player(const Player&& _Other) = delete;

public:
	Player operator=(const Player& _Other) = delete;
	Player operator=(const Player&& _Other) = delete;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

	void FakeColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void FakeColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void FakeColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;
	void SceneChangeStart() override;
	void GUIUpdate();
	void UIUpdate();
	void SlowTimeUpdate();
	void InvisibleUpdate();

private:
	void Move();
	void Move(const float4& _Value);
	void CamMove();
	void CheckDirection();
	void CheckFalling();
	void CheckGlobalKey();
	void CheckCeiling();
	void Platform();
	void Rewind(float _RewindAddDeltaTime = 0.2f, bool _UseUvShader = true);

public:
	void KnockBack();
	void Damage(GJHGameEngineActor* _Other);
	void ExplosionDamage(GJHGameEngineActor* _Other);
	void LaserDamage(GJHGameEngineActor* _Other);
	void CreateExplosion();
	void Explosion();
	void CreateDustCloud();
	void CreateJumpCloud();
	void CreateLandCloud();
	void DustCloud();
	void JumpCloud();
	void LandCloud();
	void WallCloud();
	void PlaySlashSound();

private:
	void Idle_Init();
	void Idle_State();

	void MoveStart_Init();
	void MoveStart_State();

	void Move_Init();
	void Move_State();

	void MoveEnd_Init();
	void MoveEnd_State();

	void Jump_Init();
	void Jump_State();
	void Jump_Leave();

	void Fall_Init();
	void Fall_State();

	void SitStart_Init();
	void SitStart_State();

	void SitLoop_Init();
	void SitLoop_State();

	void SitEnd_Init();
	void SitEnd_State();

	void Roll_Init();
	void Roll_State();
	void Roll_Leave();

	void WallGrab_Init();
	void WallGrab_State();
	void WallGrab_Leave();

	void Attack_Init();
	void Attack_State();
	void Attack_Leave();

	void DoorKick_Init();
	void DoorKick_State();
	void DoorKick_Leave();

	void HurtFlyStart_Init();
	void HurtFlyStart_State();

	void HurtFly_Init();
	void HurtFly_State();

	void HurtLand_Init();
	void HurtLand_State();

	void Flip_Init();
	void Flip_State();
	void Flip_Leave();

	void WakeUp_Init();
	void WakeUp_State();
	void WakeUp_Leave();

	void Shield_HurtFlyStart_Init();
	void Shield_HurtFlyStart_State();

	void Shield_HurtLand_Init();
	void Shield_HurtLand_State();

	void Shield_WakeUp_Init();
	void Shield_WakeUp_State();
};