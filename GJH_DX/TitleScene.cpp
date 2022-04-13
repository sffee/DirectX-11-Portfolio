#include "TitleScene.h"
#include <GJHGameEngineDirectory.h>
#include <GJHDirectTexture2D.h>
#include <GJHDirectSprite.h>
#include <GJHGameEngineScene.h>
#include <GJHGameEngineActor.h>
#include <GJHCamera.h>
#include <GJHActorTransform.h>
#include <GJHRenderer.h>
#include "define.h"
#include <GJHClientGameEngine.h>
#include <GJHWorldComponent.h>
#include "LogicValue.h"
#include <GJHSceneChangeRenderer.h>
#include <GJHUvDistortion.h>
#include <GJHGameEngineSound.h>
#include "MouseCursor.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Start()
{
	std::shared_ptr<GJHCamera> Cam;
	{
		ActorPtr Actor = GetScene()->CreateActor();
		Cam = Actor->CreateComponent<GJHCamera>(0, 0);

		Cam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		Cam->SetCamProjectionMode(CamProjectionMode::Orthographic);
		
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
		Actor->CreateComponent<MouseCursor>(Cam);
	}

	{
		CreateActor(ACTORTYPE::BACKGROUND, L"BackGround", { 320.f, 0.f, 100.f }, 1.1f);
		CreateActor(ACTORTYPE::BACKGROUND, L"Fence", { 315.f, 10.f, 99.f }, 1.1f);
		CreateActor(ACTORTYPE::BACKGROUND, L"ZER", { 295.f, -160.f, 4.f }, 0.8f);
		CreateActor(ACTORTYPE::BACKGROUND, L"O", { 410.f, -160.f, 3.f }, 0.8f, L"O", 0, 1, 9999.f);
		CreateActor(ACTORTYPE::BACKGROUND, L"KATANA", { 320.f, -125.f, 2.f }, 0.85f);
		CreateActor(ACTORTYPE::BACKGROUND, L"Grass", { 320.f, -330.f, 1.f}, 1.f);
		CreateActor(ACTORTYPE::BACKGROUND, L"Plants", { 320.f, -233.f }, 1.f, L"Plants", 0, 11, 0.1f);
	}

	GJHTargetPostEffect::CreatePostEffect<GJHUvDistortion>(Cam->GetCameraTarget(), L"UvDistortion", 5)->IsEffectOff();

	m_StartMapMove = false;
}

void TitleScene::Update()
{
	if (m_StartMapMove == true)
	{
		m_Time += DELTATIME(1.f, false);
		if (1.f <= m_Time)
		{
			GJHClientGameEngine::ChangeScene(L"Stage1");
		}
	}

	if (KEYDOWN(L"Enter"))
	{
		if (m_StartMapMove == false)
		{
			m_Time = 0.f;
			m_StartMapMove = true;
			GetScene()->CreateActor()->CreateComponent<GJHSceneChangeRenderer>(2, SceneChangeType::Start);
			
			SOUNDFADEOUT(L"BGM", 1.f);
			SOUNDPLAY(L"Transition_Start.wav");
		}
	}
}

void TitleScene::SceneChangeStart()
{
	ShowCursor(false);
	SOUNDPLAY(L"BGM", L"RainOnBrick.ogg", -1);
}

void TitleScene::SceneChangeEnd()
{
	SOUNDSTOP(L"BGM");
}