#include "stdafx.h"
#include "Arrow.h"
#include "HandCrossbow.h"

Arrow::Arrow(const std::string& name)
	: SpriteGo(name)
{
}

void Arrow::SetOrigin(Origins preset)
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

void Arrow::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void Arrow::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void Arrow::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void Arrow::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void Arrow::Init()
{
	SetTextureId("graphics/weapon/Arrow00.png");
	sprite.setTexture(TEXTURE_MGR.Get(textureId));

	sortingLayer = SortingLayers::Foreground;
}

void Arrow::Reset()
{
	owner = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	ownerWeapon = dynamic_cast<HandCrossbow*>(SCENE_MGR.GetCurrentScene()->FindGo("HandCrossbow"));
	if (ownerWeapon)
	{
		sortingOrder = ownerWeapon->sortingOrder - 1;
	}
}

void Arrow::Update(float dt)
{
	SetPosition(position + direction * speed * dt);

	hitbox.UpdateTr(sprite, GetLocalBounds());
}

void Arrow::LateUpdate(float dt)
{
	
}

void Arrow::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	hitbox.Draw(window);
}

void Arrow::Release()
{
}

void Arrow::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float s, int d)
{
	SetPosition(pos);
	direction = dir;
	speed = s;
	damage = d;

	SetRotation(Utils::Angle(direction) + 90);
}