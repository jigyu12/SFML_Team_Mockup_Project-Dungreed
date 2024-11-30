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

	struct CollisionState
	{
		bool Up = false;
		bool Down = false;
		bool Left = false;
		bool Right = false;
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

	float attackMoveAccumTime;
	float attackMoveTimeDelay;
	bool isAttackMove;

	float hitAccumTime;
	float hitTimeDelay;

	float deathAccumTime;
	float deathTimeDelay;
	bool isDead;

	sf::RectangleShape detectionRange;

	sf::RectangleShape detectionLine;

	sf::FloatRect movableBound;

	sf::Shader shader;

	float velocityY;
	float gravity;
	float jumpSpeed;
	bool isOnGround;

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

	/// <summary>
	/// 각 Scene의 Scene::Enter(); 의 뒤에 사용해야 적용됨.
	/// </summary>
	void SetStartDirectionX(const float dirX)
	{
		direction = Utils::GetNormal({ dirX, 0.f });
		detectionLine.setPosition(body.getPosition() + direction * -10.f);
	}

	void Jump();

	CollisionState GetCollsionState(const sf::FloatRect& monster, const sf::FloatRect& stage);
};