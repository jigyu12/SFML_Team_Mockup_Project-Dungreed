#pragma once
#include "SaveData.h"

class SaveLoadMgr : public Singleton<SaveLoadMgr>
{
public:
	friend Singleton<SaveLoadMgr>;

private:
	std::string tempSavePath = "save.json";

	SaveLoadMgr() = default;
	~SaveLoadMgr() = default;

public:
	void SaveV1(const SaveDataV1& saveData); // TEST

	void Save(const SaveDataVC& saveData);
	SaveDataVC Load();
};

