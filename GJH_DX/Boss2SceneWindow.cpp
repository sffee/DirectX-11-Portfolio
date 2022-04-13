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
	GJHGameEngineGUI::Text(L"���� ����");
	GJHGameEngineGUI::Text(m_HeadHunter->State.CurState);

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"���� ����");
	GJHGameEngineGUI::Text(m_HeadHunter->m_NextPattern);

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"�Ϲ� ���� ����");
	if (GJHGameEngineGUI::Button(L"������ �߻�") == true)
	{
		m_HeadHunter->m_NextPattern = L"TakeOutRifle";
	}

	if (GJHGameEngineGUI::Button(L"�뽬") == true)
	{
		m_HeadHunter->m_NextPattern = L"PreDash";
	}

	if (GJHGameEngineGUI::Button(L"��ź �߻�") == true)
	{
		m_HeadHunter->m_NextPattern = L"TakeOutGun";
	}

	if (GJHGameEngineGUI::Button(L"���� - �Ѿ� �Ѹ���") == true)
	{
		m_HeadHunter->m_NextPattern = L"PreJump";
	}

	GJHGameEngineGUI::NewLine();

	GJHGameEngineGUI::Text(L"Ư�� ���� ����");
	if (GJHGameEngineGUI::Button(L"������ ������ - �뽬") == true)
	{
		m_HeadHunter->m_NextPattern = L"Hit1_TeleportIn_Sweep";
	}

	if (GJHGameEngineGUI::Button(L"������ ����") == true)
	{
		m_HeadHunter->m_NextPattern = L"Hit2_TeleportIn";
	}
}