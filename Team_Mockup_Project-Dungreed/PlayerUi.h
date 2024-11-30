#pragma once
#include "GameObject.h"
class PlayerUi : public GameObject
{
protected:
	sf::Vector2f maxHpSize = { 200.f,30.f };
	sf::RectangleShape maxHp;
	sf::Text textHp;
	Player* player;

	sf::Sprite PlayerLife;
	sf::Sprite PlayerLifeBackGround;

	std::string PlayerLifeBase = "graphics/ui/PlayerLifeBase 1.png";
	std::string PlayerLifeBack = "graphics/ui/PlayerLifeBack1.png";
	int maxhp = 90;
	
public:
	PlayerUi(const std::string& name = "playerUi");
	~PlayerUi() = default;

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
	void SetHp(int hp, int max);
	int GetMaxHp() const { return maxhp; }
};

