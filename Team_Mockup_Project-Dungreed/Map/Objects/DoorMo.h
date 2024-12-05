#pragma once
#include "MapObject.h"

class DoorMo : public MapObject
{
protected:

public:
	DoorMo(const std::string& name = "");
	~DoorMo() = default;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Set(const ObjectData::Type& datum);

	void SetStatus(const Status& status) override;
};
