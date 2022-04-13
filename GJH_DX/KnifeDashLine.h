#pragma once
#include "ObjectBase.h"

class KnifeDashLine : public ObjectBase
{
public:
	KnifeDashLine();
	~KnifeDashLine();

public:
	KnifeDashLine(const KnifeDashLine& _Other) = delete;
	KnifeDashLine(const KnifeDashLine&& _Other) = delete;

public:
	KnifeDashLine operator=(const KnifeDashLine& _Other) = delete;
	KnifeDashLine operator=(const KnifeDashLine&& _Other) = delete;

private:
	float m_Angle;
	float m_Time;
	bool m_IsEnd;

private:
	float m_RenderYSize;
	float m_RenderYMaxSize;
	float m_RenderSizeDeltaTime;
	bool m_AddRenderSizeEnd;
	float m_RenderSizeSubWaitTime;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
public:
	void Attack(DIRECTION _Dir, float _RenderSizeSubWaitTime = 0.2f, float _RenderSizeDeltaTime = 3.5f);
	void Setting(float4 _Pos, float _Angle);
};