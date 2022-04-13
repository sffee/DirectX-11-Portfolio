#include "Stage3.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include "Player.h"
#include "Gate.h"
#include "Pomp.h"
#include "Grunt.h"
#include "Cop.h"
#include "ShieldCop.h"
#include "define.h"

Stage3::Stage3()
{

}

Stage3::~Stage3()
{

}

void Stage3::Start()
{
	StageBase::Start(L"Stage3");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle_0", { 52.f, -496.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	CreateActor<Grunt>(ACTORTYPE::MONSTER, L"Grunt_Idle", { 345.f, -496.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 680.f, -496.f, 0.f });
	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 867.f, -496.f, 0.f });

	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 297.f, -368.f, 0.f });
	CreateActor<Pomp>(ACTORTYPE::MONSTER, L"Pomp_Idle", { 690.f, -368.f, 0.f });
	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 1073.f, -368.f, 0.f });

	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 86.f, -240.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 340.f, -240.f, 0.f });
	CreateActor<Pomp>(ACTORTYPE::MONSTER, L"Pomp_Idle", { 471.f, -240.f, 0.f });
	CreateActor<Grunt>(ACTORTYPE::MONSTER, L"Grunt_Idle", { 830.f, -240.f, 0.f });
	CreateActor<Pomp>(ACTORTYPE::MONSTER, L"Pomp_Idle", { 1147.f, -240.f, 0.f });

	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 341.f, -496.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 742.f, -496.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 389.f, -368.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 805.f, -240.f, 1.f });

	CreateGate<Gate>(L"Stage6", { 35.f, -190.f }, -1, m_Cam);
}

void Stage3::SceneChangeStart()
{
	StageBase::SceneChangeStart();
	SOUNDPLAY(L"BGM", L"Dragon.ogg", -1);
}