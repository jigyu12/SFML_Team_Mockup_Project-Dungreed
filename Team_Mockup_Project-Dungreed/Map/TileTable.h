#pragma once

struct TileDatum
{
	int index;
	sf::Vector2f startpos;
};


class TileTable : public DataTable
{
public:
	const static TileDatum Undefined;

protected:
	std::unordered_map<int, TileDatum> table;
	std::string filePath = "tables/tile_table.csv";

public:
	TileTable() : DataTable(Types::Tile) {}
	~TileTable() = default;

	bool Load() override;
	void Release() override;

	const TileDatum& Get(int idx);
	const std::unordered_map<int, TileDatum>& GetTable();
};

