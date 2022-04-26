#include "StageBase.h"
#include <GJHGameEngineDirectory.h>
#include <GJHSpriteRenderer.h>
#include <GJHTileMapRenderer.h>
#include <GJHSpecialTileMapRenderer.h>
#include <GJHGameEngineScene.h>
#include <GJHGameEngineTime.h>
#include <GJHWorldComponent.h>
#include <GJHGUIComponent.h>
#include "PlaySceneWindow.h"
#include "AStarManager.h"
#include "UIManager.h"
#include <GJHCamera.h>
#include "Player.h"
#include "LogicValue.h"
#include <TypeComponent.h>
#include <GJHSceneChangeRenderer.h>
#include <GJHUvDistortion.h>
#include <GJHGameEngineSound.h>
#include <GJHGameEngineWindow.h>
#include "MouseCursor.h"
#include "define.h"

std::shared_ptr<GJHDirectTexture2D> StageBase::m_ColMapTexture = nullptr;
bool StageBase::m_IsDebug = false;

StageBase::StageBase()
{
}

StageBase::~StageBase()
{

}

void StageBase::Start(const GJHGameEngineString& _MapFileName)
{
	GetGUI()->CreateGUIWindow<PlaySceneWindow>(L"PlaySceneWindow");

	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::Monster);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::MonsterAttack);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::MonsterShieldAttack);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::MonsterBullet);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::Gate);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Player, (int)CollisionLayer::Door);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerSlash, (int)CollisionLayer::Monster);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerSlash, (int)CollisionLayer::MonsterBullet);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerSlash, (int)CollisionLayer::MonsterShield);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerSlash, (int)CollisionLayer::Door);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerSlash, (int)CollisionLayer::HeadHunter);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerBullet, (int)CollisionLayer::Monster);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerBullet, (int)CollisionLayer::MonsterShield);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Monster, (int)CollisionLayer::Door);
	GetWorld()->SetCollisionLink((int)CollisionLayer::Monster, (int)CollisionLayer::DoorAttack);
	GetWorld()->SetCollisionLink((int)CollisionLayer::MonsterShield, (int)CollisionLayer::Door);
	GetWorld()->SetCollisionLink((int)CollisionLayer::MonsterShield, (int)CollisionLayer::DoorAttack);
	GetWorld()->SetCollisionLink((int)CollisionLayer::HeadHunterLaser, (int)CollisionLayer::Player);
	GetWorld()->SetCollisionLink((int)CollisionLayer::HeadHunterGrenadeExplosion, (int)CollisionLayer::Player);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerFake, (int)CollisionLayer::Door);
	GetWorld()->SetCollisionLink((int)CollisionLayer::PlayerFake, (int)CollisionLayer::Gate);

	CreateMap(_MapFileName);

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_Cam = Actor->CreateComponent<GJHCamera>(0, 0);

		m_Cam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		m_Cam->SetCamProjectionMode(CamProjectionMode::Orthographic);

		std::shared_ptr<GJHActorTransform> CamTrans = Actor->FindComponent<GJHActorTransform>();
		CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, CAMSIZEY * -0.5f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_UICam = Actor->CreateComponent<GJHCamera>(1, 1);
		m_UICam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		m_UICam->SetCamProjectionMode(CamProjectionMode::Orthographic);

		std::shared_ptr<GJHActorTransform> CamTrans = Actor->FindComponent<GJHActorTransform>();
		CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, CAMSIZEY * -0.5f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHCamera> TransitionCam = Actor->CreateComponent<GJHCamera>(2, 2);
		TransitionCam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		TransitionCam->SetCamProjectionMode(CamProjectionMode::Orthographic);

		std::shared_ptr<GJHActorTransform> CamTrans = Actor->FindComponent<GJHActorTransform>();
		CamTrans->SetWorldPosition({ CAMSIZEX * 0.5f, CAMSIZEY * -0.5f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		Actor->CreateComponent<MouseCursor>(m_Cam);
	}

	{
		ActorPtr UIActor = GetScene()->CreateActor();
		m_UIManager = UIActor->CreateComponent<UIManager>();
	}
}

