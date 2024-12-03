#include "stdafx.h"
#include "ResourceIdTable.h"
#include <io.h>
#include "rapidcsv.h"

const std::string ResourceIdTable::Undefined = "";

bool ResourceIdTable::Load()
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
			auto find = table.find(row[0]);
			if (find == table.end())
			{
				std::unordered_map<std::string, std::string> map;
				map.insert({ row[1],row[2] });
				table.insert({ row[0],map });
			}
			else
			{
				auto find2 = find->second.find(row[1]);
				if (find2 == find->second.end())
				{
					find->second.insert({ row[1],row[2] });
				}
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

void ResourceIdTable::Release()
{
	//table.clear();
}

const std::string& ResourceIdTable::Get(const std::string& type, const std::string& idx)
{
	auto find = table.find(type);
	if (find == table.end())
	{
	return Undefined;
	}
	auto find2 = find->second.find(idx);
	if (find2 == find->second.end())
	{
		return Undefined;
	}
	return find2->second;
}
