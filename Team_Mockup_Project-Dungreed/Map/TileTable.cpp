#include "stdafx.h"
#include "TileTable.h"
#include <io.h>
#include "rapidcsv.h"

const DataTile TileTable::Undefined = { -1, {208.f,32.f} };

bool TileTable::Load()
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
			auto find = table.find(id);
			if (find == table.end())
			{
				DataTile dataTile;
				dataTile.index = id;
				dataTile.startpos.x = std::stof(row[1]);
				dataTile.startpos.y = std::stof(row[2]);
				table.insert({ id,dataTile });
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

void TileTable::Release()
{
	table.clear();
}

const DataTile& TileTable::Get(int idx)
{
	auto find = table.find(idx);
	if (find == table.end())
	{
		return Undefined;
	}
	return find->second;
}

const std::unordered_map<int, DataTile>& TileTable::GetTable()
{
	return table;
}
