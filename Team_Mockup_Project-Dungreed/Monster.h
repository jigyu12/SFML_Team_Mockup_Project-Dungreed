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

	bool isDamaged;

public:
	Monster(const std::string& name = "");
	virtual ~Monster() = default;

	virtual void SetPosition(const sf::Vector2f& pos) override {};
	virtual void SetRotation(float angle) override {};
	virtual void SetScale(const sf::Vector2f& scale) override {};

	virtual void SetOrigin(Origins preset) override {};
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override {};

	virtual void Init() override {};
	virtual void Release() override {};
	virtual void Reset() override {};
	virtual void Update(float dt) override {};
	virtual void Draw(sf::RenderWindow& window) override {};

	MonsterType GetMonsterType() const { return monsterType; }
	int GetOriginalDamage() const { return originalDamage; }

	void OnDamaged(int damage);
};