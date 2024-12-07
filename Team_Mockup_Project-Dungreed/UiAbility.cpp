#include "stdafx.h"
#include "UiAbility.h"
#include "MainTitleUi.h"
#include "Player.h"

UiAbility::UiAbility(const std::string& name)
	:GameObject(name)
{
}

void UiAbility::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiAbility::SetRotation(float angle)
{
	rotation = angle;
}

void UiAbility::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void UiAbility::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void UiAbility::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void UiAbility::Init()
{
	sortingLayer = SortingLayers::UI;
	
}

void UiAbility::Release()
{
}



void UiAbility::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	float fontsize = 50.f;
	sf::Font& font = FONT_MGR.Get("fonts/french.ttf");

	textDamage.setFont(font);
	textDamage.setCharacterSize(fontsize);
	textDamage.setFillColor(sf::Color::White);

	textDashDamagePercent.setFont(font);
	textDashDamagePercent.setCharacterSize(fontsize);
	textDashDamagePercent.setFillColor(sf::Color::White);

	textArmor.setFont(font);
	textArmor.setCharacterSize(fontsize);
	textArmor.setFillColor(sf::Color::White);

	textArmorPercent.setFont(font);
	textArmorPercent.setCharacterSize(fontsize);
	textArmorPercent.setFillColor(sf::Color::White);

	textCiritcalDamage.setFont(font);
	textCiritcalDamage.setCharacterSize(fontsize);
	textCiritcalDamage.setFillColor(sf::Color::White);

	textCirticalPercent.setFont(font);
	textCirticalPercent.setCharacterSize(fontsize);
	textCirticalPercent.setFillColor(sf::Color::White);


	textMovementSpeed.setFont(font);
	textMovementSpeed.setCharacterSize(fontsize);
	textMovementSpeed.setFillColor(sf::Color::White);

	textAttackSpeed.setFont(font);
	textAttackSpeed.setCharacterSize(fontsize);
	textAttackSpeed.setFillColor(sf::Color::White);
	

	abilityBase.setTexture(TEXTURE_MGR.Get("graphics/ui/playerAbility.png"));
	sf::Vector2f pos = FRAMEWORK.GetWindowSizeF();
	float baseXPos = 330.f;
	float baseYpos = (pos.y / 2) - 120.f;

	abilityBase.setPosition({ baseXPos ,baseYpos });
	Utils::SetOrigin(abilityBase, Origins::MC);


	

}

void UiAbility::SetAttackDamage(float currentDamage, float maxDamage)
{
	/*if(player->GetCurrentWeapon() == )*/
	currentDamage == player->GetCurrentAttackDamage();
	maxDamage = player->GetRealSwordMaxDamage();
	textDamage.setString(std::to_string(currentDamage) + "~" + std::to_string(maxDamage));
	Utils::SetOrigin(textDamage, Origins::TL);
}

void UiAbility::SetCriticalDamage(float criticalDamage)
{
	textCiritcalDamage.setString(std::to_string(criticalDamage));
	Utils::SetOrigin(textDamage, Origins::TL);
}

void UiAbility::SetCriticalPercent(float criticalPercent)
{
	textCirticalPercent.setString(std::to_string(criticalPercent) + "%");
	Utils::SetOrigin(textCirticalPercent, Origins::TL);
}

void UiAbility::SetAttackSpeed(float attackSpeed)
{
	textAttackSpeed.setString(std::to_string(attackSpeed));
	Utils::SetOrigin(textAttackSpeed, Origins::TL);
}

void UiAbility::SetMoveMentSpeed(float moveSpeed)
{
	textMovementSpeed.setString(std::to_string(moveSpeed));
	Utils::SetOrigin(textMovementSpeed, Origins::TL);
}

void UiAbility::SetArmor(float armor)
{
	textArmor.setString(std::to_string(armor));
	Utils::SetOrigin(textArmor, Origins::TL);
}

void UiAbility::SetArmorPercent(float armorPercent)
{
	textArmorPercent.setString(std::to_string(armorPercent) + "%");
	Utils::SetOrigin(textArmorPercent, Origins::TL);
}

void UiAbility::SetDashDamagePercent(float dashDamage)
{
	textDashDamagePercent.setString(std::to_string(dashDamage) + "%");
	Utils::SetOrigin(textDashDamagePercent, Origins::TL);
}




void UiAbility::Update(float dt)
{

}

void UiAbility::Draw(sf::RenderWindow& window)
{
	window.draw(abilityBase);
}

