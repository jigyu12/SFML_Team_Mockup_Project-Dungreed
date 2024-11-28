#pragma once

class TileMap;

class UiEditor : public GameObject
{
protected:

	sf::Sprite selectedTile;
	sf::RectangleShape selectwindow;
	TileMap* tileMap;
	std::vector<std::vector<int>> tileIndexes;
	sf::Vector2i selectedtileIndex;

public:
	UiEditor(const std::string& name = "");
	~UiEditor() = default;

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
};

