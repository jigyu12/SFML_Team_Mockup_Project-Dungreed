#include "stdafx.h"
#include "UiEditTile.h"
#include "UiEditor.h"


UiEditor::UiEditor(const std::string& name)
	: GameObject(name)
{
}

void UiEditor::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditor::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditor::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditor::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditor::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditor::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	uiEditTile = new UiEditTile();
	uiEditTile->Init();
}

void UiEditor::Release()
{
	delete uiEditTile;
}

void UiEditor::Reset()
{
	uiEditTile->Reset();

	sf::Vector2f windowsize =  FRAMEWORK.GetWindowSizeF();
	editorWindow.setSize({ 300.f, windowsize.y });
	editorWindow.setFillColor({ 100,100,100,255 });
}

void UiEditor::Update(float dt)
{
	uiEditTile->Update(dt);
}

void UiEditor::Draw(sf::RenderWindow& window)
{
	window.draw(editorWindow);
	uiEditTile->Draw(window);
}

int UiEditor::GetSelectedTileIndex()
{
	return uiEditTile->GetSelectedTileIndex();
}
