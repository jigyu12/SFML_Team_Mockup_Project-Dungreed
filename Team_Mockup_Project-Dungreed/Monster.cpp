#include "stdafx.h"

Monster::Monster(const std::string& name)
	: Character(name), monsterType(MonsterType::None), originalDamage(0), attackAccumSpeed(0.f), attackSpeedDelay(0.f)
{
	hpBack.setSize({ hpWidth, hpHeight });
	hpBack.setFillColor(sf::Color::Black);
	hpBack.setOrigin({ 0.f , 0.f });
	
	hpBarTexture.loadFromFile("graphics/ui/LifeBar.png");
	hpBar.setTexture(hpBarTexture);
	hpBar.setScale({ hpWidth * 0.85f, hpHeight / 10.f * 0.5f});
	hpBar.setOrigin({ 0.f , 0.f });
}

void Monster::OnDamaged(int damage)
{
	hp -= damage;
	SetHpUI(hp, maxhp);

	isDamaged = true;
}

void Monster::Update(float dt)
{
	if (ishpBarVisible)
	{
		hpBack.setPosition({ body.getPosition().x - hpBack.getSize().x / 2.f, body.getPosition().y });
		hpBar.setPosition({ hpBack.getPosition().x + hpBack.getSize().x / 2.f * 0.145f , hpBack.getPosition().y + hpBack.getSize().y / 2.f * 0.5f });
		
		hpBarVisibleTimeAccum += dt;
	}
}

void Monster::Draw(sf::RenderWindow& window)
{
	if (isDead)
		return;

	if (isDamaged)
	{
		ishpBarVisible = true;
	}

	if (ishpBarVisible)
	{
		if (hpBarVisibleTimeAccum < hpBarVisibleTimeDelay)
		{
			window.draw(hpBack);
			window.draw(hpBar);
		}
		else
		{
			hpBarVisibleTimeAccum = 0.f;
			ishpBarVisible = false;
		}
	}
}

void Monster::SetHpUI(int currentHp, int maxHp)
{
	hpBar.setScale({ hpWidth * Utils::Clamp01((float)currentHp / (float)maxHp) * 0.85f, hpHeight / 10.f * 0.5f });
	hpBar.setOrigin({ 0.f , 0.f });
}