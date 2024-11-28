#pragma once

struct TileMapData;

class TileMap : public GameObject
{
protected:
	sf::VertexArray va;
	std::string texId;
	sf::Texture* texture = nullptr;
	sf::Transform transform;

	sf::Vector2i cellCount;
	sf::Vector2f cellSize;

	std::vector<std::vector<int>> tileIndex;

public:
	TileMap(const std::string& name = "");
	~TileMap() = default;

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
	void Draw(sf::RenderWindow& window) override;

	void Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<std::vector<int>>& tileData);
	void Set(const TileMapData& tileMapData);
	void SetTexture(const std::string& texId);
	void SetTile(const sf::Vector2f& mousepos, const TileDatum& tile);
	TileMapData GetTileMapData();
	sf::Vector2i GetTileIndex(const sf::Vector2f& mousepos);
	void UpdateTransform();
};
