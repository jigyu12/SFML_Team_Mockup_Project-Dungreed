#pragma once

#include "SpriteGo.h"

class Player;

class SkellBossLaser : public SpriteGo
{
protected:
	sf::Vector2f direction;

	int damage;

	Player* target;

	Animator animator;

	std::vector<sf::Sprite*> bodys;
	std::vector <Animator*> bodyAnimators;
	std::vector <HitBox*> bodyHitBoxes;
	int numberOfBodys;

	float idleTimeAccum;
	float idleTimeDelay;

	bool isFire;

public:
	SkellBossLaser(const std::string& name = "SkellBossLaser");
	virtual ~SkellBossLaser() = default;

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

	void Fire(sf::Vector2f pos, const sf::Vector2f& s);
};