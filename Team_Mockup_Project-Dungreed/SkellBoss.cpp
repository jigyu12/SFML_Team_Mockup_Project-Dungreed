#include "stdafx.h"
#include "SkellBoss.h"
#include "Room.h"

SkellBoss::SkellBoss(const std::string& name)
	: Monster(name)
{
}

void SkellBoss::SetActive(bool active)
{
	GameObject::SetActive(active);
	leftHand->SetActive(active);
	rightHand->SetActive(active);
}

void SkellBoss::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
		origin = Utils::SetOrigin(body, originPreset);
	}
	else
	{
		std::cerr << "Cannot assign Origins::Custom" << std::endl;
	}
}

void SkellBoss::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkellBoss::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
	skellBossBackFx.setPosition({ position.x + 17.5f , position.y });
	if (leftHand != nullptr)
	{
		leftHand->SetPosition({ position.x - 100.f , position.y });
	}
	if (rightHand != nullptr)
	{
		rightHand->SetPosition({ position.x + 100.f , position.y });
	}
}

void SkellBoss::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkellBoss::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkellBoss::Init()
{
	sortingLayer = SortingLayers::Foreground;
	monsterType = MonsterType::SkellBoss;
	Scene* scene = SCENE_MGR.GetCurrentScene();
	leftHand = scene->AddGo(new SkellBossLeftHand("SkellBossLeftHand"));
	leftHand->Init();
	rightHand = scene->AddGo(new SkellBossLeftHand("SkellBossRightHand"));
	rightHand->Init();
	rightHand->SetScale({ -1.f, 1.f });
}

void SkellBoss::Reset()
{
	
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}

	leftHand->Reset();
	rightHand->Reset();

	sortingOrder = target->sortingOrder - 1;

	maxhp = 150 * ((ROOM_MGR.GetCurrentFloor() + 1) / 2);
	hp = maxhp; 
	speed = 30.f;
	originalDamage = 0;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 2.f;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;

	shootTimeDelay = 0.2f;
	shootAccumTime = shootTimeDelay;

	swordSpawnTimeDelay = 0.15f;
	afterSwordSpawnTimeDelay = 1.f;

	attackBulletTimeDelay = 5.f;
	bulletSpawnTimeDelay = 0.115f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	//SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(SkellBossState::Idle);

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);

	animatorBackFx.SetTarget(&skellBossBackFx);
	animatorBackFx.Play("animations/Boss/SkellBoss Back Idle.csv");
	//skellBossBackFx.setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	skellBossBackFx.setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });
	//skellBossBackFx.setPosition({ position.x  + 17.5f , position.y + 45.f });
	skellBossBackFx.setPosition({ position.x + 17.5f , position.y });

	angleRight = 0.f * Utils::PI;
	angleLeft = 0.5f * Utils::PI;
	angleUp = 1.f * Utils::PI;
	angleDown = 1.5f * Utils::PI;

	attackLaserTimeDelay = 1.3f + 0.5f; // SkellLeftHand attackSpeedDelay + targetFindTimeDelay

	//leftHand = dynamic_cast<SkellBossLeftHand*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBossLeftHand"));
	//rightHand = dynamic_cast<SkellBossLeftHand*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBossRightHand"));

	texDeath.loadFromFile("graphics/boss/BossDead/SkellBossDead_0.png");

	otherDeathsSize = 5;
	for (int i = 0; i < otherDeathsSize; ++i)
	{
		otherDeathTextures.push_back(new sf::Texture());
	}
	otherDeathTextures[0]->loadFromFile("graphics/boss/BossDead/SkellBossDead_5.png");
	otherDeathTextures[1]->loadFromFile("graphics/boss/BossDead/SkellBossDead_3.png");
	otherDeathTextures[2]->loadFromFile("graphics/boss/BossDead/SkellBossDead_4.png");
	otherDeathTextures[3]->loadFromFile("graphics/boss/BossDead/SkellBossDead_6.png");
	otherDeathTextures[4]->loadFromFile("graphics/boss/BossDead/SkellBossDead_7.png");
	for (int i = 0; i < otherDeathsSize; ++i)
	{
		otherDeathSprites.push_back(new sf::Sprite());
		otherDeathSprites[i]->setTexture(*otherDeathTextures[i]);
	}
	for (int i = 0; i < otherDeathsSize; ++i)
	{
		otherDeathHitBoxes.push_back(new HitBox());
	}
}

