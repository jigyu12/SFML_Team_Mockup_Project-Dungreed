#pragma once
class DataTable
{
public:
	enum class Types
	{
		String,
		ResourceId,
		Tile,
		Count
	};

protected:
	Types type;

public:
	DataTable(Types t) : type(t) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;
};

