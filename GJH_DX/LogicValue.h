#pragma once

class LogicValue
{
public:
	enum class LogicCollisionLayer
	{
		Player,
		PlayerSlash,
		Monster,
		MonsterAttack,
		MonsterBullet,
		Gate,
		Door,
		DoorAttack,
		MonsterShield,
		MoveBlockObject,
		HeadHunter,
		HeadHunterLaser,
		HeadHunterGrenadeExplosion,
		PlayerFake,
		PlayerBullet,
		MonsterShieldAttack
	};

private:
	LogicValue() { }
	~LogicValue() { }
};

using CollisionLayer = LogicValue::LogicCollisionLayer;