#pragma once
#include "ObjectBase.h"

class SpectrumCom;
class LaserLine;
class MonsterBullet;
class KnifeDashLine;
class Grenade;
class HeadHunter2 : public ObjectBase
{
public:
	HeadHunter2();
	~HeadHunter2();

public:
	HeadHunter2(const HeadHunter2& _Other) = delete;
	HeadHunter2(const HeadHunter2&& _Other) = delete;

public:
	HeadHunter2 operator=(const HeadHunter2& _Other) = delete;
	HeadHunter2 operator=(const HeadHunter2&& _Other) = delete;

public:
	GJHGameEngineString m_NextPattern;

public:
	GJHStateUpdater<HeadHunter2> State;
	std::shared_ptr<SpectrumCom> m_Spectrum;

private:
	std::vector<std::shared_ptr<LaserLine>> m_LaserLineList;
	int m_LaserLineIndex;
	bool m_AimFire;
	int m_AimLaserCount;

private:
	std::vector<std::shared_ptr<MonsterBullet>> m_BulletList;
	float m_JumpBulletAngle;
	float m_JumpBulletTime;
	int m_BulletIndex;

private:
	std::shared_ptr<KnifeDashLine> m_KnifeDashLine;
	float4 m_DashMoveVec;

private:
	std::vector<std::shared_ptr<Grenade>> m_GrenadeList;
	int m_GrenadeIndex;
	int m_GrenadeCount;

private:
	std::vector<float4> Hit2TeleportPos;
	int m_TeleportCount;

private:
	float m_Time;
	bool m_SceneEnd;
	int m_PrevNormalPatternIndex;

private:
	float m_SweepAngle;
	float m_DodgeCooltime;
	bool m_Hit2LaserFire;
	bool m_SoundPlay;

private:
	int m_HurtCount;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;
	
private:
	void CalcCooltime();
	void AddLaserLineIndex();
	void AddGrenadeCount();
	void LookPlayer();

private:
	void RandomPattern();
	bool Escape();
	void Aim();
	void CreateGunFireEffect(float _Angle);
	void CreateBullet(float _Angle);
	void CreateGrenade();
	void CreateHurtEffect();
	void PlayAppearSound();
	void PlayHurtSound();

private:
	void Idle_Init();
	void Idle_State();

	void TakeOutRifle_Init();
	void TakeOutRifle_State();

	void Aim_Rifle_Init();
	void Aim_Rifle_State();

	void Aim_Rifle_Cooltime_Init();
	void Aim_Rifle_Cooltime_State();

	void PutBackRifle_Init();
	void PutBackRifle_State();

	void PreDash_Init();
	void PreDash_State();

	void Dash_Init();
	void Dash_State();
	void Dash_Leave();

	void Dash_End_Ground_Init();
	void Dash_End_Ground_State();

	void TakeOutGun_Init();
	void TakeOutGun_State();

	void Shoot_Init();
	void Shoot_State();

	void Shoot_Cooltime_Init();
	void Shoot_Cooltime_State();

	void PutBackGun_Init();
	void PutBackGun_State();

	void DodgeRoll_Init();
	void DodgeRoll_State();

	void PreJump_Init();
	void PreJump_State();

	void Jump_Init();
	void Jump_State();

	void Wall_Idle_Init();
	void Wall_Idle_State();

	void Wall_Jump_Init();
	void Wall_Jump_State();

	void Wall_Jump_Land_Init();
	void Wall_Jump_Land_State();

	void Hit1_TeleportIn_Sweep_Init();
	void Hit1_TeleportIn_Sweep_State();

	void Hit1_Sweep_Init();
	void Hit1_Sweep_State();

	void Hit1_Dash_Init();
	void Hit1_Dash_State();
	void Hit1_Dash_Leave();

	void Hit1_Dash_End_Ground_Init();
	void Hit1_Dash_End_Ground_State();

	void Hit2_TeleportIn_Init();
	void Hit2_TeleportIn_State();

	void Hit2_TeleportOut_Init();
	void Hit2_TeleportOut_State();

	void Hit2_Left_TeleportIn_Sweep_Init();
	void Hit2_Left_TeleportIn_Sweep_State();

	void Hit2_Left_Sweep_Init();
	void Hit2_Left_Sweep_State();

	void Hit2_Left_TeleportOut_Sweep_Init();
	void Hit2_Left_TeleportOut_Sweep_State();

	void Hit2_Right_TeleportIn_Sweep_Init();
	void Hit2_Right_TeleportIn_Sweep_State();

	void Hit2_Right_Sweep_Init();
	void Hit2_Right_Sweep_State();

	void Hit2_Right_TeleportOut_Sweep_Init();
	void Hit2_Right_TeleportOut_Sweep_State();

	void Hit2_TeleportIn_Ground_Init();
	void Hit2_TeleportIn_Ground_State();

	void Hit2_TeleportOut_Ground_Init();
	void Hit2_TeleportOut_Ground_State();

	void Hit2_Wall_TeleportIn_Init();
	void Hit2_Wall_TeleportIn_State();

	void Hurt_Init();
	void Hurt_State();

	void Hurt_Invisible_Init();
	void Hurt_Invisible_State();

	void DieFly_Init();
	void DieFly_State();

	void DieLand_Init();
	void DieLand_State();
};