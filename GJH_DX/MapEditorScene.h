#pragma once
#include <GJHContentsSceneComponent.h>

#define TILESIZEX 16.f
#define TILESIZEY 16.f

class GJHActorTransform;
class GJHCamera;
class GJHRenderer;
class GJHSpriteRenderer;
class GJHTileMapRenderer;
class GJHSpecialTileMapRenderer;
class MapEditorScene : public GJHContentsSceneComponent
{
public:
	static std::shared_ptr<GJHCamera> m_Cam;
	static std::shared_ptr<GJHActorTransform> m_CamTrans;
	
public:
	static std::shared_ptr<GJHActorTransform> GetCamTrans()
	{
		return m_CamTrans;
	}

public:
	static std::shared_ptr<GJHSpriteRenderer> m_BackGroundImage;
	static std::shared_ptr<GJHTileMapRenderer> m_BackTileMapRenderer;
	static std::shared_ptr<GJHTileMapRenderer> m_ForeTileMapRenderer;
	static std::shared_ptr<GJHTileMapRenderer> m_PreviewTileMapRenderer;
	static std::shared_ptr<GJHTileMapRenderer> m_CollisionTileMapRenderer;
	static std::shared_ptr<GJHSpecialTileMapRenderer> m_SpecialTileMapRenderer;

public:
	static bool m_IsShowCollisionTile;
	static bool m_IsShowSpecialTile;
	static bool m_IsDoorLinkMode;

private:
	std::shared_ptr<GJHRenderer> m_DoorLineRender;
	float4 m_SelectDoorTilePos;

private:
	float4 m_MapSize;

public:
	MapEditorScene();
	~MapEditorScene();

public:
	MapEditorScene(const MapEditorScene& _Other) = delete;
	MapEditorScene(const MapEditorScene&& _Other) = delete;

public:
	MapEditorScene operator=(const MapEditorScene& _Other) = delete;
	MapEditorScene operator=(const MapEditorScene&& _Other) = delete;

private:
	void CheckKey();
	void PrevRenderUpdate();
	void SpecialTileMapDoorLineUpdate();
	void CamPosCheck();

public:
	void Start() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;
};