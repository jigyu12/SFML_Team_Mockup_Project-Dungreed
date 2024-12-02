#pragma once

struct ObjectAnimationDatum
{
	std::string name;
	std::string path;
};

class ObjectAnimationTable : public DataTable
{
public:
	const static ObjectAnimationDatum Undefined;

protected:
	std::unordered_map<MapObjectType, std::vector<ObjectAnimationDatum>> table;
	std::string filePath = "tables/object_animation_table.csv";

public:
	ObjectAnimationTable() : DataTable(Types::ObjectAnimation) {}
	~ObjectAnimationTable() = default;

	bool Load() override;
	void Release() override;

	const std::vector<ObjectAnimationDatum>& Get(MapObjectType type);
	const std::unordered_map<MapObjectType, std::vector<ObjectAnimationDatum>>& GetTable();
};
