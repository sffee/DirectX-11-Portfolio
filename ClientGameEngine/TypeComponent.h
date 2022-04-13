#pragma once
#include "GJHContentsComponent.h"

class TypeComponent : public GJHContentsComponent
{
private:
	ACTORTYPE m_ActorType;

public:
	TypeComponent();
	~TypeComponent();

public:
	TypeComponent(const TypeComponent& _Other) = delete;
	TypeComponent(const TypeComponent&& _Other) = delete;

public:
	TypeComponent operator=(const TypeComponent& _Other) = delete;
	TypeComponent operator=(const TypeComponent&& _Other) = delete;

public:
	void Start(ACTORTYPE _Type)
	{
		m_ActorType = _Type;
	}

	void SetActorType(ACTORTYPE _Type)
	{
		m_ActorType = _Type;
	}

	ACTORTYPE GetActorType()
	{
		return m_ActorType;
	}
};