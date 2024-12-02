#pragma once

#include "Scene.h"

class Player;
class Room;
class Bat;
class SkeletonDog;
class Weapon;

class SceneGame : public Scene
{
protected:
	Player* player;
	std::vector<Weapon*> weaponList;
	//std::vector<Bat*> batList;
	//std::vector<SkeletonDog*> skeletonDogList;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

