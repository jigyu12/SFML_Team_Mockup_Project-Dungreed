#include "stdafx.h"
#include "PortalEffect.h"

PortalEffect::PortalEffect(const std::string& name)
	: GameObject(name)
{
}

void PortalEffect::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void PortalEffect::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void PortalEffect::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void PortalEffect::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void PortalEffect::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void PortalEffect::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 10;
}

void PortalEffect::Release()
{
}

void PortalEffect::Reset()
{
	SetOrigin(Origins::MC);
}

void PortalEffect::Update(float dt)
{
	SetPosition(position + velocity * dt);

	int opacity = 255*(Utils::Clamp01(1 - Utils::SqrMagnitude(position - startpoint) / 300.f));
	body.setColor({ 255,255,255, (sf::Uint8)opacity });

	if (!bounds.intersects(body.getGlobalBounds()))
	{
		Set(bounds, dir);
	}
}

void PortalEffect::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void PortalEffect::Set(const sf::FloatRect& bounds, int dir)
{
	this->dir = dir;
	Set(bounds);
}

void PortalEffect::Set(const sf::FloatRect& bounds)
{
	this->bounds = bounds;
	int rand = Utils::RandomRange(0, 5);

	body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "PortalEffect" + std::to_string(rand))),true);
	SetOrigin(originPreset);

	switch (this->dir)
	{
	case (int)HitBoxData::Type::PortalUp:
		position.x = Utils::RandomRange(bounds.left, bounds.left + bounds.width);
		position.y = bounds.top;
		position.y += Utils::RandomRange(0.f, 4.f);
		velocity.x = 0.f;
		velocity.y = 10.f;
		break;
	case (int)HitBoxData::Type::PortalDown:
		position.x = Utils::RandomRange(bounds.left, bounds.left + bounds.width);
		position.y = bounds.top + bounds.height;
		position.y -= Utils::RandomRange(0.f, 4.f);
		velocity.x = 0.f;
		velocity.y = -10.f;
		break;
	case (int)HitBoxData::Type::PortalLeft:
		position.x = bounds.left;
		position.y = Utils::RandomRange(bounds.top, bounds.top + bounds.height);
		position.x += Utils::RandomRange(0.f, 4.f);
		velocity.x = 10.f;
		velocity.y = 0.f;
		break;
	case (int)HitBoxData::Type::PortalRight:
		position.x = bounds.left + bounds.width;
		position.y = Utils::RandomRange(bounds.top, bounds.top + bounds.height);
		position.x -= Utils::RandomRange(0.f, 4.f);
		velocity.x = -10.f;
		velocity.y = 0.f;
		break;
	}
	startpoint = position;
	SetPosition(position);
}
