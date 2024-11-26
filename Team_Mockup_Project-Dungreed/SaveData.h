#pragma once

struct SaveZombie
{
	int type = 0;
	sf::Vector2f position;
	float rotation;
	sf::Vector2f scale;
	int hp = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveZombie, type, position, rotation, scale, hp)
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

	int highscore = 0;
	SaveData* VersionUp() override;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV1, version, highscore)
};

struct SaveDataV2 : public SaveData
{
	SaveDataV2() { version = 2; }

	int highscore = 0;
	int gold = 100;

	std::vector<SaveZombie> zombies;

	SaveData* VersionUp() override { return nullptr; }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveDataV2, version, highscore, gold, zombies)
};

typedef SaveDataV2 SaveDataVC;
