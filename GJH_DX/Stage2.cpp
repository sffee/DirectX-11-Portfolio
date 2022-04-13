#include "Stage2.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include "Player.h"
#include "Pomp.h"
#include "Grunt.h"
#include "Cop.h"
#include "ShieldCop.h"
#include "Gate.h"
#include "Door.h"
#include "define.h"

Stage2::Stage2()
{

}

Stage2::~Stage2()
{

}

void Stage2::Start()
{
	StageBase::Start(L"Stage2");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle_0", { 942.f, -160.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 654.f, -160.f, 0.f });

	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 59.f, -304.f, 0.f });
	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 390.f, -304.f, 0.f });
	CreateActor<Grunt>(ACTORTYPE::MONSTER, L"Grunt_Idle", { 645.f, -304.f, 0.f });
	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 792.f, -304.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 800.f, -304.f, 0.f });

	CreateActor<Cop>(ACTORTYPE::MONSTER, L"Cop_Idle", { 501.f, -448.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 582.f, -448.f, 0.f });
	CreateActor<Pomp>(ACTORTYPE::MONSTER, L"Pomp_Idle", { 709.f, -448.f, 0.f });

	CreateActor<Grunt>(ACTORTYPE::MONSTER, L"Grunt_Idle", { 695.f, -576.f, 0.f });
	CreateActor<ShieldCop>(ACTORTYPE::MONSTER, L"ShieldCop_Idle", { 695.f, -576.f, 0.f });

	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 708.f, -160.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 741.f, -304.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 323.f, -304.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 453.f, -448.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 691.f, -448.f, 1.f });
	CreateActor<Door>(ACTORTYPE::DOOR, L"Door_Iron_Idle", { 676.f, -576.f, 1.f });

	CreateGate<Gate>(L"Stage3", { 970.f, -530.f }, 1, m_Cam);
}

void Stage2::SceneChangeStart()
{
	StageBase::SceneChangeStart();
	SOUNDPLAY(L"BGM", L"Dragon.ogg", -1);
}