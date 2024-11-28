#pragma once

struct DataTile
{
	int index;
	sf::Vector2f startpos;
};


class TileTable : public DataTable
{
public:
	const static DataTile Undefined;

protected:
	std::unordered_map<int, DataTile> table;
	std::string filePath = "tables/tile_table.csv";

public:
	TileTable() : DataTable(Types::Tile) {}
	~TileTable() = default;

	bool Load() override;
	void Release() override;

	const DataTile& Get(int idx);
	const std::unordered_map<int, DataTile>& GetTable();
};

