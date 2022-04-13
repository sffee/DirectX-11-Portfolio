#pragma once
#include <GJHGameEngineGUI.h>

class HeadHunter2;
class Boss2SceneWindow : public GJHGUIWindow
{
private:
	HeadHunter2* m_HeadHunter;

public:
	void SetHeadHunter(HeadHunter2* _HeadHunter)
	{
		m_HeadHunter = _HeadHunter;
	}

public:
	Boss2SceneWindow();
	~Boss2SceneWindow();

public:
	Boss2SceneWindow(const Boss2SceneWindow& _Other) = delete;
	Boss2SceneWindow(const Boss2SceneWindow&& _Other) = delete;

public:
	Boss2SceneWindow operator=(const Boss2SceneWindow& _Other) = delete;
	Boss2SceneWindow operator=(const Boss2SceneWindow&& _Other) = delete;

public:
	virtual void OnGUI() override;
};