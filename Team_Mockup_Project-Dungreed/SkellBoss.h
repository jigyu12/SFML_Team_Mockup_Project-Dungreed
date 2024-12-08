#pragma once

#include "Monster.h"
#include "SkellBossLeftHand.h"
#include "SkellBossParticle.h"
#include "SkellBossSword.h"
#include "SkellBossBullet.h"

class SkellBoss : public Monster
{
public:
	enum class SkellBossState
	{
		None = -1,

		Idle,
		AttackLaser,
		AttackBullet,
		AttackSword,
		Death,

		Count
	};

protected:
	SkellBossState state;

	Animator animator;

	sf::Shader shader;

	float hitAccumTime;
	float hitTimeDelay;

	float shootAccumTime;
	float shootTimeDelay;

	Animator animatorBackFx;
	sf::Sprite skellBossBackFx;

	ObjectPool<SkellBossParticle> particlePool;

	ObjectPool<SkellBossSword> swordPool;
	std::vector<SkellBossSword*> swords;

	ObjectPool<SkellBossBullet> bulletPool;
	std::vector<SkellBossBullet*> bullets;

	int swordCount;
	float swordSpawnTimeAccum;
	float swordSpawnTimeDelay;

	float afterSwordSpawnTimeAccum;
	float afterSwordSpawnTimeDelay;

	float attackBulletTimeAccum;
	float attackBulletTimeDelay;
	float bulletSpawnTimeAccum;
	float bulletSpawnTimeDelay;

	bool isattackBulletIng;
	bool isattackBulletEnd;

	float angleRight;
	float angleLeft;
	float angleUp;
	float angleDown;
	float randDir;

	int attackLaserRandPatternValue;
	float attackLaserTimeAccum;
	float attackLaserTimeDelay;
	int laserAttackTimeCount;

	SkellBossLeftHand* leftHand;
	SkellBossLeftHand* rightHand;
	SkellBossLeftHand* beforeHand;

	int laserAttackCountMax;

	sf::Texture texDeath;

	std::vector<sf::Texture*> otherDeathTextures;
	std::vector<sf::Sprite*> otherDeathSprites;
	std::vector<HitBox*> otherDeathHitBoxes;
	int otherDeathsSize;

	float gravity;

public:
	SkellBoss(const std::string& name = "SkellBoss");
	virtual ~SkellBoss() = default;

	void SetActive(bool active) override;

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

	float GetCurrentHpRatio() const { return Utils::Clamp01((float)hp / maxhp); }

	void SetState(SkellBossState state);
	void UpdateIdle(float dt);
	void UpdateAttackLaser(float dt);
	void UpdateAttackBullet(float dt);
	void UpdateAttackSword(float dt);
	void UpdateDeath(float dt);

	virtual sf::FloatRect GetLocalBounds() const
	{
		return body.getLocalBounds();
	}
	virtual sf::FloatRect GetGlobalBounds() const
	{
		return body.getGlobalBounds();
	}

	std::vector<SkellBossSword*>& GetSwords() { return swords; }

	void ShootBullet(sf::Vector2f direction);
	void ShootSword(int index);
	void ShootParticle();
};