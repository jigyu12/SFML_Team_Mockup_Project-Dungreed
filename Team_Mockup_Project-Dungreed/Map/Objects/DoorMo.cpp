#include "stdafx.h"
#include "DoorMo.h"

DoorMo::DoorMo(const std::string& name)
	: MapObject(name)
{
}

void DoorMo::SetPosition(const sf::Vector2f& pos)
{
	MapObject::SetPosition(pos);
	key.setPosition(body.getTransform().transformPoint({ 22,11 }));
}

void DoorMo::Update(float dt)
{
	MapObject::Update(dt);

	timer += dt;
	if (timer > 0.5f
		&& status == MapObject::Status::Close
		&& !animator.IsPlaying())
	{
		animator.Resume();
	}
	hitbox.UpdateTr(body, body.getLocalBounds());
	if (player != nullptr)
	{
		collide = Utils::CheckCollision(hitbox, player->GetHitBox());
	}
	if (collide
		&& InputMgr::GetKeyDown(sf::Keyboard::E)
		&& status == MapObject::Status::Open)
	{
		SetStatus(MapObject::Status::Broken);
		ROOM_MGR.NextFloor();
	}
}

void DoorMo::Draw(sf::RenderWindow& window)
{
	MapObject::Draw(window);
	if (status == MapObject::Status::Open
		&& collide)
	{
		window.draw(key);
	}
}

void DoorMo::Reset()
{
	MapObject::Reset();
	body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "DoorIcon")));
	SetOrigin(Origins::BC);
	SetPosition(position);
	key.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "KeyboardEIcon")));
}

void DoorMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	animator.AddEvent("doorclose", 10, [this]() {SetStatus(Status::Broken);});
	player = SCENE_MGR.GetCurrentScene()->FindGo("Player");
}

void DoorMo::SetStatus(const Status& status)
{
	this->status = status;
	switch (status)
	{
	case MapObject::Status::Idle:
		break;
	case MapObject::Status::Open:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "DoorIcon")));
		break;
	case MapObject::Status::Close:
		animator.Play(RESOURCEID_TABLE->Get("Animation", "DoorClose"));
		SetOrigin(Origins::BC);
		timer = 0.f;
		animator.Stop();
		break;
	case MapObject::Status::Broken:
		break;
	}
	SetOrigin(Origins::BC);
	SetPosition(position);
}
