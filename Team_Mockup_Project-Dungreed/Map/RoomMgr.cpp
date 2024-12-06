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

void RoomMgr::Init()
{
	int count = 0;
	std::string path = RESOURCEID_TABLE->Get("Map", "1FEnter0");
	while (path != "")
	{
		MapDataVC mapdata = MapDataLoader::Load(path);
		int dir = 0;
		for (int i = 0; i < mapdata.roomData.connection.size();++i)
		{
			dir += mapdata.roomData.connection[i] ? 0x01 << i : 0;
		}
		entranceRooms.insert({ dir, mapdata });
		path = RESOURCEID_TABLE->Get("Map", "1FEnter" + std::to_string(++count));
	}
	count = 0;
	path = RESOURCEID_TABLE->Get("Map", "1FRoom0");
	while (path != "")
	{
		MapDataVC mapdata = MapDataLoader::Load(path);
		int dir = 0;
		for (int i = 0; i < mapdata.roomData.connection.size();++i)
		{
			dir += mapdata.roomData.connection[i] ? 0x01 << i : 0;
		}
		normalRooms.insert({ dir, mapdata });
		path = RESOURCEID_TABLE->Get("Map", "1FRoom" + std::to_string(++count));
	}
	count = 0;
	path = RESOURCEID_TABLE->Get("Map", "1FExit0");
	while (path != "")
	{
		MapDataVC mapdata = MapDataLoader::Load(path);
		int dir = 0;
		for (int i = 0; i < mapdata.roomData.connection.size();++i)
		{
			dir += mapdata.roomData.connection[i] ? 0x01 << i : 0;
		}
		exitRooms.insert({ dir, mapdata });
		path = RESOURCEID_TABLE->Get("Map", "1FExit" + std::to_string(++count));
	}
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


	SetCurrentRoom(1, { 0,0 });

	FloorData floorData;
	floorData.maxCount = 20;
	floorData.minDepth = 4;
	json j = floorData;
	std::ofstream f(RESOURCEID_TABLE->Get("Map", "FloorData"));
	f << j.dump(4) << std::endl;
	f.close();

	currentRoom->EnterRoom(HitBoxData::Type::PortalDown);
}

void RoomMgr::Start()
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

	std::ifstream f(RESOURCEID_TABLE->Get("Map", "FloorData"));
	json j = json::parse(f);

	FloorData floordata = j.get<FloorData>();
	f.close();

	floors.insert({ 1,CreateFloor() });

	Room* room = new Room("2FEnter1R");
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

	SetCurrentRoom(1, { 0,0 });

	currentRoom->SetActive(true);
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


std::unordered_map<sf::Vector2i, Room*, Vector2iHash> RoomMgr::CreateFloor()
{
	std::unordered_map<sf::Vector2i, Room*, Vector2iHash> floor;
	std::queue<std::pair<sf::Vector2i, int>> createQueue;
	std::vector<std::pair<sf::Vector2i, int>> deadends;
	Room* room = new Room();
	room->Init();
	room->Reset();
	MapDataVC mapdata = entranceRooms[Utils::RandomRange(0, entranceRooms.size())];
	room->SetMapData(mapdata);
	room->SetActive(false);
	scene->AddGo(room);
	floor.insert({ {0,0} ,room });

	for (int i = 0;i < mapdata.roomData.connection.size();++i)
	{
		if (mapdata.roomData.connection[i])
		{
			createQueue.push({ {0,0} ,i });
		}
	}

	while (createQueue.size() > 0)
	{
		const auto& pair = createQueue.front();
		CreateRoom(floor, createQueue, pair.first, pair.second);
		createQueue.pop();
	}

	for (const auto& room : floor)
	{
		std::vector<bool> vec = room.second->GetConnections();
		int count = 0;
		int dir = 0;
		for (int i = 0;i < vec.size();++i)
		{
			if (vec[i])
			{
				++count;
				dir = i;
			}
		}
		if (count == 1)
		{
			deadends.push_back({ room.first, dir });
		}
	}
	std::pair<sf::Vector2i, int> deadend = deadends[Utils::RandomRange(0, deadends.size() - 1)];

	floor[deadend.first]->SetMapData(exitRooms[deadend.second]);

	return floor;
}

void RoomMgr::CreateRoom(std::unordered_map<sf::Vector2i, Room*, Vector2iHash>& floor, std::queue<std::pair<sf::Vector2i, int>>& queue, const sf::Vector2i& mother, int dir)
{
	sf::Vector2i me = mother + dirVector[dir];
	dir = dirFlip[dir];
	std::bitset<4> createroom;

	bool ok = false;

	while (!ok)
	{
		int pathcount = 0;
		for (int i = 0; i < 4;++i)
		{
			createroom[i] = false;
		}
		for (int i = 0; i < 4;++i)
		{
			if (i == dir
				|| Utils::RandomRange(0, 1) != 0)
			{
				continue;
			}
			auto it = floor.find(me + dirVector[i]);
			if (it != floor.end())
			{
				continue;
			}
			createroom[i] = true;
			++pathcount;
		}
		if (std::abs(me.x) + std::abs(me.y) < floorData.minDepth && pathcount == 0)
		{
			continue;
		}
		ok = true;
	}
	Room* room = new Room();
	room->Init();
	room->Reset();
	if (floor.size() > floorData.maxCount)
	{
		room->SetMapData(normalRooms[0x01 << dir]);
	}
	else
	{
		room->SetMapData(normalRooms[(int)createroom.to_ulong()]);
		for (int i = 0; i < 4;++i)
		{
			if (createroom[i])
			{
				queue.push({ me,i });
			}
		}
	}
	room->SetActive(false);
	scene->AddGo(room);
	floor.insert({ me ,room });
}

