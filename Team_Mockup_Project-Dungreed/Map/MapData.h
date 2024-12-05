#pragma once

struct TileMapData
{
	std::string texId;
	std::string name;
	sf::Vector2f cellsize;
	sf::Vector2i cellcount;

	std::vector<std::vector<int>> tileIndexes;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TileMapData, texId, name, cellsize, cellcount, tileIndexes)
};

struct ObjectData
{
	enum class Type
	{
		Torch,
		SealStone,
		Door,
		Box,
		BigBox,
		OakDrum,
		SkullTable,
		Table,
		Count,
	};

	Type type;
	sf::Vector2f position;
	float rotation;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectData, type, position, rotation)
};

struct HitBoxData
{
	enum class Type
	{
		PortalUp,
		PortalDown,
		PortalLeft,
		PortalRight,
		Immovable,
		Downable,
		SpawnTrigger,
		Spike,
		Count,
	};

	sf::Vector2f size;
	sf::Vector2f position;
	float rotation = 0.f;

	Type type;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(HitBoxData, size, position, rotation, type)
};

struct SpawnData
{
	Monster::MonsterType type;
	sf::Vector2f position;
	int wave;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SpawnData, type, position, wave)
};

struct RoomData
{
	RoomData();

	enum class Type
	{
		Normal,
		Enter,
		Exit,
		Count,
	};
	Type type;
	std::vector<bool> connection;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RoomData, type, connection)
};

struct MapData
{
public:
	const static int TileMapCount = 3;
	const static int StartPointCount = 4;

	int version = 0;
	virtual MapData* VersionUp() = 0;
};

struct MapDataV1 : public MapData
{
	MapDataV1();

	std::vector<sf::Vector2f> playerStartPoint;
	TileMapData tileMapData;
	std::vector<ObjectData> objectData;
	std::vector<HitBoxData> hitBoxData;


	MapData* VersionUp() override;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV1, version, playerStartPoint, tileMapData, objectData, hitBoxData)
};

struct MapDataV2 : public MapData
{
	MapDataV2();

	std::vector<sf::Vector2f> playerStartPoint;
	TileMapData tileMapData;
	std::vector<ObjectData> objectData;
	std::vector<HitBoxData> hitBoxData;
	std::vector<SpawnData> spawnData;

	MapData* VersionUp() override;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV2, version, playerStartPoint, tileMapData, objectData, hitBoxData, spawnData)
};

struct MapDataV3 : public MapData
{
	MapDataV3();

	std::vector<sf::Vector2f> playerStartPoint;
	std::vector<TileMapData> tileMapData;
	std::vector<ObjectData> objectData;
	std::vector<HitBoxData> hitBoxData;
	std::vector<SpawnData> monsterSpawnData;

	MapData* VersionUp() override;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV3, version, playerStartPoint, tileMapData, objectData, hitBoxData, monsterSpawnData)
};


struct MapDataV4 : public MapData
{
	MapDataV4();

	std::vector<sf::Vector2f> playerStartPoint;
	std::vector<TileMapData> tileMapData;
	std::vector<ObjectData> objectData;
	std::vector<HitBoxData> hitBoxData;
	std::vector<SpawnData> monsterSpawnData;
	RoomData roomData;

	MapData* VersionUp() override { return this; }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV4, version, playerStartPoint, tileMapData, objectData, hitBoxData, monsterSpawnData, roomData)
};

typedef MapDataV4 MapDataVC;

class MapDataLoader
{
protected:
	const static int currentVersion = 4;
	MapDataLoader() = delete;
	~MapDataLoader() = delete;
public:
	static void Save(MapDataVC mapData, const std::string& path = "");
	static void Save(MapDataVC mapData, const std::wstring& path = L"");
	static MapDataVC Load(const std::string& path);
	static MapDataVC Load(const std::wstring& path);
};

