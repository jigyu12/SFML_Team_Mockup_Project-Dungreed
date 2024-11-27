#include "stdafx.h"
#include "SceneDev2.h"
#include "Bat.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{

}

void SceneDev2::Init()
{
	{
		player = AddGo(new Player());
		player->SetOrigin(Origins::MC);
		player->SetPosition({ 0.f,0.f });
	}
	{
		Bat* bat = AddGo(new Bat());
		bat->SetPosition({ 0.f, 0.f });
		bat->SetOrigin(Origins::MC);
		batList.push_back(bat);
	}

	Scene::Init();
}

void SceneDev2::Enter()
{
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	Scene::Enter();
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
