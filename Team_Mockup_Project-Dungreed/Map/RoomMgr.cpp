#include "stdafx.h"
#include "RoomMgr.h"
#include "Room.h"
#include "Scene.h"

Room* RoomMgr::GetCurrentRoom()
{
	auto findit = rooms.find(currentRoom.y);

	if (findit == rooms.end())
	{
		return rooms[0][0];
	}

	auto findit2 = findit->second.find(currentRoom.x);

	if (findit2 == findit->second.end())
	{
		return rooms[0][0];
	}

	return findit2->second;
}

void RoomMgr::RoomChange(const HitBoxData::Type& portalType)
{
	sf::Vector2i nextroom = currentRoom;
	switch (portalType)
	{
	case HitBoxData::Type::PortalUp:
		--nextroom.y;
		break;
	case HitBoxData::Type::PortalDown:
		++nextroom.y;
		break;
	case HitBoxData::Type::PortalLeft:
		--nextroom.x;
		break;
	case HitBoxData::Type::PortalRight:
		++nextroom.x;
		break;
	}
	auto findit = rooms.find(nextroom.y);

	if (findit == rooms.end())
	{
		return;
	}

	auto findit2 = findit->second.find(nextroom.x);

	if (findit2 == findit->second.end())
	{
		return;
	}
	GetCurrentRoom()->SetActive(false);
	findit2->second->SetActive(true);
	findit2->second->EnterRoom(portalType);
}

void RoomMgr::Load(const std::string& path)
{
	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (auto& vroom : rooms)
	{
		for (auto& room : vroom.second)
		{
			scene->RemoveGo(room.second);
			delete room.second;
		}
	}
	rooms.clear();
	currentRoom = { 0,0 };
}
