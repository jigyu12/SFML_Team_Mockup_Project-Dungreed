#pragma once
#include "Scene.h"

class Room;
class Player;

class SceneDev1 : public Scene
{
protected:
	Player* player;
	Room* room;

public:
	SceneDev1();
	~SceneDev1() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;
	

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

