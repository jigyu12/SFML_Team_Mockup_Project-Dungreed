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
}

void UiEditor::Release()
{
	delete uiEditTile;
}

void UiEditor::Reset()
{
	uiEditTile->Reset();
	uiEditHitBox->Reset();

	this->editingTileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("tileMap"));

	editorWindow.setFillColor({ 100,100,100,255 });

	saveButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonsave.png"));

	loadButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonload.png"));
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

					if (editingTileMap != nullptr)
					{
						mapData.tileMapData = editingTileMap->GetTileMapData();
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

					if (editingTileMap != nullptr)
					{
						editingTileMap->Set(mapdata.tileMapData);
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