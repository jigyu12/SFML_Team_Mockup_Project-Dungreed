#pragma once
#include "Scene.h"
class MainTitleUi;

class MainTitleScene :
    public Scene
{
protected:
	MainTitleUi* mainTitleui;
public:
	MainTitleScene();
	virtual ~MainTitleScene() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};

