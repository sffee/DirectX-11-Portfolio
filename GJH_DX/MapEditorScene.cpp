#include "MapEditorScene.h"
#include <GJHGameEngineScene.h>
#include <GJHGameEngineActor.h>
#include <GJHActorTransform.h>
#include <GJHCamera.h>
#include <GJHSpriteRenderer.h>
#include <GJHGameEngineDirectory.h>
#include <GJHDirectTexture2D.h>
#include <GJHDirectSprite.h>
#include <GJHSpriteAnimation.h>
#include <GJHCollisionComponent.h>
#include <GJHWorldComponent.h>
#include <GJHGameEngineCollision.h>
#include "LogicValue.h"
#include <GJHGUIComponent.h>
#include "MapEditorWindow.h"
#include "define.h"
#include <GJHTileMapRenderer.h>
#include <GJHSpecialTileMapRenderer.h>
#include <GJHGameEngineWindow.h>

std::shared_ptr<GJHSpriteRenderer> MapEditorScene::m_BackGroundImage = nullptr;
std::shared_ptr<GJHCamera> MapEditorScene::m_Cam = nullptr;
std::shared_ptr<GJHActorTransform> MapEditorScene::m_CamTrans = nullptr;
std::shared_ptr<GJHTileMapRenderer> MapEditorScene::m_BackTileMapRenderer = nullptr;
std::shared_ptr<GJHTileMapRenderer> MapEditorScene::m_ForeTileMapRenderer = nullptr;
std::shared_ptr<GJHTileMapRenderer> MapEditorScene::m_PreviewTileMapRenderer = nullptr;
std::shared_ptr<GJHTileMapRenderer> MapEditorScene::m_CollisionTileMapRenderer = nullptr;
std::shared_ptr<GJHSpecialTileMapRenderer> MapEditorScene::m_SpecialTileMapRenderer = nullptr;
bool MapEditorScene::m_IsShowCollisionTile = true;
bool MapEditorScene::m_IsShowSpecialTile = true;
bool MapEditorScene::m_IsDoorLinkMode = false;

MapEditorScene::MapEditorScene()
{

}

MapEditorScene::~MapEditorScene()
{

}

