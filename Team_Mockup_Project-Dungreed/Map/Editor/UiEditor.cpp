#include "stdafx.h"
#include "UiEditTile.h"
#include "UiEditHitBox.h"
#include "UiEditor.h"
#include "TileMap.h"
#include "FileDialog.h"
#include "MapData.h"

UiEditor::UiEditor(const std::string& name)
	: GameObject(name)
{
}

void UiEditor::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	editorWindow.setPosition(position);
}

void UiEditor::SetRotation(float angle)
{
	rotation = angle;
	editorWindow.setRotation(rotation);
}

void UiEditor::SetScale(const sf::Vector2f& s)
{
	scale = s;
	editorWindow.setScale(scale);
}

void UiEditor::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(editorWindow, originPreset);
		saveButton.setOrigin(origin - sf::Vector2f(150.f, 100.f));
		loadButton.setOrigin(origin - sf::Vector2f(250.f, 100.f));
	}
}

void UiEditor::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditor::SetSize(const sf::Vector2f& size)
{
	editorWindow.setSize(size);
}

void UiEditor::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	uiEditTile = new UiEditTile();
	uiEditTile->Init();

	uiEditHitBox = new UiEditHitBox();
	uiEditHitBox->Init();

	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (int i = 0;i < MapData::TileMapCount;++i)
	{
		TileMap* tilemap = new TileMap("tileMap" + std::to_string(i));
		tilemap->Init();
		tilemap->sortingOrder - i;
		editingTileMaps.push_back(tilemap);
	}
}

void UiEditor::Release()
{
	delete uiEditTile;
}

void UiEditor::Reset()
{
	uiEditTile->Reset();
	uiEditHitBox->Reset();
	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (int i = 0;i < MapData::TileMapCount;++i)
	{
		editingTileMaps[i]->Reset();
		editingTileMaps[i]->SetTexture("graphics/map/Map.png");
		editingTileMaps[i]->Set({ 10,10 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, 0)));
		editingTileMaps[i]->SetShowGridLine(true);
		scene->AddGo(editingTileMaps[i]);
	}

	editorWindow.setFillColor({ 100,100,100,255 });

	saveButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonsave.png"));

	loadButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonload.png"));
	SetSelectedTileLayer(-1);
}

void UiEditor::Update(float dt)
{
	switch (currentGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		uiEditTile->Update(dt);
		break;
	case UiEditor::GroupBox::HitBox:
		uiEditHitBox->Update(dt);
		break;
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
		if (saveButton.getGlobalBounds().contains(mousepos))
		{
			FileDialog::OpenDialog([this](const std::wstring& path)
				{
					MapDataVC mapData;

					for (int i = 0; i < MapData::TileMapCount;++i)
					{
						mapData.tileMapData[i] = editingTileMaps[i]->GetTileMapData();
					}
					if (uiEditHitBox != nullptr)
					{
						mapData.hitBoxData = uiEditHitBox->GetHitBoxData();
					}
					MapDataLoader::Save(mapData, path);
				}, false);
			InputMgr::ResetMouseButton(sf::Mouse::Left);
		}
		else if (loadButton.getGlobalBounds().contains(mousepos))
		{
			FileDialog::OpenDialog([this](const std::wstring& path)
				{
					const MapDataVC& mapdata = MapDataLoader::Load(path);

					for (int i = 0; i < MapData::TileMapCount;++i)
					{
						editingTileMaps[i]->Set(mapdata.tileMapData[i]);
					}
					if (uiEditHitBox != nullptr)
					{
						uiEditHitBox->SetHitBoxData(mapdata.hitBoxData);
					}
				}, true);
			InputMgr::ResetMouseButton(sf::Mouse::Left);
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		currentGroupBox = GroupBox::Tile;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		currentGroupBox = GroupBox::HitBox;
	}
}

void UiEditor::Draw(sf::RenderWindow& window)
{
	window.draw(editorWindow);
	window.draw(loadButton);
	window.draw(saveButton);

	switch (currentGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		uiEditTile->Draw(window);
		break;
	case UiEditor::GroupBox::HitBox:
		uiEditHitBox->Draw(window);
		break;
	}
}

void UiEditor::SetSelectedTileLayer(int tilenum)
{
	selectedTile = tilenum;
	switch (tilenum)
	{
	case -1:
		for (int i = 0;i < editingTileMaps.size();++i)
		{
			editingTileMaps[i]->SetActive(true);
		}
		editingTileMaps[1]->SetOpaque(255);
		break;
	default:
		for (int i = 0;i < editingTileMaps.size();++i)
		{
			editingTileMaps[i]->SetActive(i == tilenum);
		}
		editingTileMaps[1]->SetActive(true);
		editingTileMaps[1]->SetOpaque(tilenum != 1 ? 128 : 255);

		break;
	}
}

void UiEditor::ResizeTileMaps(const sf::Vector2i& count)
{
	for (int i = 0;i < editingTileMaps.size();++i)
	{
		editingTileMaps[i]->Resize(count);
	}
}
