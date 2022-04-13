#include "MapEditorWindow.h"
#include <GJHGameEngineDirectory.h>
#include "MapEditorScene.h"
#include <GJHSpriteRenderer.h>
#include <GJHTileMapRenderer.h>
#include <GJHSpecialTileMapRenderer.h>
#include "GJHDirectRenderTarget.h"
#include <GJHGameEngineWindow.h>
#include <GJHCamera.h>
#include "define.h"

std::vector<GJHGameEngineString> MapEditorWindow::m_BackGroundImageVec;
std::vector<GJHGameEngineString> MapEditorWindow::m_BackTileImageVec;
std::vector<GJHGameEngineString> MapEditorWindow::m_ForeTileImageVec;
std::vector<GJHGameEngineString> MapEditorWindow::m_SpecialTileImageVec;
std::vector<GJHGameEngineString> MapEditorWindow::m_SaveFileVec;
int MapEditorWindow::m_SelectTileIndex = 0;
int MapEditorWindow::m_SelectCollisionTileIndex = 0;
int MapEditorWindow::m_SelectSpecialTileIndex = 0;
int MapEditorWindow::m_LayerModeSelect = 0;
MapEditorWindow::TAB MapEditorWindow::m_TabSelect = TAB::TILE;
MapEditorWindow::TAB MapEditorWindow::m_PrevTabSelect = TAB::TILE;

void MapEditorWindow::ChangeBackGroundImageComboFunc(const GJHGameEngineString& _Value)
{
	MapEditorScene::m_BackGroundImage->TextureSetting(_Value, true);

	float4 Pos = MapEditorScene::m_BackGroundImage->GetWorldScale();
	Pos.ReverseY();

	MapEditorScene::m_BackGroundImage->SetWorldPosition(Pos.HalfVector2D());
}

void MapEditorWindow::ChangeTileImageComboFunc(const GJHGameEngineString& _Value)
{
	if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
	{
		MapEditorScene::m_BackTileMapRenderer->ChangeTileMap(_Value, { TILESIZEX, TILESIZEY });
	}
	else if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::FOREGROUND)
	{
		MapEditorScene::m_ForeTileMapRenderer->ChangeTileMap(_Value, { TILESIZEX, TILESIZEY });
	}
}

MapEditorWindow::MapEditorWindow() :
	m_BackGroundImageSelect(0),
	m_BackTileImageSelect(0),
	m_ForeTileImageSelect(0),
	m_SpecialTileImageSelect(0),
	m_MouseOverTileIndex(0),
	m_NoneSelectTileAlpha(0.5f),
	m_SelectTileAlphaEnable(true)
{
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");
		NewDir.Move(L"Map");
		NewDir.Move(L"BackGround");

		auto Arr = NewDir.AllFile();

		for (size_t i = 0; i < Arr.size(); i++)
		{
			m_BackGroundImageVec.push_back(Arr[i].FileName());
		}

		GJHGameEngineString Str = L"None.png";
		m_BackGroundImageSelect = (int)std::distance(m_BackGroundImageVec.begin(), std::find(m_BackGroundImageVec.begin(), m_BackGroundImageVec.end(), Str));
	}

	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");
		NewDir.Move(L"Map");
		NewDir.Move(L"Tile");
		NewDir.Move(L"BackGround");

		auto Arr = NewDir.AllFile();

		for (size_t i = 0; i < Arr.size(); i++)
		{
			m_BackTileImageVec.push_back(Arr[i].FileName());
		}
	}

	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");
		NewDir.Move(L"Map");
		NewDir.Move(L"Tile");
		NewDir.Move(L"ForeGround");

		auto Arr = NewDir.AllFile();

		for (size_t i = 0; i < Arr.size(); i++)
		{
			m_ForeTileImageVec.push_back(Arr[i].FileName());
		}
	}

	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");
		NewDir.Move(L"Map");
		NewDir.Move(L"Tile");
		NewDir.Move(L"Special");

		auto Arr = NewDir.AllFile();

		for (size_t i = 0; i < Arr.size(); i++)
		{
			m_SpecialTileImageVec.push_back(Arr[i].FileName());
		}
	}

	SaveFileLoading();
}

