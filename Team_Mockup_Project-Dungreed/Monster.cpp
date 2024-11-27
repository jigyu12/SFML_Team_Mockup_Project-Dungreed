#include "stdafx.h"

Monster::Monster(const std::string& name)
	: Character(name), monsterType(MonsterType::None), originalDamage(0), attackAccumSpeed(0.f), attackSpeedDelay(0.f)
{
}