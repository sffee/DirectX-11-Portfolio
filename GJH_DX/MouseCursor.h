#pragma once
#include <GJHContentsComponent.h>

class GJHSpriteRenderer;
class GJHCamera;
class MouseCursor : public GJHContentsComponent
{
public:
	MouseCursor();
	~MouseCursor();

public:
	MouseCursor(const MouseCursor& _Other) = delete;
	MouseCursor(const MouseCursor&& _Other) = delete;

public:
	MouseCursor operator=(const MouseCursor& _Other) = delete;
	MouseCursor operator=(const MouseCursor&& _Other) = delete;

private:
	std::shared_ptr<GJHActorTransform> m_Trans;
	std::shared_ptr<GJHSpriteRenderer> m_Render;
	std::shared_ptr<GJHActorTransform> m_CamTrans;

public:
	void Start(std::shared_ptr<GJHCamera> _Cam);
	void Update() override;
};