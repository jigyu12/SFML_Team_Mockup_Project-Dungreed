#pragma once
#include "Scene.h"

class Room;
class Bat;
class SkeletonDog;

class SceneDev2 : public Scene
{
protected:
	Room* room;
	Player* player;
	std::vector<Bat*> batList;
	std::vector<SkeletonDog*> skeletonDogList;
	
public:
	SceneDev2();
	~SceneDev2() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

