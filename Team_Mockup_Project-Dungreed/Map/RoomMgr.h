#pragma once
#include "MapData.h"

class Room;

struct RoomData
{
	std::unordered_map<int,std::unordered_map<int, std::string>> roomCoord;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RoomData, roomCoord)
};


class RoomMgr : public Singleton<RoomMgr>
{
public:
	friend Singleton<RoomMgr>;
protected:

	std::unordered_map<int,std::unordered_map<int, Room*>> rooms;

	sf::Vector2i currentRoom;

	RoomMgr() = default;
	~RoomMgr() = default;
public:

	void RoomChange(const HitBoxData::Type& portalType);
	void Load(const std::string& path);
	Room* GetCurrentRoom();
};

#define ROOM_MGR (RoomMgr::Instance())
