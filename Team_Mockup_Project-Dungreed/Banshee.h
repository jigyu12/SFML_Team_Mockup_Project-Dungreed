#pragma once

#include "Monster.h"
#include "BansheeBullet.h"

class Banshee : public Monster
{
public:
	enum class BansheeState
	{
		None = -1,

		Idle,
		Attack,
		Death,

		Count
	};

protected:
	BansheeState state;

	Animator animator;

	sf::Shader shader;

	sf::Vector2f direction;

	sf::CircleShape detectionRange;

	float hitTimeAccum;
	float hitTimeDelay;

	float idleTimeAccum;
	float idleTimeDelay;

	float attackTimeAccum;
	float attackTimeDelay;

	float deathTimeAccum;
	float deathTimeDelay;

	ObjectPool<BansheeBullet> bulletPool;

	int shootBulletNumber;

public:
	Banshee(const std::string& name = "Banshee");
	virtual ~Banshee() = default;

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

	void SetState(BansheeState state);
	void UpdateIdle(float dt);
	void UpdateRandMove(float dt);
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

	void Shoot();
};