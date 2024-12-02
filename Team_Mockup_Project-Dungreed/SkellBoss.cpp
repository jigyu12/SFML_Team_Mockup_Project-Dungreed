#include "stdafx.h"
#include "SkellBoss.h"

SkellBoss::SkellBoss(const std::string& name)
	: Monster(name)
{
}

void SkellBoss::SetOrigin(Origins preset)
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

void SkellBoss::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkellBoss::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void SkellBoss::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkellBoss::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkellBoss::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::SkellBoss;
}

void SkellBoss::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	hp = 80;
	speed = 30.f;
	originalDamage = 0;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 3.f;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);
	animator.Play("animations/Boss/SkellBoss Idle.csv");

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	state = SkellBossState::Idle;

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);
}

void SkellBoss::Update(float dt)
{
	switch (state)
	{
	case SkellBoss::SkellBossState::Idle: 
	{
		UpdateIdle(dt);
	}
		break;
	case SkellBoss::SkellBossState::AttackLaser:
	{
		UpdateAttackLaser(dt);
	}
		break;
	case SkellBoss::SkellBossState::AttackBullet:
	{
		UpdateAttackBullet(dt);
	}
		break;
	case SkellBoss::SkellBossState::AttackSword:
	{
		UpdateAttackSword(dt);
	}
		break;
	case SkellBoss::SkellBossState::Death:
	{
		UpdateDeath(dt);
	}
		break;
	}

	if (isDamaged && hp > 0)
	{
		hitAccumTime += dt;
		if (hitAccumTime >= hitTimeDelay)
		{
			isDamaged = false;
			hitAccumTime = 0.f;
		}
	}
	else if (isDamaged && hp <= 0)
	{
		isDead = true;
		state = SkellBossState::Death;
	}

	animator.Update(dt);

	hitbox.UpdateTr(body, GetLocalBounds());
}

void SkellBoss::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
}

void SkellBoss::Draw(sf::RenderWindow& window)
{
	if (!isDamaged && hp > 0)
	{
		window.draw(body);
	}
	else if (isDamaged && hp <= 0)
	{
		window.draw(body);
	}
	else
	{
		window.draw(body, &shader);
	}

	hitbox.Draw(window);
}

void SkellBoss::Release()
{
}

void SkellBoss::SetStatus(SkellBossState state)
{
	this->state = state;

	switch (state)
	{
	case SkellBoss::SkellBossState::Idle: 
	{

	}
		break;
	case SkellBoss::SkellBossState::AttackLaser:
	{

	}
		break;
	case SkellBoss::SkellBossState::AttackBullet:
	{

	}
		break;
	case SkellBoss::SkellBossState::AttackSword:
	{

	}
		break;
	case SkellBoss::SkellBossState::Death:
	{

	}
		break;
	}
}

void SkellBoss::UpdateIdle(float dt)
{
}

void SkellBoss::UpdateAttackLaser(float dt)
{
}

void SkellBoss::UpdateAttackBullet(float dt)
{
}

void SkellBoss::UpdateAttackSword(float dt)
{
}

void SkellBoss::UpdateDeath(float dt)
{
}