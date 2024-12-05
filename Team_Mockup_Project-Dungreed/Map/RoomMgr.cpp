#include "stdafx.h"
#include "RoomMgr.h"
#include "Room.h"
#include "SceneGame.h"

Room* RoomMgr::GetCurrentRoom()
{
	return currentRoom;
}

void RoomMgr::SetCurrentRoom(int floor, sf::Vector2i coord)
{
	currentFloor = floor;
	currentRoomCoord = coord;
	auto findit = floors.find(currentFloor);

	if (findit == floors.end())
	{
		currentRoom = nullptr;
		return;
	}

	auto findit2 = findit->second.find(currentRoomCoord);

	if (findit2 == findit->second.end())
	{
		currentRoom = nullptr;
		return;
	}
	currentRoom = findit2->second;
}

void RoomMgr::NextFloor()
{
	scene->ClearTookObject();
	currentRoom->SetActive(false);
	SetCurrentRoom(++currentFloor, { 0,0 });
	currentRoom->SetActive(true);
	currentRoom->EnterRoom(HitBoxData::Type::PortalDown);
}

void RoomMgr::Reset()
{
	scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
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
	scene->ClearTookObject();
	floors.clear();

	Room* room = new Room("1FEnterLR");
	room->Init();
	room->Reset();
	room->LoadMapData(RESOURCEID_TABLE->Get("Map", "1FEnterLR"));
	scene->AddGo(room);
	room->SetActive(true);
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
	SetCurrentRoom(1, { 0,0 });

	json j = floorData;
	std::ofstream f(RESOURCEID_TABLE->Get("Map", "FloorData"));
	f << j.dump(4) << std::endl;
	f.close();

	currentRoom->EnterRoom(HitBoxData::Type::PortalDown);
}

bool RoomMgr::RoomChange(const HitBoxData::Type& portalType)
{
	sf::Vector2i nextroom = currentRoomCoord;
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

	scene->ClearTookObject();

	currentRoom->SetActive(false);

	currentRoom = findit2->second;
	currentRoomCoord = nextroom;

	currentRoom->SetActive(true);
	currentRoom->EnterRoom(entertype);

	return true;
}

void RoomMgr::Reset(const std::string& path)
{
	scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
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
	scene->ClearTookObject();
	floors.clear();

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

	SetCurrentRoom(1, { 0,0 });

	currentRoom->SetActive(true);
	currentRoom->EnterRoom(HitBoxData::Type::PortalDown);
}
