#pragma once
#include "MapData.h"

class TileMap;
class Player;
class MapObject;
class Monster;

class Room : public GameObject
{
protected:
	sf::Sprite subBackground;

	std::vector<TileMap*> tileMaps;

	MapDataVC mapData;
	std::vector<std::pair<HitBox*, HitBoxData>> hitBoxes;
	std::vector<std::pair<MapObject*, ObjectData>> objects;

	sf::FloatRect viewbounds;

	std::vector<Monster*> monsters;
	Player* player;
	Scene* scene;
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

	void LoadMapData(const std::string& path);
	void SaveMapData(const std::string& path);

	const std::vector<std::pair<HitBox*, HitBoxData>>& GetHitBoxes() const;
	void EnterRoom(HitBoxData::Type connection);
	const sf::FloatRect& GetViewBounds() { return viewbounds; }
};
