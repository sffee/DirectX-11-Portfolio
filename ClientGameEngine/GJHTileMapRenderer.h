#pragma once
#include "GJHRenderer.h"
#include <GJHGameEngineMath.h>
#include <vector>

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
class GJHTileMapRenderer : public GJHRenderer
{
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
    GJHTileMapRenderer();
    ~GJHTileMapRenderer();

public:
    GJHTileMapRenderer(const GJHTileMapRenderer& _Other) = delete;
    GJHTileMapRenderer(const GJHTileMapRenderer&& _Other) = delete;

public:
    GJHTileMapRenderer& operator=(const GJHTileMapRenderer& _Other) = delete;
    GJHTileMapRenderer& operator=(const GJHTileMapRenderer&& _Other) = delete;

public:
    void Start(int _Order = 0);
	void Render(GJHCamera* _Camera) override;

public:
    void SetTile(float4 _Pos, int _SpriteIndex);
    void SetTile(int _X, int _Y, int _SpriteIndex);
    void TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize);

public:
    std::vector<std::vector<int>> m_TileMap;

public:
    void Clear();

public:
    void SetColor(float4 _Value);

public:
    void CreateTileMap(const GJHGameEngineString& _TileName, float4 _Size);
    void ChangeTileMap(const GJHGameEngineString& _TileName, float4 _Size);

public:
    void Capture(std::shared_ptr<GJHCamera> _Camera, const float4& _MapSize, const GJHGameEngineString& _FileName);
    void Capture(std::shared_ptr<GJHCamera> _Camera, std::shared_ptr<GJHTileMapRenderer>& _OtherRenderer, const float4& _MapSize, const GJHGameEngineString& _FileName);
    void CaptureRender(std::shared_ptr<GJHCamera> _Camera, const float4 _Size);
};

