#pragma once
#include "MonsterBase.h"

class InstanceObject;
class MonsterBullet;
class Cop : public MonsterBase
{
public:
	Cop();
	~Cop();

public:
	Cop(const Cop& _Other) = delete;
	Cop(const Cop&& _Other) = delete;

public:
	Cop operator=(const Cop& _Other) = delete;
	Cop operator=(const Cop&& _Other) = delete;

private:
	GJHStateUpdater<Cop> State;
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
	bool m_IsFireRotationUp;
	bool m_IsFireRotationDown;
	float m_FireGunRotationTime;

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
	void CreateLeftArm();
	void CreateRightArm();
	void CreateGun();
	void CreateBullet();
	void Fire();
	void CreateGunFireEffect(float _Angle);

private:
	void Idle_Init();
	void Idle_State();

	void Walk_Init();
	void Walk_State();

	void Run_Init();
	void Run_State();

	void Attack_Init();
	void Attack_State();
	void Attack_Leave();

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