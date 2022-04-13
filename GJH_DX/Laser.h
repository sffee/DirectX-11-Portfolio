#pragma once
#include "ObjectBase.h"

class Laser : public ObjectBase
{
public:
	Laser();
	~Laser();

public:
	Laser(const Laser& _Other) = delete;
	Laser(const Laser&& _Other) = delete;

public:
	Laser operator=(const Laser& _Other) = delete;
	Laser operator=(const Laser&& _Other) = delete;

private:
	float m_RenderYSize;
	bool m_IsEnd;
	bool m_IsFinish;
	float m_Angle;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;

private:
	bool CheckPlayerCol();

public:
	void Setting(float4 _Pos, float _Angle);
	void Fire(DIRECTION _Dir);
	void Finish();
	void PlayLaserShotSound();
};