#include "stdafx.h"
#include "UiEditObject.h"

UiEditObject::UiEditObject(const std::string& name)
	: GameObject(name)
{
}

void UiEditObject::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditObject::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditObject::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditObject::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditObject::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditObject::Init()
{
}

void UiEditObject::Release()
{
}

void UiEditObject::Reset()
{
}

void UiEditObject::Update(float dt)
{
}

void UiEditObject::Draw(sf::RenderWindow& window)
{
}
