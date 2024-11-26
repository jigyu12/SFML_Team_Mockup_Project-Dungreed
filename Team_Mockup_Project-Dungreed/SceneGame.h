#pragma once
#include "Scene.h"

class SceneGame : public Scene
{
protected:

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

