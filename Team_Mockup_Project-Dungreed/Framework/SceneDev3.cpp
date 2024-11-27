#include "stdafx.h"
#include "SceneDev3.h"
#include "MapData.h"
#include "TileMap.h"
#include "Room.h";

SceneDev3::SceneDev3() : Scene(SceneIds::Dev2)
{

}

void SceneDev3::Init()
{
	room = AddGo(new Room("tilemap"));

	Scene::Init();
}

void SceneDev3::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

}

void SceneDev3::Exit()
{
	Scene::Exit();
}

void SceneDev3::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad7))
	{
		room->SaveMapData("map.json");
	
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad8))
	{
		room->LoadMapData("map.json");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad5))
	{
		Variables::isDrawHitBox = !Variables::isDrawHitBox;
	}

}

void SceneDev3::Draw(sf::RenderWindow& window)
{
	window.clear({ 51,49,67 });
	Scene::Draw(window);
}
