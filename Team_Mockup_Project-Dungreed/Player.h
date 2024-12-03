#pragma once

#include "Character.h"

class Monster;
class PlayerUi;

class Weapon;

class Player : public Character
{
public:
	enum class Status
	{
		Ground,
		Jump,
		DownJump,
		Dash,
		Dead,
	};

protected:
	Status status;
	Weapon* weaponSlot1;
	Weapon* weaponSlot2;

	sf::Vector2f velocity;
	sf::Vector2f look;
	
	PlayerUi* playerui;
	sf::Color originalPlayerColor;
	
	HitBox* DownPlatform;
	std::string swordId = "graphics/weapon/Sword.png";
	std::string playerDeadId = "graphics/player/CharDie.png";
	float gravity;
	float jumpForce;
	float jumpTimer;

	float dashTimer;
	float dashSpeed = 300.f;
	float downSpeed = 100.f;

	float invincibilityTimer = 0.f;

	bool isDamaged = false;
	bool isDead = false;
	

	float dashCoolTimer = 1.f;
	sf::Vector2f dashDirection = look;
	
public:
	Player(const std::string& name = "Player");
	virtual ~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	virtual sf::FloatRect GetLocalBounds() const 
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetStatus(Status status);
	void UpdateGrounded(float dt);
	void UpdateJump(float dt);
	void UpdateDownJump(float dt);
	void UpdateDash(float dt);

	void Jump();
	void OnDamage(int monsterDamage);
	int GetCurrentHp() const { return hp; }
	int GetMaxHp() const { return maxhp; }

	float GetCurrentCoolTime() const { return dashCoolTimer; }
	float GetCurrentHpRatio() const { return (float)hp/maxhp; }
	

	sf::Vector2f GetPlayerLookNormal() const { return Utils::GetNormal(look); }

	void SetWeaponToWeaponSlot1(Weapon* weapon, bool isCurrentWeapon = false);
	void SetWeaponToWeaponSlot2(Weapon* weapon, bool isCurrentWeapon = false);
	void SwitchWeaponSlot(sf::Keyboard::Key key);
};