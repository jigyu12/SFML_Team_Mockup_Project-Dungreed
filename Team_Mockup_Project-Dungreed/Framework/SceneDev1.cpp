#include "stdafx.h"
#include "SceneDev1.h"

SceneDev1::SceneDev1() : Scene(SceneIds::Dev1)
{
}

void SceneDev1::Init()
{

	Scene::Init();
}

void SceneDev1::Enter()
{
	Scene::Enter();

	worldView.setCenter(0.f, 0.f);
	worldView.setSize(FRAMEWORK.GetWindowSizeF());
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
