#pragma once
#include "GameObject.h"
#include "MainTitleUi.h"

class Player;
class Weapon;

class UiAbility : public GameObject
{
protected:

	sf::Sprite abilityBase;

	//텍스트

	sf::Text textDamage;
	sf::Text textCiritcalDamage;
	sf::Text textCirticalPercent;
	sf::Text textAttackSpeed;
	sf::Text textMovementSpeed;
	sf::Text textArmor;
	sf::Text textArmorPercent;
	sf::Text textDashDamagePercent;

	SavePlayerStatus playerStatus;

	Player* player;
	
public:

	UiAbility(const std::string& name = "uiAbility");
	~UiAbility() = default;

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

	//문자세팅
	void SetAttackDamage(int currentDamage, int maxDamage);
	
	void SetCriticalDamage(int criticalDamage);
	void SetCriticalPercent(int criticalPercent);
	void SetAttackSpeed(float attackSpeed);
	void SetMoveMentSpeed(int moveSpeed);
	void SetArmor(int armor);
	void SetArmorPercent(int armorPercent);
	void SetDashDamagePercent(int dashDamage);
};
/*
* float attackDamage;
	int level;
	float criticalDamage;
	float criticalPercent;
	float exp;
	float armor;
*/
