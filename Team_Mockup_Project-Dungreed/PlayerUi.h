#pragma once
#include "GameObject.h"
class PlayerUi : public GameObject
{
protected:
	sf::Vector2f maxHpSize = { 200.f,30.f };
	sf::RectangleShape maxHp;
	sf::Text textHp;
	
	Player* player;
	//����
	sf::Sprite PlayerLife;
	sf::Sprite PlayerLifeBackGround;
	sf::Sprite PlayerHpBar;
	//�뽬 �̹���

	//�뽬 �����������
	sf::Sprite PlayerDashFirstFrame;
	sf::Sprite PlayerDashSecondFrame;
	
	//�뽬 ä����������
	sf::Sprite PlayerDashFirstGauge;
	sf::Sprite PlayerDashSecondGauge;
	
	


	std::string PlayerLifeBase = "graphics/ui/PlayerLifeBase 1.png";
	std::string PlayerLifeBack = "graphics/ui/PlayerLifeBack1.png";
	std::string PlayerRedBar = "graphics/ui/LifeBar.png";

	//�뽬 (�����������)
	std::string PlayerDashBase0 = "graphics/ui/DashCountBase0.png";
	std::string playerDashBase2 = "graphics/ui/DashCountBase2.png";

	//�뽬 (ä���������� �̹���)
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

};

