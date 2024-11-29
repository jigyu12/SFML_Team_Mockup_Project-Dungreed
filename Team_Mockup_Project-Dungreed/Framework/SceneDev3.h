#pragma once

class Room;
class Player;

class SceneDev3 : public Scene
{
protected:

	Room* roome;
	Room* room1;

	Player* player;

	sf::VertexArray vaLight;

public:
	SceneDev3();
	~SceneDev3() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

};