MapEditorWindow::~MapEditorWindow()
{

}

void MapEditorWindow::OnGUI()
{
	{
		GJHGameEngineGUI::Combo(L"배경 이미지", m_BackGroundImageSelect, m_BackGroundImageVec, &MapEditorWindow::ChangeBackGroundImageComboFunc);
		GJHGameEngineGUI::NewLine();
	}

	{
		GJHGameEngineGUI::Text(L"충돌 타일 보기(F1) :");
		GJHGameEngineGUI::SameLine();

		if (MapEditorScene::m_IsShowCollisionTile == true)
		{
			GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
		}
		else
		{
			GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
		}

		GJHGameEngineGUI::Text(L"특수 타일 보기(F2) :");
		GJHGameEngineGUI::SameLine();

		if (MapEditorScene::m_IsShowSpecialTile == true)
		{
			GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
		}
		else
		{
			GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
		}

		GJHGameEngineGUI::Text(L"선택 안한 레이어 알파값 적용(F3) :");
		GJHGameEngineGUI::SameLine();

		if (m_SelectTileAlphaEnable == true)
		{
			GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
		}
		else
		{
			GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
		}


		GJHGameEngineGUI::NewLine();
	}

	TileWindow();

	{
		GJHGameEngineGUI::NewLine();
		GJHGameEngineGUI::AddLine(GJHGameEngineGUI::GetCursorScreenPos(), GJHGameEngineGUI::GetCursorScreenPos() + float4(GJHGameEngineGUI::GetWindowContentRegionWidth(), 0.f), { 255.f, 255.f, 255.f, 255.f });
		GJHGameEngineGUI::NewLine();
	}

	SaveWindow();

	{
		GJHGameEngineGUI::NewLine();
		GJHGameEngineGUI::AddLine(GJHGameEngineGUI::GetCursorScreenPos(), GJHGameEngineGUI::GetCursorScreenPos() + float4(GJHGameEngineGUI::GetWindowContentRegionWidth(), 0.f), { 255.f, 255.f, 255.f, 255.f });
		GJHGameEngineGUI::NewLine();
	}

	{
		float4 CamPos = MapEditorScene::m_CamTrans->GetWorldPosition();
		CamPos.x -= CAMSIZEX * 0.5f;
		CamPos.y += CAMSIZEY * 0.5f;
		GJHGameEngineGUI::Text(L"CamPos : ", CamPos.x, L", ", CamPos.y);

		float4 MousePos = GJHGameEngineWindow::GetMousePos();
		float4 WindowSize = GJHGameEngineWindow::GetSize();
		MousePos.x /= (WindowSize.x / CAMSIZEX);
		MousePos.y /= (WindowSize.y / CAMSIZEY);
		MousePos.y *= -1.f;
		MousePos += CamPos;
		GJHGameEngineGUI::Text(L"MousePos : ", MousePos.x, L", ", MousePos.y);

		int X = (int)(MousePos.x / TILESIZEX);
		int Y = abs((int)(MousePos.y / TILESIZEY));

		GJHGameEngineGUI::Text(L"MouseOverTileIndex : ", X, L", ", Y);
	}
}

