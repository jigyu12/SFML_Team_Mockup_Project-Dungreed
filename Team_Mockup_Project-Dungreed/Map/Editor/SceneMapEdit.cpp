#include "stdafx.h"
#include "SceneMapEdit.h"


SceneMapEdit::SceneMapEdit() : Scene(SceneIds::Dev2)
{

}

void SceneMapEdit::Init()
{
	Scene::Init();
}

void SceneMapEdit::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 4.f;
	size.y /= 4.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
}

void SceneMapEdit::Exit()
{
	Scene::Exit();
}

void SceneMapEdit::Update(float dt)
{
	Scene::Update(dt);
}

void SceneMapEdit::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}