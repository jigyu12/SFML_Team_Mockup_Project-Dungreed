#include "stdafx.h"
#include "SealStoneMo.h"

SealStoneMo::SealStoneMo(const std::string& name)
	: MapObject(name)
{
}

void SealStoneMo::Update(float dt)
{
	MapObject::Update(dt);

	hitbox.UpdateTr(body, { 1,2,64,16 });
	hitbox.rect.setOutlineColor(sf::Color::Red);
}

void SealStoneMo::Draw(sf::RenderWindow& window)
{
	MapObject::Draw(window);
	hitbox.Draw(window);
}

void SealStoneMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	animator.AddEvent("sealstoneopen", 7, [this]() {SetStatus(Status::Broken);});
}

void SealStoneMo::SetStatus(const Status& status)
{
	this->status = status;
	switch (status)
	{
	case MapObject::Status::Idle:
		SetActive(false);
		break;
	case MapObject::Status::Open:
		animator.Play(RESOURCEID_TABLE->Get("Animation", "SealStoneOpen"));
		break;
	case MapObject::Status::Close:
		SetActive(true);
		animator.Play(RESOURCEID_TABLE->Get("Animation", "SealStoneClose"));
		animator.PlayQueue(RESOURCEID_TABLE->Get("Animation", "SealStoneIdle"));
		SetOrigin(originPreset);
		break;
	case MapObject::Status::Broken:
		SetActive(false);
		break;
	}
	SetOrigin(Origins::BC);
}
