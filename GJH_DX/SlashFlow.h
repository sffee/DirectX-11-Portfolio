#pragma once
#include "EffectBase.h"

class SlashFlow : public EffectBase
{
private:
	enum class ColorType
	{
		MAZENTA,
		CYAN
	};

private:
	bool m_Start;
	float m_DeathTime;
	float m_Angle;
	float4 m_TargetPos;
	ColorType m_ColorType;
	float m_ColorChangeTime;
	DIRECTION m_MoveDir;

public:
	SlashFlow();
	~SlashFlow();

public:
	SlashFlow(const SlashFlow& _Other) = delete;
	SlashFlow(const SlashFlow&& _Other) = delete;

public:
	SlashFlow operator=(const SlashFlow& _Other) = delete;
	SlashFlow operator=(const SlashFlow&& _Other) = delete;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, float _Angle, float4 _TargetPos, DIRECTION _Dir);
	void Update() override;
};