void MapEditorWindow::TileWindow()
{
	if (GJHGameEngineGUI::BeginTabBar(0))
	{
		if (GJHGameEngineGUI::BeginTabItem(L"타일"))
		{
			m_TabSelect = TAB::TILE;

			if (m_TabSelect != m_PrevTabSelect)
			{
				m_PrevTabSelect = m_TabSelect;

				if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
					MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(m_BackTileImageVec[m_BackTileImageSelect], { TILESIZEX, TILESIZEY });
				else if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::FOREGROUND)
					MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(m_ForeTileImageVec[m_ForeTileImageSelect], { TILESIZEX, TILESIZEY });
			}

			{
				if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
					GJHGameEngineGUI::Combo(L"타일 이미지", m_BackTileImageSelect, m_BackTileImageVec, &MapEditorWindow::ChangeTileImageComboFunc);
				else if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::FOREGROUND)
					GJHGameEngineGUI::Combo(L"타일 이미지", m_ForeTileImageSelect, m_ForeTileImageVec, &MapEditorWindow::ChangeTileImageComboFunc);
			}

			{
				if (GJHGameEngineGUI::RadioButton(L"BackGround", &m_LayerModeSelect, (int)LAYERMODE::BACKGROUND))
				{
					m_SelectTileIndex = 0;

					CheckLayerModeColor();
				}

				GJHGameEngineGUI::SameLine();
				
				if (GJHGameEngineGUI::RadioButton(L"ForeGround", &m_LayerModeSelect, (int)LAYERMODE::FOREGROUND))
				{
					m_SelectTileIndex = 0;
					
					CheckLayerModeColor();
				}
			}

			{
				m_TileSpriteStartPos = GJHGameEngineGUI::GetCursorScreenPos();

				m_TileSprite = ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
					? GJHDirectSprite::Find(m_BackTileImageVec[m_BackTileImageSelect])
					: GJHDirectSprite::Find(m_ForeTileImageVec[m_ForeTileImageSelect]);

				GJHGameEngineGUI::Image(0, m_TileSprite->GetTexture(), m_TileSprite->GetTexture()->GetSize());

				int SpriteYSize = m_TileSprite->GetCutDataYSize();
				int YLineCount = m_TileSprite->GetTexture()->GetSizeY() / SpriteYSize;
				for (int i = 0; i <= YLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x, m_TileSpriteStartPos.y + (i * SpriteYSize));
					float4 EndPos = float4(m_TileSpriteStartPos.x + m_TileSprite->GetTexture()->GetSizeX(),
						m_TileSpriteStartPos.y + (i * SpriteYSize));
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				int SpriteXSize = m_TileSprite->GetCutDataXSize();
				int XLineCount = m_TileSprite->GetTexture()->GetSizeX() / SpriteXSize;
				for (int i = 0; i <= XLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize), m_TileSpriteStartPos.y);
					float4 EndPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize),
						m_TileSpriteStartPos.y + m_TileSprite->GetTexture()->GetSizeY());
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				if (m_MouseOverTileIndex != -1 &&
					m_MouseOverTileIndex != m_SelectTileIndex)
				{
					float4 TileSize = m_TileSprite->GetCutDataSize();
					int TileXCount = m_TileSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_MouseOverTileIndex % TileXCount) * TileSize.x),
						m_TileSpriteStartPos.y + ((m_MouseOverTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 0.f, 255.f, 0.f, 255.f }, 2.5f);
				}

				if (m_SelectTileIndex != -1)
				{
					float4 TileSize = m_TileSprite->GetCutDataSize();
					int TileXCount = m_TileSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_SelectTileIndex % TileXCount) * TileSize.x),
						m_TileSpriteStartPos.y + ((m_SelectTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 255.f, 0.f, 0.f, 255.f }, 2.5f);
				}
			}

			GJHGameEngineGUI::EndTabItem();
		}

		if (GJHGameEngineGUI::BeginTabItem(L"충돌체"))
		{
			m_TabSelect = TAB::COLLISION;

			if (m_TabSelect != m_PrevTabSelect)
			{
				m_PrevTabSelect = m_TabSelect;
				MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(L"MapCollisionTile.png", { TILESIZEX, TILESIZEY });
			}

			{
				m_TileSpriteStartPos = GJHGameEngineGUI::GetCursorScreenPos();

				m_CollisionSprite = GJHDirectSprite::Find(L"MapCollisionTile.png");

				GJHGameEngineGUI::Image(0, m_CollisionSprite->GetTexture(), m_CollisionSprite->GetTexture()->GetSize());

				int SpriteYSize = m_CollisionSprite->GetCutDataYSize();
				int YLineCount = m_CollisionSprite->GetTexture()->GetSizeY() / SpriteYSize;
				for (int i = 0; i <= YLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x, m_TileSpriteStartPos.y + (i * SpriteYSize));
					float4 EndPos = float4(m_TileSpriteStartPos.x + m_CollisionSprite->GetTexture()->GetSizeX(), m_TileSpriteStartPos.y + (i * SpriteYSize));
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				int SpriteXSize = m_CollisionSprite->GetCutDataXSize();
				int XLineCount = m_CollisionSprite->GetTexture()->GetSizeX() / SpriteXSize;
				for (int i = 0; i <= XLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize), m_TileSpriteStartPos.y);
					float4 EndPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize), m_TileSpriteStartPos.y + m_CollisionSprite->GetTexture()->GetSizeY());
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				if (m_MouseOverTileIndex != -1 &&
					m_MouseOverTileIndex != m_SelectCollisionTileIndex)
				{
					float4 TileSize = m_CollisionSprite->GetCutDataSize();
					int TileXCount = m_CollisionSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_MouseOverTileIndex % TileXCount) * TileSize.x), m_TileSpriteStartPos.y + ((m_MouseOverTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 0.f, 255.f, 0.f, 255.f }, 2.5f);
				}

				if (m_SelectCollisionTileIndex != -1)
				{
					float4 TileSize = m_CollisionSprite->GetCutDataSize();
					int TileXCount = m_CollisionSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_SelectCollisionTileIndex % TileXCount) * TileSize.x), m_TileSpriteStartPos.y + ((m_SelectCollisionTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 255.f, 0.f, 0.f, 255.f }, 2.5f);
				}
			}

			GJHGameEngineString TileType;
			int TileIndex = m_SelectCollisionTileIndex % 4;
			switch (TileIndex)
			{
			case 0:
				TileType = L"이동 불가";
				break;
			case 1:
				TileType = L"플랫폼";
				break;
			case 2:
				TileType = L"탈 수 있는 벽";
				break;
			case 3:
				TileType = L"탈 수 없는 벽";
				break;
			default:
				break;
			}

			GJHGameEngineGUI::Text(L"타일 타입 : ", TileType);

			GJHGameEngineGUI::EndTabItem();
		}

		if (GJHGameEngineGUI::BeginTabItem(L"특수"))
		{
			m_TabSelect = TAB::SPECIAL;

			if (m_TabSelect != m_PrevTabSelect)
			{
				m_PrevTabSelect = m_TabSelect;
				MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(L"SpecialTile.png", { TILESIZEX, TILESIZEY });
			}

			{
				m_TileSpriteStartPos = GJHGameEngineGUI::GetCursorScreenPos();

				m_SpecialSprite = GJHDirectSprite::Find(L"SpecialTile.png");

				GJHGameEngineGUI::Image(0, m_SpecialSprite->GetTexture(), m_SpecialSprite->GetTexture()->GetSize());

				int SpriteYSize = m_SpecialSprite->GetCutDataYSize();
				int YLineCount = m_SpecialSprite->GetTexture()->GetSizeY() / SpriteYSize;
				for (int i = 0; i <= YLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x, m_TileSpriteStartPos.y + (i * SpriteYSize));
					float4 EndPos = float4(m_TileSpriteStartPos.x + m_SpecialSprite->GetTexture()->GetSizeX(), m_TileSpriteStartPos.y + (i * SpriteYSize));
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				int SpriteXSize = m_SpecialSprite->GetCutDataXSize();
				int XLineCount = m_SpecialSprite->GetTexture()->GetSizeX() / SpriteXSize;
				for (int i = 0; i <= XLineCount; ++i)
				{
					float4 StartPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize), m_TileSpriteStartPos.y);
					float4 EndPos = float4(m_TileSpriteStartPos.x + (i * SpriteXSize), m_TileSpriteStartPos.y + m_SpecialSprite->GetTexture()->GetSizeY());
					GJHGameEngineGUI::AddLine(StartPos, EndPos, { 255.f, 255.f, 255.f, 255.f });
				}

				if (m_MouseOverTileIndex != -1 &&
					m_MouseOverTileIndex != m_SelectSpecialTileIndex)
				{
					float4 TileSize = m_SpecialSprite->GetCutDataSize();
					int TileXCount = m_SpecialSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_MouseOverTileIndex % TileXCount) * TileSize.x), m_TileSpriteStartPos.y + ((m_MouseOverTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 0.f, 255.f, 0.f, 255.f }, 2.5f);
				}

				if (m_SelectSpecialTileIndex != -1)
				{
					float4 TileSize = m_SpecialSprite->GetCutDataSize();
					int TileXCount = m_SpecialSprite->GetCutDataXCount();

					float4 Start = float4(m_TileSpriteStartPos.x + ((m_SelectSpecialTileIndex % TileXCount) * TileSize.x), m_TileSpriteStartPos.y + ((m_SelectSpecialTileIndex / TileXCount) * TileSize.y));
					float4 End = Start + TileSize;

					GJHGameEngineGUI::AddRect(Start, End, { 255.f, 0.f, 0.f, 255.f }, 2.5f);
				}
			}

			GJHGameEngineGUI::EndTabItem();
		}
	}

	GJHGameEngineGUI::EndTabBar();
}

