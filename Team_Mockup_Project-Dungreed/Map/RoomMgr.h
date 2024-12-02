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

struct RoomData
{
	std::unordered_map<sf::Vector2i, std::string, Vector2iHash> roomCoord;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RoomData, roomCoord)
};

class RoomMgr : public Singleton<RoomMgr>
{
public:
	friend Singleton<RoomMgr>;
protected:

	std::unordered_map<sf::Vector2i, Room*, Vector2iHash> rooms;

	sf::Vector2i currentRoom;

	RoomMgr() = default;
	~RoomMgr() = default;
public:

	void Reset();

	bool RoomChange(const HitBoxData::Type& portalType);
	void Start(const std::string& path);
	Room* GetCurrentRoom();
};

#define ROOM_MGR (RoomMgr::Instance())
