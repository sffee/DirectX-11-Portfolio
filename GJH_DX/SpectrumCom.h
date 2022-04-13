#pragma once
#include <GJHContentsSceneComponent.h>

class GJHGameEngineActor;
class GJHSpriteRenderer;
class SpectrumCom : public GJHContentsSceneComponent
{
private:
	bool m_IsStart;
	float m_Time;
	float m_CreateTime;
	float m_LifeTime;
	float m_MaxAlpha;
	float4 m_Color;
	bool m_OnlyColor;
	float m_TransformZ;
	ACTORTYPE m_ActorType;

private:
	bool m_IsPlayer;

private:
	std::shared_ptr<GJHSpriteRenderer> m_ParentRender;
	float4 m_RenderScale;
	GJHGameEngineActor* m_ParentActor;

public:
	SpectrumCom();
	~SpectrumCom();

public:
	SpectrumCom(const SpectrumCom& _Other) = delete;
	SpectrumCom(const SpectrumCom&& _Other) = delete;

public:
	SpectrumCom operator=(const SpectrumCom& _Other) = delete;
	SpectrumCom operator=(const SpectrumCom&& _Other) = delete;

public:
	void SetColor(float4 _Color)
	{
		m_Color = _Color;
	}

	void SetCreateTime(float _CreateTime)
	{
		m_CreateTime = _CreateTime;
	}

	void SetLifeTime(float _LifeTime)
	{
		m_LifeTime = _LifeTime;
	}

	void SetMaxAlpha(float _MaxAlpha)
	{
		m_MaxAlpha = _MaxAlpha;
	}

	void SetTransformZ(float _Value)
	{
		m_TransformZ = _Value;
	}

	void SetActorType(ACTORTYPE _Type)
	{
		m_ActorType = _Type;
	}

	void SetIsPlayer(bool _Set)
	{
		m_IsPlayer = _Set;
	}

	bool GetIsStart()
	{
		return m_IsStart;
	}

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale, GJHGameEngineActor* _ParentActor, float _Createtime, float _LifeTime, float _MaxAlpha, float4 _Color, bool _OnlyColor);
	void Update() override;

public:
	void Start();
	void End();
};