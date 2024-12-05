#pragma once

#include "MapObject.h"

class DoorMo : public MapObject
{
protected:

	float timer;
	sf::Sprite key;
	bool collide;
	GameObject* player;

public:
	DoorMo(const std::string& name = "");
	~DoorMo() = default;

	void SetPosition(const sf::Vector2f& pos) override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Reset() override;

	void Set(const ObjectData::Type& type) override;
	void SetStatus(const Status& status) override;
};
