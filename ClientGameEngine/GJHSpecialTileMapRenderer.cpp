#include "GJHSpecialTileMapRenderer.h"
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
#include <GJHGameEngineActor.h>
#include <algorithm>

bool GJHSpecialTileMapRenderer::m_IsDebugRender = false;

GJHSpecialTileMapRenderer::GJHSpecialTileMapRenderer() :
	m_CutData(0.f, 0.f, 1.f, 1.f),
	m_Color(1.f, 1.f, 1.f, 1.f),
	OnlyColor(0.f, 0.f, 0.f, 0.f)
{

}

GJHSpecialTileMapRenderer::~GJHSpecialTileMapRenderer()
{

}

void GJHSpecialTileMapRenderer::Start(int _Order)
{
	GJHRenderer::Start(_Order);

	CreateRenderPlayer(L"Texture2DMat", L"Rect");
	CBufferSettingLink(L"LastEffectColor", m_Color);
	CBufferSettingLink(L"LastOnlyColor", OnlyColor);
	CBufferSettingLink(L"SpriteCutData", m_CutData);

	m_DebugRender = GetActor()->CreateComponent<GJHRenderer>();
	m_DebugRender->CreateRenderPlayer(L"BoxMat", L"Ray");
	m_DebugRender->CBufferSettingNew(L"ColorData", float4::RED);
}

void GJHSpecialTileMapRenderer::Render(GJHCamera* _Camera)
{
	if (m_RenderEnable == false
		|| m_IsDebugRender == false)
	{
		return;
	}

	GJHRenderingTransformData& CameraData = _Camera->GetCameraTransformMatrix();
	CalRenderDataCopy();

	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = CameraData.View;
	RendererData.Projection = CameraData.Projection;
	RendererData.CalRenderingMatrix();

	for (auto& Map : m_TileMap)
	{
		if ((int)Map.second == -1)
		{
			continue;
		}

		float4 RenderPos = float4(m_TileSize.x * Map.first.x, -m_TileSize.y * Map.first.y);
		RenderPos += m_TilePivot;
		RenderPos.z = 0.f;
		RendererData.Position.Position(RenderPos);

		float4 Scale = m_TileSize;
		Scale.z = 0.f;
		RendererData.Scale.Scale(Scale);

		RendererData.CalWorld();
		RendererData.CalRenderingMatrix();

		m_CutData = m_TileSprite->GetCutData((int)Map.second);

		for (size_t i = 0; i < m_PlayerList.size(); i++)
		{
			m_PlayerList[i]->Render();
		}
	}

	for (auto& StartLink : m_DoorLinkList)
	{
		for (auto& EndLink : StartLink.second)
		{
			float4 StartPos = float4(m_TileSize.x * StartLink.first.x + m_TileSize.x * 0.5f, m_TileSize.y * -StartLink.first.y + m_TileSize.y * -0.5f, -50.f);
			float4 EndPos = float4(m_TileSize.x * EndLink.x + m_TileSize.x * 0.5f, m_TileSize.y * -EndLink.y + m_TileSize.y * -0.5f, -50.f);
			float Len = float4::CalLen(EndPos - StartPos);
			float Angle = float4::CalVectorAngle360(1, EndPos, StartPos);

			m_DebugRender->CalRenderDataCopy();

			GJHRenderingTransformData& DebugRendererData = m_DebugRender->GetRenderingData();
			DebugRendererData.View = CameraData.View;
			DebugRendererData.Projection = CameraData.Projection;
			DebugRendererData.CalRenderingMatrix();

			DebugRendererData.Position.Position(StartPos);
			DebugRendererData.Scale.Scale({ Len, 1.f, 0.f });
			DebugRendererData.Rotation.RotationXYZDeg({ 0.f, 0.f, Angle });

			DebugRendererData.CalWorld();
			DebugRendererData.CalRenderingMatrix();

			m_DebugRender->GetRenderPlayer(0)->Render();
		}
	}
}

void GJHSpecialTileMapRenderer::CreateTileMap(const GJHGameEngineString& _TileName, float4 _Size)
{
	ChangeTileMap(_TileName, _Size);
	Clear();
}

void GJHSpecialTileMapRenderer::ChangeTileMap(const GJHGameEngineString& _TileName, float4 _Size)
{
	GJHRenderer::TextureSetting(L"Tex", _TileName);

	m_TileSprite = GJHDirectSprite::Find(_TileName);
	m_TileSize = _Size;

	m_TilePivot = m_TileSize.HalfVector2D();
	m_TilePivot.y *= -1.0f;

	m_TileCountX = 200;
	m_TileCountY = 200;
}

