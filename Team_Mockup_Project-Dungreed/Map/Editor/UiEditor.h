#pragma once

class TileMap;
class UiEditTile;
class UiEditHitBox;

class UiEditor : public GameObject
{
protected:
	enum class GroupBox
	{
		Tile,
		HitBox,
	};
	GroupBox currentGroupBox;

	sf::RectangleShape editorWindow;

	sf::Sprite saveButton;
	sf::Sprite loadButton;

	std::vector<TileMap*> editingTileMaps;

	int selectedTile;

	UiEditTile* uiEditTile;
	UiEditHitBox* uiEditHitBox;

public:
	UiEditor(const std::string& name = "");
	~UiEditor() = default;

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

	void SetSelectedTileLayer(int tilenum);
	int GetSelectedTileLayer() { return selectedTile; }
	TileMap* GetSelectedTileMap() { return selectedTile < 0 ? editingTileMaps[0] : editingTileMaps[selectedTile]; }

	void ResizeTileMaps(const sf::Vector2i& size);
};

