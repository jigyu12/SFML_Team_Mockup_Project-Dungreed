#pragma once

class TileMap;
class UiEditor;

class UiEditTile : public GameObject
{
protected:
	enum class UiType
	{
		Layer,
		Page,
		CountX,
		CountY,
		TypeCount,
	};

	sf::RectangleShape boxWindow;

	std::vector<sf::Sprite> upButtons;
	std::vector<sf::Text> buttonTexts;
	std::vector<sf::Sprite> downButtons;

	TileMap* selectedTile;
	sf::Text selectedTileText;

	std::vector<TileMap*> tileList;
	TileMap* editingTileMap;
	
	UiEditor* uieditor;

	int selectedTileIndex;
	int selectedPage;
	int currentPage;
	int pagecount;

public:
	UiEditTile(const std::string& name = "");
	~UiEditTile() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void SetSize(const sf::Vector2f& size);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	int GetSelectedTileIndex() const { return selectedTileIndex; }
};
