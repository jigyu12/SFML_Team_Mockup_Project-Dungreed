#include "stdafx.h"
#include "PlayerUi.h"



PlayerUi::PlayerUi(const std::string& name)
	:GameObject(name)
{
}

void PlayerUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	SetPosition(position);
}

void PlayerUi::SetRotation(float angle)
{
	rotation = angle;
	SetRotation(rotation);
}

void PlayerUi::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	SetScale(this->scale);
}

void PlayerUi::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void PlayerUi::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void PlayerUi::Init()
{
	sortingLayer = SortingLayers::UI;
}

void PlayerUi::Release()
{
}

void PlayerUi::Reset()
{
	float fontsize = 9.f;
	sf::Font& font = FONT_MGR.Get("Font/french.ttf");
	textHp.setFont(font);
	textHp.setCharacterSize(fontsize);
	textHp.setFillColor(sf::Color::Black);
	Utils::SetOrigin(textHp, Origins::TL);

	PlayerLife.setTexture(TEXTURE_MGR.Get(PlayerLifeBase));
	PlayerLifeBackGround.setTexture(TEXTURE_MGR.Get(PlayerLifeBack));


	PlayerLife.setOrigin(Utils::SetOrigin(PlayerLife, Origins::TL));
	PlayerLife.setPosition({ -158.f,-88.f });

	textHp.setPosition({ -130.f,-80.f });
	SetHp(90, 90);
}

void PlayerUi::Update(float dt)
{
}

void PlayerUi::Draw(sf::RenderWindow& window)
{
	
}

void PlayerUi::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	maxHp.setSize({ maxHpSize.x * value,maxHpSize.y });
	textHp.setString(std::to_string(hp) + " / " + std::to_string(max));
	Utils::SetOrigin(textHp, Origins::BL);
}