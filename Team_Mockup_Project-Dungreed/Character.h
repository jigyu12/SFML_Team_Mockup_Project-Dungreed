#pragma once

class Character : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;

	int hp;
	int maxhp;
	float speed;
	sf::Vector2f direction;
	
public:
	Character(const std::string& name = "");
	virtual ~Character() = default;

	virtual void SetPosition(const sf::Vector2f& pos) override = 0;
	virtual void SetRotation(float angle) override = 0;
	virtual void SetScale(const sf::Vector2f& scale) override = 0;

	virtual void SetOrigin(Origins preset) override = 0;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override = 0;

	virtual void Init() override = 0;
	virtual void Release() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Draw(sf::RenderWindow& window) override = 0;
	
};