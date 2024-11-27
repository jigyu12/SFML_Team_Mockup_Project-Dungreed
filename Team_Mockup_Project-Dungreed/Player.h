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

protected:
	Status status;

	sf::Vector2f velocity;
	sf::Vector2f gravity = { 0.f,300.f };
	sf::Vector2f look;
	std::string playerId = "Dungreed Resources/Sprite/CharWalk0.png";

	float jumpForce;
public:
	Player(const std::string& name = "Player");
	virtual ~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

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

