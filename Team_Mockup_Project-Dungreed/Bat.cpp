#include "stdafx.h"
#include "Bat.h"

Bat::Bat(const std::string& name)
	: Monster(name)
{
}

void Bat::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Bat::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Bat::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Bat::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
		origin = Utils::SetOrigin(body, originPreset);
	}
	else
	{
		std::cerr << "Cannot assign Origins::Custom" << std::endl;
	}
}

void Bat::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Bat::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::Bat;
}

void Bat::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return; 
	}
	sortingOrder = target->sortingOrder - 1;
	direction = Utils::GetNormal(target->GetPosition() - position);
	

	hp = 6;
	speed = 300.f;
	originalDamage = 10;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 1.f;

	animator.SetTarget(&body);
	animator.Play("animations/Bat Idle.csv");
}

void Bat::Update(float dt)
{
	direction = Utils::GetNormal(target->GetPosition() - position);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normailize(direction);
	}
	sf::Vector2f newPosition = position + direction * speed * dt;
	SetPosition(newPosition);
	
	animator.Update(dt);

	hitbox.UpdateTr(body, GetLocalBounds());
}

void Bat::LateUpdate(float dt)
{
}

void Bat::Draw(sf::RenderWindow& window)
{
	window.draw(body);

	hitbox.Draw(window);
}

void Bat::Release()
{
}