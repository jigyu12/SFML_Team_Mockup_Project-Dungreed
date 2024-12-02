#include "stdafx.h"
#include "SkellBossLeftHand.h"

SkellBossLeftHand::SkellBossLeftHand(const std::string& name)
	: Monster(name)
{
}

void SkellBossLeftHand::SetOrigin(Origins preset)
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

void SkellBossLeftHand::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkellBossLeftHand::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void SkellBossLeftHand::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkellBossLeftHand::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkellBossLeftHand::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::SkellBoss;
}

void SkellBossLeftHand::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	hp = 1;
	speed = 30.f;
	originalDamage = 0;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 0.5f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);
	animator.Play("animations/Boss/SkellBoss Hand Idle.csv");

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	state = SkellBossLeftHandState::Idle;
}

void SkellBossLeftHand::Update(float dt)
{
	switch (state)
	{
	case SkellBossLeftHand::SkellBossLeftHandState::Idle:
	{
		UpdateIdle(dt);
	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::AttackLaser:
	{
		UpdateAttackLaser(dt);
	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::Death:
	{
		UpdateDeath(dt);
	}
		break;
	}

	animator.Update(dt);
}

void SkellBossLeftHand::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
}

void SkellBossLeftHand::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void SkellBossLeftHand::Release()
{
}

void SkellBossLeftHand::SetStatus(SkellBossLeftHandState state)
{
	this->state = state;

	switch (state)
	{
	case SkellBossLeftHand::SkellBossLeftHandState::Idle: 
	{

	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::AttackLaser:
	{

	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::Death:
	{

	}
		break;
	}
}

void SkellBossLeftHand::UpdateIdle(float dt)
{
}

void SkellBossLeftHand::UpdateAttackLaser(float dt)
{
}

void SkellBossLeftHand::UpdateDeath(float dt)
{
}