void MapEditorWindow::SaveWindow()
{
	static ImGuiTextFilter Filter("");
	Filter.Draw(" ", 200.f);

	GJHGameEngineGUI::BeginChild(L"", float4(GJHGameEngineGUI::GetWindowContentRegionWidth() * 0.4f, 150.f), true);
	GJHGameEngineGUI::Filter(m_SaveFileVec, Filter);
	GJHGameEngineGUI::EndChild();

	if (GJHGameEngineGUI::Button(L"Save") == true)
	{
		float4 CamTransBackUp = MapEditorScene::m_CamTrans->GetWorldPosition();
		MapEditorScene::m_CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, CAMSIZEY * -0.5f, -10.0f });
		MapEditorScene::m_Cam->CalCameraMatrix();

		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"MapData");

		GJHGameEngineString FileName(NewDir.FullPath() + Filter.InputBuf + L".map");

		GJHGameEngineFile File = GJHGameEngineFile(FileName);
		File.Open(L"wb");

		File << m_BackGroundImageVec[m_BackGroundImageSelect];

		File << m_BackTileImageVec[m_BackTileImageSelect];
		File << MapEditorScene::m_BackTileMapRenderer->m_TileSize;
		File << MapEditorScene::m_BackTileMapRenderer->m_TileMap;

		File << m_ForeTileImageVec[m_ForeTileImageSelect];
		File << MapEditorScene::m_ForeTileMapRenderer->m_TileSize;
		File << MapEditorScene::m_ForeTileMapRenderer->m_TileMap;

		File << MapEditorScene::m_CollisionTileMapRenderer->m_TileSize;
		File << MapEditorScene::m_CollisionTileMapRenderer->m_TileMap;

		File << MapEditorScene::m_SpecialTileMapRenderer->m_TileSize;
		File << MapEditorScene::m_SpecialTileMapRenderer->m_TileMap;
		File << MapEditorScene::m_SpecialTileMapRenderer->m_DoorLinkList;

		{
			GJHGameEngineDirectory NewDir;
			NewDir.MoveParent(L"Portfolio");
			NewDir.Move(L"Resources");
			NewDir.Move(L"Texture");
			NewDir.Move(L"Map");
			NewDir.Move(L"Collision");

			float4 MapCount;
			for (int y = 0; y < MapEditorScene::m_CollisionTileMapRenderer->m_TileCountY; y++)
			{
				for (int x = 0; x < MapEditorScene::m_CollisionTileMapRenderer->m_TileCountX; x++)
				{
					if (MapEditorScene::m_CollisionTileMapRenderer->m_TileMap[y][x] != -1 &&
						MapCount.x < x)
					{
						MapCount.x = (float)x;
					}

					if (MapEditorScene::m_CollisionTileMapRenderer->m_TileMap[y][x] != -1 &&
						MapCount.y < y)
					{
						MapCount.y = (float)y;
					}
				}
			}

			float4 MapSize;
			MapSize.x = (MapCount.x + 1) * MapEditorScene::m_CollisionTileMapRenderer->m_TileSize.x;
			MapSize.y = (MapCount.y + 1) * MapEditorScene::m_CollisionTileMapRenderer->m_TileSize.y;

			if (0.f < MapSize.x && 0.f < MapSize.y)
			{
				MapEditorScene::m_PreviewTileMapRenderer->Clear();

				GJHGameEngineString FileName = NewDir.FullPath() + Filter.InputBuf + L"_Collision.png";
				MapEditorScene::m_CollisionTileMapRenderer->Capture(MapEditorScene::m_Cam, MapSize, FileName);
			}

			{
				NewDir.MoveParent(L"Map");
				NewDir.Move(L"MapCapture");

				MapEditorScene::m_BackTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, 1.f });
				MapEditorScene::m_ForeTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, 1.f });

				GJHGameEngineString FileName = NewDir.FullPath() + Filter.InputBuf + L"_Capture.png";
				MapEditorScene::m_BackTileMapRenderer->Capture(MapEditorScene::m_Cam, MapEditorScene::m_ForeTileMapRenderer, MapSize, FileName);

				CheckLayerModeColor();
			}

			File << MapSize;
		}

		SaveFileLoading();

		MapEditorScene::m_CamTrans->SetWorldPosition(CamTransBackUp);
	}

	GJHGameEngineGUI::SameLine();

	if (GJHGameEngineGUI::Button(L"Load") == true)
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"MapData");

		GJHGameEngineString FileName(NewDir.FullPath() + Filter.InputBuf + L".map");

		GJHGameEngineFile File = GJHGameEngineFile(FileName);
		File.Open(L"rb");

		GJHGameEngineString Str;
		{
			File >> Str;
			m_BackGroundImageSelect = (int)std::distance(m_BackGroundImageVec.begin(), std::find(m_BackGroundImageVec.begin(), m_BackGroundImageVec.end(), Str));
			MapEditorWindow::ChangeBackGroundImageComboFunc(m_BackGroundImageVec[m_BackGroundImageSelect]);
		}

		{
			m_LayerModeSelect = (int)LAYERMODE::BACKGROUND;

			File >> Str;
			m_BackTileImageSelect = (int)std::distance(m_BackTileImageVec.begin(), std::find(m_BackTileImageVec.begin(), m_BackTileImageVec.end(), Str));
			MapEditorWindow::ChangeTileImageComboFunc(m_BackTileImageVec[m_BackTileImageSelect]);

			float4 TileSize;
			File >> TileSize;

			MapEditorScene::m_BackTileMapRenderer->CreateTileMap(m_BackTileImageVec[m_BackTileImageSelect], TileSize);
			File >> MapEditorScene::m_BackTileMapRenderer->m_TileMap;
		}

		{
			m_LayerModeSelect = (int)LAYERMODE::FOREGROUND;

			File >> Str;
			m_ForeTileImageSelect = (int)std::distance(m_ForeTileImageVec.begin(), std::find(m_ForeTileImageVec.begin(), m_ForeTileImageVec.end(), Str));
			MapEditorWindow::ChangeTileImageComboFunc(m_ForeTileImageVec[m_ForeTileImageSelect]);

			float4 TileSize;
			File >> TileSize;

			MapEditorScene::m_ForeTileMapRenderer->CreateTileMap(m_ForeTileImageVec[m_ForeTileImageSelect], TileSize);
			File >> MapEditorScene::m_ForeTileMapRenderer->m_TileMap;

			m_LayerModeSelect = (int)LAYERMODE::BACKGROUND;
		}

		{
			float4 ColTileSize;
			File >> ColTileSize;

			MapEditorScene::m_CollisionTileMapRenderer->CreateTileMap(L"MapCollisionTile.png", ColTileSize);
			File >> MapEditorScene::m_CollisionTileMapRenderer->m_TileMap;
		}

		MapEditorScene::m_SpecialTileMapRenderer->Clear();

		{
			float4 SpecialTileSize;
			File >> SpecialTileSize;

			MapEditorScene::m_SpecialTileMapRenderer->CreateTileMap(L"SpecialTile.png", SpecialTileSize);
			File >> MapEditorScene::m_SpecialTileMapRenderer->m_TileMap;
			File >> MapEditorScene::m_SpecialTileMapRenderer->m_DoorLinkList;
		}
	}

	GJHGameEngineGUI::SameLine();

	if (GJHGameEngineGUI::Button(L"Clear") == true)
	{
		MapEditorScene::m_BackTileMapRenderer->Clear();
		MapEditorScene::m_ForeTileMapRenderer->Clear();
		MapEditorScene::m_CollisionTileMapRenderer->Clear();
		MapEditorScene::m_SpecialTileMapRenderer->Clear();
	}
}

