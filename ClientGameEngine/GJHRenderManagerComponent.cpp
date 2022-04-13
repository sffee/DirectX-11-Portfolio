#include "GJHRenderManagerComponent.h"
#include <GJHGameEngineDevice.h>
#include "GJHRenderer.h"
#include "GJHCamera.h"
#include "GJHGameEngineGUI.h"
#include "imgui.h"
#include <GJHGameEngineScene.h>
#include <GJHDirectRenderTarget.h>
#include <GJHTargetPostEffect.h>
#include "GJHMaskRenderer.h"

std::map<int, std::shared_ptr<GJHDirectRenderTarget>> GJHRenderManagerComponent::m_MaskTarget;

GJHRenderManagerComponent::GJHRenderManagerComponent()
{

}

GJHRenderManagerComponent::~GJHRenderManagerComponent()
{

}

void GJHRenderManagerComponent::RenderPrev()
{
	GJHGameEngineDevice::BackBufferSetting();
}

void GJHRenderManagerComponent::RenderNext()
{

}

bool ZSort(GJHRenderer* _Left, GJHRenderer* _Right)
{
	return _Right->GetWorldPosition().z < _Left->GetWorldPosition().z;
}

void GJHRenderManagerComponent::Render()
{
	GJHGameEngineDevice::Reset();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			_Cam->CamTargetClear();
		}
	}

	GetScene()->DebugRender();

	GJHGameEngineDevice::Reset();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			if (_Cam->IsUpdate() == false)
			{
				continue;
			}

			GJHGameEngineDevice::Reset();
			_Cam->CamTargetSetting();

			for (size_t i = 0; i < _Cam->GetViewList().size(); i++)
			{
				int Key = _Cam->GetViewList()[i];

				std::list<GJHRenderer*>& _RenderList = m_RendererList[Key];

				_RenderList.sort(ZSort);

				for (auto& _Render : _RenderList)
				{
					_Render->Render(_Cam);
				}
			}
		}
	}

	GJHGameEngineDevice::Reset();
	
	GJHTargetPostEffect::ProcessEffect((int)PostEffectOrder::CamMergePrev);
	
	GJHGameEngineDevice::Reset();
	
	GJHGameEngineDevice::BackBufferSetting();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			GJHGameEngineDevice::GetBackBuffer()->Merge(_Cam->GetCameraTarget());
		}
	}

	GJHGameEngineDevice::Reset();
	
	GJHGameEngineGUI::GUIRenderStart();
	GetScene()->OnGUI();
	GJHGameEngineGUI::GUIWindowRender();
	GJHGameEngineGUI::GUIRenderEnd();

	GJHGameEngineDevice::Present();
}

void GJHRenderManagerComponent::PushCamera(GJHCamera* _Camera)
{
	if (m_CameraList.find(_Camera->GetOrder()) == m_CameraList.end())
	{
		m_CameraList[_Camera->GetOrder()];
	}

	m_CameraList[_Camera->GetOrder()].push_back(_Camera);
}

void GJHRenderManagerComponent::PushMaskRenderer(GJHMaskRenderer* _Render)
{
	if (m_MaskRenderList.find(_Render->GetOrder()) == m_MaskRenderList.end())
	{
		m_RendererList[_Render->GetOrder()];
	}

	m_MaskRenderList[_Render->GetOrder()].push_back(_Render);
}

void GJHRenderManagerComponent::PushRenderer(GJHRenderer* _Render)
{
	if (m_RendererList.find(_Render->GetOrder()) == m_RendererList.end())
	{
		m_RendererList[_Render->GetOrder()];
	}

	m_RendererList[_Render->GetOrder()].push_back(_Render);
}

void GJHRenderManagerComponent::Release()
{
	for (auto& _RenderList : m_CameraList)
	{
		ReleaseList(_RenderList.second);
	}

	for (auto& _RenderList : m_RendererList)
	{
		ReleaseList(_RenderList.second);
	}
}

void GJHRenderManagerComponent::CreateMaskTarget(int _MaskTargetIndex, float4 _Scale, float4 _ClearColor, DXGI_FORMAT _Fmt)
{
	if (m_MaskTarget.find(_MaskTargetIndex) != m_MaskTarget.end())
	{
		return;
	}

	m_MaskTarget[_MaskTargetIndex] = std::make_shared<GJHDirectRenderTarget>();
	m_MaskTarget[_MaskTargetIndex]->CreateRenderTarget(_Scale, _ClearColor);
}

std::shared_ptr<GJHDirectRenderTarget> GJHRenderManagerComponent::GetMask(int _MaskTargetIndex)
{
	return m_MaskTarget[_MaskTargetIndex];
}