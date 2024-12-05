#include "stdafx.h"
#include "DoorMo.h"

DoorMo::DoorMo(const std::string& name)
	: MapObject(name)
{
}

void DoorMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	animator.AddEvent("dooropen", 10, [this]() {SetStatus(Status::Broken);});
	animator.AddEvent("doorclose", 10, [this]() {SetStatus(Status::Broken);});

}

void DoorMo::SetStatus(const Status& status)
{
	this->status = status;
	switch (status)
	{
	case MapObject::Status::Idle:
		animator.Resume();
		break;
	case MapObject::Status::Open:
		animator.Play(RESOURCEID_TABLE->Get("Animation", "DoorOpen"));
		SetOrigin(Origins::BC);
		animator.Stop();
		break;
	case MapObject::Status::Close:
		animator.Play(RESOURCEID_TABLE->Get("Animation", "DoorClose"));
		SetOrigin(Origins::BC);
		animator.Stop();
		break;
	case MapObject::Status::Broken:
		break;
	}
	SetOrigin(Origins::BC);
}
