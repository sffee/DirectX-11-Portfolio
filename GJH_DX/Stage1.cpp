#include "Stage1.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include "Player.h"
#include "Pomp.h"
#include "Grunt.h"
#include "Cop.h"
#include "Gate.h"
#include "Door.h"
#include "ShieldCop.h"
#include "UIManager.h"
#include <GJHSceneChangeRenderer.h>
#include <GJHGameEngineSound.h>
#include "MouseCursor.h"
#include "define.h"

Stage1::Stage1()
{

}

Stage1::~Stage1()
{

}

void Stage1::Start()
{
	StageBase::Start(L"Stage1");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle", { 600.f, -303.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	CreateActor<Pomp>(ACTORTYPE::MONSTER, L"Pomp_Idle", { 190.f, -175.f, 0.f });
	CreateActor<Grunt>(ACTORTYPE::MONSTER, L"Grunt_Idle", { 510.f, -175.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 635.f, -175.f, 0.f });

	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 500.f, -175.f, 1.f });

	CreateGate<Gate>(L"Stage2", { 20.f, -130.f }, -1, m_Cam);
}

void Stage1::SceneChangeStart()
{
	StageBase::SceneChangeStart();
	SOUNDPLAY(L"BGM", L"Dragon.ogg", -1);
}