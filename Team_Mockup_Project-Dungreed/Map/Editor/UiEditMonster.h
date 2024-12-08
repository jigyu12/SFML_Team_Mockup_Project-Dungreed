#pragma once

class UiEditor;
class Button;
class TextGo;

class UiEditMonster : public GameObject
{
protected:
	enum class EditStatus
	{
		Create,
		Move,
	};
	EditStatus status;

	sf::RectangleShape boxWindow;

	sf::RectangleShape* selectedMonster;
	std::vector<Button*> monsterList;
	std::unordered_map<sf::RectangleShape*, SpawnData> spawnData;
	UiEditor* uieditor;

	Button* waveUpButton;
	TextGo* waveText;
	Button* waveDownButton;

	sf::Vector2f startPos;

	Monster::MonsterType selectedType;

public:
	UiEditMonster(const std::string& name = "");
	~UiEditMonster() = default;

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

	std::vector<SpawnData> GetSpawnData() const;
	void SetSpawnData(const std::vector<SpawnData>& data);
	void SetSpawnRect(sf::RectangleShape* shape, const Monster::MonsterType& type);
	void ClearSpawnData();
	void UpDownSpawnWave(bool up);
};
