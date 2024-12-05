#pragma once

#include "MapObject.h"

class BackgroundMo : public MapObject
{
protected:

public:
	BackgroundMo(const std::string& name = "");
	~BackgroundMo() = default;

	void Set(const ObjectData::Type& type) override;
};