void MapEditorScene::Start()
{
	GetGUI()->CreateGUIWindow<MapEditorWindow>(L"MapEditorWindow");

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_Cam = Actor->CreateComponent<GJHCamera>(0, 0);

		m_Cam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		m_Cam->SetCamProjectionMode(CamProjectionMode::Orthographic);

		m_CamTrans = Actor->FindComponent<GJHActorTransform>();
		m_CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, CAMSIZEY * -0.5f, -10.0f });
	}

	{
		ActorPtr Actor = CreateActor(ACTORTYPE::BACKGROUND, L"None", { 0.f, 0.f, 0.f });
		m_BackGroundImage = Actor->FindComponent<GJHSpriteRenderer>();
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();

		Ptr->SetLocalPosition({ 0.f, 0.f, -2.f } );

		m_BackTileMapRenderer = Actor->CreateComponent<GJHTileMapRenderer>();
		m_BackTileMapRenderer->CreateTileMap(L"Prison_BackGround.png", { TILESIZEX, TILESIZEY });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();

		Ptr->SetLocalPosition({ 0.f, 0.f, -3.f });

		m_ForeTileMapRenderer = Actor->CreateComponent<GJHTileMapRenderer>();
		m_ForeTileMapRenderer->CreateTileMap(L"Prison_ForeGround.png", { TILESIZEX, TILESIZEY });
		m_ForeTileMapRenderer->SetColor({ 1.f, 1.f, 1.f, 0.5f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();

		Ptr->SetLocalPosition({ 0.f, 0.f, -9.f });

		m_PreviewTileMapRenderer = Actor->CreateComponent<GJHTileMapRenderer>();
		m_PreviewTileMapRenderer->CreateTileMap(L"Prison_BackGround.png", { TILESIZEX, TILESIZEY });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();

		Ptr->SetLocalPosition({ 0.f, 0.f, -4.f });

		m_CollisionTileMapRenderer = Actor->CreateComponent<GJHTileMapRenderer>();
		m_CollisionTileMapRenderer->CreateTileMap(L"MapCollisionTile.png", { TILESIZEX, TILESIZEY });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();

		Ptr->SetLocalPosition({ 0.f, 0.f, -5.f });

		m_SpecialTileMapRenderer = Actor->CreateComponent<GJHSpecialTileMapRenderer>();
		m_SpecialTileMapRenderer->CreateTileMap(L"SpecialTile.png", { TILESIZEX, TILESIZEY });
	}

	{
		m_MapSize.x = m_BackTileMapRenderer->m_TileCountX * m_BackTileMapRenderer->m_TileSize.x;
		m_MapSize.y = m_BackTileMapRenderer->m_TileCountY * m_BackTileMapRenderer->m_TileSize.y;
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_DoorLineRender = Actor->CreateComponent<GJHRenderer>();
		m_DoorLineRender->CreateRenderPlayer(L"BoxMat", L"Ray");
		m_DoorLineRender->CBufferSettingNew(L"ColorData", float4::RED);
	}
}

void MapEditorScene::Update()
{
	CheckKey();
	PrevRenderUpdate();
	SpecialTileMapDoorLineUpdate();
	CamPosCheck();
}

void MapEditorScene::CheckKey()
{
	if (KEYPRESS(L"LMove"))
	{
		m_CamTrans->SetWorldMove(GJHGameEngineVector::LEFT * GJHGameEngineTime::FDeltaTime(200.f));
	}

	if (KEYPRESS(L"RMove"))
	{
		m_CamTrans->SetWorldMove(GJHGameEngineVector::RIGHT * GJHGameEngineTime::FDeltaTime(200.f));
	}

	if (KEYPRESS(L"Jump"))
	{
		m_CamTrans->SetWorldMove(GJHGameEngineVector::UP * GJHGameEngineTime::FDeltaTime(200.f));
	}

	if (KEYPRESS(L"Sit"))
	{
		m_CamTrans->SetWorldMove(GJHGameEngineVector::DOWN * GJHGameEngineTime::FDeltaTime(200.f));
	}

	if (KEYDOWN(L"F1"))
	{
		m_IsShowCollisionTile = !m_IsShowCollisionTile;
		m_CollisionTileMapRenderer->SetRenderEnable(m_IsShowCollisionTile);
	}

	if (KEYDOWN(L"F2"))
	{
		m_IsShowSpecialTile = !m_IsShowSpecialTile;
		m_SpecialTileMapRenderer->SetRenderEnable(m_IsShowSpecialTile);
	}

	if (KEYPRESS(L"LShift") && KEYDOWN(L"MouseClick")
		&& GJHGameEngineWindow::GetWinActive() == true)
	{
		float4 MousePos = GJHGameEngineWindow::GetMousePos();
		float4 WindowSize = GJHGameEngineWindow::GetSize();

		if (0.f < MousePos.x &&
			MousePos.x < WindowSize.x &&
			0.f < MousePos.y &&
			MousePos.y < WindowSize.y)
		{
			float XRatio = WindowSize.x / CAMSIZEX;
			float YRatio = WindowSize.y / CAMSIZEY;

			float4 CamTrans = m_CamTrans->GetWorldPosition();
			CamTrans.y *= -1.f;
			CamTrans -= float4(CAMSIZEX * 0.5f, CAMSIZEY * 0.5f);

			MousePos.x = MousePos.x / XRatio + CamTrans.x;
			MousePos.y = MousePos.y / YRatio + CamTrans.y;

			TILETYPE TileType = m_SpecialTileMapRenderer->GetTileType(MousePos);
			if (TileType == TILETYPE::DOOR)
			{
				if (m_IsDoorLinkMode == false)
				{
					m_IsDoorLinkMode = true;
					m_PreviewTileMapRenderer->SetRenderEnable(false);

					m_SelectDoorTilePos = m_SpecialTileMapRenderer->GetTilePos(MousePos);
					float4 TilePos = m_SelectDoorTilePos + float4(m_SpecialTileMapRenderer->m_TileSize.x * 0.5f, m_SpecialTileMapRenderer->m_TileSize.y * -0.5f, -10.f);
					m_DoorLineRender->SetWorldPosition(TilePos);
				}
				else
				{
					float4 TilePos = m_SpecialTileMapRenderer->GetTilePos(MousePos);
					if (m_SelectDoorTilePos != TilePos)
					{
						m_IsDoorLinkMode = false;
						m_PreviewTileMapRenderer->SetRenderEnable(true);

						m_SpecialTileMapRenderer->SetDoorLink(m_SpecialTileMapRenderer->GetTileIndex(m_SelectDoorTilePos), m_SpecialTileMapRenderer->GetTileIndex(TilePos));
						m_DoorLineRender->SetWorldScaleX(0.f);
					}
				}
			}
		}

		return;
	}

	if (KEYPRESS(L"MouseClick")
		&& m_IsDoorLinkMode == false
		&& GJHGameEngineWindow::GetWinActive() == true)
	{
		float4 MousePos = GJHGameEngineWindow::GetMousePos();
		float4 WindowSize = GJHGameEngineWindow::GetSize();

		if (0.f < MousePos.x &&
			MousePos.x < WindowSize.x &&
			0.f < MousePos.y &&
			MousePos.y < WindowSize.y)
		{
			float XRatio = WindowSize.x / CAMSIZEX;
			float YRatio = WindowSize.y / CAMSIZEY;

			float4 CamTrans = m_CamTrans->GetWorldPosition();
			CamTrans.y *= -1.f;
			CamTrans -= float4(CAMSIZEX * 0.5f, CAMSIZEY * 0.5f);

			MousePos.x = MousePos.x / XRatio + CamTrans.x;
			MousePos.y = MousePos.y / YRatio + CamTrans.y;

			if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::TILE)
			{
				if ((MapEditorWindow::LAYERMODE)MapEditorWindow::m_LayerModeSelect == MapEditorWindow::LAYERMODE::BACKGROUND)
					m_BackTileMapRenderer->SetTile(MousePos, MapEditorWindow::m_SelectTileIndex);
				else if ((MapEditorWindow::LAYERMODE)MapEditorWindow::m_LayerModeSelect == MapEditorWindow::LAYERMODE::FOREGROUND)
					m_ForeTileMapRenderer->SetTile(MousePos, MapEditorWindow::m_SelectTileIndex);
			}
			else if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::COLLISION)
				m_CollisionTileMapRenderer->SetTile(MousePos, MapEditorWindow::m_SelectCollisionTileIndex);
			else if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::SPECIAL)
				m_SpecialTileMapRenderer->SetTile(MousePos, MapEditorWindow::m_SelectSpecialTileIndex);
		}
	}

	if (KEYPRESS(L"MouseRClick")
		&& m_IsDoorLinkMode == false
		&& GJHGameEngineWindow::GetWinActive() == true)
	{
		float4 MousePos = GJHGameEngineWindow::GetMousePos();
		float4 WindowSize = GJHGameEngineWindow::GetSize();

		if (0.f < MousePos.x &&
			MousePos.x < WindowSize.x &&
			0.f < MousePos.y &&
			MousePos.y < WindowSize.y)
		{
			float XRatio = WindowSize.x / CAMSIZEX;
			float YRatio = WindowSize.y / CAMSIZEY;

			float4 CamTrans = m_CamTrans->GetWorldPosition();
			CamTrans.y *= -1.f;
			CamTrans -= float4(CAMSIZEX * 0.5f, CAMSIZEY * 0.5f);

			MousePos.x = MousePos.x / XRatio + CamTrans.x;
			MousePos.y = MousePos.y / YRatio + CamTrans.y;

			if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::TILE)
			{
				if ((MapEditorWindow::LAYERMODE)MapEditorWindow::m_LayerModeSelect == MapEditorWindow::LAYERMODE::BACKGROUND)
				{
					m_BackTileMapRenderer->SetTile(MousePos, -1);
				}
				else if ((MapEditorWindow::LAYERMODE)MapEditorWindow::m_LayerModeSelect == MapEditorWindow::LAYERMODE::FOREGROUND)
				{
					m_ForeTileMapRenderer->SetTile(MousePos, -1);
				}
			}
			else if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::COLLISION)
			{
				m_CollisionTileMapRenderer->SetTile(MousePos, -1);
			}
			else if (MapEditorWindow::m_TabSelect == MapEditorWindow::TAB::SPECIAL)
			{
				m_SpecialTileMapRenderer->SetTile(MousePos, -1);
				m_SpecialTileMapRenderer->DeleteDoorLink(m_SpecialTileMapRenderer->GetTileIndex(MousePos));
			}
		}
	}

	if (KEYDOWN(L"MouseRClick")
		&& m_IsDoorLinkMode == true
		&& GJHGameEngineWindow::GetWinActive() == true)
	{
		m_IsDoorLinkMode = false;
		m_DoorLineRender->SetWorldScaleX(0.f);
	}
}

