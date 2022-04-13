#pragma once
#include <GJHContentsSceneComponent.h>

class GJHSpriteRenderer;
class EndingScene : public GJHContentsSceneComponent
{
public:
	EndingScene();
	~EndingScene();

public:
	EndingScene(const EndingScene& _Other) = delete;
	EndingScene(const EndingScene&& _Other) = delete;

public:
	EndingScene operator=(const EndingScene& _Other) = delete;
	EndingScene operator=(const EndingScene&& _Other) = delete;

private:
	std::shared_ptr<GJHSpriteRenderer> m_Render;
	float Color;

public:
	void Start() override;
	void Update() override;
};