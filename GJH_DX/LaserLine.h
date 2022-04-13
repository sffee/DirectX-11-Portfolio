#pragma once
#include "ObjectBase.h"

class Laser;
class LaserLine : public ObjectBase
{
public:
	enum class MODE
	{
		NONE,
		AUTOFIRE
	};

public:
	LaserLine();
	~LaserLine();

public:
	LaserLine(const LaserLine& _Other) = delete;
	LaserLine(const LaserLine&& _Other) = delete;

public:
	LaserLine operator=(const LaserLine& _Other) = delete;
	LaserLine operator=(const LaserLine&& _Other) = delete;

private:
	std::shared_ptr<Laser> m_Laser;

private:
	float m_Angle;
	float m_RenderYSize;
	bool m_IsEnd;
	float m_Time;
	float m_RenderSizeDeltaTime;
	
private:
	MODE m_Mode;

public:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;

public:
	void SetMode(MODE _Mode)
	{
		m_Mode = _Mode;
	}

public:
	void Attack(DIRECTION _Dir, float _RenderSizeDeltaTime = 3.5f);
	void SetDir(DIRECTION _Dir);
	void Setting(float4 _Pos, float _Angle);
	void LaserSetting();
	void LaserFire();
	void LaserFinish();
};