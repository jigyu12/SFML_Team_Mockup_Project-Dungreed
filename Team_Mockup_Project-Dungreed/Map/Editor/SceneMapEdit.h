#pragma once

class UiEditor;
class TileMap;

class SceneMapEdit :
	public Scene
{
protected:
	UiEditor* uiEditor;


	float speed;

public:
	SceneMapEdit();
	~SceneMapEdit() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

