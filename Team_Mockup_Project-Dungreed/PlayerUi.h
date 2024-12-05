#pragma once
#include "GameObject.h"


class PlayerUi : public GameObject
{
protected:
	sf::Vector2f maxHpSize = { 200.f,30.f };
	sf::RectangleShape maxHp;
	sf::Text textHp;
	sf::Text level;
	
	Player* player;
	//피통
	sf::Sprite playerLife;
	sf::Sprite playerLifeBackGround;
	sf::Sprite playerHpBar;
	//대쉬 이미지

	//대쉬 비워져있을때
	sf::Sprite playerDashFirstFrame;
	sf::Sprite playerDashSecondFrame;
	
	//대쉬 채워져있을때
	sf::Sprite playerDashFirstGauge;
	sf::Sprite playerDashSecondGauge;
	//
	


	std::string playerLifeBase = "graphics/ui/PlayerLifeBase 1.png";
	std::string playerLifeBack = "graphics/ui/PlayerLifeBack1.png";
	std::string playerRedBar = "graphics/ui/LifeBar.png";

	//대쉬 (비워져있을때)
	std::string playerDashBase0 = "graphics/ui/DashCountBase0.png";
	std::string playerDashBase2 = "graphics/ui/DashCountBase2.png";

	//대쉬 (채워져있을때 이미지)
	std::string playerDashBase01 = "graphics/ui/DashCount.png";
	std::string playerDashBase21 = "graphics/ui/DashCount.png";
	
	


	
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
	void SetLevel(int level);
};

