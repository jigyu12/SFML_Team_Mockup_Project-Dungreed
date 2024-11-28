#include "stdafx.h"
#include "MapData.h"
#include <io.h>

void MapDataLoader::Save(MapDataVC mapData, const std::string& path)
{
	if (path == "NULL" && path == "")
	{
		return;
	}

	json j = mapData;
	std::ofstream f("maps/" + path);
	f << j.dump(4) << std::endl;
	f.close();
}

MapDataVC MapDataLoader::Load(const std::string& path)
{
	if (_access(("maps/" + path).c_str(), 0) == -1)
	{
		return MapDataVC();
	}

	std::ifstream f("maps/" + path);
	json j = json::parse(f);

	int version = j["version"];
	MapData* saveData = nullptr;
	switch (version)
	{
	case 1:
	{
		MapDataV1 v1 = j.get<MapDataV1>();
		saveData = new MapDataV1(v1);
		break;
	}
	case 2:
	{
		MapDataV2 v2 = j.get<MapDataV2>();
		saveData = new MapDataV2(v2);
		break;
	}
	}
	f.close();

	while (saveData != nullptr && saveData->version < currentVersion)
	{
		MapData* oldData = saveData;
		saveData = saveData->VersionUp();
		delete oldData;
	}

	MapDataVC mapData = *((MapDataVC*)saveData);
	delete saveData;
	return mapData;
}

MapDataV1::MapDataV1()
{
	version = 1;
	playerStartPoint.resize(4);
}

MapData* MapDataV1::VersionUp()
{
	MapDataV2* newData = new MapDataV2();

	newData->hitBoxData = hitBoxData;
	newData->objectData = objectData;
	newData->playerStartPoint = playerStartPoint;
	newData->tileMapData = tileMapData;

	return newData;
}

MapDataV2::MapDataV2()
{
	version = 2;
	playerStartPoint.resize(4);
}
