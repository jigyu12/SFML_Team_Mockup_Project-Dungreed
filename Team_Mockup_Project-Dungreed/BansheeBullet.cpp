#include "stdafx.h"
#include "BansheeBullet.h"
#include "Banshee.h"
#include "Player.h"

BansheeBullet::BansheeBullet(const std::string& name)
	: SpriteGo(name)
{
}

void BansheeBullet::SetOrigin(Origins preset)
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

void BansheeBullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void BansheeBullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void BansheeBullet::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void BansheeBullet::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void BansheeBullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void BansheeBullet::Reset()
{
	animator.SetTarget(&sprite);
	animator.Play("animations/Banshee Bullet Idle.csv");

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	// (������� �ʴ� �ڵ�) Banshee�� ���� ���� �� �� Ư�� Banshee�� ã�� �� ���� �� ���Ƽ� �ּ� ó����
	//owner = dynamic_cast<Banshee*>(SCENE_MGR.GetCurrentScene()->FindGo("Banshee"));
	
	target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sortingOrder = target->sortingOrder + 1;

	speed = 70.f;
	damage = 10;

	idleTimeAccum = 0.f;
	idleTimeDelay = 2.f;

	destroyTimeAccum = 0.f;
	destroyTimeDelay = 0.7f;
	isDestroy = false;
}

void BansheeBullet::Update(float dt)
{
	idleTimeAccum += dt;

	SetPosition(position + direction * speed * dt);

	animator.Update(dt);

	if (!isDestroy)
		hitbox.UpdateTr(sprite, GetLocalBounds());
	else
		hitbox.UpdateTr(sprite, { 0.f,0.f,0.f,0.f });
}

void BansheeBullet::LateUpdate(float dt)
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
			animator.Play("animations/Banshee Bullet Destroy.csv");
		}
	}
	else if (Utils::CheckCollision(target->GetHitBox(), hitbox) && !target->IsDamaged() && !target->IsDead())
	{
		if (!isDestroy)
		{
			target->OnDamage(damage);

			speed = 0.f;
			isDestroy = true;
			animator.Play("animations/Banshee Bullet Destroy.csv");
		}
	}
}

void BansheeBullet::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	hitbox.Draw(window);
}

void BansheeBullet::Release()
{
}

void BansheeBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
	SetPosition(pos);
	direction = dir;
}