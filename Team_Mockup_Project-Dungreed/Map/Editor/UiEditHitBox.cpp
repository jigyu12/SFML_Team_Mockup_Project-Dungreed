#include "stdafx.h"
#include "UiEditHitBox.h"

UiEditHitBox::UiEditHitBox(const std::string& name)
	: GameObject(name)
{
}

void UiEditHitBox::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditHitBox::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditHitBox::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditHitBox::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditHitBox::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditHitBox::Init()
{
}

void UiEditHitBox::Release()
{
}

void UiEditHitBox::Reset()
{
}

void UiEditHitBox::Update(float dt)
{
}

void UiEditHitBox::Draw(sf::RenderWindow& window)
{
}
