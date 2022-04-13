#include "GJHRenderer.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineDebug.h>
#include "GJHCamera.h"
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include "GJHActorTransform.h"

GJHRenderer::GJHRenderer() :
	m_pActorTransform(nullptr),
	m_ChangeOrder(false),
	m_RenderEnable(true)
{

}

GJHRenderer::~GJHRenderer()
{

}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::CreateRenderPlayer(const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh)
{
	std::shared_ptr<GJHRenderPlayer> NewRenderPlayer = std::make_shared<GJHRenderPlayer>();

	NewRenderPlayer->SetMaterial(_Material);
	NewRenderPlayer->SetMesh(_Mesh);

	if (NewRenderPlayer->IsCBufferSetting(L"RenderingTransformData"))
	{
		GJHRenderingTransformData& RendererData = GetRenderingData();
		NewRenderPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
	}

	m_PlayerList.push_back(NewRenderPlayer);

	return NewRenderPlayer;
}

void GJHRenderer::Start(int _Order)
{
	SetOrder(_Order);

	{
		GJHActorTransform::Start();

		ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();

		if (Trans == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
		}

		this->AttachParent(Trans);

		std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");

		Ptr->PushRenderer(this);
	}
}

void GJHRenderer::Start(int _Order, const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh)
{
	CreateRenderPlayer(_Material, _Mesh);
	Start(_Order);
}

void GJHRenderer::Render(GJHCamera* _Camera)
{
	if (m_RenderEnable == false)
	{
		return;
	}

	GJHRenderingTransformData& CameraData = _Camera->GetCameraTransformMatrix();
	CalRenderDataCopy();

	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = CameraData.View;
	RendererData.Projection = CameraData.Projection;
	RendererData.CalRenderingMatrix();

	for (size_t i = 0; i < m_PlayerList.size(); i++)
	{
		m_PlayerList[i]->Render();
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _Res);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _TexName);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _ShaderType, _TexName);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _ShaderType, _Res);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _SmpName);
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _ShaderType, _SmpName);
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectSampler> _Res, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _ShaderType, _Res);
}

std::vector<std::shared_ptr<GJHDirectTexture2D>> GJHRenderer::GetTexture(const GJHGameEngineString& _Name, int _RenderPlayerIndex)
{
	return m_PlayerList[_RenderPlayerIndex]->GetTexture(_Name);
}

std::shared_ptr<GJHDirectTexture2D> GJHRenderer::GetTexture(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _RenderPlayerIndex)
{
	return m_PlayerList[_RenderPlayerIndex]->GetTexture(_Name, _ShaderType);
}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::GetRenderPlayer(size_t _Index)
{
	return m_PlayerList[_Index];
}

void GJHRenderer::SetVertexShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetVertexShader(_Name);
}

void GJHRenderer::SetGeometryShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetGeometryShader(_Name);
}

void GJHRenderer::SetPixelShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetPixelShader(_Name);
}

void GJHRenderer::SetRasterizer(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetRasterizer(_Name);
}

void GJHRenderer::SetDepthStencil(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetDepthStencil(_Name);
}

void GJHRenderer::SetBlend(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetBlend(_Name);
}

void GJHRenderer::SetMesh(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetMesh(_Name);
}