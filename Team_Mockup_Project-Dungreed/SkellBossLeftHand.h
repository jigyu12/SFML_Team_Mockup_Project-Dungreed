#pragma once

#include "Monster.h"

class SkellBossLeftHand : public Monster
{
public:
	enum class SkellBossLeftHandState
	{
		None = -1,

		Idle,
		AttackLaser,
		Death,

		Count
	};

protected:
	SkellBossLeftHandState state;

	Animator animator;

public:
	SkellBossLeftHand(const std::string& name = "SkellBossLeftHand");
	virtual ~SkellBossLeftHand() = default;

	virtual void SetOrigin(Origins preset) override;
	virtual void SetOrigin(const sf::Vector2f& newOrigin) override;
	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetRotation(float angle) override;
	virtual void SetScale(const sf::Vector2f& scale) override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void LateUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	void SetStatus(SkellBossLeftHandState state);
	void UpdateIdle(float dt);
	void UpdateAttackLaser(float dt);
	void UpdateDeath(float dt);

	virtual sf::FloatRect GetLocalBounds() const
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}
};