void SkellBoss::Update(float dt)
{
	switch (state)
	{
	case SkellBoss::SkellBossState::Idle:
	{
		UpdateIdle(dt);
	}
	break;
	case SkellBoss::SkellBossState::AttackLaser:
	{
		UpdateAttackLaser(dt);
	}
	break;
	case SkellBoss::SkellBossState::AttackBullet:
	{
		UpdateAttackBullet(dt);
	}
	break;
	case SkellBoss::SkellBossState::AttackSword:
	{
		UpdateAttackSword(dt);
	}
	break;
	case SkellBoss::SkellBossState::Death:
	{
		UpdateDeath(dt);
	}
	break;
	}

	if (isDamaged && hp > 0)
	{
		hitAccumTime += dt;
		if (hitAccumTime >= hitTimeDelay)
		{
			isDamaged = false;
			hitAccumTime = 0.f;
		}
	}
	else if (isDamaged && hp <= 0)
	{
		isDead = true;
		state = SkellBossState::Death;
	}

	shootAccumTime += dt;
	if (shootAccumTime >= shootTimeDelay && !isDead)
	{
		shootAccumTime = 0.f;

		ShootParticle();
	}

	if (state != SkellBossState::Death)
	{
		animator.Update(dt);
		animatorBackFx.Update(dt);
	}
	else
	{
		for (int i = 0; i < otherDeathsSize; ++i)
		{
			otherDeathHitBoxes[i]->UpdateTr(*otherDeathSprites[i], (*otherDeathSprites[i]).getLocalBounds());
		}
	}

	hitbox.UpdateTr(body, GetLocalBounds());
}

void SkellBoss::LateUpdate(float dt)
{
	//SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	if (state == SkellBossState::Death)
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().top });
	else
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	if (isDead)
	{
		auto hitboxBounds = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		float tempSpeed1 = 1.f;
		for (auto& startHitBox : hitboxBounds)
		{
			if (Utils::CheckCollision(hitbox, *startHitBox.first))
			{
				if (startHitBox.second.type == HitBoxData::Type::Downable)
					tempSpeed1 = 0.f;
			}
		}

		SetPosition({ position.x , position.y + (gravity += dt * 10.f) * dt * tempSpeed1 });

		for (int i = 0; i < otherDeathsSize; ++i)
		{
			float tempSpeed2 = 1.f;

			for (auto& startHitBox : hitboxBounds)
			{
				if (Utils::CheckCollision(*otherDeathHitBoxes[i], *startHitBox.first))
				{
					if (startHitBox.second.type == HitBoxData::Type::Downable)
						tempSpeed2 = 0.f;
				}
			}

			otherDeathSprites[i]->setPosition({ otherDeathSprites[i]->getPosition().x, otherDeathSprites[i]->getPosition().y + gravity * dt * tempSpeed2 });
		}
	}

	skellBossBackFx.setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
}

void SkellBoss::Draw(sf::RenderWindow& window)
{
	if (isDead)
	{
		for (int i = 0; i < otherDeathsSize; ++i)
		{
			window.draw(*otherDeathSprites[i]);
			otherDeathHitBoxes[i]->Draw(window);
		}
	}

	if (!isDamaged && hp > 0)
	{
		window.draw(skellBossBackFx);
		window.draw(body);
	}
	else if (isDamaged && hp <= 0)
	{
		window.draw(body);
	}
	else
	{
		window.draw(skellBossBackFx);
		window.draw(body, &shader);
	}

	hitbox.Draw(window);
}

void SkellBoss::Release()
{
	for (auto& tex : otherDeathTextures)
	{
		delete tex;
	}

	for (auto& spt : otherDeathSprites)
	{
		delete spt;
	}

	for (auto& hitBox : otherDeathHitBoxes)
	{
		delete hitBox;
	}

	for (auto& bullet : bullets)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(bullet);
	}
	bullets.clear();
}

