#include "stdafx.h"
#include "Banshee.h"

Banshee::Banshee(const std::string& name)
	: Monster(name)
{
}

void Banshee::SetOrigin(Origins preset)
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

void Banshee::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Banshee::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Banshee::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Banshee::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Banshee::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::Banshee;
}

void Banshee::Reset()
{
	SOUND_MGR.PlaySfx("sound/Sfx/monster/SpawnMonster.wav");
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	maxhp = 40 *((ROOM_MGR.GetCurrentFloor() + 1) / 2);
	hp = maxhp;
	originalDamage = 0;
	direction = { Utils::RandomRange(0, 1) < 1 ? -1.f : 1.f , 0.f };
	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
	}

	hitTimeDelay = 0.1f;

	idleTimeDelay = 2.5f;

	attackTimeDelay = 0.6f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(BansheeState::Idle);


	{
		detectionRange.setFillColor(sf::Color::Transparent);
		detectionRange.setOutlineColor(sf::Color::Blue);
		detectionRange.setOutlineThickness(1.f);
		detectionRange.setPosition(body.getPosition());
		detectionRange.setRadius(100.f);
		detectionRange.setOrigin({ detectionRange.getLocalBounds().width / 2.f, detectionRange.getLocalBounds().height / 2.f });
	}

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);

	shootBulletNumber = 10;
}

void Banshee::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());

	switch (state)
	{
	case Banshee::BansheeState::Idle: 
	{
		UpdateIdle(dt);
	}
		break;
	case Banshee::BansheeState::Attack:
	{
		UpdateAttack(dt);
	}
		break;
	case Banshee::BansheeState::Death:
	{
		UpdateDeath(dt);
	}
		break;
	}

	if (isDamaged && hp > 0)
	{
		hitTimeAccum += dt;
		if (hitTimeAccum >= hitTimeDelay)
		{
			isDamaged = false;
			hitTimeAccum = 0.f;
		}
	}
	else if (isDamaged && hp <= 0)
	{
		if (!isDead)
			SetState(BansheeState::Death);
	}

	detectionRange.setPosition(body.getPosition());

	animator.Update(dt);

	Monster::Update(dt);
}

void Banshee::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });
}

void Banshee::Draw(sf::RenderWindow& window)
{
	if (!isDamaged && hp > 0)
	{
		window.draw(body);
	}
	else if (isDamaged && hp <= 0)
	{
		window.draw(body);
	}
	else
	{
		window.draw(body, &shader);
	}

	if (Variables::isDrawHitBox)
	{
		window.draw(detectionRange);
	}

	hitbox.Draw(window);

	Monster::Draw(window);
}

void Banshee::Release()
{
}

void Banshee::SetState(BansheeState state)
{
	this->state = state;

	switch (state)
	{
	case Banshee::BansheeState::Idle:
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/Banshee Idle.csv");
	}
		break;
	case Banshee::BansheeState::Attack:
	{
		attackTimeAccum = 0.f;

		Shoot();

		animator.Play("animations/Banshee Attack.csv");
	}
		break;
	case Banshee::BansheeState::Death:
	{
		deathTimeAccum = 0.f;

		isDead = true;

		animator.Play("animations/Monster Die.csv");
		SOUND_MGR.PlaySfx("sound/Sfx/monster/MonsterDie.wav");
		target->AddCurrentExp(1);
	}
		break;
	}
}

void Banshee::UpdateIdle(float dt)
{
	idleTimeAccum += dt;

	direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y });
	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
	}

	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius() && idleTimeAccum >= idleTimeDelay)
	{
		SetState(BansheeState::Attack);

		return;
	}
}

void Banshee::UpdateAttack(float dt)
{
	attackTimeAccum += dt;

	if (attackTimeAccum >= attackTimeDelay)
	{
		SetState(BansheeState::Idle);

		return;
	}
}

void Banshee::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}

void Banshee::Shoot()
{
	float angle = 2.f * Utils::PI;
	for (int i = 0; i < shootBulletNumber; i++)
	{
		BansheeBullet* bullet = bulletPool.Take();
		SCENE_MGR.GetCurrentScene()->AddGo(bullet);
		bullet->Fire({ position.x , position.y - body.getLocalBounds().width / 2.f}, Utils::OnUnitCircle(angle / (float)shootBulletNumber * (i + 1)));
	}
}