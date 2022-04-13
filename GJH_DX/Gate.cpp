#include "Gate.h"
#include <GJHCollisionComponent.h>
#include <GJHClientGameEngine.h>
#include "LogicValue.h"
#include <GJHSceneChangeRenderer.h>
#include <GJHGameEngineSound.h>
#include "MonsterBase.h"
#include <GJHCamera.h>
#include "define.h"

Gate::Gate()
{

}

Gate::~Gate()
{

}

void Gate::Start(const GJHGameEngineString& _NextSceneName, int _Dir, std::shared_ptr<GJHCamera> _Cam)
{
	m_Collision = CreateComponent<GJHCollisionComponent>((int)CollisionLayer::Gate, COLTYPE::AABB);

	m_Collision->AddEnterEvent(&Gate::ColEnter, *this);
	m_Collision->SetLocalScale({ 20.f, 100.f, 0.f });
	m_Collision->SetDebugRenderColor({ 1.f, 0.f, 1.f, 1.f });

	m_NextSceneName = _NextSceneName;

	m_StartMapMove = false;

	//InstanceObjectData Data;
	//Data.AnimationName = L"Text";
	//Data.TextureName = L"Go_Text";
	//Data.Pos = 
	//Data.Dir = DIRECTION::RIGHT;
	//Data.IsDeath = false;
	//Data.IsRecord = false;
	//Data.AnimationEndHide = false;
	//Data.ActorType = ACTORTYPE::UI;

	//{
	//	std::shared_ptr<GJHGameEngineActor> Actor = CreateInstanceObject(Data);
	//	m_TextRender = Actor->FindComponent<GJHSpriteRenderer>();
	//	m_TextRender->SetRenderEnable(false);
	//	m_TextTrans = Actor->FindComponent<GJHActorTransform>();
	//}

	InstanceObjectData Data;
	Data.AnimationName = L"Arrow";
	Data.TextureName = L"Go_Arrow";
	Data.Dir = (DIRECTION)_Dir;
	Data.IsDeath = false;
	Data.IsRecord = false;
	Data.AnimationEndHide = false;
	Data.ActorType = ACTORTYPE::UI;

	std::shared_ptr<GJHGameEngineActor> Actor = CreateInstanceObject(Data);
	m_ArrowRender = Actor->FindComponent<GJHSpriteRenderer>();
	m_ArrowRender->SetRenderEnable(false);
	m_ArrowTrans = Actor->FindComponent<GJHActorTransform>();

	m_Dir = _Dir;
	m_RenderGap = 10.f;
	m_GapTime = 0.f;
	m_CheckRenderGap = false;

	m_CamTrans = _Cam->GetActor()->FindComponent<GJHActorTransform>();
}

void Gate::Update()
{
	if (MonsterBase::GetMonsterCount() == 0)
	{
		SetGoPosition();

		m_ArrowRender->SetRenderEnable(true);
		//m_TextRender->SetRenderEnable(true);
	}
	else
	{
		m_RenderGap = 0.f;
		m_GapTime = 0.f;
		m_CheckRenderGap = false;
	}

	if (m_StartMapMove == true)
	{
		m_Time += DELTATIME(1.f, false);
		if (1.f <= m_Time)
		{
			GJHClientGameEngine::ChangeScene(m_NextSceneName);
		}
	}
}

void Gate::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	if (m_StartMapMove == true
		|| MonsterBase::GetMonsterCount() != 0)
	{
		return;
	}

	m_Time = 0.f;
	m_StartMapMove = true;
	std::shared_ptr<GJHSceneChangeRenderer> Render = GetScene()->CreateActor()->CreateComponent<GJHSceneChangeRenderer>(2, SceneChangeType::Start);
	SOUNDPLAY(L"Transition_Start.wav");
}

void Gate::SetGoPosition()
{
	//float4 Position = GetActorTrans()->GetWorldPosition();
	//float XPosition = (m_Dir == 1) ? 620.f : 0.f;

	//float4 CamStartPos = m_CamTrans->GetWorldPosition() + float4(CAMSIZEX * -0.5f, CAMSIZEY * 0.5f);
	//float4 CamEndPos = CamStartPos + float4(CAMSIZEX, -CAMSIZEY);

	//{
	//	float4 DrawPos;

	//	if (Position.y < CamStartPos.y && CamEndPos.y < Position.y)
	//	{
	//		DrawPos = float4(XPosition, Position.y) + float4((10.f * m_Dir * -1.f) + m_RenderGap * m_Dir, 50.f) + float4(0.f, CAMSIZEY * 0.5f);
	//	}
	//	else
	//	{
	//		DrawPos = float4(XPosition, Position.y) + float4((10.f * m_Dir * -1.f) + m_RenderGap * m_Dir, 0.f) - float4(0.f, CAMSIZEY * 0.5f);

	//		if (DrawPos.y < CamEndPos.y)
	//		{
	//			DrawPos.y = -340.f;
	//		}
	//		else if (CamStartPos.y < DrawPos.y)
	//		{
	//			DrawPos.y = -80.f;
	//		}
	//	}

	//	m_ArrowTrans->SetWorldPosition(DrawPos);
	//}

	float4 Position = GetActorTrans()->GetWorldPosition();
	float4 DrawPos = Position + float4(-20.f * (float)m_Dir + m_RenderGap * (float)m_Dir, 0.f);

	m_ArrowTrans->SetWorldPosition(DrawPos);

	if (m_CheckRenderGap == false)
	{
		m_RenderGap += DELTATIME(15.f, false);
		if (10.f <= m_RenderGap)
		{
			m_CheckRenderGap = true;
			m_GapTime = 0.2f;
		}
	}
	else
	{
		m_GapTime -= DELTATIME(1.f, false);
		if (m_GapTime <= 0.f)
		{
			m_CheckRenderGap = false;
			m_RenderGap = 0.f;
		}
	}
}