#pragma once
#include "Scene.h"

class Bat;

class SceneDev2 : public Scene
{
protected:
	Player* player;
	std::vector<Bat*> batList;

public:
	SceneDev2();
	~SceneDev2() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