void GJHSpecialTileMapRenderer::SetTile(float4 _Pos, int _SpriteIndex)
{
	SetTile(GetTileIndex(_Pos), _SpriteIndex);
}


void GJHSpecialTileMapRenderer::SetTile(const TileInfo& _TileInfo, int _SpriteIndex)
{
	if (_TileInfo.y < 0 || m_TileCountY < _TileInfo.y)
	{
		return;
	}

	if (_TileInfo.x < 0 || m_TileCountX < _TileInfo.x)
	{
		return;
	}

	TileInfo Info(_TileInfo.x, _TileInfo.y);
	
	TILETYPE Type;
	switch (_SpriteIndex)
	{
	case (int)TILETYPE::DELETETILE:
		Type = TILETYPE::DELETETILE;
		break;
	case (int)TILETYPE::STAIRWAY:
		Type = TILETYPE::STAIRWAY;
		break;
	case (int)TILETYPE::STAIRWAYINOUT:
		Type = TILETYPE::STAIRWAYINOUT;
		break;
	case (int)TILETYPE::DOOR:
		Type = TILETYPE::DOOR;
		break;
	}
	
	std::map<TileInfo, TILETYPE>::iterator Find = m_TileMap.find(Info);

	if (Type == TILETYPE::DELETETILE)
	{
		if (Find != m_TileMap.end())
		{
			m_TileMap.erase(Find->first);
			return;
		}
	}
	else
	{
		if (Find == m_TileMap.end())
		{
			m_TileMap.insert(std::make_pair(Info, Type));
			return;
		}

		if (Find->second != Type)
		{
			m_TileMap[Find->first] = Type;
		}
	}
}

void GJHSpecialTileMapRenderer::SetTileMap(const std::map<TileInfo, TILETYPE>& _TileMap)
{
	m_TileMap = _TileMap;
}

void GJHSpecialTileMapRenderer::SetDoorLink(const TileInfo& _Start, const TileInfo& _End)
{
	{
		auto Find = m_DoorLinkList.find(_Start);
		if (Find == m_DoorLinkList.end())
		{
			m_DoorLinkList.insert(std::make_pair(_Start, std::set<TileInfo>()));
		}
	}

	{
		auto Find = m_DoorLinkList.find(_End);
		if (Find == m_DoorLinkList.end())
		{
			m_DoorLinkList.insert(std::make_pair(_End, std::set<TileInfo>()));
		}
	}

	m_DoorLinkList[_Start].insert(_End);
	m_DoorLinkList[_End].insert(_Start);
}

void GJHSpecialTileMapRenderer::SetDoorLink(const std::map<TileInfo, std::set<TileInfo>>& _DoorList)
{
	m_DoorLinkList = _DoorList;
}

void GJHSpecialTileMapRenderer::DeleteDoorLink(const TileInfo& _Info)
{
	if (m_DoorLinkList.find(_Info) != m_DoorLinkList.end())
	{
		m_DoorLinkList.erase(_Info);
	}

	for (auto& List : m_DoorLinkList)
	{
		for (auto& Info : List.second)
		{
			if (Info == _Info)
			{
				List.second.erase(_Info);
				break;
			}
		}
	}
}

void GJHSpecialTileMapRenderer::Clear()
{
	m_TileMap.clear();
	m_DoorLinkList.clear();
}

void GJHSpecialTileMapRenderer::SetColor(float4 _Value)
{
	m_Color = _Value;
}

void GJHSpecialTileMapRenderer::TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize)
{
	GJHRenderer::TextureSetting(L"Tex", _TexName, _AutoSize);
}

float4 GJHSpecialTileMapRenderer::GetTilePos(const float4& _Pos)
{
	TileInfo Index = GetTileIndex(_Pos);

	return float4(static_cast<float>(Index.x * m_TileSize.x), -static_cast<float>(Index.y * m_TileSize.y));
}

TileInfo GJHSpecialTileMapRenderer::GetTileIndex(const float4& _Pos)
{
	return TileInfo(abs(static_cast<int>(_Pos.x / m_TileSize.x)), abs(static_cast<int>(_Pos.y / m_TileSize.y)));
}

TILETYPE GJHSpecialTileMapRenderer::GetTileType(const float4& _Pos)
{
	std::map<TileInfo, TILETYPE>::iterator Find = m_TileMap.find(GetTileIndex(_Pos));

	if (Find == m_TileMap.end())
	{
		return TILETYPE::NOTFOUND;
	}

	return Find->second;
}