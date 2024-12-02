#pragma once

class TileMap;
class UiEditor;

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

	sf::RectangleShape* selectedHitBox;
	std::unordered_map<sf::RectangleShape*, HitBoxData::Type> hitboxes;
	UiEditor* uieditor;
	sf::Vector2f startPos;
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
	void SetHitBoxData(const std::vector<HitBoxData>& data);
	void ClearHitBoxData();
};
