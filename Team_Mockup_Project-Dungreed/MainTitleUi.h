#pragma once
#include "Button.h"
#include "BirdGo.h"

class MainTitleUi :
    public GameObject
{
public:
	enum class Status
	{
		Aniplaytime,
		AniStoptime,
	};



protected:
	Status status;
	
	std::vector<BirdGo> birds;
	//���ηε�
	sf::Sprite mainbody;

	//����ȭ�鱸��
	sf::Sprite backCloud;
	sf::Sprite frontCloud;

	//���� ȭ���ư
	sf::Sprite exitButton;
	sf::Sprite playButton;
	//���� �ΰ�
	sf::Sprite mainLogo;
	
	Button startButton;
	Button continueButton;
	Button editorButton;
	Button endButton;

	Animator animator;

	sf::Vector2f bCloudPos;
	sf::Vector2f fCloudPos;

	float aniTimer = 0.f;
	float frontCloudSpeed = 100.f;
	float backCloudSpeed = 250.f;

public:
	MainTitleUi(const std::string& name = "MainTitleUi");
	~MainTitleUi() = default;

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
	void AniplayertimeUpdate(float dt);
	void AniStopTimeUpdate(float dt);
	void ButtonUpdate(float dt);

	Status GetStatus() const { return status; }
	void SetStatus(Status status);
};

