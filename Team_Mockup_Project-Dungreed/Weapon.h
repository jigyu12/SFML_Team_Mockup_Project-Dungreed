#pragma once

#include "Player.h"

class Weapon : public SpriteGo
{
public:
	enum class WeaponType
	{
		None = -1,

		Melee,
		Ranged,

		Count
	};

	struct CollisionState
	{
		bool Up = false;
		bool Down = false;
		bool Left = false;
		bool Right = false;
	};

protected:
	WeaponType weaponType;

	Player* owner;

	int originalDamageMin;
	int originalDamageMax;

	float attackSpeedAccumTime;
	float attackSpeedDelayTime;

	bool isCurrentWeapon = false;

	float velocityY = 0.f;
	float gravity = 98.f * 2.f;
	float jumpSpeed = -90.0f;
	bool isOnGround = false;

public:
	Weapon(const std::string& name = "Weapon");
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

	virtual sf::FloatRect GetLocalBounds() const 
	{
		return sprite.getLocalBounds();
	}

	virtual sf::FloatRect GetGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}
	
	WeaponType GetWeaponType() const { return weaponType; }
	int GetOriginalDamageMin() const { return originalDamageMin; }
	int GetOriginalDamageMax() const { return originalDamageMax; }
	float GetAttackSpeed() const { return attackSpeedDelayTime; }

	void SetOwnerPlayer(Player* player) { owner = player; }
	void SetIsCurrentWeapon(bool isCurrWeapon) { isCurrentWeapon = isCurrWeapon; }

	CollisionState GetCollsionState(const sf::FloatRect& weapon, const sf::FloatRect& stage);
};