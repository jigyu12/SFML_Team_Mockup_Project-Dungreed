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
class SceneGame;

struct FloorData
{
	//std::unordered_map<int, std::unordered_map<sf::Vector2i, std::string, Vector2iHash>> floors;
	//
	//NLOHMANN_DEFINE_TYPE_INTRUSIVE(FloorData, floors)

	int minDepth;
	int maxCount;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FloorData, minDepth, maxCount)
};

class RoomMgr : public Singleton<RoomMgr>
{
public:
	friend Singleton<RoomMgr>;
protected:

	std::unordered_map<int, std::unordered_map<sf::Vector2i, Room*, Vector2iHash>> floors;

	std::unordered_map<int, MapDataVC> entranceRooms;
	std::unordered_map<int, MapDataVC> normalRooms;
	std::unordered_map<int, MapDataVC> exitRooms;

	std::vector<sf::Vector2i> dirVector = { {0,-1},{0,1}, {-1,0}, {1,0} };
	std::vector<int> dirFlip = { 1,0,3,2 };
	FloorData floorData;

	SceneGame* scene;

	sf::Vector2i currentRoomCoord;
	Room* currentRoom;
	int currentFloor;

	RoomMgr() = default;
	~RoomMgr() = default;
public:

	void Init();

	void Reset();
	void Start();

	std::unordered_map<sf::Vector2i, Room*, Vector2iHash> CreateFloor();
	void CreateRoom(std::unordered_map<sf::Vector2i, Room*, Vector2iHash>& floor, std::queue<std::pair<sf::Vector2i, int>>& queue, const sf::Vector2i& mother, int dir);

	bool RoomChange(const HitBoxData::Type& portalType);
	Room* GetCurrentRoom();
	void SetCurrentRoom(int floor, sf::Vector2i coord);
	int GetCurrentFloor() { return currentFloor; }
	void NextFloor();
};

#define ROOM_MGR (RoomMgr::Instance())
