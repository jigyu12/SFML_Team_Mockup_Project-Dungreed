#pragma once

class LightGo : public GameObject
{
protected:

	sf::VertexArray vaLight;
	sf::Transform transform;

	sf::RenderStates rs;

	float radius;
	sf::Color centercolor;

public:
	LightGo(const std::string& name = "");
	~LightGo() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetRadius(float radius);
	void SetCenterColor(const sf::Color& color);
	void UpdateTransform();
};
