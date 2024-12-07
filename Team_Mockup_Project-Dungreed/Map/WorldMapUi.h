#pragma once

class TextGo;

class WorldMapUi : public GameObject
{
public:
	struct RoomIcons
	{
		sf::Vector2i coord;
		RoomData::Type type;
		sf::Sprite roomIcon;
		std::vector<std::pair<int,sf::RectangleShape>> connectors;
		sf::Sprite typeIcon;
	};
protected:

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	std::vector<RoomIcons> rooms;
	TextGo* floorText;
	sf::RectangleShape playerRect;

	float timer;

public:
	WorldMapUi(const std::string& name = "");
	~WorldMapUi() = default;

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
	
	void RefreshData();
};
