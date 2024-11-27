#pragma once
#include "MapData.h"

class TileMap;

class Room : public GameObject
{
public:
	enum class HitboxAttribute
	{
		Immovable,
		Downable,
	};

protected:

	TileMap* tileMap;

	MapDataVC mapData;

	std::vector<std::pair<HitBox,HitBoxData>> hitBoxes;

public:
	Room(const std::string& name = "");
	~Room() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	const std::vector<std::pair<HitBox, HitBoxData>> GetHitBoxes() { return hitBoxes; }

	void LoadMapData(const std::string& path);
	void SaveMapData(const std::string& path);
};
