#pragma once
#include "ObjectBase.h"

class MonsterBullet : public ObjectBase
{
public:
	MonsterBullet();
	~MonsterBullet();

public:
	MonsterBullet(const MonsterBullet& _Other) = delete;
	MonsterBullet(const MonsterBullet&& _Other) = delete;

public:
	MonsterBullet operator=(const MonsterBullet& _Other) = delete;
	MonsterBullet operator=(const MonsterBullet&& _Other) = delete;

private:
	float4 m_MoveVec;
	float m_Angle;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;

public:
	void Fire(float4 _Pos, float _Angle);
	void PlayHitSound();
};