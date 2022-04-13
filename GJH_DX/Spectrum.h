#pragma once
#include "EffectBase.h"

class Spectrum : public EffectBase
{
private:
	float m_Time;
	float m_LifeTime;
	float m_MaxAlpha;
	bool m_IsPlayer;

public:
	Spectrum();
	~Spectrum();

public:
	Spectrum(const Spectrum& _Other) = delete;
	Spectrum(const Spectrum&& _Other) = delete;

public:
	Spectrum operator=(const Spectrum& _Other) = delete;
	Spectrum operator=(const Spectrum&& _Other) = delete;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, int _Dir, float _Cooltime, float _MaxAlpha, float4 m_Color, bool _OnlyColor, float m_TransformZ, bool _IsPlayer);
	void Update() override;
};