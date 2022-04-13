#pragma once
#include <GJHContentsSceneComponent.h>
#include "define.h"

class GJHTileMapRenderer;
class GJHSpriteRenderer;
class GJHSpecialTileMapRenderer;
class GJHDirectTexture2D;
class Player;
class UIManager;
class StageBase : public GJHContentsSceneComponent
{
public:
	static std::shared_ptr<GJHDirectTexture2D> m_ColMapTexture;
	static bool m_IsDebug;

public:
	static bool GetIsDebug()
	{
		return m_IsDebug;
	}

protected:
	std::shared_ptr<GJHSpriteRenderer> m_BackGroundImageRenderer;
	std::shared_ptr<GJHSpriteRenderer> m_TileRenderer;
	std::shared_ptr<GJHSpriteRenderer> m_DebugColTileMapRenderer;
	std::shared_ptr<GJHTileMapRenderer> m_ColTileMapRenderer;
	std::shared_ptr<GJHSpecialTileMapRenderer> m_SpecialTileMapRenderer;
	GJHGameEngineString m_ColMapTextureName;

protected:
	std::shared_ptr<GJHCamera> m_Cam;
	std::shared_ptr<GJHCamera> m_UICam;
	std::shared_ptr<UIManager> m_UIManager;

private:
	float4 m_MapSize;

public:
	StageBase();
	~StageBase();

public:
	StageBase(const StageBase& _Other) = delete;
	StageBase(const StageBase&& _Other) = delete;

public:
	StageBase operator=(const StageBase& _Other) = delete;
	StageBase operator=(const StageBase&& _Other) = delete;

public:
	void Start(const GJHGameEngineString& _MapFileName);
	void Update() override;
	void SceneChangeStart() override;

protected:
	void CreateMap(const GJHGameEngineString& _MapFileName);
};