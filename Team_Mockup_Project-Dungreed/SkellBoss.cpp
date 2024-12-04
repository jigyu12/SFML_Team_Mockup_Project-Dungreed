#include "stdafx.h"
#include "SkellBoss.h"

SkellBoss::SkellBoss(const std::string& name)
	: Monster(name)
{
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
}

void SkellBoss::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	hp = 80;
	speed = 30.f;
	originalDamage = 0;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 3.f;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;

	shootTimeDelay = 0.2f;
	shootAccumTime = shootTimeDelay;

	swordSpawnTimeDelay = 0.15f;
	afterSwordSpawnTimeDelay = 3.f;

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
	skellBossBackFx.setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height});
	//skellBossBackFx.setPosition({ position.x  + 17.5f , position.y + 45.f });
	skellBossBackFx.setPosition({ position.x  + 17.5f , position.y });

	angleRight = 0.f * Utils::PI;
	angleLeft = 0.5f * Utils::PI;
	angleUp = 1.f * Utils::PI;
	angleDown = 1.5f * Utils::PI;
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

	animator.Update(dt);
	animatorBackFx.Update(dt);

	hitbox.UpdateTr(body, GetLocalBounds());
}

void SkellBoss::LateUpdate(float dt)
{
	//SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	skellBossBackFx.setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
}

void SkellBoss::Draw(sf::RenderWindow& window)
{
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

	}
		break;
	}
}

void SkellBoss::UpdateIdle(float dt)
{
	if (hp <= 0)
	{
		SetState(SkellBossState::Death);

		return;
	}

	attackAccumSpeed += dt;

	if (attackAccumSpeed >= attackSpeedDelay)
	{
		SetState(SkellBossState::AttackLaser);

		/*float randAttackValue = Utils::RandomRange(0, 3);

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
		}*/
	}
}

void SkellBoss::UpdateAttackLaser(float dt)
{
	if (hp <= 0)
	{
		SetState(SkellBossState::Death);

		return;
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

		SetState(SkellBossState::Death);

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
	SkellBossBullet* bullet = bulletPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(bullet);
	bullets.push_back(bullet);

	bullet->Fire({ skellBossBackFx.getPosition().x - 10.f, skellBossBackFx.getPosition().y - 40.f }, direction);
}

void SkellBoss::ShootSword(int index)
{
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