void SkellBoss::SetState(SkellBossState state)
{
	this->state = state;

	switch (state)
	{
	case SkellBoss::SkellBossState::Idle:
	{
		attackAccumSpeed = 0.f;

		animator.Play("animations/Boss/SkellBoss Idle.csv");
	}
	break;
	case SkellBoss::SkellBossState::AttackLaser:
	{
		attackLaserRandPatternValue = Utils::RandomValue() < 0.5f ? -1 : 1;
		if (attackLaserRandPatternValue < 0)
		{
			beforeHand = rightHand;
		}
		else
		{
			beforeHand = leftHand;
		}

		attackLaserTimeAccum = 0.f;

		laserAttackTimeCount = 0;

		laserAttackCountMax = Utils::RandomRange(1, 3);
	}
	break;
	case SkellBoss::SkellBossState::AttackBullet:
	{
		attackBulletTimeAccum = 0.f;
		bulletSpawnTimeAccum = 0.f;

		angleRight = 0.f * Utils::PI;
		angleLeft = 0.5f * Utils::PI;
		angleUp = 1.f * Utils::PI;
		angleDown = 1.5f * Utils::PI;

		randDir = Utils::RandomValue() < 0.5f ? -1.f : 1.f;

		isattackBulletIng = false;
		isattackBulletEnd = false;

		animator.Play("animations/Boss/SkellBoss AttackBullet start.csv");
	}
	break;
	case SkellBoss::SkellBossState::AttackSword:
	{
		swordCount = 0;
		swordSpawnTimeAccum = 0.f;
		afterSwordSpawnTimeAccum = 0.f;
	}
	break;
	case SkellBoss::SkellBossState::Death:
	{

		SOUND_MGR.PlaySfx("sound/Sfx/monster/MonsterDie.wav");
		animator.Stop();

		{
			otherDeathSprites[2]->setOrigin({ otherDeathSprites[2]->getLocalBounds().width / 2.f , otherDeathSprites[2]->getLocalBounds().height });
			otherDeathSprites[2]->setPosition({ position.x + 7.5f + 20.f, position.y - 26.5f });

			otherDeathSprites[1]->setOrigin({ otherDeathSprites[1]->getLocalBounds().width / 2.f , otherDeathSprites[1]->getLocalBounds().height });
			otherDeathSprites[1]->setPosition({ position.x + 7.5f - 20.f, position.y - 26.5f });

			otherDeathSprites[0]->setOrigin({ otherDeathSprites[0]->getLocalBounds().width / 2.f , otherDeathSprites[0]->getLocalBounds().height });
			otherDeathSprites[0]->setPosition({ position.x + 7.5f, position.y + 4.f });

			otherDeathSprites[3]->setOrigin({ otherDeathSprites[3]->getLocalBounds().width / 2.f , otherDeathSprites[3]->getLocalBounds().height });
			otherDeathSprites[3]->setPosition({ position.x + 7.5f - 18.5f, position.y - 10.f });

			otherDeathSprites[4]->setOrigin({ otherDeathSprites[4]->getLocalBounds().width / 2.f , otherDeathSprites[4]->getLocalBounds().height });
			otherDeathSprites[4]->setPosition({ position.x + 7.5f + 18.5f, position.y - 10.f });
		}
		SetPosition({ position.x, position.y - body.getLocalBounds().height });

		body.setTexture(texDeath, true);

		gravity = 0.98f;

		target->AddCurrentExp(4);
	}
	break;
	}
}

void SkellBoss::UpdateIdle(float dt)
{
	if (hp <= 0)
	{
		SetState(SkellBossState::Death);
		leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);
		rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);

		return;
	}

	attackAccumSpeed += dt;

	if (attackAccumSpeed >= attackSpeedDelay)
	{
		float randAttackValue = Utils::RandomRange(0.f, 3.f);

		SetState(SkellBossState::AttackLaser);

		if (randAttackValue <= 1.f)
		{
			SetState(SkellBossState::AttackLaser);
		}
		else if (randAttackValue <= 2.f)
		{
			SetState(SkellBossState::AttackBullet);
		}
		else
		{
			SetState(SkellBossState::AttackSword);
		}
	}
}

void SkellBoss::UpdateAttackLaser(float dt)
{
	if (hp <= 0)
	{
		SetState(SkellBossState::Death);
		leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);
		rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);

		return;
	}

	if (laserAttackTimeCount >= laserAttackCountMax)
	{
		SetState(SkellBossState::Idle);

		return;
	}

	if (attackLaserRandPatternValue < 0)
	{
		attackLaserTimeAccum += dt;
		if (attackLaserTimeAccum >= attackLaserTimeDelay)
		{
			attackLaserTimeAccum = 0.f;

			if (beforeHand == leftHand)
			{
				rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::AttackLaser);
				laserAttackTimeCount++;
				beforeHand = rightHand;
			}
			else
			{
				leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::AttackLaser);
				laserAttackTimeCount++;
				beforeHand = leftHand;
			}
		}
	}
	else
	{
		attackLaserTimeAccum += dt;
		if (attackLaserTimeAccum >= attackLaserTimeDelay)
		{
			attackLaserTimeAccum = 0.f;

			if (beforeHand == leftHand)
			{
				rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::AttackLaser);
				laserAttackTimeCount++;
				beforeHand = rightHand;
			}
			else
			{
				leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::AttackLaser);
				laserAttackTimeCount++;
				beforeHand = leftHand;
			}
		}
	}
}