void MapEditorScene::CamPosCheck()
{
	float4 OriginCamPos = m_CamTrans->GetWorldPosition();
	float4 CamPos;
	CamPos.x = OriginCamPos.x - CAMSIZEX * 0.5f;
	CamPos.y = OriginCamPos.y + CAMSIZEY * 0.5f;

	float4 CamMaxPos;
	CamMaxPos.x = CamPos.x + CAMSIZEX;
	CamMaxPos.y = CamPos.y - CAMSIZEY;

	if (CamPos.x < 0.f)
	{
		m_CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, m_CamTrans->GetWorldPosition().y, OriginCamPos.z });
	}
	if (m_MapSize.x < CamMaxPos.x)
	{
		m_CamTrans->SetWorldPosition({ m_MapSize.x, m_CamTrans->GetWorldPosition().y, OriginCamPos.z });
	}

	if (0.f < CamPos.y)
	{
		m_CamTrans->SetWorldPosition({ m_CamTrans->GetWorldPosition().x, CAMSIZEY * -0.5f, OriginCamPos.z });
	}
	if (m_MapSize.y < CamMaxPos.y)
	{
		m_CamTrans->SetWorldPosition({ m_CamTrans->GetWorldPosition().x, m_MapSize.y, OriginCamPos.z });
	}
}

