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
		Ghost,
		Banshee,

		SkellBoss,

		SkelBow,
		SkelSword,

		Count
	};

protected:
	MonsterType monsterType;

	Player* target;
	int originalDamage;
	
	float attackAccumSpeed;
	float attackSpeedDelay;

	bool isDamaged;
	bool isDead;

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
	int GetMaxHp() const { return maxhp; }
	int GetCurrentHp() const { return hp; }

	void OnDamaged(int damage);
	bool IsDead() const { return isDead; }
};