void SkellBoss::UpdateAttackBullet(float dt)
{
	if (hp <= 0)
	{
		for (auto& bullet : bullets)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(bullet);
		}
		bullets.clear();

		SetState(SkellBossState::Death);
		leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);
		rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);

		return;
	}

	attackBulletTimeAccum += dt;
	if (attackBulletTimeAccum < attackBulletTimeDelay)
	{
		if (attackBulletTimeAccum >= attackBulletTimeDelay / 10.f)
		{
			if (!isattackBulletIng)
			{
				isattackBulletIng = true;

				animator.Play("animations/Boss/SkellBoss AttackBullet ing.csv");
			}
		}

		if (attackBulletTimeAccum >= attackBulletTimeDelay * 9.65f / 10.f)
		{
			if (!isattackBulletEnd)
			{
				isattackBulletEnd = true;

				animator.Play("animations/Boss/SkellBoss AttackBullet end.csv");
			}
		}

		bulletSpawnTimeAccum += dt;
		if (bulletSpawnTimeAccum >= bulletSpawnTimeDelay)
		{
			bulletSpawnTimeAccum = 0.f;

			ShootBullet(Utils::OnUnitCircle(angleRight += dt * 10 * randDir));
			ShootBullet(Utils::OnUnitCircle(angleLeft += dt * 10 * randDir));
			ShootBullet(Utils::OnUnitCircle(angleUp += dt * 10 * randDir));
			ShootBullet(Utils::OnUnitCircle(angleDown += dt * 10 * randDir));
		}
	}
	else
	{
		SetState(SkellBossState::Idle);
	}
}

void SkellBoss::UpdateAttackSword(float dt)
{
	if (hp <= 0)
	{
		for (auto& sword : swords)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(sword);
		}

		SetState(SkellBossState::Death);
		leftHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);
		rightHand->SetState(SkellBossLeftHand::SkellBossLeftHandState::Death);

		return;
	}

	swordSpawnTimeAccum += dt;


	if (swordCount >= 6)
	{
		afterSwordSpawnTimeAccum += dt;

		if (afterSwordSpawnTimeAccum >= afterSwordSpawnTimeDelay)
		{
			SetState(SkellBossState::Idle);
		}

		return;
	}

	if (swordSpawnTimeAccum >= swordSpawnTimeDelay)
	{
		swordSpawnTimeAccum = 0.f;

		ShootSword(swordCount);

		swordCount++;
	}
}

void SkellBoss::UpdateDeath(float dt)
{

}

void SkellBoss::ShootBullet(sf::Vector2f direction)
{
	SOUND_MGR.PlaySfx("sound/Sfx/boss/Resources_Audio_BelialBullet.mp3");

	SkellBossBullet* bullet = bulletPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(bullet);
	bullets.push_back(bullet);

	bullet->Fire({ skellBossBackFx.getPosition().x - 10.f, skellBossBackFx.getPosition().y - 40.f }, direction);
}

void SkellBoss::ShootSword(int index)
{
	SOUND_MGR.PlaySfx("sound/Sfx/boss/slimeball.wav");

	SkellBossSword* sword = swordPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(sword);
	swords.push_back(sword);
	sf::Vector2f randDir = Utils::GetNormal(Utils::RandomInUnitCircle());

	sword->Fire({ skellBossBackFx.getPosition().x - 100.f + 40.f * index, skellBossBackFx.getPosition().y - 80.f });
}

void SkellBoss::ShootParticle()
{
	SkellBossParticle* particle = particlePool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(particle);
	sf::Vector2f randDir = Utils::GetNormal(Utils::RandomInUnitCircle());

	particle->Fire({ skellBossBackFx.getPosition().x - 10.f, skellBossBackFx.getPosition().y - 20.f }, randDir, 50.f);
}