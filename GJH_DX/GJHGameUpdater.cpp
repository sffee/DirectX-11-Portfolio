#include "GJHGameUpdater.h"
#include <GJHClientGameEngine.h>
#include "TitleScene.h"
#include "MapEditorScene.h"
#include <GJHGameEngineGUI.h>
#include "GJHGlobalAppWindow.h"
#include <GJHContentsSceneComponent.h>
#include "define.h"
#include <GJHGameEngineDirectory.h>
#include <GJHDirectTexture2D.h>
#include <GJHDirectSprite.h>
#include "RenderingWindow.h"
#include <GJHGameEngineSound.h>
#include <GJHGameEngineThread.h>
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "Stage6.h"
#include "EndingScene.h"

#pragma comment(lib, "ClientGameEngine")

bool GJHGameUpdater::IsLoadingEnd = false;

void GJHGameUpdater::ObjectInputCheck()
{
}

void GJHGameUpdater::CamInputCheck()
{
}

void GJHGameUpdater::ResourceLoading()
{
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");

		auto Arr = NewDir.AllFile();

		for (size_t i = 0; i < Arr.size(); i++)
		{
			GJHDirectTexture2D::Load(Arr[i].FullPath());
		}
	}

	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"Portfolio");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Sound");

		{
			auto Arr = NewDir.AllFile(L"ogg");

			for (size_t i = 0; i < Arr.size(); i++)
			{
				GJHGameEngineSound::Load(Arr[i].FullPath());
			}
		}

		{
			auto Arr = NewDir.AllFile(L"wav");

			for (size_t i = 0; i < Arr.size(); i++)
			{
				GJHGameEngineSound::Load(Arr[i].FullPath());
			}
		}
	}

	GJHDirectSprite::Create(L"MapCollisionTile.png", { 4, 8 });
	GJHDirectSprite::Create(L"Prison_BackGround.png", { 27, 28 });
	GJHDirectSprite::Create(L"Prison_ForeGround.png", { 27, 28 });
	GJHDirectSprite::Create(L"SpecialTile.png", { 3, 1 });

	GJHDirectSprite::Create(L"BackGround");
	GJHDirectSprite::Create(L"Fence");
	GJHDirectSprite::Create(L"KATANA");
	GJHDirectSprite::Create(L"ZER");
	GJHDirectSprite::Create(L"O");
	GJHDirectSprite::Create(L"Grass");
	GJHDirectSprite::Create(L"Plants");

	GJHDirectSprite::Create(L"Player_Attack");
	GJHDirectSprite::Create(L"Player_Dash");
	GJHDirectSprite::Create(L"Player_HurtFlyStart");
	GJHDirectSprite::Create(L"Player_HurtFly");
	GJHDirectSprite::Create(L"Player_HurtLand");
	GJHDirectSprite::Create(L"Player_DoorKick");
	GJHDirectSprite::Create(L"Player_Idle");
	GJHDirectSprite::Create(L"Player_Jump");
	GJHDirectSprite::Create(L"Player_Fall");
	GJHDirectSprite::Create(L"Player_MoveStart");
	GJHDirectSprite::Create(L"Player_Move");
	GJHDirectSprite::Create(L"Player_MoveEnd");
	GJHDirectSprite::Create(L"Player_Roll");
	GJHDirectSprite::Create(L"Player_SitStart");
	GJHDirectSprite::Create(L"Player_Sit");
	GJHDirectSprite::Create(L"Player_SitEnd");
	GJHDirectSprite::Create(L"Player_WakeUp");
	GJHDirectSprite::Create(L"Player_WallGrab");
	GJHDirectSprite::Create(L"Player_WallSlide");
	GJHDirectSprite::Create(L"Player_Slash");
	GJHDirectSprite::Create(L"Player_SlashFlow");
	GJHDirectSprite::Create(L"Player_SlashImpact");
	GJHDirectSprite::Create(L"Player_SlashSuccess");
	GJHDirectSprite::Create(L"Player_Flip");

	GJHDirectSprite::Create(L"Pomp_Attack");
	GJHDirectSprite::Create(L"Pomp_EnterStair");
	GJHDirectSprite::Create(L"Pomp_Fall");
	GJHDirectSprite::Create(L"Pomp_HurtFly");
	GJHDirectSprite::Create(L"Pomp_HurtGround");
	GJHDirectSprite::Create(L"Pomp_Idle");
	GJHDirectSprite::Create(L"Pomp_KnockDown");
	GJHDirectSprite::Create(L"Pomp_LeaveStair");
	GJHDirectSprite::Create(L"Pomp_Run");
	GJHDirectSprite::Create(L"Pomp_Turn");
	GJHDirectSprite::Create(L"Pomp_Walk");
	GJHDirectSprite::Create(L"Pomp_Slash");

	GJHDirectSprite::Create(L"Grunt_Attack");
	GJHDirectSprite::Create(L"Grunt_Fall");
	GJHDirectSprite::Create(L"Grunt_HurtFly");
	GJHDirectSprite::Create(L"Grunt_HurtGround");
	GJHDirectSprite::Create(L"Grunt_Idle");
	GJHDirectSprite::Create(L"Grunt_Run");
	GJHDirectSprite::Create(L"Grunt_Turn");
	GJHDirectSprite::Create(L"Grunt_Walk");
	GJHDirectSprite::Create(L"Grunt_Slash");

	GJHDirectSprite::Create(L"Cop_HurtFly");
	GJHDirectSprite::Create(L"Cop_HurtGround");
	GJHDirectSprite::Create(L"Cop_Idle");
	GJHDirectSprite::Create(L"Cop_Run");
	GJHDirectSprite::Create(L"Cop_Turn");
	GJHDirectSprite::Create(L"Cop_Walk");
	GJHDirectSprite::Create(L"Cop_Attack");
	GJHDirectSprite::Create(L"Cop_Gun");
	GJHDirectSprite::Create(L"Cop_LeftArm");
	GJHDirectSprite::Create(L"Cop_RightArm");

	GJHDirectSprite::Create(L"ShieldCop_HurtFly");
	GJHDirectSprite::Create(L"ShieldCop_HurtGround");
	GJHDirectSprite::Create(L"ShieldCop_Idle");
	GJHDirectSprite::Create(L"ShieldCop_Run");
	GJHDirectSprite::Create(L"ShieldCop_Turn");
	GJHDirectSprite::Create(L"ShieldCop_Walk");
	GJHDirectSprite::Create(L"ShieldCop_Aim");
	GJHDirectSprite::Create(L"ShieldCop_Bash");
	GJHDirectSprite::Create(L"ShieldCop_KnockBack");

	GJHDirectSprite::Create(L"HeadHunter_AfterHurt");
	GJHDirectSprite::Create(L"HeadHunter_AfterHurt_Smoke");
	GJHDirectSprite::Create(L"HeadHunter_Aim_Rifle");
	GJHDirectSprite::Create(L"HeadHunter_Bomb_Run");
	GJHDirectSprite::Create(L"HeadHunter_Dash");
	GJHDirectSprite::Create(L"HeadHunter_Dash_End_Ground");
	GJHDirectSprite::Create(L"HeadHunter_Dash_SlowDown");
	GJHDirectSprite::Create(L"HeadHunter_Dead");
	GJHDirectSprite::Create(L"HeadHunter_DieFly");
	GJHDirectSprite::Create(L"HeadHunter_DieLand");
	GJHDirectSprite::Create(L"HeadHunter_DodgeRoll");
	GJHDirectSprite::Create(L"HeadHunter_Exit_Door");
	GJHDirectSprite::Create(L"HeadHunter_Hurt");
	GJHDirectSprite::Create(L"HeadHunter_HurtGround");
	GJHDirectSprite::Create(L"HeadHunter_Idle");
	GJHDirectSprite::Create(L"HeadHunter_Invisible");
	GJHDirectSprite::Create(L"HeadHunter_Jump");
	GJHDirectSprite::Create(L"HeadHunter_PreDash");
	GJHDirectSprite::Create(L"HeadHunter_PreJump");
	GJHDirectSprite::Create(L"HeadHunter_PutBackGun");
	GJHDirectSprite::Create(L"HeadHunter_PutBackRifle");
	GJHDirectSprite::Create(L"HeadHunter_Recover");
	GJHDirectSprite::Create(L"HeadHunter_Reveal_Bomb");
	GJHDirectSprite::Create(L"HeadHunter_Shoot");
	GJHDirectSprite::Create(L"HeadHunter_Sweep");
	GJHDirectSprite::Create(L"HeadHunter_TakeOutGun");
	GJHDirectSprite::Create(L"HeadHunter_TakeOutRifle");
	GJHDirectSprite::Create(L"HeadHunter_TeleportIn");
	GJHDirectSprite::Create(L"HeadHunter_TeleportIn_Ground");
	GJHDirectSprite::Create(L"HeadHunter_TeleportIn_Sweep");
	GJHDirectSprite::Create(L"HeadHunter_TeleportOut");
	GJHDirectSprite::Create(L"HeadHunter_TeleportOut_Ground");
	GJHDirectSprite::Create(L"HeadHunter_TeleportOut_Sweep");
	GJHDirectSprite::Create(L"HeadHunter_Wall_Idle");
	GJHDirectSprite::Create(L"HeadHunter_Wall_Jump");
	GJHDirectSprite::Create(L"HeadHunter_Wall_Jump_Land");

	GJHDirectSprite::Create(L"HeadHunter_Laser");
	GJHDirectSprite::Create(L"Grenade");
	GJHDirectSprite::Create(L"Grenade_Circle");
	GJHDirectSprite::Create(L"Grenade_Range");
	GJHDirectSprite::Create(L"KnifeDashLine");

	GJHDirectSprite::Create(L"Door_Iron_Idle");
	GJHDirectSprite::Create(L"Door_Iron_Damage");
	GJHDirectSprite::Create(L"Door_Iron_Glow");

	GJHDirectSprite::Create(L"Monster_Player_Detect");
	GJHDirectSprite::Create(L"Bullet");
	GJHDirectSprite::Create(L"GunFire3");
	GJHDirectSprite::Create(L"GunFire4");
	GJHDirectSprite::Create(L"GunFire5");
	GJHDirectSprite::Create(L"Explosion");

	GJHDirectSprite::Create(L"DustCloud");
	GJHDirectSprite::Create(L"JumpCloud");
	GJHDirectSprite::Create(L"LandCloud");

	GJHDirectSprite::Create(L"StartTransition");
	GJHDirectSprite::Create(L"EndTransition");

	GJHDirectSprite::Create(L"Go_Arrow");
	GJHDirectSprite::Create(L"Go_Text");

	GJHDirectSprite::Create(L"Cursor");
	GJHDirectSprite::Create(L"ThankYou");
}

