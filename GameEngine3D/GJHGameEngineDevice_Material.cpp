#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHMaterial.h"

void GJHGameEngineDevice::MaterialInit()
{
	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"TargetCopyMaterial");
		Mat->SetBlend(L"TargetMergeBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"TargetMergeShader.hlsl");
		Mat->SetPixelShader(L"TargetMergeShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Grid2D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"Grid2D.hlsl");
		Mat->SetPixelShader(L"Grid2D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Texture2DMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"2DTextureShader.hlsl");
		Mat->SetPixelShader(L"2DTextureShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Particle");
		Mat->SetBlend(L"ParticleBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"Particle.hlsl");
		Mat->SetGeometryShader(L"Particle.hlsl");
		Mat->SetPixelShader(L"Particle.hlsl");
		Mat->SetDrawType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Texture2DMaskMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"2DTextureMaskShader.hlsl");
		Mat->SetPixelShader(L"2DTextureMaskShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"OutLine");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"OutLine.hlsl");
		Mat->SetPixelShader(L"OutLine.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Uvdistortion");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"Uvdistortion.hlsl");
		Mat->SetPixelShader(L"Uvdistortion.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"MaskCulling");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"MaskCulling.hlsl");
		Mat->SetPixelShader(L"MaskCulling.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"CircleMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"CircleShader.hlsl");
		Mat->SetPixelShader(L"CircleShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"BoxMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"BoxShader.hlsl");
		Mat->SetPixelShader(L"BoxShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"SceneChangeStartMat");
		Mat->SetBlend(L"NoneAlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"SceneChangeStartShader.hlsl");
		Mat->SetPixelShader(L"SceneChangeStartShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"SceneChangeEndMat");
		Mat->SetBlend(L"NoneAlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"SceneChangeEndShader.hlsl");
		Mat->SetPixelShader(L"SceneChangeEndShader.hlsl");
	}
}