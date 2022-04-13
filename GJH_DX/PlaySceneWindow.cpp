#include "PlaySceneWindow.h"
#include <GJHCollisionComponent.h>
#include <GJHRayComponent.h>
#include <GJHSpecialTileMapRenderer.h>
#include "StageBase.h"
#include "Player.h"

//float4 PlaySceneWindow::m_CharPos;
//float4 PlaySceneWindow::m_MousePos;
//float4 PlaySceneWindow::m_CamPos;

PlaySceneWindow::PlaySceneWindow()
{

}

PlaySceneWindow::~PlaySceneWindow()
{

}

void PlaySceneWindow::OnGUI()
{
	GJHGameEngineGUI::Text(L"강제 시간 되감기(F1)");
	GJHGameEngineGUI::NewLine();
	GJHGameEngineGUI::AddLine(GJHGameEngineGUI::GetCursorScreenPos(), GJHGameEngineGUI::GetCursorScreenPos() + float4(GJHGameEngineGUI::GetWindowContentRegionWidth(), 0.f), { 255.f, 255.f, 255.f, 255.f });
	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"충돌체 보기(F2)");
	GJHGameEngineGUI::SameLine();
	if (GJHCollisionComponent::IsDebugRender() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	GJHGameEngineGUI::Text(L"Ray 보기(F3)");
	GJHGameEngineGUI::SameLine();
	GJHGameEngineGUI::SameLine();

	if (GJHRayComponent::IsDebugRender() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	GJHGameEngineGUI::Text(L"특수 타일 보기(F4)");
	GJHGameEngineGUI::SameLine();
	if (GJHSpecialTileMapRenderer::IsDebugRender() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	GJHGameEngineGUI::Text(L"배터리 무한(F5)");
	GJHGameEngineGUI::SameLine();
	if (Player::GetInfinityBattery() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	GJHGameEngineGUI::Text(L"무적(F6)");
	GJHGameEngineGUI::SameLine();
	if (Player::GetIsNotDie() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	GJHGameEngineGUI::Text(L"충돌맵 보기(F7)");
	GJHGameEngineGUI::SameLine();
	if (StageBase::GetIsDebug() == true)
	{
		GJHGameEngineGUI::TextColored({ 0.f, 255.f, 0.f, 1.f }, L"ON");
	}
	else
	{
		GJHGameEngineGUI::TextColored({ 255.f, 0.f, 0.f, 1.f }, L"OFF");
	}

	//GJHGameEngineGUI::Text(L"CharPos : ", m_CharPos);
	//GJHGameEngineGUI::Text(L"MousePos : ", m_MousePos);
	//GJHGameEngineGUI::Text(L"CamPos : ", m_CamPos);
}