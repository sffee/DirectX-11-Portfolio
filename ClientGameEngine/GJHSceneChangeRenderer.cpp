#include "GJHSceneChangeRenderer.h"
#include <GJHDirectSprite.h>
#include <GJHGameEngineTime.h>
#include <GJHGameEngineActor.h>

GJHSceneChangeRenderer::GJHSceneChangeRenderer()
{

}

GJHSceneChangeRenderer::~GJHSceneChangeRenderer()
{

}

void GJHSceneChangeRenderer::Start(int _Order, SceneChangeType _Type)
{
	GJHRenderer::Start(_Order);

	m_DeltaTime.x = 0.f;

	if (_Type == SceneChangeType::Start)
	{
		CreateRenderPlayer(L"SceneChangeStartMat", L"FullRect");
		TextureSetting(L"StartTransition.png");
	}
	else if (_Type == SceneChangeType::End)
	{
		CreateRenderPlayer(L"SceneChangeEndMat", L"FullRect");
		TextureSetting(L"EndTransition.png");
	}

	CBufferSettingLink(L"DeltaTimeData", m_DeltaTime);
}

void GJHSceneChangeRenderer::Update()
{
	m_DeltaTime.x += GJHGameEngineTime::FDeltaTime(2.5f, false);
}

void GJHSceneChangeRenderer::Render(GJHCamera* _Camera)
{
	m_PlayerList[0]->Render();
}

void GJHSceneChangeRenderer::TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize)
{
	GJHRenderer::TextureSetting(L"Tex", _TexName, _AutoSize);
}