#include "stdafx.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Bat.h"
#include "SkeletonDog.h"
#include "Weapon.h"
#include "ShortSword.h"
#include "HandCrossbow.h"
#include "PlayerUi.h"
#include "Room.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	{
		player = AddGo(new Player());

		// 두 무기 중에 하나는 SetWeaponToWeaponSlot->true로 켜져 있어야 시작 무기로 사용됨 
		{
			ShortSword* shortSword = AddGo(new ShortSword());
			weaponList.push_back(shortSword);
			shortSword->SetPosition({ 120.f, -80.f });
			shortSword->SetOwnerPlayer(player);
			player->SetWeaponToWeaponSlot1(shortSword, true);
		}
		{
			HandCrossbow* handCrossbow = AddGo(new HandCrossbow());
			weaponList.push_back(handCrossbow);
			handCrossbow->SetPosition({ -120.f, -80.f });
			handCrossbow->SetOwnerPlayer(player);
			player->SetWeaponToWeaponSlot2(handCrossbow);
		}
	}
	/*{
		Bat* bat = AddGo(new Bat());
		bat->SetPosition({ 140.f, -80.f });
		batList.push_back(bat);
	}
	{
		SkeletonDog* skeletonDog = AddGo(new SkeletonDog());
		skeletonDog->SetPosition({ 0.f, 0.f });
		skeletonDogList.push_back(skeletonDog);
	}*/
	{
		PlayerUi* playerui = AddGo(new PlayerUi());
	}


	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);
	ROOM_MGR.Reset();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	sf::Vector2f viewCenter = player->GetPosition();
	sf::FloatRect viewBounds = ROOM_MGR.GetCurrentRoom()->GetViewBounds();
	viewCenter.x = Utils::Clamp(viewCenter.x, viewBounds.left, viewBounds.left + viewBounds.width);
	viewCenter.y = Utils::Clamp(viewCenter.y, viewBounds.top, viewBounds.top + viewBounds.height);

	worldView.setCenter(viewCenter);

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		SCENE_MGR.ChangeScene(SceneIds::MapEdit);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	window.clear({ 51,49,67 });
	Scene::Draw(window);
}