#include "stdafx.h"
#include "PlayerUi.h"
#include "UiAbility.h"



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

	level.setFont(font);
	level.setCharacterSize(fontsize);
	level.setFillColor(sf::Color::Black);
	SetLevel(player->GetCurrentLevel());
	level.setPosition({ 65.f,18.f });
	

	playerLife.setTexture(TEXTURE_MGR.Get(playerLifeBase));
	playerLifeBackGround.setTexture(TEXTURE_MGR.Get(playerLifeBack));
	playerHpBar.setTexture(TEXTURE_MGR.Get(playerRedBar));
	//채워진거
	playerDashFirstFrame.setTexture(TEXTURE_MGR.Get(playerDashBase0));
	playerDashSecondFrame.setTexture(TEXTURE_MGR.Get(playerDashBase2));
	//안채워진거
	playerDashFirstGauge.setTexture(TEXTURE_MGR.Get(playerDashBase01));
	playerDashSecondGauge.setTexture(TEXTURE_MGR.Get(playerDashBase21));




	//대쉬 (사용전)

	playerDashFirstFrame.setPosition({ 5.f,95.f });
	playerDashFirstFrame.setScale({ 6.f,5.5f });

	playerDashSecondFrame.setPosition({ 70.f,95.f });
	playerDashSecondFrame.setScale({ 6.f,5.5f });

	//대쉬 (사용후)
	
	playerDashFirstGauge.setPosition({ 5.f+12.f,95.f + 10.5f });
	playerDashFirstGauge.setScale({ 6.f,5.5f });

	playerDashSecondGauge.setPosition({ 70.f+6.f,95.f + 10.5f });
	playerDashSecondGauge.setScale({ 6.f,5.5f });



	//플레이어 hp바
	playerLifeBackGround.setPosition({ 5.f,5.f });
	playerLifeBackGround.setScale({ 6.f,5.5f });

	playerLife.setPosition({ 5.f,5.f });
	playerLife.setScale({ 6.f,5.5f });

	playerHpBar.setPosition({ 137.f,19.f });
	playerHpBar.setScale({ 300.f,6.f });

	textHp.setPosition({ 200.f,55.f });

	SetHp(player->GetCurrentHp(), player->GetMaxHp());
	level.setPosition({ 65.f,18.f });
	Utils::SetOrigin(level, Origins::TL);



	
}

void PlayerUi::Update(float dt)
{
	float dashTime = player->GetCurrentCoolTime();
	if (dashTime < 1.f)
	{
		playerDashFirstGauge.setScale({ 6.f* dashTime,5.5f });
		playerDashFirstGauge.setColor({ 100,100,100 });
	}
	if (dashTime >= 1.f
		&& dashTime < 2.f)
	{
		playerDashFirstGauge.setScale({ 6.f,5.5f });
		playerDashFirstGauge.setColor(sf::Color::White);
		playerDashSecondGauge.setScale({ 6.f*(dashTime-1),5.5f });
		playerDashSecondGauge.setColor({ 100,100,100 });
	}
	if (dashTime >= 2.f)
	{
		playerDashFirstGauge.setScale({ 6.f,5.5f });
		playerDashFirstGauge.setColor(sf::Color::White);
		playerDashSecondGauge.setScale({ 6.f ,5.5f });
		playerDashSecondGauge.setColor(sf::Color::White);
	}

	SetLevel(player->GetCurrentLevel());
	
	playerHpBar.setScale(300.f * player->GetCurrentHpRatio(), playerHpBar.getScale().y);
	
}

void PlayerUi::Draw(sf::RenderWindow& window)
{
	window.draw(playerLifeBackGround);
	window.draw(playerHpBar);
	window.draw(playerLife);
	window.draw(textHp);
	window.draw(playerDashFirstFrame);
	window.draw(playerDashSecondFrame);
	window.draw(playerDashFirstGauge);
	if (player->GetCurrentCoolTime() >= 1.f)
	{
		window.draw(playerDashSecondGauge);
	}
	window.draw(level);

	
}

void PlayerUi::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	//maxHp.setSize({ maxHpSize.x * value,maxHpSize.y });
	textHp.setString(std::to_string(hp) + " / " + std::to_string(max));
	Utils::SetOrigin(textHp, Origins::BL);
}

void PlayerUi::SetLevel(int level)
{
	this->level.setString(std::to_string(level));
	Utils::SetOrigin(this->level, Origins::TL);
}