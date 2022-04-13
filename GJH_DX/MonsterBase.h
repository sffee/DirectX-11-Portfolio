#pragma once
#include "ObjectBase.h"

#define TILESIZE 16.f

class MonsterBase : public ObjectBase
{
protected:
	static int m_MonsterCount;

public:
	static void SetMonsterCount(int _Count)
	{
		m_MonsterCount = _Count;
	}

	static int GetMonsterCount()
	{
		return m_MonsterCount;
	}

protected:
	bool m_IsDamage;
	bool m_IsDamageFly;

protected:
	float m_PatrolTime;

protected:
	float m_DamageMoveTime;
	float4 m_KnockBackVec;

private:
	bool m_SlashImpactCreate;
	float m_SlashImpactAngle;

protected:
	bool m_ChangeDirection;
	float m_RenderColor;
	bool m_PlayDetectEffect;

protected:
	std::vector<std::pair<float4, int>> m_AStarPath;
	bool m_OnStairway;
	DIRECTION m_OnStairwayMoveDir;
	bool m_OnDoor;
	float4 m_DoorTeleportPos;

public:
	MonsterBase();
	~MonsterBase();

public:
	MonsterBase(const MonsterBase& _Other) = delete;
	MonsterBase(const MonsterBase&& _Other) = delete;

public:
	MonsterBase operator=(const MonsterBase& _Other) = delete;
	MonsterBase operator=(const MonsterBase&& _Other) = delete;

private:
	void StairwayDownMove();

protected:
	void Damage(float _Angle = 0.f);
	void DetectEffect();
	void CreateSlashSuccess();
	void CreateSlashImpact(float _Angle);
	void CreateDetectEffect();
	void KnockBack(float _Angle, float _MoveSpeed = 1300.f);
	void KnockBackUpdate(float _MoveSpeed = 400.f);

protected:
	void Start(std::shared_ptr<GJHSpriteRenderer> _Renderer, float4 _RenderScale);
	void Update() override;
	void AStarUpdate();
	void MovePosUpdate();
};