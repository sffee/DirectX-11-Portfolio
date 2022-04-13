#pragma once
#include "GJHRenderer.h"
#include <GJHGameEngineMath.h>
#include <map>
#include <vector>
#include <set>

enum class TILETYPE
{
    NOTFOUND = -2,
    DELETETILE = -1,
    STAIRWAY = 0,
    STAIRWAYINOUT = 1,
    DOOR = 2
};

struct TileInfo
{
    union
    {
        struct
        {
            int x;
            int y;
        };

        __int64 Key;
    };

    TileInfo() :
        x(0), y(0)
    {
    }

    TileInfo(int _X, int _Y) :
        x(_X), y(_Y)
    {
    }

    bool operator==(const TileInfo& _Other) const
    {
        return Key == _Other.Key;
    }

    bool operator<(const TileInfo& _Other) const
    {
        return Key < _Other.Key;
    }

    bool operator>(const TileInfo& _Other) const
    {
        return Key > _Other.Key;
    }
};

class GJHActorTransform;
class GJHDirectBlend;
class GJHDirectRasterizer;
class GJHDirectMesh;
class GJHDirectPixelShader;
class GJHDirectVertexShader;
class GJHDirectConstantBuffer;
class GJHDirectDepthStencil;
class GJHDirectTexture2D;
class GJHDirectSampler;
class GJHDirectSprite;
class GJHSpecialTileMapRenderer : public GJHRenderer
{
private:
    static bool m_IsDebugRender;

public:
    static void SetDebugRender(bool _Set)
    {
        m_IsDebugRender = _Set;
    }

    static void ToggleDebugRender()
    {
        m_IsDebugRender = !m_IsDebugRender;
    }

    static bool IsDebugRender()
    {
        return m_IsDebugRender;
    }

private:
    float4 m_CutData;
    float4 m_Color;
    float4 OnlyColor;

private:
    std::shared_ptr<GJHDirectSprite> m_TileSprite;

public:
    float4 m_TileSize;
    float4 m_TilePivot;

    int m_TileCountX;
    int m_TileCountY;

public:
    std::map<TileInfo, TILETYPE> m_TileMap;
    std::map<TileInfo, std::set<TileInfo>> m_DoorLinkList;
    std::shared_ptr<GJHRenderer> m_DebugRender;

public:
    GJHSpecialTileMapRenderer();
    ~GJHSpecialTileMapRenderer();

public:
    GJHSpecialTileMapRenderer(const GJHSpecialTileMapRenderer& _Other) = delete;
    GJHSpecialTileMapRenderer(const GJHSpecialTileMapRenderer&& _Other) = delete;

public:
    GJHSpecialTileMapRenderer& operator=(const GJHSpecialTileMapRenderer& _Other) = delete;
    GJHSpecialTileMapRenderer& operator=(const GJHSpecialTileMapRenderer&& _Other) = delete;

public:
    void Start(int _Order = 0);
    void Render(GJHCamera* _Camera) override;

public:
    void SetTile(float4 _Pos, int _SpriteIndex);
    void SetTile(const TileInfo& _TileInfo, int _SpriteIndex);
    void SetTileMap(const std::map<TileInfo, TILETYPE>& _TileMap);
    void TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize);
    void SetDoorLink(const std::map<TileInfo, std::set<TileInfo>>& _DoorList);
    void SetDoorLink(const TileInfo& _Start, const TileInfo& _End);
    void DeleteDoorLink(const TileInfo& _Info);

public:
    float4 GetTilePos(const float4& _Pos);
    TileInfo GetTileIndex(const float4& _Pos);
    TILETYPE GetTileType(const float4& _Pos);

public:
    void Clear();

public:
    void SetColor(float4 _Value);

public:
    void CreateTileMap(const GJHGameEngineString& _TileName, float4 _Size);
    void ChangeTileMap(const GJHGameEngineString& _TileName, float4 _Size);
};