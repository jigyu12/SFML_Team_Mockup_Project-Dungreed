#pragma once

#include "Player.h"

class Weapon : public GameObject
{
public:
	enum class WeaponType
	{
		None = -1,

		Melee,
		Ranged,

		Count
	};

protected:
	WeaponType weaponType;

	Player* owner;

	int originalDamage;

	float attackSpeedAccumTime;
	float attackSpeedDelayTime;

public:
	Weapon(const std::string& name = "");
	virtual ~Weapon() = default;

	virtual void SetOrigin(Origins preset) override = 0;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override = 0;
	virtual void SetPosition(const sf::Vector2f& pos) override = 0;
	virtual void SetRotation(float angle) override = 0;
	virtual void SetScale(const sf::Vector2f& scale) override = 0;

	virtual void Init() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Draw(sf::RenderWindow& window) override = 0;
	virtual void Release() override = 0;
	
	WeaponType GetWeaponType() const { return weaponType; }
	int GetOriginalDamage() const { return originalDamage; }
	float GetAttackSpeedDelay() const { return attackSpeedDelayTime; }
};