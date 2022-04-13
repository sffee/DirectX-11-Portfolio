#include "Stage6.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include <GJHGUIComponent.h>
#include "Player.h"
#include "HeadHunter2.h"
#include "define.h"
#include "Boss2SceneWindow.h"

Stage6::Stage6()
{

}

Stage6::~Stage6()
{

}

void Stage6::Start()
{
	std::shared_ptr<Boss2SceneWindow> Window = GetGUI()->CreateGUIWindow<Boss2SceneWindow>(L"Boss2SceneWindow");

	StageBase::Start(L"Stage6");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle_0", { 110.f, -280.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	std::shared_ptr<HeadHunter2> HeadHunter = CreateActor<HeadHunter2>(ACTORTYPE::MONSTER, L"HeadHunter_Idle", { 465.f, -280.f, 0.f })->FindComponent<HeadHunter2>();
	Window->SetHeadHunter(HeadHunter.get());
}

void Stage6::SceneChangeStart()
{
	StageBase::SceneChangeStart();
	SOUNDSTOP(L"BGM");
	SOUNDPLAY(L"BGM", L"FullConfession.ogg", -1);
}