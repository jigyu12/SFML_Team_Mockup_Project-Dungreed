#pragma once

#include "MapObject.h"

class DoorMo : public MapObject
{
protected:

public:
	DoorMo(const std::string& name = "");
	~DoorMo() = default;

	void Set(const ObjectData::Type& type) override;
	void SetStatus(const Status& status) override;
};
