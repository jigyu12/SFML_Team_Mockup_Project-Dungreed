#include "stdafx.h"
#include "BackgroundMo.h"

BackgroundMo::BackgroundMo(const std::string& name)
	: MapObject(name)
{
}

void BackgroundMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);

	switch (this->type)
	{
	case ObjectData::Type::Box:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Animation", "BoxIcon")));
		break;
	case ObjectData::Type::BigBox:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Animation", "BigBoxIcon")));
		break;
	case ObjectData::Type::OakDrum:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Animation", "OakDrumIcon")));
		break;
	case ObjectData::Type::Table:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Animation", "TableIcon")));
		break;
	case ObjectData::Type::SkullTable:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Animation", "SkullTableIcon")));
		break;
	}
	SetOrigin(Origins::BC);
}