#include "stdafx.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"
#include "TileMap.h"

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

	centerpos.x += InputMgr::GetAxis(Axis::Horizontal) * speed * dt;
	centerpos.y += InputMgr::GetAxis(Axis::Vertical) * speed * dt;
	worldView.setCenter(centerpos);

	if (InputMgr::GetKeyDown(sf::Keyboard::Q))
	{
		tileMap->SetTile({ 1,1 }, TILE_TABLE->Get(3).startpos);
	}
}

void SceneMapEdit::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}