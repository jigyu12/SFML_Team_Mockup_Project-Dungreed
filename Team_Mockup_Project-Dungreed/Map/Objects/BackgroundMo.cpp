#include "stdafx.h"
#include "BackgroundMo.h"

BackgroundMo::BackgroundMo(const std::string& name)
	: MapObject(name)
{
}

void BackgroundMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", ObjectData::ToString(type) + "Icon")));
	SetOrigin(Origins::BC);
}