void StageBase::CreateMap(const GJHGameEngineString& _MapFileName)
{
	GJHGameEngineDirectory NewDir;
	NewDir.MoveUp();
	NewDir.Move(L"Resources");
	NewDir.Move(L"MapData");

	GJHGameEngineString FileName(NewDir.FullPath() + _MapFileName + L".map");

	GJHGameEngineFile File = GJHGameEngineFile(FileName);
	File.Open(L"rb");

	{
		GJHGameEngineString BackGroundImageStr;
		File >> BackGroundImageStr;

		ActorPtr Actor = GetScene()->CreateActor();
		m_BackGroundImageRenderer = Actor->CreateComponent<GJHSpriteRenderer>();
		m_BackGroundImageRenderer->TextureSetting(BackGroundImageStr, true);
		Actor->CreateComponent<TypeComponent>(ACTORTYPE::BACKGROUND);

		float4 Pos = m_BackGroundImageRenderer->GetWorldScale();
		Pos.ReverseY();

		m_BackGroundImageRenderer->SetWorldPosition({ Pos.HalfVector2D().x, Pos.HalfVector2D().y, 100.f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_TileRenderer = Actor->CreateComponent<GJHSpriteRenderer>();
		Actor->CreateComponent<TypeComponent>(ACTORTYPE::BACKGROUND);

		GJHGameEngineString TextureName = _MapFileName + L"_Capture.png";

		m_TileRenderer->TextureSetting(TextureName, true);

		float4 Pos = m_TileRenderer->GetWorldScale();
		Pos.ReverseY();

		m_TileRenderer->SetWorldPosition({ Pos.HalfVector2D().x, Pos.HalfVector2D().y, 100.f });
	}

	{
		GJHGameEngineString BackTileImageStr;
		File >> BackTileImageStr;

		float4 TileSize;
		File >> TileSize;

		std::vector<std::vector<int>> TileMap;
		File >> TileMap;
	}

	{
		GJHGameEngineString ForeTileImageStr;
		File >> ForeTileImageStr;

		float4 TileSize;
		File >> TileSize;

		std::vector<std::vector<int>> TileMap;
		File >> TileMap;
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_ColTileMapRenderer = Actor->CreateComponent<GJHTileMapRenderer>();
		Actor->CreateComponent<TypeComponent>(ACTORTYPE::BACKGROUND);
		
		float4 ColTileSize;
		File >> ColTileSize;

		m_ColTileMapRenderer->CreateTileMap(L"MapCollisionTile.png", ColTileSize);

		File >> m_ColTileMapRenderer->m_TileMap;
		m_ColTileMapRenderer->SetRenderEnable(false);
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_SpecialTileMapRenderer = Actor->CreateComponent<GJHSpecialTileMapRenderer>();
		Actor->CreateComponent<TypeComponent>(ACTORTYPE::BACKGROUND);

		float4 TileSize;
		File >> TileSize;
		m_SpecialTileMapRenderer->CreateTileMap(L"SpecialTile.png", TileSize);
		
		std::map<TileInfo, TILETYPE> TileMap;
		File >> TileMap;
		m_SpecialTileMapRenderer->SetTileMap(TileMap);

		std::map<TileInfo, std::set<TileInfo>> DoorList;
		File >> DoorList;
		m_SpecialTileMapRenderer->SetDoorLink(DoorList);

		m_SpecialTileMapRenderer->SetWorldPosition({ 0.f, 0.f, 1.f });
	}

	File >> m_MapSize;

	m_ColMapTextureName = _MapFileName;

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_DebugColTileMapRenderer = Actor->CreateComponent<GJHSpriteRenderer>();
		Actor->CreateComponent<TypeComponent>(ACTORTYPE::BACKGROUND);

		GJHGameEngineString TextureName = _MapFileName + L"_Collision.png";

		m_DebugColTileMapRenderer->TextureSetting(TextureName, true);

		float4 Pos = m_DebugColTileMapRenderer->GetWorldScale();
		Pos.ReverseY();

		m_DebugColTileMapRenderer->SetWorldPosition({ Pos.HalfVector2D().x, Pos.HalfVector2D().y, 100.f });
		m_DebugColTileMapRenderer->SetRenderEnable(false);
	}
}

void StageBase::Update()
{
	AddStageTime(DELTATIME());

	if (KEYDOWN(L"F7"))
	{
		m_IsDebug = !m_IsDebug;
		m_DebugColTileMapRenderer->SetRenderEnable(m_IsDebug);
	}
}

void StageBase::SceneChangeStart()
{
	ResetStageTime();
	m_ColMapTexture = GJHDirectTexture2D::Find(m_ColMapTextureName + L"_Collision.png");
	m_DebugColTileMapRenderer->SetRenderEnable(m_IsDebug);

	AStarManager::CreateMapData(m_ColTileMapRenderer->m_TileMap, m_SpecialTileMapRenderer->m_TileMap, m_SpecialTileMapRenderer->m_DoorLinkList, m_MapSize);

	std::shared_ptr<GJHSceneChangeRenderer> Render = GetScene()->CreateActor()->CreateComponent<GJHSceneChangeRenderer>(2, SceneChangeType::End);
	SOUNDPLAY(L"Transition_End.wav");
}