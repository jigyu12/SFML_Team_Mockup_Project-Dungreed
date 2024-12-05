#pragma once
#include "MapObject.h"

class LightGo;

class TorchMo : public MapObject
{
protected:

	LightGo* light;

public:
	TorchMo(const std::string& name = "");
	~TorchMo() = default;

	void SetActive(bool active) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void Set(const ObjectData::Type& type) override;
};
