#pragma once

struct TileMapData
{
	std::string texId;
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
	enum class Type
	{
		PortalUp,
		PortalDown,
		PortalLeft,
		PortalRight,
		Immovable,
		Downable,
	};

	sf::Vector2f size;
	sf::Vector2f origin;
	float rotation = 0.f;

	Type type;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(HitBoxData, size, origin, rotation, type)
};

struct MapData
{
public:
	enum class Direction {
		Up,
		Down,
		Left,
		Right,
	};

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

	MapData* VersionUp() override { return nullptr; }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapDataV1, version, playerStartPoint, tileMapData, objectData, hitBoxData)
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

