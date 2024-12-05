#include "stdafx.h"
#include "UiEditTile.h"
#include "UiEditHitBox.h"
#include "UiEditMonster.h"
#include "UiEditObject.h"
#include "UiEditor.h"
#include "TileMap.h"
#include "FileDialog.h"
#include "Button.h"
#include "TextGo.h"

UiEditor::UiEditor(const std::string& name)
	: GameObject(name)
{
}

void UiEditor::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	editorWindow.setPosition(position);

	sf::Transform transform = editorWindow.getTransform();

	selectedFileName->SetPosition(transform.transformPoint(40.f, 20.f));

	buttonNew->SetPosition(transform.transformPoint(40.f, 65.f));
	buttonSave->SetPosition(transform.transformPoint(140.f, 65.f));
	buttonLoad->SetPosition(transform.transformPoint(240.f, 65.f));
	buttonReset->SetPosition(transform.transformPoint(340.f, 65.f));

	buttonTile->SetPosition(transform.transformPoint(40.f, 125.f));
	buttonHitbox->SetPosition(transform.transformPoint(140.f, 125.f));
	buttonMonster->SetPosition(transform.transformPoint(240.f, 125.f));
	buttonObject->SetPosition(transform.transformPoint(340.f, 125.f));

	uiEditTile->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditHitBox->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditMonster->SetPosition(transform.transformPoint(0.f, 200.f));
	uiEditObject->SetPosition(transform.transformPoint(0.f, 200.f));
}

void UiEditor::SetRotation(float angle)
{
	rotation = angle;
	editorWindow.setRotation(rotation);

	selectedFileName->SetRotation(rotation);

	buttonNew->SetRotation(rotation);
	buttonSave->SetRotation(rotation);
	buttonLoad->SetRotation(rotation);
	buttonReset->SetRotation(rotation);

	buttonTile->SetRotation(rotation);
	buttonHitbox->SetRotation(rotation);
	buttonMonster->SetRotation(rotation);
	buttonObject->SetRotation(rotation);

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

	selectedFileName->SetScale(scale);

	buttonNew->SetScale(scale);
	buttonSave->SetScale(scale);
	buttonLoad->SetScale(scale);
	buttonReset->SetScale(scale);

	buttonTile->SetScale(scale);
	buttonHitbox->SetScale(scale);
	buttonMonster->SetScale(scale);
	buttonObject->SetScale(scale);

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

	selectedFileName = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
	selectedFileName->Init();

	uiEditTile = new UiEditTile();
	uiEditTile->Init();

	uiEditHitBox = new UiEditHitBox();
	uiEditHitBox->Init();

	uiEditMonster = new UiEditMonster();
	uiEditMonster->Init();

	uiEditObject = new UiEditObject();
	uiEditObject->Init();

	buttonNew = new Button();
	buttonNew->Init();

	buttonSave = new Button();
	buttonSave->Init();

	buttonLoad = new Button();
	buttonLoad->Init();

	buttonReset = new Button();
	buttonReset->Init();

	buttonTile = new Button();
	buttonTile->Init();

	buttonHitbox = new Button();
	buttonHitbox->Init();

	buttonMonster = new Button();
	buttonMonster->Init();

	buttonObject = new Button();
	buttonObject->Init();

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
	uiEditTile->Release();
	delete uiEditTile;
	uiEditHitBox->Release();
	delete uiEditHitBox;
	uiEditMonster->Release();
	delete uiEditMonster;
	uiEditObject->Release();
	delete uiEditObject;

	selectedFileName->Release();
	delete selectedFileName;

	buttonNew->Release();
	delete buttonNew;
	buttonSave->Release();
	delete buttonSave;
	buttonLoad->Release();
	delete buttonLoad;
	buttonReset->Release();
	delete buttonReset;

	buttonTile->Release();
	delete buttonTile;
	buttonHitbox->Release();
	delete buttonHitbox;
	buttonMonster->Release();
	delete buttonMonster;
	buttonObject->Release();
	delete buttonObject;
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

	selectedFileName->Reset();
	selectedFileName->Set(30);
	selectedFileName->SetString("New File", true);

	buttonNew->Reset();
	buttonNew->Set({ 90.f,45.f }, 20);
	buttonNew->SetString("New", true);
	buttonNew->SetClickedEvent([this]() { NewFile(); });

	buttonSave->Reset();
	buttonSave->Set({ 90.f,45.f }, 20);
	buttonSave->SetString("Save", true);
	buttonSave->SetClickedEvent([this]() { SaveFile(); });

	buttonLoad->Reset();
	buttonLoad->Set({ 90.f,45.f }, 20);
	buttonLoad->SetString("Load", true);
	buttonLoad->SetClickedEvent([this]() { LoadFile(); });

	buttonReset->Reset();
	buttonReset->Set({ 90.f,45.f }, 20);
	buttonReset->SetString("Reset", true);
	buttonReset->SetClickedEvent([this]()
		{
			sf::View view = SCENE_MGR.GetCurrentScene()->GetWorldView();
			view.setCenter(0.f, 0.f);
			SCENE_MGR.GetCurrentScene()->SetWorldView(view);
		});

	buttonTile->Reset();
	buttonTile->Set({ 90.f,45.f }, 20);
	buttonTile->SetString("Tile", true);
	buttonTile->SetClickedEvent([this]() { ChangeGroupbox(GroupBox::Tile); });

	buttonHitbox->Reset();
	buttonHitbox->Set({ 90.f,45.f }, 20);
	buttonHitbox->SetString("Hitbox", true);
	buttonHitbox->SetClickedEvent([this]() { ChangeGroupbox(GroupBox::HitBox); });

	buttonMonster->Reset();
	buttonMonster->Set({ 90.f,45.f }, 20);
	buttonMonster->SetString("Monster", true);
	buttonMonster->SetClickedEvent([this]() { ChangeGroupbox(GroupBox::Monster); });

	buttonObject->Reset();
	buttonObject->Set({ 90.f,45.f }, 20);
	buttonObject->SetString("Object", true);
	buttonObject->SetClickedEvent([this]() { ChangeGroupbox(GroupBox::Object); });

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

	buttonNew->Update(dt);
	buttonSave->Update(dt);
	buttonLoad->Update(dt);
	buttonReset->Update(dt);

	buttonTile->Update(dt);
	buttonHitbox->Update(dt);
	buttonMonster->Update(dt);
	buttonObject->Update(dt);
}

