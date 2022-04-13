#pragma once
#include "GJHPlayerBase.h"
#include "GJHMaterial.h"
#include "GJHDirectMesh.h"

enum class RENDERINGTYPE
{
	NORMAL,
	INSTANCE,
};

class GJHDirectRenderTarget;
class GJHRenderPlayer : public GJHPlayerBase
{
private:
public:
private:
	std::shared_ptr<GJHMaterial> m_Material;
	std::shared_ptr<GJHDirectMesh> m_Mesh;

	RENDERINGTYPE m_RenderType;
	int m_InstanceCount;

public:
	void SetRenderType(RENDERINGTYPE _RenderType)
	{
		m_RenderType = _RenderType;
	}

	void SetInstanceCount(int _InstanceCount)
	{
		m_InstanceCount = _InstanceCount;
	}

public:
	void SetVertexShader(const GJHGameEngineString& _Name);
	void SetGeometryShader(const GJHGameEngineString& _Name);
	void SetPixelShader(const GJHGameEngineString& _Name);
	void SetRasterizer(const GJHGameEngineString& _Name);
	void SetDepthStencil(const GJHGameEngineString& _Name);
	void SetBlend(const GJHGameEngineString& _Name);

public:
	void ReLoadingSettingData();

	void SetMaterial(const GJHGameEngineString& _Name);
	void SetMaterial(std::shared_ptr<GJHMaterial> _Mat);
	void SetMesh(const GJHGameEngineString& _Name);

public:
	GJHRenderPlayer();
	~GJHRenderPlayer();

public:
	GJHRenderPlayer(const GJHRenderPlayer& _Other) = delete;
	GJHRenderPlayer(const GJHRenderPlayer&& _Other) = delete;

public:
	GJHRenderPlayer& operator=(const GJHRenderPlayer& _Other) = delete;
	GJHRenderPlayer& operator=(const GJHRenderPlayer&& _Other) = delete;

public:
	void Render();
	void Render(std::shared_ptr<GJHDirectRenderTarget> _Target);
};

