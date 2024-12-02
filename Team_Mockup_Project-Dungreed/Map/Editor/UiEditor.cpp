#include "stdafx.h"
#include "UiEditTile.h"
#include "UiEditHitBox.h"
#include "UiEditMonster.h"
#include "UiEditObject.h"
#include "UiEditor.h"
#include "TileMap.h"
#include "FileDialog.h"

UiEditor::UiEditor(const std::string& name)
	: GameObject(name)
{
}

void UiEditor::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	editorWindow.setPosition(position);

	sf::Transform transform = editorWindow.getTransform();

	selectedFileName.setPosition(transform.transformPoint(40.f, 20.f));

	newButton.setPosition(transform.transformPoint(40.f, 65.f));
	saveButton.setPosition(transform.transformPoint(140.f, 65.f));
	loadButton.setPosition(transform.transformPoint(240.f, 65.f));

	tileButton.setPosition(transform.transformPoint(40.f, 125.f));
	hitboxButton.setPosition(transform.transformPoint(140.f, 125.f));
	monsterButton.setPosition(transform.transformPoint(240.f, 125.f));
	objectButton.setPosition(transform.transformPoint(340.f, 125.f));

	uiEditTile->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditHitBox->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditMonster->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditObject->SetPosition(transform.transformPoint(0.f, 200.f));
}

void UiEditor::SetRotation(float angle)
{
	rotation = angle;
	editorWindow.setRotation(rotation);

	selectedFileName.setRotation(rotation);

	newButton.setRotation(rotation);
	saveButton.setRotation(rotation);
	loadButton.setRotation(rotation);

	tileButton.setRotation(rotation);
	hitboxButton.setRotation(rotation);
	monsterButton.setRotation(rotation);
	objectButton.setRotation(rotation);

	uiEditTile->SetRotation(rotation);
	uiEditHitBox->SetRotation(rotation);
	uiEditMonster->SetRotation(rotation);
	uiEditObject->SetRotation(rotation);

	SetPosition(position);
}

void UiEditor::SetScale(const sf::Vector2f& s)
{
	scale = s;
	editorWindow.setScale(scale);

	selectedFileName.setScale(scale);

	newButton.setScale(scale);
	saveButton.setScale(scale);
	loadButton.setScale(scale);

	tileButton.setScale(scale);
	hitboxButton.setScale(scale);
	monsterButton.setScale(scale);
	objectButton.setScale(scale);

	uiEditTile->SetScale(scale);
	uiEditHitBox->SetScale(scale);
	uiEditMonster->SetScale(scale);
	uiEditObject->SetScale(scale);
	SetPosition(position);
}

void UiEditor::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(editorWindow, originPreset);
	}
	SetPosition(position);
}

void UiEditor::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	editorWindow.setOrigin(origin);
	SetPosition(position);
}

void UiEditor::SetSize(const sf::Vector2f& size)
{
	editorWindow.setSize(size);

	sf::Vector2f groupboxSize = size;
	groupboxSize.y -= 200.f;

	uiEditTile->SetSize(groupboxSize);
	uiEditHitBox->SetSize(groupboxSize);
	uiEditMonster->SetSize(groupboxSize);
	uiEditObject->SetSize(groupboxSize);
}

void UiEditor::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	uiEditTile = new UiEditTile();
	uiEditTile->Init();

	uiEditHitBox = new UiEditHitBox();
	uiEditHitBox->Init();

	uiEditMonster = new UiEditMonster();
	uiEditMonster->Init();

	uiEditObject = new UiEditObject();
	uiEditObject->Init();

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
	delete uiEditHitBox;
	delete uiEditMonster;
	delete uiEditObject;
}

