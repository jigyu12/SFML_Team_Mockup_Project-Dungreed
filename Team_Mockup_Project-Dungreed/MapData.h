#pragma once

struct GateData
{
	sf::FloatRect up;
	sf::FloatRect down;
	sf::FloatRect left;
	sf::FloatRect right;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GateData, up, down, left, right)
};

struct PlayerSpawnData
{
	sf::Vector2f up;
	sf::Vector2f down;
	sf::Vector2f left;
	sf::Vector2f right;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerSpawnData, up, down, left, right)
};

struct TileMapData
{
	std::string texId ;
	std::string name;
	sf::Vector2f cellsize;
	sf::Vector2i cellcount;

	std::vector<sf::Vector2f> tile;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TileMapData, texId, name, cellsize, cellcount, tile)
};

struct ObjectData
{
	int type;
	sf::Vector2f position;
	int origin;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectData, type, position, origin)
};

struct HitBoxData
{
	sf::Vector2f size;
	sf::Vector2f origin;
	float rotation;

	int type;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(HitBoxData, size, origin, rotation,type)
};

struct MapData
{
public:
	int version = 0;
	virtual MapData* VersionUp() = 0;
};

struct MapDataV1 : public MapData
{
	MapDataV1() { version = 1; }

	GateData gateData;
	PlayerSpawnData playerSpawnData;
	TileMapData tileMapData;
	std::vector<ObjectData> objectData;
	std::vector<HitBoxData> hitBoxData;

	MapData* VersionUp() override { return nullptr; }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV1, version, gateData, playerSpawnData, tileMapData, objectData, hitBoxData)
};

typedef MapDataV1 MapDataVC;

class MapDataLoader
{
	const static int currentVersion = 1;
protected:
	MapDataVC mapData;
	std::string path = "NULL";
public:
	void Save(const std::string& path = "");
	bool Load(const std::string& path);

	MapDataVC& Get() { return mapData; }
	const MapDataVC& Get() const { return mapData; }
};

