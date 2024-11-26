#include "stdafx.h"
#include "SceneDev3.h"

SceneDev3::SceneDev3() : Scene(SceneIds::Dev2)
{

}

void SceneDev3::Init()
{
	Scene::Init();
}

void SceneDev3::Enter()
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

void SceneDev3::Exit()
{
	Scene::Exit();
}

void SceneDev3::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev3::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
