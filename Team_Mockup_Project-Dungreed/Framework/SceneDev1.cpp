#include "stdafx.h"
#include "SceneDev1.h"
#include "Player.h"
#include "Room.h";

SceneDev1::SceneDev1() : Scene(SceneIds::Dev1)
{
}

void SceneDev1::Init()
{
	player = AddGo(new Player("player"));
	room = AddGo(new Room("tilemap"));

	Scene::Init();
}

void SceneDev1::Enter()
{
	Scene::Enter();

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
	room->SaveMapData("1froom1.json");
	room->LoadMapData("1froom1.json");

}

void SceneDev1::Exit()
{
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SaveDataV1 test;
		test.highscore = 123;
		SaveLoadMgr::Instance().SaveV1(test);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SaveDataVC test = SaveLoadMgr::Instance().Load();
		std::cout << test.version << test.highscore << " " << test.gold << std::endl;
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