void MapEditorWindow::Update()
{
	KeyCheck();
}

int MapEditorWindow::GetSelectTileIndex()
{
	float4 MousePos = GJHGameEngineGUI::GetMousePos();

	std::shared_ptr<GJHDirectSprite> TileSprite = (m_TabSelect == TAB::TILE) ? m_TileSprite : (m_TabSelect == TAB::COLLISION) ? m_CollisionSprite : m_SpecialSprite;

	float4 TileSpriteStartPos = float4(m_TileSpriteStartPos.x - ImGui::GetWindowPos().x, m_TileSpriteStartPos.y - ImGui::GetWindowPos().y);
	float4 TileSpriteEndPos = float4(TileSpriteStartPos.x + TileSprite->GetTexture()->GetSizeX(), TileSpriteStartPos.y + TileSprite->GetTexture()->GetSizeY());

	if (MousePos.x < TileSpriteStartPos.x ||
		TileSpriteEndPos.x < MousePos.x ||
		MousePos.y < TileSpriteStartPos.y ||
		TileSpriteEndPos.y < MousePos.y)
	{
		SetGuiStyle((int)Style::None);

		return -1;
	}

	SetGuiStyle((int)Style::NoMove | (int)Style::None);

	MousePos -= TileSpriteStartPos;

	int X = static_cast<int>(MousePos.x / TileSprite->GetCutDataXSize());
	int Y = static_cast<int>(MousePos.y / TileSprite->GetCutDataYSize());

	int Index = X + (Y * TileSprite->GetCutDataXCount());

	return Index;
}

