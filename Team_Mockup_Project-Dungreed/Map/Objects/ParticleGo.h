#pragma once

class Room;

class ObjectParticle : public GameObject
{
protected:

	sf::Sprite body;
	
	sf::Vector2f velocity;
	sf::Vector2f gravity;
	float speed;

	std::function<void()> returnThis;
public:
	ObjectParticle(const std::string& name = "");
	~ObjectParticle() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Start(const std::string& name);

	void SetReturnThis(std::function<void()> func) { returnThis = func; }
};
