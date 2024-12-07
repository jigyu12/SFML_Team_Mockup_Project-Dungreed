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

	std::list<Monster*> DamagedMonster;

	SavePlayerStatus playerStatus;

	float telePortTimer = 0.f;
	float deadAniTimer = 0.f;

	float gravity;
	float jumpForce;
	float jumpTimer;

	float dashTimer;
	float dashSpeed = 300.f;
	float downSpeed = 100.f;

	int deadMonsterCount = 0;

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
	void UpdateDead(float dt);

	void Jump();

	void OnDamage(int monsterDamage);
	//ÀúÀå

	int GetCurrentLevel() const { return playerStatus.level; }
	float GetCurrnetExp() const { return playerStatus.exp; }
	float GetCurrentAttackDamage() const { return playerStatus.attackDamage; }
	float GetCurrentCriticalDamage() const { return playerStatus.criticalDamage; }
	float GetCurrentArmor() const { return playerStatus.armor; }
	float GetCurrentArmorPercent() const { return playerStatus.armorPercent; }
	float GetCurrentMovementSpeed() const { return playerStatus.movementSpeed; }
	float GetCurrentCriticalPercent() const { return playerStatus.criticalPercent; }
	float GetCurrentDashDamage() const { return playerStatus.dashDamage; }
	float GetCurrentAttackSpeed() const { return playerStatus.attackSpeed; }

	void AddCurrentExp(float exp) 
	{ 
		this->playerStatus.exp += exp; 
	}
	//

	int GetCurrentHp() const { return hp; }
	int GetMaxHp() const { return maxhp; }
	bool IsDamaged() const { return isDamaged; }
	bool IsDead() const { return isDead; }

	

	float GetCurrentCoolTime() const { return dashCoolTimer; }
	float GetCurrentHpRatio() const { return Utils::Clamp01((float)hp / maxhp); }

	int GetRealDamage();
	

	int CalculationDamage(int damage);

	sf::Vector2f GetPlayerLookNormal() const { return Utils::GetNormal(look); }

	void SetWeaponToWeaponSlot1(Weapon* weapon, bool isCurrentWeapon = false);
	void SetWeaponToWeaponSlot2(Weapon* weapon, bool isCurrentWeapon = false);
	void SwitchWeaponSlot(sf::Keyboard::Key key);

	Weapon* GetCurrentWeapon() const ;
	Weapon* GetWeaponSlot1() const { return weaponSlot1 ;}
	Weapon* GetWeaponSlot2() const { return weaponSlot2 ;}

	
	
};
