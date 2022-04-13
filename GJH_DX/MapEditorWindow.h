#pragma once
#include <GJHGameEngineGUI.h>
#include <GJHDirectTexture2D.h>

class MapEditorWindow : public GJHGUIWindow
{
public:
	enum class LAYERMODE
	{
		BACKGROUND,
		FOREGROUND
	};

	enum class TAB
	{
		TILE,
		COLLISION,
		SPECIAL
	};

public:
	static std::vector<GJHGameEngineString> m_BackGroundImageVec;
	static std::vector<GJHGameEngineString> m_BackTileImageVec;
	static std::vector<GJHGameEngineString> m_ForeTileImageVec;
	static std::vector<GJHGameEngineString> m_SpecialTileImageVec;
	static std::vector<GJHGameEngineString> m_SaveFileVec;

public:
	static int m_SelectTileIndex;
	static int m_SelectCollisionTileIndex;
	static int m_SelectSpecialTileIndex;
	static int m_LayerModeSelect;
	static TAB m_TabSelect;
	static TAB m_PrevTabSelect;

private:
	int m_MouseOverTileIndex;

private:
	int m_BackGroundImageSelect;
	int m_BackTileImageSelect;
	int m_ForeTileImageSelect;
	int m_SpecialTileImageSelect;

private:
	float m_NoneSelectTileAlpha;
	bool m_SelectTileAlphaEnable;

private:
	float4 m_TileSpriteStartPos;
	std::shared_ptr<GJHDirectSprite> m_TileSprite;
	std::shared_ptr<GJHDirectSprite> m_CollisionSprite;
	std::shared_ptr<GJHDirectSprite> m_SpecialSprite;

public:
	static void ChangeBackGroundImageComboFunc(const GJHGameEngineString& _Value);
	static void ChangeTileImageComboFunc(const GJHGameEngineString& _Value);

public:
	MapEditorWindow();
	~MapEditorWindow();

public:
	MapEditorWindow(const MapEditorWindow& _Other) = delete;
	MapEditorWindow(const MapEditorWindow&& _Other) = delete;

public:
	MapEditorWindow operator=(const MapEditorWindow& _Other) = delete;
	MapEditorWindow operator=(const MapEditorWindow&& _Other) = delete;

public:
	void OnGUI() override;
	void Update() override;

private:
	void TileWindow();
	void SaveWindow();

private:
	void SaveFileLoading();
	void CheckLayerModeColor();

private:
	int GetSelectTileIndex();
	void KeyCheck();
};