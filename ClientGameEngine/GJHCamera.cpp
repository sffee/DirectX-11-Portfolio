#include "GJHCamera.h"
#include "GJHActorTransform.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include <GJHDirectRenderTarget.h>
#include <GJHGameEngineWindow.h>
#include "GJHRenderer.h"

GJHCamera::GJHCamera() :
	m_Near(0.1f),
	m_Far(1000.f),
	m_Fov(60.f),
	m_ProjectionSize(100.f, 100.f),
	m_Trans(nullptr),
	m_Mode(CamProjectionMode::Perspective)
{
}

GJHCamera::~GJHCamera()
{

}

void GJHCamera::InitCam(int _Order)
{
	SetOrder(_Order);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	Ptr->PushCamera(this);

	std::shared_ptr<GJHActorTransform> TransForm = GetActor()->FindComponent<GJHActorTransform>();
	m_Trans = TransForm.get();

	m_CameraTarget = std::make_shared<GJHDirectRenderTarget>();
	m_CameraTarget->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::ZERO);

	m_CameraTarget->SetDepthTarget(GJHGameEngineDevice::GetBackBuffer()->GetDepthTarget());
}

void GJHCamera::RenderPrev()
{
	CalCameraMatrix();
}

void GJHCamera::CalCameraMatrix()
{
	m_Trans->m_RenderingData.View.View(m_Trans->GetWorldPosition(), m_Trans->GetWorldZDir(), m_Trans->GetWorldYDir());

	switch (m_Mode)
	{
	case CamProjectionMode::Orthographic:
		m_Trans->m_RenderingData.Projection.OrthographicLH(m_ProjectionSize.x, m_ProjectionSize.y, m_Near, m_Far);
		break;
	case CamProjectionMode::Perspective:
		m_Trans->m_RenderingData.Projection.PerspectiveFovDegLH(m_Fov, m_ProjectionSize.x, m_ProjectionSize.y, m_Near, m_Far);
		break;
	default:
		break;
	}
}

void GJHCamera::CamTargetClear()
{
	m_CameraTarget->Clear();
}

void GJHCamera::CamTargetSetting()
{
	m_CameraTarget->Setting();
}