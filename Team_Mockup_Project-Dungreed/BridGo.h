#pragma once
#include "GameObject.h"
class BridGo :
    public GameObject
{
protected:
	Animator animator;

	float speed;

public:
    BridGo(const std::string& name = "");
    virtual ~BridGo() = default;


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
};

