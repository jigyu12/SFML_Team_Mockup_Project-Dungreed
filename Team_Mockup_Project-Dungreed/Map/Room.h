#pragma once

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
	std::vector<std::pair<Monster*, SpawnData>> monsters;

	sf::FloatRect viewbounds;
	sf::Vector2f subBGCenter;

	Player* player;
	Scene* scene;

	HitBoxData::Type enteredPortal;

	int wave;
	bool cleared;

public:
	Room(const std::string& name = "");
	~Room() = default;

	void SetActive(bool active) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void LoadMapData(const std::string& path);
	void SetMapData(const MapDataVC& mapData);

	const sf::Vector2f& GetSubBGCenter() { return subBGCenter; }
	const sf::FloatRect& GetViewBounds() { return viewbounds; }

	std::vector<std::pair<HitBox*, HitBoxData>> GetHitBoxes() const;
	std::vector<Monster*> GetMonsters() const;
	std::vector<MapObject*> GetBreakableObjects() const;

	std::vector<bool> GetConnections() { return mapData.roomData.connection; }

	void SetViewBounds();

	bool isCleared() { return cleared; }
	void EnterRoom(HitBoxData::Type connection);
	void ClearMonsters();

};
