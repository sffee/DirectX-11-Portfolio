#include "EndingScene.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineScene.h>
#include <GJHCamera.h>
#include <GJHSpriteRenderer.h>
#include "define.h"

EndingScene::EndingScene()
{

}

EndingScene::~EndingScene()
{

}

void EndingScene::Start()
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
		m_Render = CreateActor(ACTORTYPE::BACKGROUND, L"ThankYou", { 320.f, -180.f, 0.f }, 1.f)->FindComponent<GJHSpriteRenderer>();
		m_Render->SetColor(float4::ZERO);
	}

	Color = 0.f;
}

void EndingScene::Update()
{
	Color += DELTATIME(0.5f, false);
	if (1.f <= Color)
	{
		Color = 1.f;
	}

	m_Render->SetColor(float4(Color, Color, Color, Color));
}