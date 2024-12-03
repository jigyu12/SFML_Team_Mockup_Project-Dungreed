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
	PlayerHpBar.setTexture(TEXTURE_MGR.Get(PlayerRedBar));
	//채워진거
	PlayerDashFirstFrame.setTexture(TEXTURE_MGR.Get(PlayerDashBase0));
	PlayerDashSecondFrame.setTexture(TEXTURE_MGR.Get(playerDashBase2));
	//안채워진거
	PlayerDashFirstGauge.setTexture(TEXTURE_MGR.Get(playerDashBase01));
	PlayerDashSecondGauge.setTexture(TEXTURE_MGR.Get(playerDashBase21));




	//대쉬 (사용전)

	PlayerDashFirstFrame.setPosition({ 5.f,95.f });
	PlayerDashFirstFrame.setScale({ 6.f,5.5f });

	PlayerDashSecondFrame.setPosition({ 70.f,95.f });
	PlayerDashSecondFrame.setScale({ 6.f,5.5f });

	//대쉬 (사용후)
	
	PlayerDashFirstGauge.setPosition({ 5.f+12.f,95.f + 10.5f });
	PlayerDashFirstGauge.setScale({ 6.f,5.5f });

	PlayerDashSecondGauge.setPosition({ 70.f+6.f,95.f + 10.5f });
	PlayerDashSecondGauge.setScale({ 6.f,5.5f });



	//플레이어 hp바
	PlayerLifeBackGround.setPosition({ 5.f,5.f });
	PlayerLifeBackGround.setScale({ 6.f,5.5f });

	PlayerLife.setPosition({ 5.f,5.f });
	PlayerLife.setScale({ 6.f,5.5f });

	PlayerHpBar.setPosition({ 137.f,19.f });
	PlayerHpBar.setScale({ 300.f,6.f });

	textHp.setPosition({ 200.f,55.f });
	SetHp(player->GetCurrentHp(), player->GetMaxHp());



	sf::FloatRect playerBar = PlayerLife.getGlobalBounds();
}

void PlayerUi::Update(float dt)
{
	float dashTime = player->GetCurrentCoolTime();
	if (dashTime < 1.f)
	{
		PlayerDashFirstGauge.setScale({ 6.f* dashTime,5.5f });
		PlayerDashFirstGauge.setColor({ 100,100,100 });
	}
	if (dashTime >= 1.f
		&& dashTime < 2.f)
	{
		PlayerDashFirstGauge.setScale({ 6.f,5.5f });
		PlayerDashFirstGauge.setColor(sf::Color::White);
		PlayerDashSecondGauge.setScale({ 6.f*(dashTime-1),5.5f });
		PlayerDashSecondGauge.setColor({ 100,100,100 });
	}
	if (dashTime >= 2.f)
	{
		PlayerDashFirstGauge.setScale({ 6.f,5.5f });
		PlayerDashFirstGauge.setColor(sf::Color::White);
		PlayerDashSecondGauge.setScale({ 6.f ,5.5f });
		PlayerDashSecondGauge.setColor(sf::Color::White);
	}

	
		PlayerHpBar.setScale(300.f * player->GetCurrentHpRatio(), PlayerHpBar.getScale().y);
	
}

void PlayerUi::Draw(sf::RenderWindow& window)
{
	window.draw(PlayerLifeBackGround);
	window.draw(PlayerHpBar);
	window.draw(PlayerLife);
	window.draw(textHp);
	window.draw(PlayerDashFirstFrame);
	window.draw(PlayerDashSecondFrame);
	window.draw(PlayerDashFirstGauge);
	if (player->GetCurrentCoolTime() >= 1.f)
	{
		window.draw(PlayerDashSecondGauge);
	}
	
}

void PlayerUi::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	//maxHp.setSize({ maxHpSize.x * value,maxHpSize.y });
	textHp.setString(std::to_string(hp) + " / " + std::to_string(max));
	Utils::SetOrigin(textHp, Origins::BL);
}