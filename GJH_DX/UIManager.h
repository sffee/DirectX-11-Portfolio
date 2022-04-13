#pragma once
#include "ObjectBase.h"

class UIManager : public ObjectBase
{


public:
	UIManager();
	~UIManager();

public:
	UIManager(const UIManager& _Other) = delete;
	UIManager(const UIManager&& _Other) = delete;

public:
	UIManager operator=(const UIManager& _Other) = delete;
	UIManager operator=(const UIManager&& _Other) = delete;

private:
	std::vector<std::shared_ptr<GJHSpriteRenderer>> m_BatteryList;
	std::shared_ptr<GJHSpriteRenderer> m_TimeGauge;
	std::shared_ptr<GJHSpriteRenderer> m_ThrowItemIcon;

private:
	float m_TimeGaugeXScale;

public:
	void Start() override;
	void Update() override;

private:
	void BatteryUpdate();
	void TimerUpdate();
};