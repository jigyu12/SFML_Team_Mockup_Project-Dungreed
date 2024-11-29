#pragma once

class TileMap;

class UiEditTile : public GameObject
{
protected:

	sf::Sprite selectedTile;

	sf::Sprite xUpButton;
	sf::Sprite xDownButton;

	sf::Sprite yUpButton;
	sf::Sprite yDownButton;

	TileMap* tileList;
	TileMap* tileMap;

	int selectedTileIndex;
	int pagenumber;

public:
	UiEditTile(const std::string& name = "");
	~UiEditTile() = default;

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

	int GetSelectedTileIndex() { return selectedTileIndex; }
};