void GJHGameUpdater::MakeKey()
{
	CREATEKEY(L"LMove", 'A');
	CREATEKEY(L"RMove", 'D');
	CREATEKEY(L"Jump", 'W');
	CREATEKEY(L"Sit", 'S');
	CREATEKEY(L"MouseClick", VK_LBUTTON);
	CREATEKEY(L"MouseRClick", VK_RBUTTON);
	CREATEKEY(L"Attack", VK_LBUTTON);
	CREATEKEY(L"Dash", VK_RBUTTON);
	CREATEKEY(L"Roll", VK_SPACE);
	CREATEKEY(L"SlowMotion", VK_LSHIFT);
	CREATEKEY(L"LShift", VK_LSHIFT);
	CREATEKEY(L"F1", VK_F1);
	CREATEKEY(L"F2", VK_F2);
	CREATEKEY(L"F3", VK_F3);
	CREATEKEY(L"F4", VK_F4);
	CREATEKEY(L"F5", VK_F5);
	CREATEKEY(L"F6", VK_F6);
	CREATEKEY(L"F7", VK_F7);
	CREATEKEY(L"F8", VK_F8);
	CREATEKEY(L"F9", VK_F9);
	CREATEKEY(L"F10", VK_F10);
	CREATEKEY(L"F11", VK_F11);
	CREATEKEY(L"F12", VK_F12);
	CREATEKEY(L"Numpad0", VK_NUMPAD0);
	CREATEKEY(L"Numpad1", VK_NUMPAD1);
	CREATEKEY(L"Numpad2", VK_NUMPAD2);
	CREATEKEY(L"Numpad3", VK_NUMPAD3);
	CREATEKEY(L"Numpad4", VK_NUMPAD4);
	CREATEKEY(L"Numpad5", VK_NUMPAD5);
	CREATEKEY(L"Numpad6", VK_NUMPAD6);
	CREATEKEY(L"Numpad7", VK_NUMPAD7);
	CREATEKEY(L"Numpad8", VK_NUMPAD8);
	CREATEKEY(L"Numpad9", VK_NUMPAD9);
	CREATEKEY(L"NumpadMul", VK_MULTIPLY);
	CREATEKEY(L"Enter", VK_RETURN);
	CREATEKEY(L"Space", VK_SPACE);
}

