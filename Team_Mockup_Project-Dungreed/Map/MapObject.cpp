#include "stdafx.h"
#include "MapObject.h"

MapObject::MapObject(const std::string& name)
	: GameObject(name)
{
}

void MapObject::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void MapObject::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void MapObject::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void MapObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void MapObject::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void MapObject::Init()
{
}

void MapObject::Release()
{
}

void MapObject::Reset()
{
}

void MapObject::Update(float dt)
{
}

void MapObject::Draw(sf::RenderWindow& window)
{
}
