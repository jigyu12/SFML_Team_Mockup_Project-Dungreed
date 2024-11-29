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

}

void UiEditor::Release()
{
	delete uiEditTile;
}

void UiEditor::Reset()
{
	uiEditTile->Reset();

	this->tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("tileMap"));

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


	if (InputMgr::GetMouseButtonUp(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
		if (saveButton.getGlobalBounds().contains(mousepos))
		{
			FileDialog::OpenDialog([this](const std::wstring& path)
				{
					MapDataVC mapData;

					if (tileMap != nullptr)
					{
						mapData.tileMapData = tileMap->GetTileMapData();
					}
					MapDataLoader::Save(mapData, path);
				}, false);
		}
		else if (loadButton.getGlobalBounds().contains(mousepos))
		{
			FileDialog::OpenDialog([this](const std::wstring& path)
				{
					if (tileMap != nullptr)
					{
						tileMap->Set(MapDataLoader::Load(path).tileMapData);
					}
				}, true);
		}
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
		break;
	}
}

int UiEditor::GetSelectedTileIndex()
{
	return uiEditTile->GetSelectedTileIndex();
}
