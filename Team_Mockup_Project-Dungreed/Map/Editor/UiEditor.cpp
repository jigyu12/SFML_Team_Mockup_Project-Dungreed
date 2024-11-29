#include "stdafx.h"
#include "UiEditTile.h"
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

	this->tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("tileMap"));

	sf::Vector2f windowsize = FRAMEWORK.GetWindowSizeF();
	editorWindow.setSize({ 480.f, windowsize.y });
	editorWindow.setFillColor({ 100,100,100,255 });

	saveButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonsave.png"));
	saveButton.setPosition(150.f, 100.f);
	saveButton.setScale(0.25f, 0.25f);

	loadButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonload.png"));
	loadButton.setPosition(250.f, 100.f);
	loadButton.setScale(0.25f, 0.25f);
}

void UiEditor::Update(float dt)
{
	uiEditTile->Update(dt);


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
	uiEditTile->Draw(window);
	window.draw(loadButton);
	window.draw(saveButton);
}

int UiEditor::GetSelectedTileIndex()
{
	return uiEditTile->GetSelectedTileIndex();
}
