#pragma once

struct TileMapData;

class TileMap : public GameObject
{
protected:
	sf::VertexArray va;
	sf::VertexArray gridLine;
	std::string texId;
	sf::Texture* texture;
	sf::Transform transform;

	sf::Vector2i cellCount;
	sf::Vector2f cellSize;

	std::vector<std::vector<int>> tileIndexes;
	bool showGridLine;
public:
	TileMap(const std::string& name = "");
	~TileMap() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void UpdateTransform();

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetTexture(const std::string& texId);
	void Set(const TileMapData& tileMapData);
	void Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<std::vector<int>>& tileData);
	void SetTile(const sf::Vector2f& mousepos, const TileDatum& tile);
	
	TileMapData GetTileMapData();
	sf::Vector2i GetTilePosition(const sf::Vector2f& mousepos);
	int GetTileIndex(const sf::Vector2f& mousepos);

	const sf::Vector2i& GetCellCount() { return cellCount; }
	void Resize(const sf::Vector2i& size);

	bool IsShowGridLine() { return showGridLine; }
	void SetShowGridLine(bool show) { showGridLine = show; }
};
