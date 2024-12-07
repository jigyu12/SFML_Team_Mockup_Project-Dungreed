#pragma once

#include "SpriteGo.h"

class SkelBowArrow : public SpriteGo
{
protected:
	sf::Vector2f direction;

	float speed;

	int damage;

	Player* target;

	float idleTimeAccum;
	float idleTimeDelay;

public:
	SkelBowArrow(const std::string& name = "SkelBowArrow");
	virtual ~SkelBowArrow() = default;

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

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir);
};