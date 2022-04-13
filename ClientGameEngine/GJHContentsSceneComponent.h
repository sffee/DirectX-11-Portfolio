#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineActor.h>
#include "GJHActorTransform.h"

class GJHGUIComponent;
class GJHRenderManagerComponent;
class GJHWorldComponent;
class GJHSpriteRenderer;
class GJHDirectTexture2D;
class GJHContentsSceneComponent : public GJHGameEngineComponent
{
private:
	static float4 m_CamSize;
	static float m_StageTime;

public:
	static void SetCamSize(const float4& _CamSize)
	{
		m_CamSize = _CamSize;
	}

	static float GetStageTime()
	{
		return m_StageTime;
	}

	static void AddStageTime(const float _Value)
	{
		m_StageTime += _Value;
	}

	static void ResetStageTime()
	{
		m_StageTime = 0.f;
	}

public:
	enum class ACTORTYPE
	{
		NONE,
		PLAYER,
		MONSTER,
		BULLET,
		ITEM,
		EFFECT,
		BACKGROUND,
		DOOR,
		GATE,
		UI
	};

private:
	std::shared_ptr<GJHWorldComponent> World;
	std::shared_ptr<GJHRenderManagerComponent> Render;
	std::shared_ptr<GJHGUIComponent> GUI;

public:
	std::shared_ptr<GJHGUIComponent> GetGUI()
	{
		return GUI;
	}

	std::shared_ptr<GJHRenderManagerComponent> GetRenderManager()
	{
		return Render;
	}

	std::shared_ptr<GJHWorldComponent> GetWorld()
	{
		return World;
	}

public:
	GJHContentsSceneComponent();
	~GJHContentsSceneComponent();

public:
	GJHContentsSceneComponent(const GJHContentsSceneComponent& _Other) = delete;
	GJHContentsSceneComponent(const GJHContentsSceneComponent&& _Other) = delete;

public:
	GJHContentsSceneComponent operator=(const GJHContentsSceneComponent& _Other) = delete;
	GJHContentsSceneComponent operator=(const GJHContentsSceneComponent&& _Other) = delete;

public:
	template<typename T>
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, const float4& _Position, const float4& _Scale = float4(1.f, 1.f), const GJHGameEngineString& _AniName = L"", const int _StartFrameIndex = -1, const int _EndFrameIndex = -1, const float _FrameTime = 1.f, const bool _Loop = true, const bool _IsPieceAnimation = true)
	{
		std::shared_ptr<GJHGameEngineActor> Actor = CreateActor(_ActorType, _TexName, _Position, _Scale, _AniName, _StartFrameIndex, _EndFrameIndex, _FrameTime, _Loop, _IsPieceAnimation);
		Actor->CreateComponent<T>(Actor->FindComponent<GJHSpriteRenderer>(), _Scale);

		return Actor;
	}

	template<typename T>
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, const float4& _Position, const float _Scale, const GJHGameEngineString& _AniName = L"", const int _StartFrameIndex = -1, const int _EndFrameIndex = -1, const float _FrameTime = 1.f, const bool _Loop = true, const bool _IsPieceAnimation = true)
	{
		return CreateActor<T>(_ActorType, _TexName, _Position, { _Scale, _Scale }, _AniName, _StartFrameIndex, _EndFrameIndex, _FrameTime, _Loop, _IsPieceAnimation);
	}
	
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, float4 _Position, const float4& _Scale = float4(1.f, 1.f), const GJHGameEngineString& _AniName = L"", const int _StartFrameIndex = -1, const int _EndFrameIndex = -1, const float _FrameTime = 1.f, const bool _Loop = true, const bool _IsPieceAnimation = true);
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, const GJHGameEngineString& _TexName, float4 _Position, const float _Scale, const GJHGameEngineString& _AniName = L"", const int _StartFrameIndex = -1, const int _EndFrameIndex = -1, const float _FrameTime = 1.f, const bool _Loop = true, const bool _IsPieceAnimation = true);

public:
	template<typename T, typename ... REST>
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, const float4& _Position, const float4& _Scale = float4(1.f, 1.f), GJHGameEngineActor* _ParentActor = nullptr, REST ... _Arg)
	{
		std::shared_ptr<GJHGameEngineActor> Actor = CreateActor(_ActorType, _Texture, _Position, _Scale);
		Actor->CreateComponent<T>(Actor->FindComponent<GJHSpriteRenderer>(), _Scale, _ParentActor, _Arg...);

		return Actor;
	}

	template<typename T, typename ... REST>
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, const float4& _Position, const float _Scale, GJHGameEngineActor* _ParentActor = nullptr, REST ... _Arg)
	{
		return CreateActor<T>(_ActorType, _Texture, _Position, { _Scale, _Scale }, _ParentActor, _Arg...);
	}

	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, float4 _Position, const float4& _Scale = float4(1.f, 1.f));
	std::shared_ptr<GJHGameEngineActor> CreateActor(ACTORTYPE _ActorType, std::shared_ptr<GJHDirectTexture2D> _Texture, float4 _Position, const float _Scale);

public:
	template<typename T, typename ... REST>
	std::shared_ptr<GJHGameEngineActor> CreateEffect(const GJHGameEngineString& _TexName, const float4& _Position, const float4& _Scale = float4(1.f, 1.f), GJHGameEngineActor* _ParentActor = nullptr, REST ... _Arg)
	{
		std::shared_ptr<GJHGameEngineActor> Actor = CreateEffect(_TexName, _Position, _Scale);
		Actor->CreateComponent<T>(Actor->FindComponent<GJHSpriteRenderer>(), _Scale, _ParentActor, _Arg...);

		return Actor;
	}

	template<typename T, typename ... REST>
	std::shared_ptr<GJHGameEngineActor> CreateEffect(const GJHGameEngineString& _TexName, const float4& _Position, const float _Scale, GJHGameEngineActor* _ParentActor = nullptr, REST ... _Arg)
	{
		return CreateEffect<T>(_TexName, _Position, { _Scale, _Scale }, _ParentActor, _Arg...);
	}

	std::shared_ptr<GJHGameEngineActor> CreateEffect(const GJHGameEngineString& _TexName, float4 _Position, const float4& _Scale = float4(1.f, 1.f));
	std::shared_ptr<GJHGameEngineActor> CreateEffect(const GJHGameEngineString& _TexName, float4 _Position, const float _Scale);

public:
	template<typename T>
	std::shared_ptr<GJHGameEngineActor> CreateGate(const GJHGameEngineString& _NextSceneName, float4 _Position, int _Dir, std::shared_ptr<GJHCamera> _Cam)
	{
		std::shared_ptr<GJHGameEngineActor> Actor = CreateGate(_NextSceneName, _Position);

		Actor->CreateComponent<T>(_NextSceneName, _Dir, _Cam);

		return Actor;
	}

	std::shared_ptr<GJHGameEngineActor> CreateGate(const GJHGameEngineString& _NextSceneName, float4 _Position);

public:
	std::shared_ptr<GJHGameEngineActor> CreateUI(const GJHGameEngineString& _TexName, float4 _Position, float _Scale = 1.f);

public:
	void Begin() override;
};