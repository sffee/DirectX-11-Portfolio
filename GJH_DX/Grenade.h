#pragma once
#include "ObjectBase.h"

class InstanceObject;
class SpectrumCom;
class Grenade : public ObjectBase
{
public:
	Grenade();
	~Grenade();

public:
	Grenade(const Grenade& _Other) = delete;
	Grenade(const Grenade&& _Other) = delete;

public:
	Grenade operator=(const Grenade& _Other) = delete;
	Grenade operator=(const Grenade&& _Other) = delete;

private:
	std::shared_ptr<InstanceObject> m_GlowEffect;
	std::shared_ptr<GJHSpriteRenderer> m_GlowEffectRender;
	float m_GlowEffectAlpha;

private:
	std::shared_ptr<SpectrumCom> m_Spectrum;

private:
	std::shared_ptr<InstanceObject> m_RangeEffect;
	std::shared_ptr<GJHSpriteRenderer> m_RangeEffectRender;
	float m_RangeEffectScale;

private:
	std::vector<std::shared_ptr<InstanceObject>> m_ExplosionList;

private:
	GJHStateUpdater<Grenade> State;

private:
	float m_Time;
	bool m_IsEnd;
	bool m_Bounce;
	bool m_BoomStart;
	float m_BoomStartWaitTime;
	bool m_BlinkRangeEffect;
	float m_GravityTemp;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;

private:
	void CreateRangeEffect();
	void CreateExplosion();
	void Explosion();

public:
	void Attack(DIRECTION _Dir, float4 _Pos, float _Gravity);

private:
	void Start_Init();
	void Start_State();

	void Bounce_Init();
	void Bounce_State();

	void ExpandRange_Init();
	void ExpandRange_State();

	void BlinkRange_Init();
	void BlinkRange_State();

	void Explosion_Init();
	void Explosion_State();
};