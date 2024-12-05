#pragma once

#include "MapObject.h"

class BreakableMo : public MapObject
{
protected:

public:
	BreakableMo(const std::string& name = "");
	~BreakableMo() = default;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Set(const ObjectData::Type& type) override;
	void SetStatus(const Status& status) override;
	void OnDamaged(int damage) override;
};
