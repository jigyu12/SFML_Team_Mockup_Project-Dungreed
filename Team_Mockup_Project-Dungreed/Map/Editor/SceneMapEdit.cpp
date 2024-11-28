#include "stdafx.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"
#include "TileMap.h"
#include "MapData.h"

SceneMapEdit::SceneMapEdit() : Scene(SceneIds::Dev2)
{

}

void SceneMapEdit::Init()
{
	speed = 200.f;

	uiEditor = AddGo(new UiEditor("uieditor"));
	tileMap = AddGo(new TileMap("tileMap"));
	Scene::Init();
}

void SceneMapEdit::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	tileMap->SetTexture("graphics/map/Map.png");
	tileMap->Set({ 10,10 }, { 16.f,16.f }, std::vector<std::vector<int>>());

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(centerpos);
}

void SceneMapEdit::Exit()
{
	Scene::Exit();
}

void SceneMapEdit::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetAxisRaw(Axis::Horizontal) != 0.f || InputMgr::GetAxisRaw(Axis::Vertical) != 0.f)
	{
		centerpos.x += InputMgr::GetAxis(Axis::Horizontal) * speed * dt;
		centerpos.y += InputMgr::GetAxis(Axis::Vertical) * speed * dt;
		worldView.setCenter(centerpos);
	}

	if (InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
		if (tileMap->GetGlobalBounds().contains(mousepos))
		{
			tileMap->SetTile(mousepos, TILE_TABLE->Get(uiEditor->GetSelectedTileIndex()));
		}
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad6))
	{
		sf::Vector2i count = tileMap->GetCellCount();
		tileMap->Resize({ count.x + 1, count.y });
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Numpad4))
	{
		sf::Vector2i count = tileMap->GetCellCount();
		tileMap->Resize({ count.x - 1, count.y });
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Numpad2))
	{
		sf::Vector2i count = tileMap->GetCellCount();
		tileMap->Resize({ count.x, count.y + 1 });
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Numpad8))
	{
		sf::Vector2i count = tileMap->GetCellCount();
		tileMap->Resize({ count.x , count.y - 1 });
	}
}

void SceneMapEdit::Draw(sf::RenderWindow& window)
{
	
	Scene::Draw(window);
}

void SceneMapEdit::Save()
{
	MapDataVC mapData;

	mapData.tileMapData = tileMap->GetTileMapData();

	MapDataLoader::Save(mapData, "temp.json");
}

void SceneMapEdit::Load()
{

	tileMap->Set(MapDataLoader::Load("temp.json").tileMapData);
}
