#pragma once
#include "ObjectBase.h"
#include "define.h"

class InstanceObject : public ObjectBase
{
private:
	InstanceObjectData m_Data;
	bool m_IsEnd;

public:
	InstanceObject();
	~InstanceObject();

public:
	InstanceObject(const InstanceObject& _Other) = delete;
	InstanceObject(const InstanceObject&& _Other) = delete;

public:
	InstanceObject operator=(const InstanceObject& _Other) = delete;
	InstanceObject operator=(const InstanceObject&& _Other) = delete;

public:
	void Start(InstanceObjectData _Data, std::shared_ptr<GJHSpriteRenderer> _Renderer);
	void Update() override;

public:
	void Start(float4 _Pos = float4::ZERO);
	void End();
};

