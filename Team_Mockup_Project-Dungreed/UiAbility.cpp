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
	textDamage.setPosition({ 145,250 });


	textDashDamagePercent.setFont(font);
	textDashDamagePercent.setCharacterSize(fontsize);
	textDashDamagePercent.setFillColor(sf::Color::White);

	textArmor.setFont(font);
	textArmor.setCharacterSize(fontsize);
	textArmor.setFillColor(sf::Color::White);
	textArmor.setPosition({ 200,355 });

	textArmorPercent.setFont(font);
	textArmorPercent.setCharacterSize(fontsize);
	textArmorPercent.setFillColor(sf::Color::White);
	textArmorPercent.setPosition({ 480,460 });

	textCiritcalDamage.setFont(font);
	textCiritcalDamage.setCharacterSize(fontsize);
	textCiritcalDamage.setFillColor(sf::Color::White);
	textCiritcalDamage.setPosition({ 200 , 460 });

	textCirticalPercent.setFont(font);
	textCirticalPercent.setCharacterSize(fontsize);
	textCirticalPercent.setFillColor(sf::Color::White);
	textCirticalPercent.setPosition({ 465, 355 });

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


void UiAbility::Update(float dt)
{

}

void UiAbility::LateUpdate(float dt)
{
	SetAttackDamage(player->GetCurrentAttackDamage(), player->GetRealDamage());
	SetCriticalDamage(player->GetCurrentCriticalDamage());
	SetCriticalPercent(player->GetCurrentCriticalPercent());
	SetArmor(player->GetCurrentArmor());
	SetArmorPercent(player->GetCurrentArmorPercent());
}

void UiAbility::SetAttackDamage(int currentDamage, int maxDamage)
{
	textDamage.setString(std::to_string(currentDamage) + " ~ " + std::to_string(maxDamage));
	Utils::SetOrigin(textDamage, Origins::TL);
}



void UiAbility::SetCriticalDamage(int criticalDamage)
{
	textCiritcalDamage.setString(std::to_string(criticalDamage));
	Utils::SetOrigin(textDamage, Origins::TL);
}

void UiAbility::SetCriticalPercent(int criticalPercent)
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

void UiAbility::SetArmor(int armor)
{
	textArmor.setString(std::to_string(armor));
	Utils::SetOrigin(textArmor, Origins::TL);
}

void UiAbility::SetArmorPercent(int armorPercent)
{
	textArmorPercent.setString(std::to_string(armorPercent) + "%");
	Utils::SetOrigin(textArmorPercent, Origins::TL);
}

void UiAbility::SetDashDamagePercent(float dashDamage)
{
	textDashDamagePercent.setString(std::to_string(dashDamage) + "%");
	Utils::SetOrigin(textDashDamagePercent, Origins::TL);
}





void UiAbility::Draw(sf::RenderWindow& window)
{
	window.draw(abilityBase);
	window.draw(textDamage);
	window.draw(textCiritcalDamage);
	window.draw(textCirticalPercent);
	window.draw(textArmor);
	window.draw(textArmorPercent);
}

