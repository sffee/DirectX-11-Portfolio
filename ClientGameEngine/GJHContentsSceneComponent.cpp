#include "GJHContentsSceneComponent.h"
#include <GJHGameEngineScene.h>
#include "GJHActorTransform.h"
#include "GJHSpriteRenderer.h"
#include "GJHSpriteAnimation.h"
#include "GJHWorldComponent.h"
#include "GJHRenderManagerComponent.h"
#include "GJHGUIComponent.h"
#include <GJHDirectTexture2D.h>
#include "TypeComponent.h"

float GJHContentsSceneComponent::m_StageTime = 0.f;
float4 GJHContentsSceneComponent::m_CamSize;

GJHContentsSceneComponent::GJHContentsSceneComponent()
{

}

GJHContentsSceneComponent::~GJHContentsSceneComponent()
{

}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, float4 _Position, const float4& _Scale, const GJHGameEngineString& _AniName, int _StartFrameIndex, int _EndFrameIndex, float _FrameTime, bool _Loop, bool _IsPieceAnimation)
{
	GJHGameEngineString Number = (_StartFrameIndex != -1) ? GJHGameEngineString(L"_") + GJHGameEngineString(_StartFrameIndex) : L"";
	GJHGameEngineString Text = _TexName + L".png";
	
	std::shared_ptr<GJHDirectTexture2D> Sprite = GJHDirectTexture2D::Find(Text);
	if (Sprite == nullptr)
	{
		Text = _TexName + L"_0.png";
		Sprite = GJHDirectTexture2D::Find(Text);
	}
	
	ActorPtr Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	float4 Position = _Position;
	Transform->SetWorldPosition(Position);
	
	float4 Scale;
	Scale.x = (_Scale.x < 0) ? m_CamSize.x : Sprite->GetSizeX() * _Scale.x;
	Scale.y = (_Scale.y < 0) ? m_CamSize.y : Sprite->GetSizeY() * _Scale.y;
	
	std::shared_ptr<GJHSpriteRenderer> Renderer = Actor->CreateComponent<GJHSpriteRenderer>();
	Renderer->TextureSetting(Text);
	Renderer->SetWorldScale(Scale);

	if (_StartFrameIndex != -1 && _EndFrameIndex != -1)
	{
		AnimationPtr Animation = Actor->CreateComponent<GJHSpriteAnimation>();
		Animation->CreateAnimation(_AniName, _TexName, _StartFrameIndex, _EndFrameIndex, _FrameTime, _Loop, _IsPieceAnimation);
		Animation->ChangeAnimation(_AniName);
	}

	Actor->CreateComponent<TypeComponent>(_ActorType);

	return Actor;
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, float4 _Position, const float _Scale, const GJHGameEngineString& _AniName, const int _StartFrameIndex, const int _EndFrameIndex, const float _FrameTime, const bool _Loop, const bool _IsPieceAnimation)
{
	return CreateActor(_ActorType, _TexName, _Position, { _Scale, _Scale }, _AniName, _StartFrameIndex, _EndFrameIndex, _FrameTime, _Loop, _IsPieceAnimation);
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, float4 _Position, const float4& _Scale)
{
	ActorPtr Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	float4 Position = _Position;
	Transform->SetWorldPosition(Position);

	float4 Scale;
	Scale.x = (_Scale.x < 0) ? m_CamSize.x : _Texture->GetSizeX() * _Scale.x;
	Scale.y = (_Scale.y < 0) ? m_CamSize.y : _Texture->GetSizeY() * _Scale.y;

	std::shared_ptr<GJHSpriteRenderer> Renderer = Actor->CreateComponent<GJHSpriteRenderer>();
	Renderer->TextureSetting(_Texture);
	Renderer->SetWorldScale(Scale);

	Actor->CreateComponent<TypeComponent>(_ActorType);

	return Actor;
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, float4 _Position, const float _Scale)
{
	return CreateActor(_ActorType, _Texture, _Position, { _Scale, _Scale });
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateEffect(const GJHGameEngineString& _TexName, float4 _Position, const float4& _Scale)
{
	GJHGameEngineString Text = _TexName + L".png";

	std::shared_ptr<GJHDirectTexture2D> Sprite = GJHDirectTexture2D::Find(Text);
	if (Sprite == nullptr)
	{
		Text = _TexName + GJHGameEngineString(L"_0") + L".png";
		Sprite = GJHDirectTexture2D::Find(Text);
	}

	ActorPtr Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	Transform->SetWorldPosition(_Position);

	std::shared_ptr<GJHSpriteRenderer> Renderer = Actor->CreateComponent<GJHSpriteRenderer>();
	Renderer->TextureSetting(Text);
	Renderer->SetWorldScale(_Scale);

	Actor->CreateComponent<TypeComponent>(ACTORTYPE::EFFECT);

	return Actor;
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateEffect(const GJHGameEngineString& _TexName, float4 _Position, const float _Scale)
{
	return CreateEffect(_TexName, _Position, { _Scale, _Scale });
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateGate(const GJHGameEngineString& _NextSceneName, float4 _Position)
{
	std::shared_ptr<GJHGameEngineActor> Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	float4 Position = _Position;
	Transform->SetWorldPosition(Position);

	Actor->CreateComponent<TypeComponent>(ACTORTYPE::GATE);

	return Actor;
}

std::shared_ptr<GJHGameEngineActor> GJHContentsSceneComponent::CreateUI(const GJHGameEngineString& _TexName, float4 _Position, float _Scale)
{
	GJHGameEngineString Text = _TexName + L".png";

	std::shared_ptr<GJHDirectTexture2D> Sprite = GJHDirectTexture2D::Find(Text);
	if (Sprite == nullptr)
	{
		Text = _TexName + L"_0.png";
		Sprite = GJHDirectTexture2D::Find(Text);
	}

	ActorPtr Actor = GetScene()->CreateActor();

	std::shared_ptr<GJHActorTransform> Transform = Actor->FindComponent<GJHActorTransform>();
	float4 Position = _Position;
	Transform->SetWorldPosition(Position);

	float4 Scale = Sprite->GetSize() * float4(_Scale, _Scale);

	std::shared_ptr<GJHSpriteRenderer> Renderer = Actor->CreateComponent<GJHSpriteRenderer>(1);
	Renderer->TextureSetting(Text);
	Renderer->SetWorldScale(Scale);

	Actor->CreateComponent<TypeComponent>(ACTORTYPE::UI);

	return Actor;
}

void GJHContentsSceneComponent::Begin()
{
	World = GetScene()->FindComponent<GJHWorldComponent>(L"World");
	Render = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	GUI = GetScene()->FindComponent<GJHGUIComponent>(L"GUI");
}