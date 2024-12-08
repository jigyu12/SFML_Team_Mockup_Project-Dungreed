#pragma once
#include "MapObject.h"

class SealStoneMo : public MapObject
{
protected:

public:
	SealStoneMo(const std::string& name = "");
	~SealStoneMo() = default;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Set(const ObjectData::Type& datum);

	void SetStatus(const Status& status) override;
};
