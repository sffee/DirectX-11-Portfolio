#include "Boss2SceneWindow.h"
#include "HeadHunter2.h"

Boss2SceneWindow::Boss2SceneWindow()
{

}

Boss2SceneWindow::~Boss2SceneWindow()
{

}

void Boss2SceneWindow::OnGUI()
{
	GJHGameEngineGUI::Text(L"현재 패턴");
	GJHGameEngineGUI::Text(m_HeadHunter->State.CurState);

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"예약 패턴");
	GJHGameEngineGUI::Text(m_HeadHunter->m_NextPattern);

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"일반 패턴 예약");
	if (GJHGameEngineGUI::Button(L"레이저 발사") == true)
	{
		m_HeadHunter->m_NextPattern = L"TakeOutRifle";
	}

	if (GJHGameEngineGUI::Button(L"대쉬") == true)
	{
		m_HeadHunter->m_NextPattern = L"PreDash";
	}

	if (GJHGameEngineGUI::Button(L"폭탄 발사") == true)
	{
		m_HeadHunter->m_NextPattern = L"TakeOutGun";
	}

	if (GJHGameEngineGUI::Button(L"점프 - 총알 뿌리기") == true)
	{
		m_HeadHunter->m_NextPattern = L"PreJump";
	}

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"특수 패턴 예약");
	if (GJHGameEngineGUI::Button(L"레이저 돌리기 - 대쉬") == true)
	{
		m_HeadHunter->m_NextPattern = L"Hit1_TeleportIn_Sweep";
	}

	if (GJHGameEngineGUI::Button(L"레이저 난사") == true)
	{
		m_HeadHunter->m_NextPattern = L"Hit2_TeleportIn";
	}
}