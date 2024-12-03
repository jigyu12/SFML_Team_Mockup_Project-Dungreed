#include "stdafx.h"
#include "SkellBossBullet.h"
#include "SkellBoss.h"
#include "Player.h"

SkellBossBullet::SkellBossBullet(const std::string& name)
	: SpriteGo(name)
{
}

void SkellBossBullet::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
		origin = Utils::SetOrigin(sprite, originPreset);
	}
	else
	{
		std::cerr << "Cannot assign Origins::Custom" << std::endl;
	}
}

void SkellBossBullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SkellBossBullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void SkellBossBullet::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void SkellBossBullet::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void SkellBossBullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void SkellBossBullet::Reset()
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	animator.SetTarget(&sprite);
	animator.Play("animations/Boss/SkellBoss Bullet Idle.csv");

	owner = dynamic_cast<SkellBoss*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBoss"));
	sortingOrder = owner->sortingOrder + 1;

	target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	speed = 100.f;
	damage = 6;

	idleTimeAccum = 0.f;
	idleTimeDelay = 10.f;

	destroyTimeAccum = 0.f;
	destroyTimeDelay = 1.f;
	isDestroy = false;
}

void SkellBossBullet::Update(float dt)
{
	idleTimeAccum += dt;

	SetPosition(position + direction * speed * dt);

	animator.Update(dt);

	if(!isDestroy)
		hitbox.UpdateTr(sprite, GetLocalBounds());
	else
		hitbox.UpdateTr(sprite, {0.f,0.f,0.f,0.f});
}

void SkellBossBullet::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	if (isDestroy)
	{
		destroyTimeAccum += dt;

		if (destroyTimeAccum >= destroyTimeDelay)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		}

		return;
	}

	if (idleTimeAccum >= idleTimeDelay)
	{
		if (!isDestroy)
		{
			speed = 0.f;
			isDestroy = true;
			animator.Play("animations/Boss/SkellBoss Bullet Destroy.csv");
		}
	}
	else if (Utils::CheckCollision(target->GetHitBox(), hitbox) && !target->IsDamaged() && !target->IsDead())
	{
		if (!isDestroy)
		{
			target->OnDamage(damage);

			speed = 0.f;
			isDestroy = true;
			animator.Play("animations/Boss/SkellBoss Bullet Destroy.csv");
		}
	}
}

void SkellBossBullet::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	hitbox.Draw(window);
}

void SkellBossBullet::Release()
{
}

void SkellBossBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
	SetPosition(pos);
	direction = dir;

	SetRotation(Utils::Angle(direction));
}