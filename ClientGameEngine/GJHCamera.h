#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineMath.h>
#include "GJHRenderingStruct.h"
#include "GJHActorTransform.h"
#include <vector>
#include <GJHTargetPostEffect.h>

class GJHRenderer;
class GJHDirectRenderTarget;
enum class CamProjectionMode
{
	Orthographic,
	Perspective
};

class GJHActorTransform;
class GJHCamera : public GJHGameEngineComponent
{

private:
	CamProjectionMode m_Mode;
	GJHActorTransform* m_Trans;
	
	float m_Fov;
	float m_Near;
	float m_Far;

	float4 m_ProjectionSize;

	std::shared_ptr<GJHDirectRenderTarget> m_CameraTarget;

public:
	std::shared_ptr<GJHDirectRenderTarget> GetCameraTarget()
	{
		return m_CameraTarget;
	}

public:
	void CamTargetClear();
	void CamTargetSetting();

public:
	int m_Order;
	bool m_ChangeOrder;

public:
	void SetOrder(int _Order)
	{
		m_Order = _Order;
		m_ChangeOrder = true;
	}

	int GetOrder()
	{
		return m_Order;
	}

public:
	void SetCamProjectionSize(float4 _Size)
	{
		m_ProjectionSize = _Size;
	}

	float4 GetCamProjectionSize()
	{
		return m_ProjectionSize;
	}

	void SetCamProjectionMode(CamProjectionMode _Mode)
	{
		m_Mode = _Mode;
	}

	inline GJHRenderingTransformData& GetCameraTransformMatrix()
	{
		return m_Trans->GetRenderingData();
	}

public:
	GJHCamera();
	~GJHCamera();

public:
	GJHCamera(const GJHCamera& _Other) = delete;
	GJHCamera(const GJHCamera&& _Other) = delete;

public:
	GJHCamera operator=(const GJHCamera& _Other) = delete;
	GJHCamera operator=(const GJHCamera&& _Other) = delete;

private:
	std::vector<int> m_ViewList;

public:
	std::vector<int>& GetViewList()
	{
		return m_ViewList;
	}

public:
	void InitCam(int _Order);

public:
	template<typename ... REST>
	void Start(int _Order, REST ... _Arg)
	{
		InitCam(_Order);
		PushViewList(_Arg...);
	}

	template<typename ... REST>
	void PushViewList(int _ViewOrder, REST ... _Arg)
	{
		m_ViewList.push_back(_ViewOrder);
		PushViewList(_Arg...);
	}

	void PushViewList()
	{

	}

	void RenderPrev() override;

public:
	void CalCameraMatrix();
};