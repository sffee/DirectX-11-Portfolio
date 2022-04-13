#pragma once
#include "GJHRenderer.h"

enum class SceneChangeType
{
	Start,
	End
};

class GJHDirectSprite;
class GJHSceneChangeRenderer : public GJHRenderer
{
private:
	float4 m_DeltaTime;
	SceneChangeType m_SceneChangeType;

public:
	void Start(int _Order, SceneChangeType _Type);

public:
	GJHSceneChangeRenderer();
	~GJHSceneChangeRenderer();

public:
	GJHSceneChangeRenderer(const GJHSceneChangeRenderer& _Other) = delete;
	GJHSceneChangeRenderer(const GJHSceneChangeRenderer&& _Other) = delete;

public:
	GJHSceneChangeRenderer operator=(const GJHSceneChangeRenderer& _Other) = delete;
	GJHSceneChangeRenderer operator=(const GJHSceneChangeRenderer&& _Other) = delete;

public:
	void Render(GJHCamera* _Camera) override;
	void Update() override;

public:
	void TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize = false);
};