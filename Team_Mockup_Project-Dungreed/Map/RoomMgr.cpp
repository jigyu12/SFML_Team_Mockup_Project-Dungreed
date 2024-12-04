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
	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (auto& room : rooms)
	{
		scene->RemoveGo(room.second);
		room.second->ClearMonsters();
		room.second->Release();
		delete room.second;
	}
	scene->ApplyRemoveGO();
	rooms.clear();

	currentRoom = { 0,0 };

	Room* room = new Room("00room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map","2FBoss"));
	scene->AddGo(room);
	rooms.insert({ { 0,0 }, room });

	room = new Room("10room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom1UL"));
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ { 1,0 }, room });

	room = new Room("1-1room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom2DLR"));
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ { 1,-1 }, room });

	room = new Room("0-1room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom4ULR"));
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ {0,-1 }, room });

	room = new Room("-10room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom5LR"));
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ {-1,0 }, room });

	room = new Room("-20room");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom3UDLR"));
	room->SetActive(false);
	scene->AddGo(room);
	rooms.insert({ {-2,0 }, room });

	rooms[currentRoom]->EnterRoom(HitBoxData::Type::PortalDown);
}

bool RoomMgr::RoomChange(const HitBoxData::Type& portalType)
{
	sf::Vector2i nextroom = currentRoom;
	HitBoxData::Type entertype = HitBoxData::Type::PortalDown;
	switch (portalType)
	{
	case HitBoxData::Type::PortalUp:
		--nextroom.y;
		break;
	case HitBoxData::Type::PortalDown:
		entertype = HitBoxData::Type::PortalUp;
		++nextroom.y;
		break;
	case HitBoxData::Type::PortalLeft:
		entertype = HitBoxData::Type::PortalRight;
		--nextroom.x;
		break;
	case HitBoxData::Type::PortalRight:
		entertype = HitBoxData::Type::PortalLeft;
		++nextroom.x;
		break;
	}
	auto findit = rooms.find(nextroom);

	if (findit == rooms.end())
	{
		return false;
	}

	GetCurrentRoom()->SetActive(false);
	findit->second->SetActive(true);
	findit->second->EnterRoom(entertype);
	currentRoom = nextroom;
	return true;
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
