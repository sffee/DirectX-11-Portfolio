#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineTransform.h>
#include <map>
#include <GJHGameEngineDevice.h>

enum class PostEffectOrder
{
	CamMergePrev = 5
};

class GJHCamera;
class GJHRenderer;
class GJHMaskRenderer;
class GJHGameEngineTransform;
class GJHDirectRenderTarget;
class GJHRenderManagerComponent : public GJHGameEngineComponent
{
private:
	static std::map<int, std::shared_ptr<GJHDirectRenderTarget>> m_MaskTarget;

public:
	static void CreateMaskTarget(int _MaskTargetIndex, float4 _Scale, float4 _ClearColor, DXGI_FORMAT _Fmt = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_SINT);
	static std::shared_ptr<GJHDirectRenderTarget> GetMask(int _MaskTargetIndex);

private:
	friend GJHMaskRenderer;
	friend GJHRenderer;
	friend GJHCamera;

private:
	std::map<int, std::list<GJHCamera*>> m_CameraList;
	std::map<int, std::list<GJHRenderer*>> m_RendererList;

	std::map<int, std::list<GJHMaskRenderer*>> m_MaskRenderList;

public:
	std::map<int, std::list<GJHCamera*>>& GetCameraList()
	{
		return m_CameraList;
	}

	std::map<int, std::list<GJHRenderer*>>& GetRendererList()
	{
		return m_RendererList;
	}

private:
	void PushMaskRenderer(GJHMaskRenderer* _Render);
	void PushRenderer(GJHRenderer* _Render);
	void PushCamera(GJHCamera* _Camera);

public:
	GJHRenderManagerComponent();
	~GJHRenderManagerComponent();

public:
	GJHRenderManagerComponent(const GJHRenderManagerComponent& _Other) = delete;
	GJHRenderManagerComponent(const GJHRenderManagerComponent&& _Other) = delete;

public:
	GJHRenderManagerComponent operator=(const GJHRenderManagerComponent& _Other) = delete;
	GJHRenderManagerComponent operator=(const GJHRenderManagerComponent&& _Other) = delete;

public:
	void RenderPrev() override;
	void RenderNext() override;
	void Render() override;
	void Release() override;
};