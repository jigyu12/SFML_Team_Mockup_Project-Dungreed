#pragma once

class Room;

class SceneDev3 : public Scene
{
protected:

	Room* room;

public:
	SceneDev3();
	~SceneDev3() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

