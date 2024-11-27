#pragma once
#include "MapData.h"

class TileMap;
class Player;

class Room : public GameObject
{
protected:

	TileMap* tileMap;

	MapDataVC mapData;

	std::vector<std::pair<HitBox*, HitBoxData>> hitBoxes;

	std::vector<sf::Sprite> objects;

	std::function<void()> change;

	Player* player;

	std::vector<Room*> connectedRoom;
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

	void SetConnectedRoom(Room* room, HitBoxData::Type connection);

	const std::vector<std::pair<HitBox*, HitBoxData>>& GetHitBoxes() const;
	void EnterPortal(HitBox* portal);
	void ExitPortal(HitBoxData::Type connection);
};
