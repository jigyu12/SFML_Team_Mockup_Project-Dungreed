#include "stdafx.h"
#include "UiEditMonster.h"

UiEditMonster::UiEditMonster(const std::string& name)
	: GameObject(name)
{
}

void UiEditMonster::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditMonster::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditMonster::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditMonster::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditMonster::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditMonster::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditMonster::Init()
{
}

void UiEditMonster::Release()
{
}

void UiEditMonster::Reset()
{
}

void UiEditMonster::Update(float dt)
{
}

void UiEditMonster::Draw(sf::RenderWindow& window)
{
}