void GJHGameUpdater::UpdateStart()
{
	GJHGameEngineWindow::Start(float4{ 1280, 720 }, GJHGameEngineString(L"Katana Zero"), { 300, 150 });
	GJHGameEngineDevice::Init(GJHGameEngineWindow::GetWindowHWnd(), GJHGameEngineWindow::GetSize());
	GJHGameEngineGUI::Init();

	GJHClientGameEngine::EngineStart();

	GJHContentsSceneComponent::SetCamSize(float4(CAMSIZEX, CAMSIZEY));

	ResourceLoading();
	MakeKey();

	std::shared_ptr<GJHGlobalAppWindow> GWindow = GJHGameEngineGUI::CreateGUIWindow<GJHGlobalAppWindow>(L"EngineStatus");
	GJHGameEngineGUI::CreateGUIWindow<RenderingWindow>(L"RenderingStatus");

	GWindow->SceneName.push_back(L"TitleScene");
	GWindow->SceneName.push_back(L"MapEditorScene");
	GWindow->SceneName.push_back(L"Stage1");
	GWindow->SceneName.push_back(L"Stage2");
	GWindow->SceneName.push_back(L"Stage3");
	GWindow->SceneName.push_back(L"Stage6");

	GJHClientGameEngine::CreateScene<TitleScene>(L"TitleScene");
	GJHClientGameEngine::CreateScene<MapEditorScene>(L"MapEditorScene");
	GJHClientGameEngine::CreateScene<Stage1>(L"Stage1");
	GJHClientGameEngine::CreateScene<Stage2>(L"Stage2");
	GJHClientGameEngine::CreateScene<Stage3>(L"Stage3");
	GJHClientGameEngine::CreateScene<Stage6>(L"Stage6");
	GJHClientGameEngine::CreateScene<EndingScene>(L"EndingScene");

	GJHClientGameEngine::ChangeScene(L"TitleScene");
}

void GJHGameUpdater::DeadTimeUpdate()
{
	GJHGameEngineSound::Update();
	GJHClientGameEngine::EngineProgress();
}

void GJHGameUpdater::End()
{
	GJHGameEngineGUI::Release();
	GJHGameEngineDevice::Release();
	GJHGameEngineWindow::End();
}