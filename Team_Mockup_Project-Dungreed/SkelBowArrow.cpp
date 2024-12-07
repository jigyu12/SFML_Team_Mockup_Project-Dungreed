#include "stdafx.h"
#include "SkelBowArrow.h"
#include "Room.h"

SkelBowArrow::SkelBowArrow(const std::string& name)
	: SpriteGo(name)
{
}

void SkelBowArrow::SetOrigin(Origins preset)
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

void SkelBowArrow::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SkelBowArrow::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void SkelBowArrow::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void SkelBowArrow::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void SkelBowArrow::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void SkelBowArrow::Reset()
{
	SetTextureId("graphics/monster/Skels/CrossbowArrow.png");
	sprite.setTexture(TEXTURE_MGR.Get(textureId));

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sortingOrder = target->sortingOrder - 2;

	speed = 150.f;
	damage = 5;

	idleTimeAccum = 0.f;
	idleTimeDelay = 10.f;
	
	//SetScale({ 0.6f, 0.6f });
}

void SkelBowArrow::Update(float dt)
{
	idleTimeAccum += dt;

	SetPosition(position + direction * speed * dt);

	hitbox.UpdateTr(sprite, GetLocalBounds());
}

void SkelBowArrow::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	if (idleTimeAccum >= idleTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}

	if (Utils::CheckCollision(target->GetHitBox(), hitbox) && !target->IsDamaged() && !target->IsDead())
	{
		target->OnDamage(damage);

		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}

	const auto& roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
	for (auto& roomHitbox : roomHitboxes)
	{
		if (Utils::CheckCollision(*roomHitbox.first, hitbox))
		{
			if (roomHitbox.second.type == HitBoxData::Type::Immovable)
				SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		}
	}
}

void SkelBowArrow::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	hitbox.Draw(window);
}

void SkelBowArrow::Release()
{
}

void SkelBowArrow::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir)
{
	SetPosition(pos);
	direction = dir;

	SetRotation(Utils::Angle(direction) + 90);
}