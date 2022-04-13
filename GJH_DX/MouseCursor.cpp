#include "MouseCursor.h"
#include <GJHActorTransform.h>
#include <GJHCamera.h>
#include <GJHGameEngineWindow.h>
#include <GJHSpriteRenderer.h>
#include "define.h"

MouseCursor::MouseCursor()
{

}

MouseCursor::~MouseCursor()
{

}

void MouseCursor::Start(std::shared_ptr<GJHCamera> _Cam)
{
	m_Trans = GetActor()->CreateComponent<GJHActorTransform>();
	m_Render = GetActor()->CreateComponent<GJHSpriteRenderer>();
	m_CamTrans = _Cam->GetActor()->FindComponent<GJHActorTransform>();

	m_Render->TextureSetting(L"Cursor_0.png", true);
}

void MouseCursor::Update()
{
	float4 MousePos = GJHGameEngineWindow::GetMousePos();
	float4 WindowSize = GJHGameEngineWindow::GetSize();
	MousePos.x /= (WindowSize.x / CAMSIZEX);
	MousePos.y /= (WindowSize.y / CAMSIZEY);

	MousePos.y *= -1.f;

	MousePos = MousePos + (m_CamTrans->GetWorldPosition() - float4(CAMSIZEX * 0.5f, CAMSIZEY * -0.5f));
	MousePos.z = -10.f;

	m_Render->SetWorldPosition(MousePos);
}