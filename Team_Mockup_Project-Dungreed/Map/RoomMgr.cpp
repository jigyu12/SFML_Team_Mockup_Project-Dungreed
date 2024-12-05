#include "stdafx.h"
#include "RoomMgr.h"
#include "Room.h"
#include "Scene.h"

Room* RoomMgr::GetCurrentRoom()
{
	auto findit = floors.find(currentFloor);

	if (findit == floors.end())
	{
		return nullptr;
	}

	auto findit2 = findit->second.find(currentRoom);

	if (findit2 == findit->second.end())
	{
		return nullptr;
	}

	return findit2->second;
}

void RoomMgr::NextFloor()
{
	GetCurrentRoom()->SetActive(false);
	currentRoom = { 0,0 };
	++currentFloor;
	GetCurrentRoom()->SetActive(true);
	floors[currentFloor][currentRoom]->EnterRoom(HitBoxData::Type::PortalDown);
}

void RoomMgr::Reset()
{
	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (auto& floor : floors)
	{
		for (auto& room : floor.second)
		{
			scene->RemoveGo(room.second);
			room.second->ClearMonsters();
			room.second->Release();
			delete room.second;
		}
		floor.second.clear();
	}
	scene->ApplyRemoveGO();
	floors.clear();

	currentRoom = { 0,0 };
	currentFloor = 1;

	Room* room = new Room("1FEnterLR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FEnterLR"));
	scene->AddGo(room);
	floors[1].insert({ {0,0}, room });

	room = new Room("1FExitL");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FExitL"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[1].insert({ {1,0}, room });

	room = new Room("2FEnter1R");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "2FEnter1R"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[2].insert({ {0,0}, room });

	room = new Room("2FBoss");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "2FBoss"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[2].insert({ {1,0}, room });

	room = new Room("1FRoom2DLR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom2DLR"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[1].insert({ {1,-1}, room });

	room = new Room("1FRoom4ULR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom4ULR"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[1].insert({ {0,-1}, room });

	room = new Room("1FRoom5LR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom5LR"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[1].insert({ {-1,0}, room });

	room = new Room("1FRoom3UDLR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FRoom3UDLR"));
	room->SetActive(false);
	scene->AddGo(room);
	floors[1].insert({ {-2,0}, room });

	FloorData floorData;

	for (const auto& floor : floors)
	{
		for (const auto& datum : floor.second)
		{
			floorData.floors[floor.first][datum.first] = datum.second->GetName();
		}
	}

	json j = floorData;
	std::ofstream f(RESOURCEID_TABLE->Get("Map","FloorData"));
	f << j.dump(4) << std::endl;
	f.close();

	floors[currentFloor][currentRoom]->EnterRoom(HitBoxData::Type::PortalDown);
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
	auto findit = floors.find(currentFloor);

	if (findit == floors.end())
	{
		return false;
	}
	auto findit2 = findit->second.find(nextroom);
	if (findit2 == findit->second.end())
	{
		return false;
	}

	GetCurrentRoom()->SetActive(false);
	findit2->second->SetActive(true);
	findit2->second->EnterRoom(entertype);
	currentRoom = nextroom;
	return true;
}

void RoomMgr::Reset(const std::string& path)
{
	Scene* scene = SCENE_MGR.GetCurrentScene();
	for (auto& floor : floors)
	{
		for (auto& room : floor.second)
		{
			scene->RemoveGo(room.second);
			room.second->ClearMonsters();
			room.second->Release();
			delete room.second;
		}
		floor.second.clear();
	}
	scene->ApplyRemoveGO();
	floors.clear();

	currentRoom = { 0,0 };
	currentFloor = 1;

	std::ifstream f(path);
	json j = json::parse(f);

	FloorData floordata = j.get<FloorData>();
	f.close();

	for (auto& floor : floordata.floors)
	{
		for (auto& datum : floor.second)
		{
			Room* room = new Room("00room");
			room->Init();
			room->Reset();
			room->LoadMapData(RESOURCEID_TABLE->Get("Map", datum.second));
			room->SetActive(false);
			scene->AddGo(room);
			floors[floor.first].insert({ datum.first, room });
		}
	}
	floors[currentFloor][currentRoom]->SetActive(true);
	floors[currentFloor][currentRoom]->EnterRoom(HitBoxData::Type::PortalDown);
}
