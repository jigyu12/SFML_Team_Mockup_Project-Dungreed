#include "stdafx.h"
#include "SceneDev3.h"
#include "MapData.h"
#include "TileMap.h"
#include "Room.h";
#include "Player.h"

SceneDev3::SceneDev3() : Scene(SceneIds::Dev2)
{

}

void SceneDev3::Init()
{
	roome = AddGo(new Room("tilemap"));
	room1 = AddGo(new Room("room1"));
	player = AddGo(new Player("player"));

	Scene::Init();
}

void SceneDev3::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 4.f;
	size.y /= 4.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);


	roome->SetConnectedRoom(room1, HitBoxData::Type::PortalRight);
	room1->SetConnectedRoom(roome, HitBoxData::Type::PortalLeft);
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
		roome->SaveMapData("1fenter.json");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad8))
	{
		room1->SetActive(false);
		roome->SetActive(true);
		roome->LoadMapData("1fenter.json");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad4))
	{
		room1->SaveMapData("1froom1.json");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad5))
	{
		roome->SetActive(false);
		room1->SetActive(true);
		room1->LoadMapData("1froom1.json");
	}

	sf::Vector2f playerpos = player->GetPosition();
	if (roome->IsActive())
	{
		const std::vector<std::pair<HitBox*, HitBoxData>>& hitboxes = roome->GetHitBoxes();
		for (auto& hitbox : hitboxes)
		{
			if (hitbox.second.type >= HitBoxData::Type::PortalUp
				&& hitbox.second.type <= HitBoxData::Type::PortalRight
				&& hitbox.first->rect.getGlobalBounds().contains(playerpos))
			{
				roome->EnterPortal(hitbox.first);
			}
			
		}
	}
	else if (room1->IsActive())
	{
		const std::vector<std::pair<HitBox*, HitBoxData>>& hitboxes = room1->GetHitBoxes();
		for (auto& hitbox : hitboxes)
		{
			if (hitbox.second.type >= HitBoxData::Type::PortalUp
				&& hitbox.second.type <= HitBoxData::Type::PortalRight
				&& hitbox.first->rect.getGlobalBounds().contains(playerpos))
			{
				room1->EnterPortal(hitbox.first);
			}
		}
	}

}

void SceneDev3::Draw(sf::RenderWindow& window)
{
	window.clear({ 51,49,67 });
	Scene::Draw(window);
}