void MapEditorScene::PrevRenderUpdate()
{
	float4 MousePos = GJHGameEngineWindow::GetMousePos();
	float4 WindowSize = GJHGameEngineWindow::GetSize();

	float XRatio = WindowSize.x / CAMSIZEX;
	float YRatio = WindowSize.y / CAMSIZEY;

	float4 CamTrans = m_CamTrans->GetWorldPosition();
	CamTrans.y *= -1.f;
	CamTrans -= float4(CAMSIZEX * 0.5f, CAMSIZEY * 0.5f);

	MousePos.x = MousePos.x / XRatio + CamTrans.x;
	MousePos.y = MousePos.y / YRatio + CamTrans.y;

	m_PreviewTileMapRenderer->Clear();

	int TileIndex;
	switch (MapEditorWindow::m_TabSelect)
	{
	case MapEditorWindow::TAB::TILE:
		TileIndex = MapEditorWindow::m_SelectTileIndex;
		break;
	case MapEditorWindow::TAB::COLLISION:
		TileIndex = MapEditorWindow::m_SelectCollisionTileIndex;
		break;
	case MapEditorWindow::TAB::SPECIAL:
		TileIndex = MapEditorWindow::m_SelectSpecialTileIndex;
		break;
	default:
		break;
	}

	m_PreviewTileMapRenderer->SetTile(MousePos, TileIndex);
}

void MapEditorScene::SpecialTileMapDoorLineUpdate()
{
	if (m_IsDoorLinkMode == false)
	{
		return;
	}

	float4 MousePos = GJHGameEngineWindow::GetMousePos();
	float4 WindowSize = GJHGameEngineWindow::GetSize();

	float XRatio = WindowSize.x / CAMSIZEX;
	float YRatio = WindowSize.y / CAMSIZEY;

	float4 CamTrans = m_CamTrans->GetWorldPosition();
	CamTrans.y *= -1.f;
	CamTrans -= float4(CAMSIZEX * 0.5f, CAMSIZEY * 0.5f);

	MousePos.x = MousePos.x / XRatio + CamTrans.x;
	MousePos.y = MousePos.y / YRatio + CamTrans.y;
	MousePos.y *= -1.f;

	float4 DoorLineStartPos = m_DoorLineRender->GetWorldPosition();
	MousePos.z = DoorLineStartPos.z;

	float Angle = float4::CalVectorAngle360(1, MousePos, DoorLineStartPos);
	m_DoorLineRender->SetWorldRotationZ(Angle);

	float Len = float4::CalLen(MousePos - DoorLineStartPos);
	m_DoorLineRender->SetWorldScaleX(Len);
}

void MapEditorScene::SceneChangeStart()
{
	ShowCursor(true);
	GJHSpecialTileMapRenderer::SetDebugRender(true);
}

void MapEditorScene::SceneChangeEnd()
{
	GJHSpecialTileMapRenderer::SetDebugRender(false);
}