void MapEditorWindow::KeyCheck()
{
	{
		int TileIndex = GetSelectTileIndex();
		m_MouseOverTileIndex = TileIndex;

		if (KEYDOWN(L"MouseClick"))
		{
			if (TileIndex != -1)
			{
				switch (m_TabSelect)
				{
				case TAB::TILE:
					m_SelectTileIndex = TileIndex;
					break;
				case TAB::COLLISION:
					m_SelectCollisionTileIndex = TileIndex;
					break;
				case TAB::SPECIAL:
					m_SelectSpecialTileIndex = TileIndex;
					break;
				default:
					break;
				}
			}
		}
	}

	{
		if (KEYDOWN(L"F3"))
		{
			m_SelectTileAlphaEnable = !m_SelectTileAlphaEnable;
			if (m_SelectTileAlphaEnable == true)
			{
				m_NoneSelectTileAlpha = 0.5f;
			}
			else
			{
				m_NoneSelectTileAlpha = 1.f;
			}

			CheckLayerModeColor();
		}
	}

	{
		if (KEYDOWN(L"Numpad8"))
		{
			std::shared_ptr<GJHDirectSprite> TileSprite = (m_TabSelect == TAB::TILE) ? m_TileSprite : (m_TabSelect == TAB::COLLISION) ? m_CollisionSprite : m_SpecialSprite;

			int TileCountX = TileSprite->GetTexture()->GetSizeX() / TileSprite->GetCutDataXSize();
			int TileCountY = TileSprite->GetTexture()->GetSizeY() / TileSprite->GetCutDataYSize();

			int& TileIndex = (m_TabSelect == TAB::TILE) ? m_SelectTileIndex : (m_TabSelect == TAB::COLLISION) ? m_SelectCollisionTileIndex : m_SelectSpecialTileIndex;

			if (TileCountX <= TileIndex)
			{
				TileIndex -= TileCountX;
			}
		}

		if (KEYDOWN(L"Numpad5"))
		{
			std::shared_ptr<GJHDirectSprite> TileSprite = (m_TabSelect == TAB::TILE) ? m_TileSprite : (m_TabSelect == TAB::COLLISION) ? m_CollisionSprite : m_SpecialSprite;

			int TileCountX = TileSprite->GetTexture()->GetSizeX() / TileSprite->GetCutDataXSize();
			int TileCountY = TileSprite->GetTexture()->GetSizeY() / TileSprite->GetCutDataYSize();

			int& TileIndex = (m_TabSelect == TAB::TILE) ? m_SelectTileIndex : (m_TabSelect == TAB::COLLISION) ? m_SelectCollisionTileIndex : m_SelectSpecialTileIndex;

			if ((TileIndex / TileCountX + 1) < TileCountY)
			{
				TileIndex += TileCountX;
			}
		}

		if (KEYDOWN(L"Numpad4"))
		{
			std::shared_ptr<GJHDirectSprite> TileSprite = (m_TabSelect == TAB::TILE) ? m_TileSprite : (m_TabSelect == TAB::COLLISION) ? m_CollisionSprite : m_SpecialSprite;

			int TileCountX = TileSprite->GetTexture()->GetSizeX() / TileSprite->GetCutDataXSize();
			int TileCountY = TileSprite->GetTexture()->GetSizeY() / TileSprite->GetCutDataYSize();

			int& TileIndex = (m_TabSelect == TAB::TILE) ? m_SelectTileIndex : (m_TabSelect == TAB::COLLISION) ? m_SelectCollisionTileIndex : m_SelectSpecialTileIndex;

			if (0 < (TileIndex % TileCountX))
			{
				TileIndex -= 1;
			}
		}

		if (KEYDOWN(L"Numpad6"))
		{
			std::shared_ptr<GJHDirectSprite> TileSprite = (m_TabSelect == TAB::TILE) ? m_TileSprite : (m_TabSelect == TAB::COLLISION) ? m_CollisionSprite : m_SpecialSprite;

			int TileCountX = TileSprite->GetTexture()->GetSizeX() / TileSprite->GetCutDataXSize();
			int TileCountY = TileSprite->GetTexture()->GetSizeY() / TileSprite->GetCutDataYSize();

			int& TileIndex = (m_TabSelect == TAB::TILE) ? m_SelectTileIndex : (m_TabSelect == TAB::COLLISION) ? m_SelectCollisionTileIndex : m_SelectSpecialTileIndex;

			if ((TileIndex % TileCountX + 1) < TileCountX)
			{
				TileIndex += 1;
			}
		}

		if (KEYDOWN(L"NumpadMul"))
		{
			m_LayerModeSelect = ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND) ? (int)LAYERMODE::FOREGROUND : (int)LAYERMODE::BACKGROUND;

			m_SelectTileIndex = 0;

			CheckLayerModeColor();
		}
	}
}

