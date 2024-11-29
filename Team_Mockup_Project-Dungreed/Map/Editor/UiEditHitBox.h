#pragma once

class UiEditHitBox : public GameObject
{
protected:

	std::list<sf::RectangleShape> hitbox;

public:
	UiEditHitBox(const std::string& name = "");
	~UiEditHitBox() = default;

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
};