void UiEditor::Reset()
{
	uiEditTile->Reset();
	uiEditHitBox->Reset();
	uiEditMonster->Reset();
	uiEditObject->Reset();
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

	newButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonnew.png"));
	saveButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonsave.png"));
	loadButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonload.png"));

	tileButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttontile.png"));
	hitboxButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonhitbox.png"));
	monsterButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonmonster.png"));
	objectButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonobject.png"));

	selectedFileName.setFont(FONT_MGR.Get("fonts/french.ttf"));
	selectedFileName.setString("New File");

	SetSelectedTileLayer(-1);
	ChangeGroupbox(GroupBox::Tile);
}

void UiEditor::Update(float dt)
{
	switch (selectedGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		uiEditTile->Update(dt);
		break;
	case UiEditor::GroupBox::HitBox:
		uiEditHitBox->Update(dt);
		break;
	case UiEditor::GroupBox::Monster:
		uiEditMonster->Update(dt);
		break;
	case UiEditor::GroupBox::Object:
		uiEditObject->Update(dt);
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
					if (uiEditMonster != nullptr)
					{
						mapData.monsterSpawnData = uiEditMonster->GetSpawnData();
					}

					MapDataLoader::Save(mapData, path);

					int index = path.rfind(L"\\");
					selectedFileName.setString(path.substr(++index));
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
					if (uiEditMonster != nullptr)
					{
						uiEditMonster->SetSpawnData(mapdata.monsterSpawnData);
					}

					int index = path.rfind(L"\\");
					selectedFileName.setString(path.substr(++index));
				}, true);
			InputMgr::ResetMouseButton(sf::Mouse::Left);
		}
		else if (newButton.getGlobalBounds().contains(mousepos))
		{
			selectedFileName.setString("New File");
			for (int i = 0;i < MapData::TileMapCount;++i)
			{
				editingTileMaps[i]->Set({ 10,10 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, 0)));
			}
			uiEditHitBox->ClearHitBoxData();
		}
		else if (tileButton.getGlobalBounds().contains(mousepos))
		{
			ChangeGroupbox(GroupBox::Tile);
		}
		else if (hitboxButton.getGlobalBounds().contains(mousepos))
		{
			ChangeGroupbox(GroupBox::HitBox);
		}
		else if (monsterButton.getGlobalBounds().contains(mousepos))
		{
			ChangeGroupbox(GroupBox::Monster);
		}
		else if (objectButton.getGlobalBounds().contains(mousepos))
		{
			ChangeGroupbox(GroupBox::Object);
		}
	}
}

void UiEditor::Draw(sf::RenderWindow& window)
{
	window.draw(editorWindow);
	window.draw(newButton);
	window.draw(loadButton);
	window.draw(saveButton);

	window.draw(selectedFileName);

	window.draw(tileButton);
	window.draw(hitboxButton);
	window.draw(monsterButton);
	window.draw(objectButton);

	switch (selectedGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		uiEditTile->Draw(window);
		break;
	case UiEditor::GroupBox::HitBox:
		uiEditHitBox->Draw(window);
		break;
	case UiEditor::GroupBox::Monster:
		uiEditMonster->Draw(window);
		break;
	case UiEditor::GroupBox::Object:
		uiEditObject->Draw(window);
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

void UiEditor::ChangeGroupbox(const UiEditor::GroupBox& selectedGroupBox)
{
	switch (this->selectedGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		tileButton.setColor(sf::Color::White);
		break;
	case UiEditor::GroupBox::HitBox:
		hitboxButton.setColor(sf::Color::White);
		break;
	case UiEditor::GroupBox::Monster:
		monsterButton.setColor(sf::Color::White);
		break;
	case UiEditor::GroupBox::Object:
		objectButton.setColor(sf::Color::White);
		break;
	}
	this->selectedGroupBox = selectedGroupBox;
	switch (this->selectedGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		tileButton.setColor({150,150,150,255});
		break;
	case UiEditor::GroupBox::HitBox:
		hitboxButton.setColor({ 150,150,150,255 });
		break;
	case UiEditor::GroupBox::Monster:
		monsterButton.setColor({ 150,150,150,255 });
		break;
	case UiEditor::GroupBox::Object:
		objectButton.setColor({ 150,150,150,255 });
		break;
	}
}
