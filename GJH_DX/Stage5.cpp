#include "Stage5.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include "Player.h"
#include "Gate.h"
#include "define.h"

Stage5::Stage5()
{

}

Stage5::~Stage5()
{

}

void Stage5::Start()
{
	StageBase::Start(L"Stage5");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle_0", { 200.f, -50.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	CreateGate<Gate>(L"Stage6", { 570.f, -200.f }, 1, m_Cam);
}