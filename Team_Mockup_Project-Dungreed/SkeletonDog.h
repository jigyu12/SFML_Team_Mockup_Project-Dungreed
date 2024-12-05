#pragma once

#include "Monster.h"

class SkeletonDog : public Monster
{
public:
	enum class SkeletonDogState
	{
		None = -1,

		Idle,
		Move,
		Attack,
		Death,

		Count
	};

protected:
	SkeletonDogState state;

	Animator animator;

	sf::Shader shader;

	sf::Vector2f direction;
	float speed;

	float gravity;
	float velocityY;

	sf::RectangleShape detectionRange;
	sf::RectangleShape detectionLine;
	HitBox detectionLineHitbox;

	float hitTimeAccum;
	float hitTimeDelay;

	float idleTimeAccum;
	float idleTimeDelay;

	float moveTimeAccum;
	float moveTimeDelay;

	float attackMoveTimeAccum;
	float attackMoveTimeDelay;
	bool isAttackMove;

	float deathTimeAccum;
	float deathTimeDelay;

	float testA = 0.f;
	float testB = 0.2f;

public:
	SkeletonDog(const std::string& name = "SkeletonDog");
	virtual ~SkeletonDog() = default;

	virtual void SetOrigin(Origins preset) override;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override;
	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetRotation(float angle) override;
	virtual void SetScale(const sf::Vector2f& scale) override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void LateUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	void SetState(SkeletonDogState state);
	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateAttack(float dt);
	void UpdateDeath(float dt);

	virtual sf::FloatRect GetLocalBounds() const
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}
};