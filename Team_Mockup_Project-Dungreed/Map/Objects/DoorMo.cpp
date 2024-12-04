#include "stdafx.h"
#include "DoorMo.h"

DoorMo::DoorMo(const std::string& name)
	: MapObject(name)
{
}

void DoorMo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void DoorMo::SetRotation(float angle)
{
	rotation = angle;
}

void DoorMo::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void DoorMo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void DoorMo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void DoorMo::Init()
{
}

void DoorMo::Release()
{
}

void DoorMo::Reset()
{
}

void DoorMo::Update(float dt)
{
}

void DoorMo::Draw(sf::RenderWindow& window)
{
}
