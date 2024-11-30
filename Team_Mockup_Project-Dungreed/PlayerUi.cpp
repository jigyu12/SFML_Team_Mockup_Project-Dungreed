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
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	float fontsize = 50.f;
	sf::Font& font = FONT_MGR.Get("fonts/french.ttf");
	textHp.setFont(font);
	textHp.setCharacterSize(fontsize);
	textHp.setFillColor(sf::Color::Black);
	Utils::SetOrigin(textHp, Origins::TL);

	PlayerLife.setTexture(TEXTURE_MGR.Get(PlayerLifeBase));
	PlayerLifeBackGround.setTexture(TEXTURE_MGR.Get(PlayerLifeBack));

	PlayerLifeBackGround.setOrigin(Utils::SetOrigin(PlayerLife, Origins::TL));
	PlayerLifeBackGround.setPosition({ 5.f,5.f });
	PlayerLifeBackGround.setScale({ 6.f,5.5f });

	PlayerLife.setOrigin(Utils::SetOrigin(PlayerLife, Origins::TL));
	PlayerLife.setPosition({ 5.f,5.f });
	PlayerLife.setScale({ 6.f,5.5f });

	textHp.setPosition({ 200.f,55.f });
	SetHp(player->GetHp(), maxhp);
}

void PlayerUi::Update(float dt)
{
}

void PlayerUi::Draw(sf::RenderWindow& window)
{
	window.draw(PlayerLifeBackGround);
	window.draw(PlayerLife);
	window.draw(textHp);
}

void PlayerUi::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	maxHp.setSize({ maxHpSize.x * value,maxHpSize.y });
	textHp.setString(std::to_string(hp) + " / " + std::to_string(max));
	Utils::SetOrigin(textHp, Origins::BL);
}