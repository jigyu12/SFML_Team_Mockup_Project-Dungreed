#pragma once

struct ObjectAnimationData
{
	std::vector<std::string> names;
	std::vector<std::string> paths;
	std::string defaultTexture;
	sf::FloatRect defaultTextureRect;
};

class ObjectAnimationTable : public DataTable
{
public:
	const static ObjectAnimationData Undefined;

protected:
	std::unordered_map<MapObjectType, ObjectAnimationData> table;
	std::string filePath = "tables/object_animation_table.csv";

public:
	ObjectAnimationTable() : DataTable(Types::ObjectAnimation) {}
	~ObjectAnimationTable() = default;

	bool Load() override;
	void Release() override;

	const ObjectAnimationData& Get(MapObjectType type);
	const std::unordered_map<MapObjectType, ObjectAnimationData>& GetTable();
};
