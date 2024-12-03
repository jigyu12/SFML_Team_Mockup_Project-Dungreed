#pragma once

class ResourceIdTable : public DataTable
{
public:
	const static std::string Undefined;

protected:
	// <T1, T2>
	//	T1: ��Ʈ�� ���̵�
	//	T2: �� ��Ʈ�� ([0] �ѱ��� [1] ���� ...)
	std::unordered_map<std::string, std::string> table;
	std::string filePath = "tables/resourceid_table.csv";

public:
	ResourceIdTable() : DataTable(DataTable::Types::ResourceId) {};
	~ResourceIdTable() = default;

	bool Load() override;
	void Release() override;

	const std::string& Get(const std::string& id);
};


