#include "stdafx.h"
#include "SkellBossParticle.h"
#include "SkellBoss.h"

SkellBossParticle::SkellBossParticle(const std::string& name)
	: SpriteGo(name)
{
}

void SkellBossParticle::SetOrigin(Origins preset)
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

void SkellBossParticle::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SkellBossParticle::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void SkellBossParticle::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void SkellBossParticle::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void SkellBossParticle::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void SkellBossParticle::Reset()
{
	animator.SetTarget(&sprite);
	animator.Play("animations/Boss/SkellBoss Particle Idle.csv");

	owner = dynamic_cast<SkellBoss*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBoss"));
	sortingOrder = owner->sortingOrder - 1;

	idleTimeAccum = 0.f;
	idleTimeDelay = 1.f;
}

void SkellBossParticle::Update(float dt)
{
	idleTimeAccum += dt;

	SetPosition(position + direction * speed * dt);

	animator.Update(dt);
}

void SkellBossParticle::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	if (idleTimeAccum >= idleTimeDelay)
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
}

void SkellBossParticle::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void SkellBossParticle::Release()
{
}

void SkellBossParticle::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float s)
{
	SetPosition(pos);
	direction = dir;
	speed = s;
}