#include "stdafx.h"
#include "SceneDev2.h"
#include "Room.h"
#include "Bat.h"
#include "SkeletonDog.h"
#include "Weapon.h"
#include "ShortSword.h"
#include "HandCrossbow.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{

}

void SceneDev2::Init()
{
	{
		room = AddGo(new Room("tilemap"));
	}
	{
		player = AddGo(new Player());

		{
			ShortSword* shortSword = AddGo(new ShortSword());
			weaponList.push_back(shortSword);
			shortSword->SetPosition({120.f, -80.f});
			//shortSword->SetOwnerPlayer(player);
			//shortSword->SetIsCurrentWeapon(true);
		}
		{
			HandCrossbow* handCrossbow = AddGo(new HandCrossbow());
			weaponList.push_back(handCrossbow);
			handCrossbow->SetPosition({ -120.f, -80.f });
			handCrossbow->SetOwnerPlayer(player);
			handCrossbow->SetIsCurrentWeapon(true);
		}
	}
	{
		Bat* bat = AddGo(new Bat());
		bat->SetPosition({ 140.f, -80.f });
		batList.push_back(bat);
	}
	{
		SkeletonDog* skeletonDog = AddGo(new SkeletonDog());
		skeletonDog->SetPosition({ 0.f, 70.f });
		skeletonDogList.push_back(skeletonDog);
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

	room->SaveMapData("1froom1.json");
	room->LoadMapData("1froom1.json");
	
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
