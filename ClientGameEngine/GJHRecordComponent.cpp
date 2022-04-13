#include "GJHRecordComponent.h"
#include <GJHGameEngineTime.h>
#include "GJHActorTransform.h"
#include "GJHSpriteRenderer.h"
#include "GJHSpriteAnimation.h"
#include <GJHGameEngineActor.h>

float GJHRecordComponent::m_RewindEndTime = 0.f;
bool GJHRecordComponent::m_RewindOn = false;
std::shared_ptr<GJHActorTransform> GJHRecordComponent::m_CamTrans;

GJHRecordComponent::GJHRecordComponent() :
	m_RecordCoolTime(0.1f),
	m_RewindStart(false)
{
}

GJHRecordComponent::~GJHRecordComponent()
{

}

void GJHRecordComponent::Update()
{
	if (m_RewindOn == true)
	{
		m_RewindStart = true;
	}
	else
	{
		m_RecordCoolTime += GJHGameEngineTime::FDeltaTime(1.f, false);
	}

	if (m_RewindStart == true)
	{
		Rewind();
	}
}

void GJHRecordComponent::Record(const FrameInfo& _Info)
{
	if (m_RecordCoolTime < 0.01f)
		return;

	FrameInfo Info;
	Info.Time = _Info.Time;
	Info.ActorPos = _Info.ActorPos;
	Info.ActorScale = _Info.ActorScale;
	Info.RenderPos = _Info.RenderPos;
	Info.RenderScale = _Info.RenderScale;
	Info.RenderRotation = _Info.RenderRotation;
	Info.AnimationName = _Info.AnimationName;
	Info.AnimationFrameIndex = _Info.AnimationFrameIndex;
	Info.ShowAnimation = _Info.ShowAnimation;
	Info.UseCamera = _Info.UseCamera;
	Info.CamPos = _Info.CamPos;
	Info.RenderEnable = _Info.RenderEnable;
	Info.RenderAlpha = _Info.RenderAlpha;

	m_FrameInfoStack.emplace(Info);

	m_RecordCoolTime = 0.f;
}

void GJHRecordComponent::Rewind()
{
	while (m_FrameInfoStack.empty() == false
		   && m_RewindEndTime < m_FrameInfoStack.top().Time)
	{
		m_FrameInfoStack.pop();
	}

	if (m_FrameInfoStack.empty())
	{
		m_RewindOn = false;
		m_RewindStart = false;

		if (m_RewindEndFunc != nullptr)
			m_RewindEndFunc();

		std::shared_ptr<GJHSpriteAnimation> Animation = GetActor()->FindComponent<GJHSpriteAnimation>();
		Animation->SetAnimationStop(false);

		return;
	}

	GJHGameEngineActor* Actor = GetActor();

	std::shared_ptr<GJHActorTransform> Trans = Actor->FindComponent<GJHActorTransform>();
	std::shared_ptr<GJHSpriteRenderer> Render = Actor->FindComponent<GJHSpriteRenderer>();
	std::shared_ptr<GJHSpriteAnimation> Animation = Actor->FindComponent<GJHSpriteAnimation>();

	FrameInfo& Info = m_FrameInfoStack.top();

	Trans->SetWorldPosition(Info.ActorPos);
	Trans->SetWorldScale(Info.ActorScale);
	Animation->ChangeAnimation(Info.AnimationName);
	Animation->SetAnimationFrameIndex(Info.AnimationFrameIndex);
	Animation->SetAnimationStop(true);
	Animation->SetShowAnimation(Info.ShowAnimation);
	Animation->Update();
	Render->SetWorldPosition(Info.RenderPos);
	Render->SetWorldScale(Info.RenderScale);
	Render->SetWorldRotation(Info.RenderRotation);
	Render->SetRenderEnable(Info.RenderEnable);
	Render->SetAlpha(Info.RenderAlpha);

	if (Info.UseCamera == true)
		m_CamTrans->SetWorldPosition(Info.CamPos);

	m_FrameInfoStack.pop();
}

void GJHRecordComponent::Clear()
{
	while (m_FrameInfoStack.empty() == false)
	{
		m_FrameInfoStack.pop();
	}
}