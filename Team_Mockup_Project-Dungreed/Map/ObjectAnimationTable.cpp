#include "stdafx.h"
#include "ObjectAnimationTable.h"
#include <io.h>
#include "rapidcsv.h"

const ObjectAnimationDatum ObjectAnimationTable::Undefined = { "idle","animations/map/platformidle.csv" };

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
			ObjectAnimationDatum datum;
			datum.name = row[1];
			datum.path = row[2];
			if (find == table.end())
			{
				table.insert({ (MapObjectType)id,{datum} });
			}
			else
			{
				find->second.push_back(datum);
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

const std::vector<ObjectAnimationDatum>& ObjectAnimationTable::Get(MapObjectType type)
{
	auto find = table.find(type);
	if (find == table.end())
	{
		return { Undefined };
	}
	return find->second;
}

const std::unordered_map<MapObjectType, std::vector<ObjectAnimationDatum>>& ObjectAnimationTable::GetTable()
{
	return table;
}
