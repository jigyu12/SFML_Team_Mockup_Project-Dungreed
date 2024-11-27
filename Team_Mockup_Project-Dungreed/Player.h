#pragma once

#include "Character.h"

class Player : public Character
{
public:
	enum class Status
	{
		Ground,
		Jump,

	};

	struct CollisionState
	{
		bool Up = false;
		bool Down = false;
		bool Left = false;
		bool Right = false;
	};

protected:
	Status status;
	sf::Sprite sword;

	sf::Vector2f velocity;
	sf::Vector2f look;
	std::string playerId = "graphics/player/CharWalk0.png";
	std::string swordId = "graphics/player/Sword.png";

	float gravity;
	float jumpForce;
	float jumpTimer;
public:
	Player(const std::string& name = "Player");
	virtual ~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	virtual sf::FloatRect GetLocalBounds() const 
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}

	Player::CollisionState GetCollsionState(const sf::FloatRect& player, const sf::FloatRect& stage);
	
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetStatus(Status status);
	void UpdateGrounded(float dt);
	void UpdateJump(float dt);
};
