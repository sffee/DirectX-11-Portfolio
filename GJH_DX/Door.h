#pragma once
#include "ObjectBase.h"

class Door : public ObjectBase
{
public:
	Door();
	~Door();

public:
	Door(const Door& _Other) = delete;
	Door(const Door&& _Other) = delete;

public:
	Door operator=(const Door& _Other) = delete;
	Door operator=(const Door&& _Other) = delete;

private:
	GJHStateUpdater<Door> State;
	bool m_IsCamShake;
	float4 m_StartPos;

private:
	float m_Time;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void RewindEnd() override;

public:
	void SetDirection(int _Dir);
	void Damage(DIRECTION _Dir);
	void PlayHitSound();

public:
	void BindDamageFunc(std::function<void(ObjectBase::DIRECTION)>& _Func);

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

private:
	void Iron_Idle_Init();
	void Iron_Idle_State();

	void Iron_Damage_Init();
	void Iron_Damage_State();
};