void UiEditor::Draw(sf::RenderWindow& window)
{
	window.draw(editorWindow);
	//window.draw(newButton);
	buttonNew->Draw(window);
	buttonLoad->Draw(window);
	buttonSave->Draw(window);
	buttonReset->Draw(window);

	buttonTile->Draw(window);
	buttonHitbox->Draw(window);
	buttonMonster->Draw(window);
	buttonObject->Draw(window);

	selectedFileName->Draw(window);

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
		buttonTile->SetPressed(false);
		break;
	case UiEditor::GroupBox::HitBox:
		buttonHitbox->SetPressed(false);
		break;
	case UiEditor::GroupBox::Monster:
		buttonMonster->SetPressed(false);
		break;
	case UiEditor::GroupBox::Object:
		buttonObject->SetPressed(false);
		break;
	}
	this->selectedGroupBox = selectedGroupBox;
	switch (this->selectedGroupBox)
	{
	case UiEditor::GroupBox::Tile:
		buttonTile->SetPressed(true);
		break;
	case UiEditor::GroupBox::HitBox:
		buttonHitbox->SetPressed(true);
		break;
	case UiEditor::GroupBox::Monster:
		buttonMonster->SetPressed(true);
		break;
	case UiEditor::GroupBox::Object:
		buttonObject->SetPressed(true);
		break;
	}
}

void UiEditor::NewFile()
{
	selectedFileName->SetString("New File", true);
	for (int i = 0;i < MapData::TileMapCount;++i)
	{
		editingTileMaps[i]->Set({ 10,10 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, 0)));
	}
	uiEditHitBox->ClearHitBoxData();
	uiEditObject->ClearObjectData();
}

void UiEditor::SaveFile()
{
	FileDialog::OpenDialog([this](const std::wstring& path)
		{
			MapDataVC mapData;

			for (int i = 0; i < MapData::TileMapCount;++i)
			{
				mapData.tileMapData[i] = editingTileMaps[i]->GetTileMapData();
			}
			mapData.hitBoxData = uiEditHitBox->GetHitBoxData();
			mapData.playerStartPoint = uiEditHitBox->GetStartPoints();
			mapData.monsterSpawnData = uiEditMonster->GetSpawnData();
			mapData.objectData = uiEditObject->GetObjectData();

			MapDataLoader::Save(mapData, path);

			int index = path.rfind(L"\\");
			selectedFileName->SetString(path.substr(++index));
		}, false);
	InputMgr::ResetMouseButton(sf::Mouse::Left);
}

void UiEditor::LoadFile()
{
	FileDialog::OpenDialog([this](const std::wstring& path)
		{
			const MapDataVC& mapdata = MapDataLoader::Load(path);

			for (int i = 0; i < MapData::TileMapCount;++i)
			{
				editingTileMaps[i]->Set(mapdata.tileMapData[i]);
			}
			uiEditHitBox->SetHitBoxData(mapdata.hitBoxData);
			uiEditHitBox->SetStartPositionData(mapdata.playerStartPoint);
			uiEditMonster->SetSpawnData(mapdata.monsterSpawnData);
			uiEditObject->SetObjectData(mapdata.objectData);

			int index = path.rfind(L"\\");
			selectedFileName->SetString(path.substr(++index));
		}, true);
	InputMgr::ResetMouseButton(sf::Mouse::Left);
}
