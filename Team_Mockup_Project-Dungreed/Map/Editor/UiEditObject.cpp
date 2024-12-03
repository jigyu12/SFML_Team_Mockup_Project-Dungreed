#include "stdafx.h"
#include "UiEditObject.h"

UiEditObject::UiEditObject(const std::string& name)
	: GameObject(name)
{
}

void UiEditObject::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	boxWindow.setPosition(position);


	sf::Transform transform = boxWindow.getTransform();

	for (int i = 0; i < objectList.size(); ++i)
	{
		objectList[i].setPosition(transform.transformPoint(50.f + (i % 4) * 100.f, 100.f + (i / 4) * 100.f));
	}
}

void UiEditObject::SetRotation(float angle)
{
	rotation = angle;
	SetPosition(position);
}

void UiEditObject::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
	SetPosition(position);
}

void UiEditObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(boxWindow, originPreset);
		SetPosition(position);
	}
}

void UiEditObject::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	boxWindow.setOrigin(origin);
	SetPosition(position);

}

void UiEditObject::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditObject::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiEditObject::Release()
{
	ClearObjectData();
}

void UiEditObject::Reset()
{
	boxWindow.setFillColor({ 150,150,150,255 });
}

void UiEditObject::Update(float dt)
{
}

void UiEditObject::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);
}

void UiEditObject::ClearObjectData()
{
	for (auto& objectdatum : objectData)
	{
		delete objectdatum.first;
	}
	objectData.clear();
}
