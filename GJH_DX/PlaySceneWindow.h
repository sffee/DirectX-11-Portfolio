#pragma once
#include <GJHGameEngineGUI.h>

class PlaySceneWindow : public GJHGUIWindow
{
//public:
//	static float4 m_CharPos;
//	static float4 m_MousePos;
//	static float4 m_CamPos;

public:
	PlaySceneWindow();
	~PlaySceneWindow();

public:
	PlaySceneWindow(const PlaySceneWindow& _Other) = delete;
	PlaySceneWindow(const PlaySceneWindow&& _Other) = delete;

public:
	PlaySceneWindow operator=(const PlaySceneWindow& _Other) = delete;
	PlaySceneWindow operator=(const PlaySceneWindow&& _Other) = delete;

public:
	virtual void OnGUI() override;
};