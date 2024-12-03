#pragma once

class TileMap;
class Button;
class TextGo;
class UiEditTile;
class UiEditHitBox;
class UiEditMonster;
class UiEditObject;

class UiEditor : public GameObject
{
protected:
	enum class GroupBox
	{
		Tile,
		HitBox,
		Monster,
		Object,
	};
	GroupBox selectedGroupBox;

	sf::RectangleShape editorWindow;

	Button* buttonNew;
	Button* buttonSave;
	Button* buttonLoad;

	Button* buttonTile;
	Button* buttonHitbox;
	Button* buttonMonster;
	Button* buttonObject;

	TextGo* selectedFileName;

	std::vector<TileMap*> editingTileMaps;

	int selectedTile;

	UiEditTile* uiEditTile;
	UiEditHitBox* uiEditHitBox;
	UiEditMonster* uiEditMonster;
	UiEditObject* uiEditObject;

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

	void ChangeGroupbox(const UiEditor::GroupBox& selectedGroupBox);

	void NewFile();
	void SaveFile();
	void LoadFile();

};

