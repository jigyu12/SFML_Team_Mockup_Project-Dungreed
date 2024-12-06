#pragma once
#include "GameObject.h"
#include "MainTitleUi.h"

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



public:

	UiAbility(const std::string& name = "playerUi");
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
	void Draw(sf::RenderWindow& window) override;

	//문자세팅
	void SetAttackDamage(float currentDamage, float maxDamage);
	void SetCriticalDamage(float criticalDamage);
	void SetCriticalPercent(float criticalPercent);
	void SetAttackSpeed(float attackSpeed);
	void SetMoveMentSpeed(float moveSpeed);
	void SetArmor(float armor);
	void SetArmorPercent(float armorPercent);
	void SetDashDamagePercent(float dashDamage);
};
/*
* float attackDamage;
	int level;
	float criticalDamage;
	float criticalPercent;
	float exp;
	float armor;
*/
