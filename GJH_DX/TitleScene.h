#pragma once
#include <GJHContentsSceneComponent.h>

class TitleScene : public GJHContentsSceneComponent
{
public:
	TitleScene();
	~TitleScene();

public:
	TitleScene(const TitleScene& _Other) = delete;
	TitleScene(const TitleScene&& _Other) = delete;

public:
	TitleScene operator=(const TitleScene& _Other) = delete;
	TitleScene operator=(const TitleScene&& _Other) = delete;

private:
	bool m_StartMapMove;
	float m_Time;

public:
	void Start() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;
};