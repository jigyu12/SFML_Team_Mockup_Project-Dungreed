#pragma once

class PortalEffect : public GameObject
{
protected:
	sf::Sprite body;

	sf::Vector2f velocity;
	sf::Vector2f startpoint;
	int dir;
	sf::FloatRect bounds;
public:
	PortalEffect(const std::string& name = "");
	~PortalEffect() = default;

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

	void Set(const sf::FloatRect& bounds, int dir);
	void Set(const sf::FloatRect& bounds);
	int GetDir() const { return dir; }
};


