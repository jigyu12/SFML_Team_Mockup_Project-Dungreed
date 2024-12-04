#pragma once

struct Vector2iHash
{
	std::size_t operator()(const sf::Vector2i& ev) const
	{
		std::size_t hStr = std::hash<int>()(ev.x);
		std::size_t hInt = std::hash<int>()(ev.y);
		return hStr ^ (hInt << 1);
	}
};

class Room;

struct FloorData
{
	std::unordered_map<int, std::unordered_map<sf::Vector2i, std::string, Vector2iHash>> floors;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FloorData, floors)
};

class RoomMgr : public Singleton<RoomMgr>
{
public:
	friend Singleton<RoomMgr>;
protected:

	std::unordered_map<int, std::unordered_map<sf::Vector2i, Room*, Vector2iHash>> floors;

	sf::Vector2i currentRoom;
	int currentFloor;

	RoomMgr() = default;
	~RoomMgr() = default;
public:

	void Reset();
	void Reset(const std::string& path);

	bool RoomChange(const HitBoxData::Type& portalType);
	Room* GetCurrentRoom();
	int GetCurrentFloor() { return currentFloor; }
	void NextFloor();
};

#define ROOM_MGR (RoomMgr::Instance())
