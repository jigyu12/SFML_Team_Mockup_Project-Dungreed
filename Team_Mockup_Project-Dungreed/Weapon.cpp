#include "stdafx.h"

Weapon::Weapon(const std::string& name)
	: SpriteGo(name), weaponType(WeaponType::None), owner(nullptr), originalDamageMin(0), originalDamageMax(0), attackSpeedAccumTime(0.f), attackSpeedDelayTime(0.f)
{
}

Weapon::CollisionState Weapon::GetCollsionState(const sf::FloatRect& weapon, const sf::FloatRect& stage)
{
	Weapon::CollisionState state;

	if (stage.top + stage.height > weapon.top
		&& weapon.top + weapon.height > stage.top + stage.height)
		state.Up = true;
	if (stage.left + stage.width > weapon.left
		&& weapon.left + weapon.width > stage.left + stage.width)
		state.Left = true;
	if (weapon.left + weapon.width > stage.left
		&& stage.left > weapon.left)
		state.Right = true;
	if (weapon.top + weapon.height > stage.top
		&& stage.top > weapon.top)
		state.Down = true;

	return state;
}