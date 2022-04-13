#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHDirectResourcesManager.h"
#include "GJHVertexHeader.h"
#include "GJHDirectVertexShader.h"
#include "GJHDirectGeometryShader.h"
#include "GJHDirectPixelShader.h"
#include <GJHGameEngineDirectory.h>

void MaskShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"2DTextureMaskShader.hlsl"), L"TexMaskVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"2DTextureMaskShader.hlsl"), L"TexMaskPixShader");
	}
}

void PostEffectShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Uvdistortion.hlsl"), L"UvdistortionVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Uvdistortion.hlsl"), L"UvdistortionPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Grid2D.hlsl"), L"Grid2DVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Grid2D.hlsl"), L"Grid2DPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"OutLine.hlsl"), L"OutLineVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"OutLine.hlsl"), L"OutLinePix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"MaskCulling.hlsl"), L"MaskCullingVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"MaskCulling.hlsl"), L"MaskCullingPix");
	}
}

void Shader2D(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"ColorShader.hlsl"), L"VtxColorShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"ColorShader.hlsl"), L"PixColorShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"2DTextureShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"2DTextureShader.hlsl"), L"TexPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"CircleShader.hlsl"), L"CircleVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"CircleShader.hlsl"), L"CirclePixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"BoxShader.hlsl"), L"BoxVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"BoxShader.hlsl"), L"BoxPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"SceneChangeStartShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"SceneChangeStartShader.hlsl"), L"TexPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"SceneChangeEndShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"SceneChangeEndShader.hlsl"), L"TexPixShader");
	}
}

void Shader3D(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"VS_Particle");
		GJHDirectGeometryShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"GS_Particle");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"PS_Particle");
	}
}

void TargetShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"TargetMergeShader.hlsl"), L"MergeVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"TargetMergeShader.hlsl"), L"MergePix");
	}
}

void GJHGameEngineDevice::ShaderInit()
{
	GJHGameEngineDirectory Dir;
	Dir.MoveParent(L"Portfolio");
	Dir.Move(L"ShaderProject");

	Shader3D(Dir);
	TargetShader(Dir);
	MaskShader(Dir);
	Shader2D(Dir);
	PostEffectShader(Dir);
}