#pragma once

class ResourceIdTable : public DataTable
{
public:
	const static std::string Undefined;

protected:
	// <T1, T2>
	//	T1: 스트링 아이디
	//	T2: 언어별 스트링 ([0] 한국어 [1] 영어 ...)
	std::unordered_map<std::string, std::string> table;
	std::string filePath = "tables/resourceid_table.csv";

public:
	ResourceIdTable() : DataTable(DataTable::Types::ResourceId) {};
	~ResourceIdTable() = default;

	bool Load() override;
	void Release() override;

	const std::string& Get(const std::string& id);
};


