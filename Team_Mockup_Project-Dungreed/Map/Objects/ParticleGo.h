#pragma once

class Room;

class ParticleGo : public GameObject
{
protected:

	sf::Sprite body;
	
	sf::Vector2f velocity;
	sf::Vector2f gravity;
	float rotationSpeed;

	float timer;

	std::function<void()> returnThis;
public:
	ParticleGo(const std::string& name = "");
	~ParticleGo() = default;

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

	void Start(const std::string& name,const sf::Vector2f& position);

	void SetReturnThis(std::function<void()> func) { returnThis = func; }
};
