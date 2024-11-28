#include "stdafx.h"
#include "UiEditor.h"
#include "TileMap.h"

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

	tileMap = new TileMap();
	tileMap->Init();
}

void UiEditor::Release()
{
}

void UiEditor::Reset()
{
	tileMap->Reset();

	std::vector<sf::Vector2f> vectiledata;

	const std::unordered_map<int, DataTile>& tiletable = TILE_TABLE->GetTable();

	tileIndexes.resize(10);
	for (int j = 0;j < tileIndexes.size();++j)
	{
		tileIndexes[j].resize(5);
	}
	int cnt = 0;
	for (auto& tiledata : tiletable)
	{
		int i = cnt / tileIndexes.size();
		int j = cnt % tileIndexes[0].size();

		tileIndexes[j][i] = tiledata.first;
	}

	tileMap->SetTexture("graphics/map/Map.png");
	tileMap->Set({ 5,10 }, { 16.f,16.f }, tileIndexes);
	tileMap->SetScale({ 3.f, 3.f });

	tileMap->SetPosition({ 100.f,100.f });
}

void UiEditor::Update(float dt)
{
	sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());


	if (tileMap->GetGlobalBounds().contains(mousepos))
	{

	}


}

void UiEditor::Draw(sf::RenderWindow& window)
{
	tileMap->Draw(window);
}
