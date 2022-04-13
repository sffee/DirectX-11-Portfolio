#include "GJHTileMapRenderer.h"
#include <GJHDirectRasterizer.h>
#include <GJHDirectMesh.h>
#include <GJHDirectPixelShader.h>
#include <GJHDirectVertexShader.h>
#include <GJHGameEngineActor.h>
#include <GJHGameEngineDebug.h>
#include <GJHDirectDepthStencil.h>
#include "GJHActorTransform.h"
#include <GJHDirectConstantBuffer.h>
#include <GJHDirectTexture2D.h>
#include <GJHDirectSampler.h>
#include <GJHDirectBlend.h>
#include <GJHDirectSprite.h>
#include "GJHCamera.h"
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include <GJHDirectRenderTarget.h>

GJHTileMapRenderer::GJHTileMapRenderer() :
	m_CutData(0.f, 0.f, 1.f, 1.f),
	m_Color(1.f, 1.f, 1.f, 1.f),
	OnlyColor(0.f, 0.f, 0.f, 0.f)
{

}

GJHTileMapRenderer::~GJHTileMapRenderer() 
{

}

void GJHTileMapRenderer::Start(int _Order)
{
	GJHRenderer::Start(_Order);

	CreateRenderPlayer(L"Texture2DMat", L"Rect");
	CBufferSettingLink(L"LastEffectColor", m_Color);
	CBufferSettingLink(L"LastOnlyColor", OnlyColor);
	CBufferSettingLink(L"SpriteCutData", m_CutData);
}

void GJHTileMapRenderer::Render(GJHCamera* _Camera)
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

	for (size_t y = 0; y < m_TileMap.size(); y++)
	{
		for (size_t x = 0; x < m_TileMap[y].size(); x++)
		{
			if (m_TileMap[y][x] == -1)
			{
				continue;
			}
			
			float4 RenderPos = { m_TileSize.x * x, -m_TileSize.y * y, RendererData.Position.Arr[3][2] };
			RenderPos += m_TilePivot;

			RendererData.Position.Position(RenderPos);
			RendererData.Scale.Scale(m_TileSize);
			RendererData.CalWorld();
			RendererData.CalRenderingMatrix();

			m_CutData = m_TileSprite->GetCutData(m_TileMap[y][x]);

			for (size_t i = 0; i < m_PlayerList.size(); i++)
			{
				m_PlayerList[i]->Render();
			}
		}
	}
}

void GJHTileMapRenderer::CreateTileMap(const GJHGameEngineString& _TileName, float4 _Size)
{
	ChangeTileMap(_TileName, _Size);
	Clear();
}

void GJHTileMapRenderer::ChangeTileMap(const GJHGameEngineString& _TileName, float4 _Size)
{
	GJHRenderer::TextureSetting(L"Tex", _TileName);

	m_TileSprite = GJHDirectSprite::Find(_TileName);
	m_TileSize = _Size;

	m_TilePivot = m_TileSize.HalfVector2D();
	m_TilePivot.y *= -1.0f;

	m_TileCountX = 200;
	m_TileCountY = 200;
}

void GJHTileMapRenderer::SetTile(float4 _Pos, int _SpriteIndex) 
{
	SetTile(static_cast<int>(_Pos.x / m_TileSize.x), static_cast<int>(_Pos.y / m_TileSize.y), _SpriteIndex);
}


void GJHTileMapRenderer::SetTile(int _X, int _Y, int _SpriteIndex) 
{
	if (_Y < 0 || m_TileCountY < _Y)
	{
		return;
	}

	if (_X < 0 || m_TileCountX < _X)
	{
		return;
	}

	m_TileMap[_Y][_X] = _SpriteIndex;
}

void GJHTileMapRenderer::Clear()
{
	m_TileMap.clear();
	m_TileMap.resize(m_TileCountY);

	for (size_t y = 0; y < m_TileCountY; y++)
	{
		for (size_t x = 0; x < m_TileCountX; x++)
		{
			m_TileMap[y].push_back(-1);
		}
	}
}

void GJHTileMapRenderer::Capture(std::shared_ptr<GJHCamera> _Camera, const float4& _MapSize, const GJHGameEngineString& _FileName)
{	
	std::shared_ptr<GJHDirectRenderTarget> NewRenderTarget = std::make_shared<GJHDirectRenderTarget>();
	NewRenderTarget->CreateRenderTarget(_MapSize, DXGI_FORMAT_R8G8B8A8_UNORM, { 0.f, 0.f, 0.f, 1.f });
	NewRenderTarget->Clear();
	NewRenderTarget->Setting();

	float4 Size = _Camera->GetCamProjectionSize() / _MapSize;

	CaptureRender(_Camera, Size);

	NewRenderTarget->GetTex(0)->SavePngFile(_FileName);
}

void GJHTileMapRenderer::Capture(std::shared_ptr<GJHCamera> _Camera, std::shared_ptr<GJHTileMapRenderer>& _OtherRenderer, const float4& _MapSize, const GJHGameEngineString& _FileName)
{
	std::shared_ptr<GJHDirectRenderTarget> NewRenderTarget = std::make_shared<GJHDirectRenderTarget>();
	NewRenderTarget->CreateRenderTarget(_MapSize, DXGI_FORMAT_R8G8B8A8_UNORM, { 0.f, 0.f, 0.f, 1.f });
	NewRenderTarget->Clear();
	NewRenderTarget->Setting();
	
	float4 Size = _Camera->GetCamProjectionSize() / _MapSize;

	CaptureRender(_Camera, Size);
	_OtherRenderer->CaptureRender(_Camera, Size);

	NewRenderTarget->GetTex(0)->SavePngFile(_FileName);
}

void GJHTileMapRenderer::CaptureRender(std::shared_ptr<GJHCamera> _Camera, const float4 _Size)
{
	GJHRenderingTransformData& CameraData = _Camera->GetCameraTransformMatrix();
	CalRenderDataCopy();

	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = CameraData.View;
	RendererData.Projection = CameraData.Projection;
	RendererData.CalRenderingMatrix();

	for (size_t y = 0; y < m_TileMap.size(); y++)
	{
		for (size_t x = 0; x < m_TileMap[y].size(); x++)
		{
			if (m_TileMap[y][x] == -1)
			{
				continue;
			}

			float4 RenderPos = float4(m_TileSize.x * x, -m_TileSize.y * y) * _Size;
			RenderPos += m_TilePivot * _Size;
			RenderPos.z = 0.f;
			RendererData.Position.Position(RenderPos);

			float4 Scale = m_TileSize * _Size;
			Scale.z = 0.f;
			RendererData.Scale.Scale(Scale);

			RendererData.CalWorld();
			RendererData.CalRenderingMatrix();

			m_CutData = m_TileSprite->GetCutData(m_TileMap[y][x]);

			for (size_t i = 0; i < m_PlayerList.size(); i++)
			{
				m_PlayerList[i]->Render();
			}
		}
	}
}

void GJHTileMapRenderer::SetColor(float4 _Value)
{
	m_Color = _Value;
}

void GJHTileMapRenderer::TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize)
{
	GJHRenderer::TextureSetting(L"Tex", _TexName, _AutoSize);
}