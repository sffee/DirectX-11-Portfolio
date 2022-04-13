#pragma once
#include "MonsterBase.h"

class InstanceObject;
class MonsterBullet;
class ShieldCop : public MonsterBase
{
public:
	ShieldCop();
	~ShieldCop();

public:
	ShieldCop(const ShieldCop& _Other) = delete;
	ShieldCop(const ShieldCop&& _Other) = delete;

public:
	ShieldCop operator=(const ShieldCop& _Other) = delete;
	ShieldCop operator=(const ShieldCop&& _Other) = delete;

private:
	GJHStateUpdater<ShieldCop> State;
	std::shared_ptr<InstanceObject> m_LeftArm;
	std::shared_ptr<GJHSpriteRenderer> m_LeftArmRender;
	std::shared_ptr<InstanceObject> m_RightArm;
	std::shared_ptr<GJHSpriteRenderer> m_RightArmRender;
	std::shared_ptr<InstanceObject> m_Gun;
	std::shared_ptr<GJHSpriteRenderer> m_GunRender;

private:
	std::vector<std::shared_ptr<MonsterBullet>> m_BulletList;
	int m_BulletIndex;
	float m_FireCooltime;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;
	void SceneChangeStart() override;

private:
	void Fire();
	void CreateGunFireEffect(float _Angle);
	void CreateBullet();

private:
	void Idle_Init();
	void Idle_State();

	void Walk_Init();
	void Walk_State();

	void Run_Init();
	void Run_State();

	void Aim_Init();
	void Aim_State();
	void Aim_Leave();

	void AimCoolTime_Init();
	void AimCoolTime_State();

	void Bash_Init();
	void Bash_State();

	void KnockBack_Init();
	void KnockBack_State();

	void Turn_Init();
	void Turn_State();

	void HurtFly_Init();
	void HurtFly_State();

	void HurtGround_Init();
	void HurtGround_State();

	void DoorIn_Init();
	void DoorIn_State();

	void DoorOut_Init();
	void DoorOut_State();

	void PatrolTurn_Init();
	void PatrolTurn_State();

	void Patrol_Init();
	void Patrol_State();
};