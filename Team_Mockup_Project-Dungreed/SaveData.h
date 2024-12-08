#pragma once

struct SavePlayerStatus
{
	float attackDamage;
	int level;
	float criticalDamage;
	float exp;
	float armor;
	float armorPercent;
	float movementSpeed;
	float criticalPercent;
	float dashDamage;
	float attackSpeed;
	int lastFloor;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE
	(SavePlayerStatus, 
		attackDamage, level,
		criticalDamage, criticalPercent,
		exp, armor,armorPercent,
		movementSpeed,criticalPercent,
		dashDamage,attackSpeed, lastFloor);
};

struct SaveData
{
public:
	int version = 0;
	virtual SaveData* VersionUp() = 0;
};

struct SaveDataV1 : public SaveData
{
public:
	SaveDataV1() { version = 1; }
	SavePlayerStatus status;
	SaveData* VersionUp() override;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV1, version, status)
};

struct SaveDataV2 : public SaveData
{
	SaveDataV2() { version = 2; }

	

	SavePlayerStatus status;

	SaveData* VersionUp() override { return nullptr; }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV2, version, status )
};

typedef SaveDataV1 SaveDataVC;
