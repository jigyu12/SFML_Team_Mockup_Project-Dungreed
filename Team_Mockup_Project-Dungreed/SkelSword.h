#pragma once

#include "Monster.h"

class SkelSword : public Monster
{
public:
	enum class SkelSwordState
	{
		None = -1,

		Idle,
		Move,
		Attack,
		Death,

		Count
	};

protected:
	SkelSwordState state;

	Animator animator;

	sf::Shader shader;

	sf::Vector2f direction;

	sf::RectangleShape detectionRange;
	sf::RectangleShape detectionLine;
	HitBox detectionLineHitbox;

	float hitTimeAccum;
	float hitTimeDelay;

	float idleTimeAccum;
	float idleTimeDelay;

	float moveTimeAccum;
	float moveTimeDelay;

	float beforeAttackTimeAccum;
	float beforeAttackTimeDelay;
	bool isBeforeAttack;
	float attackTimeAccum;
	float attackTimeDelay;
	bool isAttack;

	float deathTimeAccum;
	float deathTimeDelay;

	float gravity;

	sf::Sprite sword;
	Animator animatorSword;

	sf::Texture handTexture;
	sf::Sprite hand1;
	sf::Sprite hand2;

	sf::RectangleShape attackBound;
	HitBox attackBoundHitbox;

public:
	SkelSword(const std::string& name = "SkelSword");
	virtual ~SkelSword() = default;

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

	void SetState(SkelSwordState state);
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

	void Attack();
};