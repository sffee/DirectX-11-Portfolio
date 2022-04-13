#include "EffectBase.h"

EffectBase::EffectBase()
{

}

EffectBase::~EffectBase()
{

}

void EffectBase::Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale)
{
	m_CheckPlatform = false;

	ObjectBase::Start(_Renderer, _RenderScale);
}