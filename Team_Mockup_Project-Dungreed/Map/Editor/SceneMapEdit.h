#pragma once
#include "Scene.h"
class SceneMapEdit :
    public Scene
{

public:
	SceneMapEdit();
	~SceneMapEdit() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

