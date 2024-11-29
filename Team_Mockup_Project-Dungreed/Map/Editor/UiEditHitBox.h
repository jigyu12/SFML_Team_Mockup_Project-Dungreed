#pragma once

#include "MapData.h"

class TileMap;

class UiEditHitBox : public GameObject
{
protected:
	enum class EditStatus
	{
		Create,
		MoveOld,
		ResizeOld,
	};

	EditStatus status;

	sf::RectangleShape boxWindow;
	TileMap* editingTileMap;
	sf::RectangleShape* selectedHitBox;
	std::unordered_map<sf::RectangleShape*, HitBoxData::Type> hitboxes;
	sf::Vector2f startPos;
public:
	UiEditHitBox(const std::string& name = "");
	~UiEditHitBox() = default;

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

	std::vector<HitBoxData> GetHitBoxData() const;
	void SetHitBoxData(const std::vector<HitBoxData>& data);
};
