#include "stdafx.h"
#include "ParticleGo.h"
#include "Room.h"

ObjectParticle::ObjectParticle(const std::string& name)
	: GameObject(name)
{
}

void ObjectParticle::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void ObjectParticle::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void ObjectParticle::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void ObjectParticle::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ObjectParticle::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ObjectParticle::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 20;
}

void ObjectParticle::Release()
{
}

void ObjectParticle::Reset()
{
	room = ROOM_MGR.GetCurrentRoom();
}

void ObjectParticle::Update(float dt)
{
}

void ObjectParticle::Draw(sf::RenderWindow& window)
{
}

void ObjectParticle::ReturnThis()
{
	if (room != nullptr)
	{
		room->ReturnObjectParticle(this);
	}
}
