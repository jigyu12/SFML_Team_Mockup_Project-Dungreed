#include "stdafx.h"
#include "RoomMgr.h"
#include "Room.h"
#include "Scene.h"

Room* RoomMgr::GetCurrentRoom()
{
	auto findit = rooms.find(currentRoom);

	if (findit == rooms.end())
	{
		return rooms[{0, 0}];
	}

	return findit->second;
}

void RoomMgr::Reset()
{
	currentRoom = { 0,0 };

	Scene* scene = SCENE_MGR.GetCurrentScene();

	Room* room = new Room("00room");
	room->Init();
	room->Reset();
	room->LoadMapData("maps/1fenter1LR.json");
	scene->AddGo(room);
	rooms.insert({ { 0,0 }, room });

	room = new Room("10room");
	room->Init();
	room->Reset();
	room->LoadMapData("maps/1froom1UL.json");
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ { 1,0 }, room });

	room = new Room("1-1room");
	room->Init();
	room->Reset();
	room->LoadMapData("maps/1froom2DLR.json");
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ { 1,-1 }, room });

	room = new Room("-10room");
	room->Init();
	room->Reset();
	room->LoadMapData("maps/1froom4ULR.json");
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ {-1,0 }, room });
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
	auto findit = rooms.find(nextroom);

	if (findit == rooms.end())
	{
		return;
	}

	GetCurrentRoom()->SetActive(false);
	findit->second->SetActive(true);
	findit->second->EnterRoom(portalType);
	currentRoom = nextroom;
}

void RoomMgr::Start(const std::string& path)
{
	//Scene* scene = SCENE_MGR.GetCurrentScene();

	for (auto& room : rooms)
	{
		//scene->RemoveGo(room.second);
		delete room.second;
	}
	rooms.clear();
}
