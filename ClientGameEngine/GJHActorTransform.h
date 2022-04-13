#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineTransForm.h>
#include "GJHRenderingStruct.h"

class GJHCamera;
class GJHWorldComponent;
class GJHActorTransform : public GJHGameEngineComponent
{
private:
	friend GJHCamera;

private:
	GJHWorldComponent* m_MasterWorld;
	std::shared_ptr<GJHGameEngineTransform> m_WorldTransform;
	GJHRenderingTransformData m_RenderingData;

protected:
	GJHGameEngineTransform* GetWorldTransform()
	{
		return m_WorldTransform.get();
	}

public:
	GJHWorldComponent* GetMasterWorld()
	{
		return m_MasterWorld;
	}

	void AttachParent(std::shared_ptr<GJHActorTransform> _Parent)
	{
		m_WorldTransform->AttachParent(_Parent->m_WorldTransform);
	}

public:
	void CalRenderDataCopy();

	inline GJHRenderingTransformData& GetRenderingData()
	{
		return m_RenderingData;
	}

public:
	float4 GetLocalZDir()
	{
		return m_WorldTransform->GetLocalZDir();
	}

	float4 GetWorldZDir()
	{
		return m_WorldTransform->GetWorldZDir();
	}

	float4 GetLocalYDir()
	{
		return m_WorldTransform->GetLocalYDir();
	}

	float4 GetWorldYDir()
	{
		return m_WorldTransform->GetWorldYDir();
	}

	float4 GetLocalXDir()
	{
		return m_WorldTransform->GetLocalXDir();
	}

	float4 GetWorldXDir()
	{
		return m_WorldTransform->GetWorldXDir();
	}

	float4 GetLocalPosition() const
	{
		return m_WorldTransform->GetLocalPosition();
	}

	float4 GetWorldPosition() const
	{
		return m_WorldTransform->GetWorldPosition();
	}

	float4 GetLocalScale() const
	{
		return m_WorldTransform->GetLocalScale();
	}

	float4 GetWorldScale() const
	{
		return m_WorldTransform->GetWorldScale();
	}

	float4 GetLocalRotation() const
	{
		return m_WorldTransform->GetLocalRotation();
	}

	float4 GetWorldRotation() const
	{
		return m_WorldTransform->GetWorldRotation();
	}

	void SetLocalScale(const float4& _Scale)
	{
		m_WorldTransform->SetLocalScale(_Scale);
	}

	void SetLocalScaleX(float _Scale)
	{
		m_WorldTransform->SetLocalScaleX(_Scale);
	}

	void SetLocalScaleY(float _Scale)
	{
		m_WorldTransform->SetLocalScaleY(_Scale);
	}

	void SetLocalScaleZ(float _Scale)
	{
		m_WorldTransform->SetLocalScaleZ(_Scale);
	}

	void SetWorldScaleX(float _Scale)
	{
		m_WorldTransform->SetWorldScaleX(_Scale);
	}

	void SetWorldScaleY(float _Scale)
	{
		m_WorldTransform->SetWorldScaleY(_Scale);
	}

	void SetWorldScaleZ(float _Scale)
	{
		m_WorldTransform->SetWorldScaleZ(_Scale);
	}

	void SetWorldScale(const float4& _Scale)
	{
		m_WorldTransform->SetWorldScale(_Scale);
	}

	void SetLocalPosition(const float4& _Pos)
	{
		m_WorldTransform->SetLocalPosition(_Pos);
	}

	void SetLocalPositionX(float _Value)
	{
		m_WorldTransform->SetLocalPositionX(_Value);
	}

	void SetLocalPositionY(float _Value)
	{
		m_WorldTransform->SetLocalPositionY(_Value);
	}

	void SetLocalPositionZ(float _Value)
	{
		m_WorldTransform->SetLocalPositionZ(_Value);
	}

	void SetWorldPosition(const float4& _Pos)
	{
		m_WorldTransform->SetWorldPosition(_Pos);
	}

	void SetWorldPositionX(float _Value)
	{
		m_WorldTransform->SetWorldPositionX(_Value);
	}

	void SetWorldPositionY(float _Value)
	{
		m_WorldTransform->SetWorldPositionY(_Value);
	}

	void SetWorldPositionZ(float _Value)
	{
		m_WorldTransform->SetWorldPositionZ(_Value);
	}

	void SetLocalRotation(const float4& _Value)
	{
		m_WorldTransform->SetLocalRotation(_Value);
	}

	void SetLocalRotationX(const float _Value)
	{
		m_WorldTransform->SetLocalRotationX(_Value);
	}

	void SetLocalRotationY(const float _Value)
	{
		m_WorldTransform->SetLocalRotationY(_Value);
	}

	void SetLocalRotationZ(const float _Value)
	{
		m_WorldTransform->SetLocalRotationZ(_Value);
	}

	void SetWorldRotation(const float4& _Value)
	{
		m_WorldTransform->SetWorldRotation(_Value);
	}

	void SetWorldRotationX(const float _Value)
	{
		m_WorldTransform->SetWorldRotationX(_Value);
	}

	void SetWorldRotationY(const float _Value)
	{
		m_WorldTransform->SetWorldRotationY(_Value);
	}

	void SetWorldRotationZ(const float _Value)
	{
		m_WorldTransform->SetWorldRotationZ(_Value);
	}

	void SetLocalMove(const float4& _Pos)
	{
		m_WorldTransform->SetLocalMove(_Pos);
	}

	void SetWorldMove(const float4& _Pos)
	{
		m_WorldTransform->SetWorldMove(_Pos);
	}

public:
	GJHActorTransform();
	~GJHActorTransform();

public:
	GJHActorTransform(const GJHActorTransform& _Other) = delete;
	GJHActorTransform(const GJHActorTransform&& _Other) = delete;

public:
	GJHActorTransform operator=(const GJHActorTransform& _Other) = delete;
	GJHActorTransform operator=(const GJHActorTransform&& _Other) = delete;

public:
	void CameraSetting(GJHCamera* _Camera);

public:
	void Start() override;
	void DeathTask() override;
};

using ActorTransPtr = std::shared_ptr<GJHActorTransform>;