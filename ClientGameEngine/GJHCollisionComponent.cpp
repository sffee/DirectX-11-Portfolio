#include "GJHCollisionComponent.h"
#include <GJHGameEngineActor.h>
#include "GJHWorldComponent.h"
#include "GJHRenderer.h"
#include "GJHSpriteRenderer.h"

bool GJHCollisionComponent::m_IsDebugRender = false;

GJHCollisionComponent::GJHCollisionComponent() :
	m_Collision(nullptr),
	m_DebugRenderColor(float4(1.f, 0.f, 0.f, 1.f)),
	m_DebugRenderEnable(true)
{

}

GJHCollisionComponent::~GJHCollisionComponent()
{

}

void GJHCollisionComponent::Start(int _Order, COLTYPE _Type)
{
	GJHActorTransform::Start();

	ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();

	if (Trans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
	}

	this->AttachParent(Trans);

	std::shared_ptr<GJHGameEngineCollision> Ptr = GetMasterWorld()->CreateCollision(_Order, _Type, this->GetWorldTransform());
	m_Collision = Ptr.get();
	m_Collision->SetParent(this);

	m_Collision->SetData(this);
	m_Collision->ColTrueEvent(&GJHCollisionComponent::ColTrue, *this);
	m_Collision->ColFalseEvent(&GJHCollisionComponent::ColFalse, *this);

	m_DebugRender = GetActor()->CreateComponent<GJHRenderer>();
	m_DebugRender->CreateRenderPlayer(L"BoxMat", L"Rect");
	m_DebugRender->CBufferSettingLink(L"ColorData", m_DebugRenderColor);
}

void GJHCollisionComponent::CallEnter(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_EnterList.size(); i++)
	{
		m_EnterList[i](_Other);
	}
}

void GJHCollisionComponent::CallStay(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_StayList.size(); i++)
	{
		m_StayList[i](_Other);
	}
}

void GJHCollisionComponent::CallExit(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_ExitList.size(); i++)
	{
		m_ExitList[i](_Other);
	}
}

void GJHCollisionComponent::ColTrue(GJHGameEngineCollision* _Other)
{
	GJHCollisionComponent* OtherCol = _Other->GetData<GJHCollisionComponent>();

	if (OtherCol == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (OtherCol == nullptr)");
	}

	if (m_OtherCol.find(OtherCol) == m_OtherCol.end())
	{
		m_OtherCol.insert(OtherCol);
		OtherCol->m_OtherCol.insert(this);
		CallEnter(OtherCol);
		OtherCol->CallEnter(this);
	}
	else
	{
		CallStay(OtherCol);
		OtherCol->CallStay(this);
	}
}

void GJHCollisionComponent::ColFalse(GJHGameEngineCollision* _Other)
{
	GJHCollisionComponent* OtherCol = _Other->GetData<GJHCollisionComponent>();

	if (OtherCol == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (OtherCol == nullptr)");
	}

	if (m_OtherCol.find(OtherCol) != m_OtherCol.end())
	{
		CallExit(OtherCol);
		OtherCol->CallExit(this);
		m_OtherCol.erase(OtherCol);
		OtherCol->m_OtherCol.erase(this);
	}
}

int GJHCollisionComponent::GetOrder()
{
	return m_Collision->GetOrder();
}

void GJHCollisionComponent::SetLocalPosition(const float4& _Pos)
{
	GJHActorTransform::SetLocalPosition(_Pos);
	m_DebugRender->SetLocalPosition(_Pos);
}

void GJHCollisionComponent::SetLocalMove(const float4& _Pos)
{
	GJHActorTransform::SetLocalMove(_Pos);
	m_DebugRender->SetLocalMove(_Pos);
}

void GJHCollisionComponent::SetLocalScale(const float4& _Scale)
{
	GJHActorTransform::SetLocalScale(_Scale);
	m_DebugRender->SetLocalScale(_Scale);
}

void GJHCollisionComponent::SetLocalRotation(const float4& _Rotation)
{
	GJHActorTransform::SetLocalRotation(_Rotation);
	m_DebugRender->SetLocalRotation(_Rotation);
}

void GJHCollisionComponent::ChangeDirection()
{
	float AnimationDir = (GetActor()->FindComponent<GJHSpriteRenderer>()->GetWorldScale().x < 0.f) ? -1.f : 1.f;
	float4 ColLocalPos = GJHActorTransform::GetLocalPosition();

	if (AnimationDir == 1.f)
	{
		ColLocalPos.x = (ColLocalPos.x < 0.f) ? ColLocalPos.x * -1.f : ColLocalPos.x;
	}
	else
	{
		ColLocalPos.x = (ColLocalPos.x < 0.f) ? ColLocalPos.x : ColLocalPos.x * AnimationDir;
	}

	GJHActorTransform::SetLocalPositionX(ColLocalPos.x);
	m_DebugRender->SetLocalPositionX(ColLocalPos.x);
}

void GJHCollisionComponent::Enable()
{
	m_Collision->SetColCheck(true);
	SetDebugRenderEnable(true);
}

void GJHCollisionComponent::Disable()
{
	m_Collision->SetColCheck(false);
	SetDebugRenderEnable(false);
}

void GJHCollisionComponent::DebugRender()
{
	m_DebugRender->SetRenderEnable(m_IsDebugRender && m_DebugRenderEnable);
}

bool GJHCollisionComponent::GetColCheck()
{
	return m_Collision->GetColCheck();
}