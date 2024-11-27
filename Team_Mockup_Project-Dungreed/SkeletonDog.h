#pragma once

#include "Monster.h"

class SkeletonDog : public Monster
{
public:
	enum class SkeleDogState
	{
		None = -1,

		Idle,
		Move,
		Attack,
		Death,

		Count
	};

protected:
	SkeleDogState state;

	float idleAccumTime;
	float idleTimeDelay;
	bool isIdle;

	float moveAccumTime;
	float moveTimeDelay;
	bool isMoving;

	float attackAccumTime;
	float attackTimeDelay;
	bool isAttack;

	float hitAccumTime;
	float hitTimeDelay;
	bool isDamaged;

	float deathAccumTime;
	float deathTimeDelay;
	bool isDead;

	sf::RectangleShape detectionRange;

	sf::FloatRect movableBound;

	sf::Shader shader;

public:
	SkeletonDog(const std::string& name = "SkeletonDog");
	virtual ~SkeletonDog() = default;

	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetRotation(float angle) override;
	virtual void SetScale(const sf::Vector2f& scale) override;

	virtual void SetOrigin(Origins preset) override;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void LateUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual sf::FloatRect GetLocalBounds() const
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}

	void Attack();
	void OnDamaged();
	void Jump();
};