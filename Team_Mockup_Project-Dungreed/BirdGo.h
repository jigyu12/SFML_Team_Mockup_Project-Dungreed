#pragma once
#include "GameObject.h"
class BirdGo :
    public GameObject
{
public:
	enum class Status
	{
		Left,
		Right,
	};

protected:
	Animator animator;
	sf::Sprite body;
	float speed;

	

public:
    BirdGo(const std::string& name = "Bird");
    virtual ~BirdGo() = default;

	Status status;
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetStatus(Status status);
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateLeft(float dt);
	void UpdateRight(float dt);
	void Draw(sf::RenderWindow& window) override;
};

