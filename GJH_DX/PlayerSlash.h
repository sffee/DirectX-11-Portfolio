#pragma once
#include "EffectBase.h"

class PlayerSlash : public EffectBase
{
private:
	std::shared_ptr<GJHRenderer> m_ParentRender;

private:
	float m_Angle;

public:
	PlayerSlash();
	~PlayerSlash();

public:
	PlayerSlash(const PlayerSlash& _Other) = delete;
	PlayerSlash(const PlayerSlash&& _Other) = delete;

public:
	PlayerSlash operator=(const PlayerSlash& _Other) = delete;
	PlayerSlash operator=(const PlayerSlash&& _Other) = delete;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor);
	void Update() override;
	void RewindEnd() override;

public:
	void AttackStart(DIRECTION _Dir, const float4& _CharPos, float _Angle);

private:
	void CreateBulletReplectEffect(float4 _Pos);
	void PlayHitSound();
};