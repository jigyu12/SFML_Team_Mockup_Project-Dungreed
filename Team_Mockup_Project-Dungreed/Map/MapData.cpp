#include "stdafx.h"
#include "MapData.h"
#include <io.h>

void MapDataLoader::Save(MapDataVC mapData, const std::string& path)
{
	Save(mapData, std::wstring().assign(path.begin(), path.end()));
}

void MapDataLoader::Save(MapDataVC mapData, const std::wstring& path)
{
	if (path == L"NULL" && path == L"")
	{
		return;
	}

	json j = mapData;
	std::ofstream f(path);
	f << j.dump(4) << std::endl;
	f.close();
}

MapDataVC MapDataLoader::Load(const std::string& path)
{
	return Load(std::wstring().assign(path.begin(), path.end()));
}

MapDataVC MapDataLoader::Load(const std::wstring& path)
{
	if (_access(std::string().assign(path.begin(), path.end()).c_str(), 0) == -1)
	{
		return MapDataVC();
	}
	std::ifstream f(path);
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
	case 3:
	{
		MapDataV3 v3 = j.get<MapDataV3>();
		saveData = new MapDataV3(v3);
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

MapData* MapDataV2::VersionUp()
{
	MapDataV3* newData = new MapDataV3();

	newData->hitBoxData = hitBoxData;
	newData->objectData = objectData;
	newData->playerStartPoint = playerStartPoint;
	newData->tileMapData[1] = tileMapData;

	newData->tileMapData[0].cellcount = tileMapData.cellcount;
	newData->tileMapData[0].cellsize = tileMapData.cellsize;
	newData->tileMapData[0].texId = tileMapData.texId;
	newData->tileMapData[0].tileIndexes.resize(tileMapData.tileIndexes.size(), std::vector<int>(tileMapData.tileIndexes[0].size(), 0));
	newData->tileMapData[2].cellcount = tileMapData.cellcount;
	newData->tileMapData[2].cellsize = tileMapData.cellsize;
	newData->tileMapData[2].texId = tileMapData.texId;
	newData->tileMapData[2].tileIndexes.resize(tileMapData.tileIndexes.size(), std::vector<int>(tileMapData.tileIndexes[0].size(), 0));

	newData->monsterSpawnData = spawnData;

	return newData;
}

MapDataV3::MapDataV3()
{
	version = 3;
	tileMapData.resize(TileMapCount);
	playerStartPoint.resize(StartPointCount);
}
