#pragma once

#include "SpriteGo.h"

class SkellBoss;
class Player;

class SkellBossSword : public SpriteGo
{
public:
	enum class SkellBossSwordState
	{
		None = -1,

		Idle,
		Attack,
		Destroy,

		Count
	};

protected:
	SkellBossSwordState state;

	sf::Vector2f direction;

	float speed;

	int damage;

	SkellBoss* owner;
	Player* target;

	float idleTimeAccum;
	float idleTimeDelay;

	float attackTimeAccum;
	float attackTimeDelay;

	float destroyTimeAccum;
	float destroyTimeDealy;

	bool isFire;

public:
	SkellBossSword(const std::string& name = "SkellBossSword");
	virtual ~SkellBossSword() = default;

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

	virtual sf::FloatRect GetLocalBounds() const
	{
		return sprite.getLocalBounds();
	}

	virtual sf::FloatRect GetGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}

	void SetState(SkellBossSwordState state);
	void UpdateIdle(float dt);
	void UpdateAttack(float dt);
	void UpdateDestroy(float dt);

	void Fire(const sf::Vector2f& pos);
};