#pragma once
#include "MonsterBase.h"

class Grunt : public MonsterBase
{
public:
	Grunt();
	~Grunt();

public:
	Grunt(const Grunt& _Other) = delete;
	Grunt(const Grunt&& _Other) = delete;

public:
	Grunt operator=(const Grunt& _Other) = delete;
	Grunt operator=(const Grunt&& _Other) = delete;

private:
	GJHStateUpdater<Grunt> State;

private:
	std::shared_ptr<GJHGameEngineActor> m_SlashActor;
	bool m_CreateSlashEffect;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

private:
	void CreateSlashEffect();

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;
	void SceneChangeStart() override;

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