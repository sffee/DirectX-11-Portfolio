#include "UIManager.h"
#include <GJHGameEngineScene.h>
#include "Player.h"
#include "InstanceObject.h"

UIManager::UIManager()
{

}

UIManager::~UIManager()
{

}

void UIManager::Start()
{
	CreateUI(L"HUD", { 320.f, -15.f });

	m_BatteryList.resize(11);
	for (int i = 0; i < 11; i++)
	{
		m_BatteryList[i] = CreateUI(L"Battery", { 14.f + (i * 5.f), -11.7f })->FindComponent<GJHSpriteRenderer>();
	}

	m_TimeGauge = CreateUI(L"TimerGauge", { 276.f, -9.f })->FindComponent<GJHSpriteRenderer>();
	m_TimeGauge->SetMesh(L"RayCenter");
	m_TimeGaugeXScale = (float)m_TimeGauge->GetTexture()->GetSizeX();

	CreateUI(L"ItemIcon_Katana", { 587.f, -12.f });
	m_ThrowItemIcon = CreateUI(L"ItemIcon_Hand", { 618.f, -12.f })->FindComponent<GJHSpriteRenderer>();
}

void UIManager::Update()
{
	BatteryUpdate();
	TimerUpdate();
}

void UIManager::BatteryUpdate()
{
	float Gauge = Player::m_BatteryGauge;

	for (int i = 0; i < 11; i++)
	{
		if (i + 0.5f <= Gauge)
		{
			m_BatteryList[i]->SetColor({ 1.f, 1.f, 1.f, 1.f });
		}
		else
		{
			m_BatteryList[i]->SetColor({ 1.f, 0.f, 0.f, 1.f });
		}
	}
}

void UIManager::TimerUpdate()
{
	float Ratio = Player::m_TimeGauge / 100.f;

	m_TimeGauge->SetWorldScaleX(m_TimeGaugeXScale * Ratio);
}