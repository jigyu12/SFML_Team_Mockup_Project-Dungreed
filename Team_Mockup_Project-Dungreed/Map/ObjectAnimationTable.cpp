#include "stdafx.h"
#include "ObjectAnimationTable.h"
#include <io.h>
#include "rapidcsv.h"

const ObjectAnimationData ObjectAnimationTable::Undefined = { {"null"},{"null"},"null",sf::FloatRect() };

bool ObjectAnimationTable::Load()
{
	Release();

	if (_access(filePath.c_str(), 0) == -1)
	{
		return false;
	}
	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));

	try
	{
		for (int i = 0; i < doc.GetRowCount(); ++i)
		{
			auto row = doc.GetRow<std::string>(i);
			int id = std::stoi(row[0]);
			auto find = table.find((MapObjectType)id);
			
			if (find == table.end())
			{
				ObjectAnimationData datum;
				datum.names.push_back(row[1]);
				datum.paths.push_back(row[2]);
				datum.defaultTexture = row[3];
				datum.defaultTextureRect = {std::stof(row[4]),std::stof(row[5]),std::stof(row[6]),std::stof(row[7]) };
				table.insert({ (MapObjectType)id,{datum} });
			}
			else
			{
				find->second.names.push_back(row[1]);
				find->second.paths.push_back(row[2]);
			}
		}
	}
	catch (std::ios::failure a)
	{
		return false;
	}
	if (table.size() == 0)
	{
		return false;
	}
	return true;
}

void ObjectAnimationTable::Release()
{
	table.clear();
}

const ObjectAnimationData& ObjectAnimationTable::Get(MapObjectType type)
{
	auto find = table.find(type);
	if (find == table.end())
	{
		return { Undefined };
	}
	return find->second;
}

const std::unordered_map<MapObjectType, ObjectAnimationData>& ObjectAnimationTable::GetTable()
{
	return table;
}
