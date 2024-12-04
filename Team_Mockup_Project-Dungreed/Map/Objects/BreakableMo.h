#pragma once

#include "MapObject.h"

class BreakableMo : public MapObject
{
protected:

public:
	BreakableMo(const std::string& name = "");
	~BreakableMo() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
