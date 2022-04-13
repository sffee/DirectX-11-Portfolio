#pragma once
#include "ObjectBase.h"

class GJHGameEngineActor;
class EffectBase : public ObjectBase
{
protected:
	GJHGameEngineActor* m_ParentActor;

public:
	EffectBase();
	~EffectBase();

public:
	EffectBase(const EffectBase& _Other) = delete;
	EffectBase(const EffectBase&& _Other) = delete;

public:
	EffectBase operator=(const EffectBase& _Other) = delete;
	EffectBase operator=(const EffectBase&& _Other) = delete;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
};