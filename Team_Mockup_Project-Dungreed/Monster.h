#pragma once

#include "Player.h"

class Monster : public Character
{
public:
	enum class MonsterType
	{
		None = -1,

		SkeletonDog,
		Bat,

		Count
	};
protected:
	MonsterType monsterType;

	Player* target;
	int originalDamage;

	float attackAccumSpeed;
	float attackSpeedDelay;

public:
	Monster(const std::string& name = "");
	virtual ~Monster() = default;

	virtual void SetPosition(const sf::Vector2f& pos) override = 0;
	virtual void SetRotation(float angle) override = 0;
	virtual void SetScale(const sf::Vector2f& scale) override = 0;

	virtual void SetOrigin(Origins preset) override = 0;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override = 0;

	virtual void Init() override = 0;
	virtual void Release() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Draw(sf::RenderWindow& window) override = 0;

	MonsterType GetMonsterType() const { return monsterType; }
	int GetOriginalDamage() const { return originalDamage; }
};