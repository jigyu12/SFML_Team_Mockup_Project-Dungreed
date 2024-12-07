#include "stdafx.h"
#include "SkelBow.h"
#include "Room.h"

SkelBow::SkelBow(const std::string& name)
	: Monster(name)
{
}

void SkelBow::SetOrigin(Origins preset)
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

void SkelBow::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkelBow::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void SkelBow::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkelBow::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkelBow::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::SkelBow;
}

void SkelBow::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	maxhp = 32 * ((ROOM_MGR.GetCurrentFloor() + 1) / 2);
	hp = maxhp;
	speed = 30.f;
	originalDamage = 0;
	direction = { Utils::RandomRange(-1, 0) < 0 ? -1.f : 1.f, 0.f };
	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
	}
	gravity = 98.f;

	hitTimeAccum = 0.f;
	hitTimeDelay = 0.1f;

	idleTimeDelay = 2.f;

	beforeAttackTimeDelay = 0.8f;
	attackTimeDelay = 1.f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);
	
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(SkelBowState::Idle);

	{
		detectionRange.setFillColor(sf::Color::Transparent);
		detectionRange.setOutlineColor(sf::Color::Blue);
		detectionRange.setOutlineThickness(1.f);
		detectionRange.setPosition(body.getPosition());
		detectionRange.setRadius(100.f);
		detectionRange.setOrigin({ detectionRange.getLocalBounds().width / 2.f, detectionRange.getLocalBounds().height / 2.f });
	}

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);

	animatorBow.SetTarget(&bow);
	animatorBow.Play("animations/Skel Bow Idle.csv");
	bow.setOrigin({ bow.getLocalBounds().width / 2.f , bow.getLocalBounds().height / 2.f });
	bow.setPosition({ position.x, position.y - GetLocalBounds().height / 2.f});

	handTexture.loadFromFile("graphics/monster/Skels/SkelHand.png");
	hand1.setTexture(handTexture);
	hand2.setTexture(handTexture);
}

void SkelBow::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());

	bow.setPosition({ position.x, position.y - GetLocalBounds().height / 2.f + 3.f  });

	switch (state)
	{
	case SkelBow::SkelBowState::Idle: 
	{
		UpdateIdle(dt);
	}
		break;
	case SkelBow::SkelBowState::Attack:
	{
		UpdateAttack(dt);
	}
		break;
	case SkelBow::SkelBowState::Death:
	{
		UpdateDeath(dt);
	}
		break;
	}

	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
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
			SetState(SkelBowState::Death);
	}

	detectionRange.setPosition(body.getPosition());

	animator.Update(dt);
	animatorBow.Update(dt);
}

void SkelBow::LateUpdate(float dt)
{
	if (isDead)
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	else
	{
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

		auto roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();

		sf::Vector2f newPosition = position + direction * speed * dt;
		SetPosition({ position.x, newPosition.y + gravity * dt });

		for (auto& roomHitbox : roomHitboxes)
		{
			CollisionState state;

			if (Utils::CheckCollision(hitbox, *roomHitbox.first, state))
			{
				if (state.Down)
				{
					position.y = std::min(position.y, state.area.top);
					SetPosition(position);
				}
			}
		}
	}
	
	bow.setOrigin({ bow.getLocalBounds().width / 2.f , bow.getLocalBounds().height / 2.f});
}

void SkelBow::Draw(sf::RenderWindow& window)
{
	if (!isDamaged && hp > 0)
	{
		window.draw(body);
		window.draw(bow);
	}
	else if (isDamaged && hp <= 0)
	{
		window.draw(body);
	}
	else
	{
		window.draw(body, &shader);
		window.draw(bow);
	}

	if (Variables::isDrawHitBox)
	{
		window.draw(detectionRange);
	}

	hitbox.Draw(window);
}

void SkelBow::Release()
{
}

void SkelBow::SetState(SkelBowState state)
{
	this->state = state;

	switch (state)
	{
	case SkelBow::SkelBowState::Idle: 
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/Skel Basic Idle.csv");
	}
		break;
	case SkelBow::SkelBowState::Attack:
	{
		beforeAttackTimeAccum = 0.f;
		attackTimeAccum = 0.f;

		isBeforeAttack = true;

		animatorBow.Play("animations/Skel Bow Attack.csv");
	}
		break;
	case SkelBow::SkelBowState::Death:
	{
		deathTimeAccum = 0.f;

		isDead = true;

		animator.Play("animations/Monster Die.csv");

		SetPosition({position.x, position.y - 8.f});
	}
		break;
	}
}

void SkelBow::UpdateIdle(float dt)
{
	idleTimeAccum += dt;

	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
	{
		bow.setRotation(Utils::Angle(Utils::GetNormal({ target->GetPosition() - position })));

		direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y });
		if (direction.x > 0)
		{
			SetScale({ 1.f, 1.f });
		}
		else
		{
			SetScale({ -1.f, 1.f });
		}
	}
	
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius() && idleTimeAccum >= idleTimeDelay)
	{
		SetState(SkelBowState::Attack);

		return;
	}
}

void SkelBow::UpdateAttack(float dt)
{
	beforeAttackTimeAccum += dt;

	if (beforeAttackTimeAccum < beforeAttackTimeDelay)
	{
		direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y });
		if (direction.x > 0)
		{
			SetScale({ 1.f, 1.f });
		}
		else
		{
			SetScale({ -1.f, 1.f });
		}

		bow.setRotation(Utils::Angle(Utils::GetNormal({ target->GetPosition() - position })));
	}
	else
	{
		if (isBeforeAttack)
		{
			isBeforeAttack = false;

			animatorBow.Play("animations/Skel Bow Idle.csv");
		}
		else
		{
			Shoot();

			SetState(SkelBowState::Idle);
		}
	}
}

void SkelBow::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}

void SkelBow::Shoot()
{
	SkelBowArrow* arrow = arrowPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(arrow);
	arrow->Fire({ bow.getPosition().x ,bow.getPosition().y }, Utils::GetNormal({target->GetPosition() - position}));
}