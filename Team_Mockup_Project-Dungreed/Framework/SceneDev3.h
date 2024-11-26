#pragma once
#include "Scene.h"

class SceneDev3 : public Scene
{
protected:

public:
	SceneDev3();
	~SceneDev3() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

