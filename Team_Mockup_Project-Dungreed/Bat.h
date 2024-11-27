#pragma once

#include "Monster.h"

class Bat : public Monster
{
public:
	enum class BatState
	{
		None = -1,

		Idle,
		Move,
		//Attack,
		Death,

		Count
	};

protected:
	BatState state;

	float idleAccumTime;
	float idleTimeDelay;
	float idleRandMoveAccumTime;
	float idleRandMoveTimeDelay;
	bool isRandMoving;
	
	float hitAccumTime;
	float hitTimeDelay;
	bool isDamaged;

	float deathAccumTime;
	float deathTimeDelay;
	bool isDead;

	sf::CircleShape detectionRange;

	sf::FloatRect movableBound;

	sf::Shader shader;

public:
	Bat(const std::string& name = "Bat");
	virtual ~Bat() = default;

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

	void OnDamaged();
};