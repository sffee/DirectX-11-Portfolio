#include "RenderingWindow.h"
#include <GJHClientGameEngine.h>
#include <GJHGameEngineTime.h>
#include <GJHSceneManager.h>
#include <GJHRenderManagerComponent.h>
#include <GJHCamera.h>
#include <GJHDirectRenderTarget.h>
#include <GJHDirectTexture2D.h>

void RenderTargetWindow::OnGUI()
{
	if (m_RenderTarget != nullptr)
	{
		float4 WindowSize = GJHGameEngineWindow::GetSize();

		if (GJHGameEngineGUI::ImageButton(m_Index, m_RenderTarget->GetTex(m_Index), WindowSize) == true)
		{
			Close();
		}
	}
}

void RenderingWindow::OnGUI()
{
	std::shared_ptr<GJHGameEngineScene> Scene = GJHClientGameEngine::GetCurScene();

	if (Scene == nullptr)
	{
		return;
	}

	std::shared_ptr<GJHRenderManagerComponent> RenderCom = Scene->FindComponent<GJHRenderManagerComponent>();

	if (RenderCom == nullptr)
	{
		GJHGameEngineGUI::Text(L"렌더 매니저 컴포넌트를 찾지 못했습니다.");
		return;
	}

	float4 WindowSize = GJHGameEngineWindow::GetSize();
	float4 WindowButtonSize = WindowSize * 0.1f;

	std::map<int, std::list<GJHCamera*>> CamList = RenderCom->GetCameraList();

	for (auto& CamGroup : CamList)
	{
		if (GJHGameEngineGUI::TreeNode(L"카메라 그룹 : ", CamGroup.first) == true)
		{
			int Count = 0;
			for (auto& Cam : CamGroup.second)
			{
				if (GJHGameEngineGUI::TreeNode(L"카메라", Count++) == true)
				{
					for (int i = 0; i < Cam->GetCameraTarget()->GetTexsize(); i++)
					{
						if (GJHGameEngineGUI::ImageButton(static_cast<int>(i), Cam->GetCameraTarget()->GetTex(i), WindowButtonSize) == true)
						{
							std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(L"카메라 메인렌더타겟", GJHGUIWindow::Style::AutoSize);
							RWindow->SetRenderTarget(Cam->GetCameraTarget().get(), static_cast<int>(i));
						}
					}

					GJHGameEngineGUI::TreePop();
				}

				GJHGameEngineGUI::Text(L"카메라", Count++);
			}

			GJHGameEngineGUI::TreePop();
		}
	}

	for (int i = 0; i < OtherTarget.size(); i++)
	{
		if (GJHGameEngineGUI::TreeNode(OtherTargetInfo[i].Name.c_str()) == true)
		{
			if (GJHGameEngineGUI::ImageButton(i, OtherTarget[i]->GetTex(0), OtherTargetInfo[i].Size) == true)
			{
				std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(OtherTargetInfo[i].Name.c_str(), GJHGUIWindow::Style::AutoSize);
				RWindow->SetRenderTarget(OtherTarget[i].get(), 0);
			}

			GJHGameEngineGUI::TreePop();
		}
	}
}

std::vector<RenderingWindow::TargetInfo> RenderingWindow::OtherTargetInfo;
std::vector<std::shared_ptr<GJHDirectRenderTarget>> RenderingWindow::OtherTarget;