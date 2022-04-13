#include "Stage4.h"
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include "Player.h"
#include "Gate.h"
#include "define.h"

Stage4::Stage4()
{

}

Stage4::~Stage4()
{

}

void Stage4::Start()
{
	StageBase::Start(L"Stage4");

	std::shared_ptr<Player> NewPlayer = CreateActor<Player>(ACTORTYPE::PLAYER, L"Player_Idle_0", { 40.f, -120.f, 0.f })->FindComponent<Player>();
	NewPlayer->SetCamera(m_Cam, m_UICam);

	CreateGate<Gate>(L"Stage5", { 470.f, -140.f }, 1, m_Cam);
}