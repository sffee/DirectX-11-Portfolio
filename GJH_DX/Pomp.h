#pragma once
#include "MonsterBase.h"

class Pomp : public MonsterBase
{
public:
	Pomp();
	~Pomp();

public:
	Pomp(const Pomp& _Other) = delete;
	Pomp(const Pomp&& _Other) = delete;

public:
	Pomp operator=(const Pomp& _Other) = delete;
	Pomp operator=(const Pomp&& _Other) = delete;

private:
	GJHStateUpdater<Pomp> State;

private:
	std::shared_ptr<GJHGameEngineActor> m_SlashActor;
	float m_Time;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;
	void SceneChangeStart() override;

private:
	void CreateSlashEffect();

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

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
	void HurtGround_Leave();

	void DoorIn_Init();
	void DoorIn_State();

	void DoorOut_Init();
	void DoorOut_State();

	void PatrolTurn_Init();
	void PatrolTurn_State();

	void Patrol_Init();
	void Patrol_State();
};