void MapEditorWindow::SaveFileLoading()
{
	{
		m_SaveFileVec.clear();
		
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"MapData");

		auto Arr = NewDir.AllFile(L"map");
		std::vector<GJHGameEngineString> Arr2;

		for (size_t i = 0; i < Arr.size(); i++)
		{
			Arr2.push_back(GJHGameEnginePath::GetFileExt(Arr[i].FullPath()));
		}

		for (size_t i = 0; i < Arr2.size(); i++)
		{
			m_SaveFileVec.push_back(Arr2[i]);
		}
	}
}

void MapEditorWindow::CheckLayerModeColor()
{
	if (m_TabSelect == TAB::TILE)
	{
		if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
		{
			MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(m_BackTileImageVec[m_BackTileImageSelect], { TILESIZEX, TILESIZEY });
		}
		else if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::FOREGROUND)
		{
			MapEditorScene::m_PreviewTileMapRenderer->ChangeTileMap(m_ForeTileImageVec[m_ForeTileImageSelect], { TILESIZEX, TILESIZEY });
		}
	}

	if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::BACKGROUND)
	{
		MapEditorScene::m_BackTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, 1.f });
		MapEditorScene::m_ForeTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, m_NoneSelectTileAlpha });
	}
	else if ((LAYERMODE)m_LayerModeSelect == LAYERMODE::FOREGROUND)
	{
		MapEditorScene::m_BackTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, m_NoneSelectTileAlpha });
		MapEditorScene::m_ForeTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, 1.f });
	}
}