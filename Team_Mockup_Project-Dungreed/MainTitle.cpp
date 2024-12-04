#include "stdafx.h"
#include "MainTitle.h"
#include "MainTitleUi.h"

MainTitleScene::MainTitleScene()
	:Scene(SceneIds::MainTitle)
{
}

void MainTitleScene::Init()
{
	mainTitleui = new MainTitleUi();
	AddGo(mainTitleui);

	Scene::Init();
}

void MainTitleScene::Release()
{
	Scene::Release();
}

void MainTitleScene::Enter()
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

void MainTitleScene::Exit()
{
	Scene::Exit();
}

void MainTitleScene::Update(float dt)
{
	Scene::Update(dt);
}

void MainTitleScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
