#pragma once

class Monster : public Character
{
protected:
	// Player target;
	int originalDamage;


public:
	Monster(const std::string& name = "");
	virtual ~Monster() = default;

	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetRotation(float angle) override;
	virtual void SetScale(const sf::Vector2f& scale) override;

	virtual void SetOrigin(Origins preset) override;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override;

	virtual void Init() override = 0;
	virtual void Release() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Draw(sf::RenderWindow& window) override = 0;
};