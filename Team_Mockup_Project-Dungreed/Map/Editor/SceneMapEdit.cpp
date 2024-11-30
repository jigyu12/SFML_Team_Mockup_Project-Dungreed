#include "stdafx.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"
#include "TileMap.h"
#include "MapData.h"
#include "FileDialog.h"

SceneMapEdit::SceneMapEdit() : Scene(SceneIds::MapEdit)
{

}

void SceneMapEdit::Init()
{
	speed = 200.f;

	uiEditor = AddGo(new UiEditor("uieditor"));

	Scene::Init();
}

void SceneMapEdit::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	
	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	uiEditor->SetPosition({ 0.f,0.f });
	uiEditor->SetSize({ 480.f,size.y });
	uiEditor->SetOrigin(Origins::TL);

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

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev3);

	}

	if (InputMgr::GetAxisRaw(Axis::Horizontal) != 0.f || InputMgr::GetAxisRaw(Axis::Vertical) != 0.f)
	{
		centerpos.x += InputMgr::GetAxis(Axis::Horizontal) * speed * dt;
		centerpos.y += InputMgr::GetAxis(Axis::Vertical) * speed * dt;
		worldView.setCenter(centerpos);
	}
}

void SceneMapEdit::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}