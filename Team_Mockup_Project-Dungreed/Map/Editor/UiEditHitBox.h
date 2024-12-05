#pragma once

class TileMap;
class UiEditor;
class Button;
class TextGo;

class UiEditHitBox : public GameObject
{
protected:
	enum class HitBoxEditStatus
	{
		Create,
		Move,
	};
	enum class EditStatus
	{
		Hitbox,
		StartPosition,
		Count,
	};

	std::vector<Button*> roomTypeButtons;

	EditStatus editStatus;
	HitBoxEditStatus hitboxStatus;
	HitBoxData::Type hitboxType;

	sf::RectangleShape boxWindow;

	std::vector<Button*> editStatusButtons;
	std::vector<Button*> hitBoxTypeButtons;
	std::vector<Button*> dirButtons;
	std::vector<sf::Vector2f> startPositions;
	std::vector<TextGo*> positionTexts;

	sf::Sprite spawnPoint;

	sf::RectangleShape* selectedHitBox;
	std::unordered_map<sf::RectangleShape*, HitBoxData::Type> hitboxes;
	UiEditor* uieditor;
	sf::Vector2f editStartPos;

	RoomData::Type selectedRoomType;

public:
	UiEditHitBox(const std::string& name = "");
	~UiEditHitBox() = default;

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

	std::vector<HitBoxData> GetHitBoxData() const;
	RoomData GetRoomData() const;
	void SetHitBoxData(const std::vector<HitBoxData>& data);
	void SetRoomData(const RoomData& data);
	void SetEditStatus(const EditStatus& status);
	std::vector<sf::Vector2f> GetStartPoints() { return startPositions; }
	void SetStartPositionData(const std::vector<sf::Vector2f>& points) { startPositions = points; }
	void ClearHitBoxData();
};
