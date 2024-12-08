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
	speed = 10.f;
	originalDamage = 0;

	attackSpeedDelay = 1.3f;
	deathTimeDelay = 1.f;
	targetFindTimeDelay = 0.5f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	SetState(SkellBossLeftHandState::Idle);

	isShoot = false;
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

void SkellBossLeftHand::SetState(SkellBossLeftHandState state)
{
	this->state = state;

	switch (state)
	{
	case SkellBossLeftHand::SkellBossLeftHandState::Idle: 
	{
		animator.Play("animations/Boss/SkellBoss Hand Idle.csv");
	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::AttackLaser:
	{
		attackAccumSpeed = 0.f;
		targetFindTimeAccum = 0.f;

		isShoot = false;

		animator.Play("animations/Boss/SkellBoss Hand Attack.csv");
	}
		break;
	case SkellBossLeftHand::SkellBossLeftHandState::Death:
	{
		deathTimeAccum = 0.f;
		animator.Stop();
	}
		break;
	}
}

void SkellBossLeftHand::UpdateIdle(float dt)
{
	if (state == SkellBossLeftHandState::Death)
		return;

	if (state == SkellBossLeftHandState::AttackLaser)
		return;
}

void SkellBossLeftHand::UpdateAttackLaser(float dt)
{
	if (state == SkellBossLeftHandState::Death)
	{
		for (auto& laser : lasers)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(laser);
		}

		return;
	}

	targetFindTimeAccum += dt;
	if (targetFindTimeAccum < targetFindTimeDelay)
	{
		SetPosition({ position.x, position.y -= (position.y - target->GetPosition().y) * speed * dt});

		return;
	}

	attackAccumSpeed += dt;

	if (!isShoot && attackAccumSpeed > attackSpeedDelay * 4.f / 10.f)
	{
		isShoot = true;

		ShootLaser();
	}

	if (attackAccumSpeed >= attackSpeedDelay)
	{
		SetState(SkellBossLeftHandState::Idle);
	}
}

void SkellBossLeftHand::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum > deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}

void SkellBossLeftHand::ShootLaser()
{
	SOUND_MGR.PlaySfx("sound/Sfx/boss/FrameWork_resources_sound_enemy_belial_slimeball.wav");

	SkellBossLaser* laser = laserPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(laser);
	lasers.push_back(laser);

	laser->Fire(position, scale);
}