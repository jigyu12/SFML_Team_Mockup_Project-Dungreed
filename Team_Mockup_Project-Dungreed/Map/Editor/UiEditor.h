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

	TileMap* editingTileMap;

	sf::Sprite saveButton;
	sf::Sprite loadButton;

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
};

