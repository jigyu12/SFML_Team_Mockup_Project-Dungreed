#pragma once

#include "Weapon.h"

class ShortSword : public Weapon
{
protected:
	sf::Vector2f look;

	bool isUp;

	Animator animatorFx;
	sf::Sprite swordSwingFx;

public:
	ShortSword(const std::string& name = "ShortSword");
	virtual ~ShortSword() = default;

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
};