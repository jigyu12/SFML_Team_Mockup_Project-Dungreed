#include "stdafx.h"
#include "Ghost.h"
#include "Room.h"

Ghost::Ghost(const std::string& name)
	: Monster(name)
{
}

void Ghost::SetOrigin(Origins preset)
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

void Ghost::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Ghost::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Ghost::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Ghost::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Ghost::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::Ghost;
}

void Ghost::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	maxhp = 12 * ((ROOM_MGR.GetCurrentFloor() + 1) / 2);
	hp = maxhp;
	speed = 30.f;
	originalDamage = 5;
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

	idleTimeDelay = 1.f;

	randMoveTimeDelay = 3.f;

	beforeAttackTimeDelay = 2.f;

	attackTimeDelay = 2.f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	SetState(GhostState::Idle);

	{
		detectionRange.setFillColor(sf::Color::Transparent);
		detectionRange.setOutlineColor(sf::Color::Blue);
		detectionRange.setOutlineThickness(1.f);
		detectionRange.setPosition(body.getPosition());
		detectionRange.setRadius(100.f);
		detectionRange.setOrigin({ detectionRange.getLocalBounds().width / 2.f, detectionRange.getLocalBounds().height / 2.f });
	}

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);
}

void Ghost::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());

	switch (state)
	{
	case Ghost::GhostState::Idle:
	{
		UpdateIdle(dt);
	}
		break;
	case Ghost::GhostState::RandMove:
	{
		UpdateRandMove(dt);
	}
		break;
	case Ghost::GhostState::Attack:
	{
		UpdateAttack(dt);
	}
		break;
	case Ghost::GhostState::Death:
	{
		UpdateDeath(dt);
	}
		break;
	}

	if (isBeforeAttack)
	{
		if (direction.x > 0)
		{
			SetScale({ 1.f, 1.f });
		}
		else
		{
			SetScale({ -1.f, 1.f });
		}
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
			SetState(GhostState::Death);
	}

	detectionRange.setPosition(body.getPosition());

	animator.Update(dt);
}

void Ghost::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
}

void Ghost::Draw(sf::RenderWindow& window)
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
}

void Ghost::Release()
{
}

void Ghost::SetState(GhostState state)
{
	this->state = state;

	switch (state)
	{
	case Ghost::GhostState::Idle: 
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/Ghost Idle.csv");
	}
		break;
	case Ghost::GhostState::RandMove:
	{
		randMoveTimeAccum = 0.f;

		do
		{
			direction = Utils::GetNormal({ Utils::RandomRange(-1.f, 1.f),Utils::RandomRange(-1.f, 1.f) });
		} while (Utils::Magnitude(direction) < EPSILON);

		movableBound = ROOM_MGR.GetCurrentRoom()->GetGlobalBounds();
	}
		break;
	case Ghost::GhostState::Attack:
	{
		beforeAttackTimeAccum = 0.f;
		attackTimeAccum = 0.f;

		isBeforeAttack = true;

		speed = 60.f;

		movableBound = ROOM_MGR.GetCurrentRoom()->GetGlobalBounds();
	}
		break;
	case Ghost::GhostState::Death:
	{
		deathTimeAccum = 0.f;

		isDead = true;

		animator.Play("animations/Monster Die.csv");

		target->AddCurrentExp(1);
	}
		break;
	}
}

void Ghost::UpdateIdle(float dt)
{
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius() && idleTimeAccum >= idleTimeDelay * 2.f / 3.f)
	{
		SetState(GhostState::Attack);

		return;
	}

	idleTimeAccum += dt;

	if (idleTimeAccum >= idleTimeDelay)
	{
		SetState(GhostState::RandMove);

		return;
	}
}

void Ghost::UpdateRandMove(float dt)
{
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
	{
		SetState(GhostState::Attack);

		return;
	}

	randMoveTimeAccum += dt;

	if (randMoveTimeAccum >= randMoveTimeDelay)
	{
		SetState(GhostState::Idle);

		return;
	}

	sf::Vector2f newPosition = position + direction * speed * dt;
	if (newPosition.x - body.getLocalBounds().width / 2.f < movableBound.left)
	{
		SetPosition({ newPosition.x = movableBound.left + body.getLocalBounds().width / 2.f, newPosition.y });
		direction = { -direction.x, -direction.y };
	}
	if (newPosition.x + body.getLocalBounds().width / 2.f > movableBound.left + movableBound.width)
	{
		SetPosition({ newPosition.x = movableBound.left + movableBound.width - body.getLocalBounds().width / 2.f, newPosition.y });
		direction = { -direction.x, -direction.y };
	}
	if (newPosition.y - body.getLocalBounds().height / 2.f < movableBound.top)
	{
		SetPosition({ newPosition.x, newPosition.y = movableBound.top + body.getLocalBounds().height / 2.f });
		direction = { -direction.x, -direction.y };
	}
	if (newPosition.y + body.getLocalBounds().height / 2.f > movableBound.top + movableBound.height)
	{
		SetPosition({ newPosition.x, newPosition.y = movableBound.top + movableBound.height - body.getLocalBounds().height / 2.f });
		direction = { -direction.x, -direction.y };
	}

	SetPosition(newPosition);
}

void Ghost::UpdateAttack(float dt)
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

		sf::Vector2f newPosition = position + direction * speed * dt;
		if (newPosition.x - body.getLocalBounds().width / 2.f < movableBound.left)
		{
			SetPosition({ newPosition.x = movableBound.left + body.getLocalBounds().width / 2.f, newPosition.y });
			direction = { -direction.x, -direction.y };
		}
		if (newPosition.x + body.getLocalBounds().width / 2.f > movableBound.left + movableBound.width)
		{
			SetPosition({ newPosition.x = movableBound.left + movableBound.width - body.getLocalBounds().width / 2.f, newPosition.y });
			direction = { -direction.x, -direction.y };
		}
		if (newPosition.y - body.getLocalBounds().height / 2.f < movableBound.top)
		{
			SetPosition({ newPosition.x, newPosition.y = movableBound.top + body.getLocalBounds().height / 2.f });
			direction = { -direction.x, -direction.y };
		}
		if (newPosition.y + body.getLocalBounds().height / 2.f > movableBound.top + movableBound.height)
		{
			SetPosition({ newPosition.x, newPosition.y = movableBound.top + movableBound.height - body.getLocalBounds().height / 2.f });
			direction = { -direction.x, -direction.y };
		}
		SetPosition(newPosition);
	}
	else
	{
		if (isBeforeAttack)
		{
			isBeforeAttack = false;

			direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y });
			if (direction.x > 0)
			{
				SetScale({ 1.f, 1.f });
			}
			else
			{
				SetScale({ -1.f, 1.f });
			}
			speed = 90.f;

			animator.Play("animations/Ghost Attack.csv");
		}
		else
		{
			attackTimeAccum += dt;

			if (attackTimeAccum < attackTimeDelay)
			{
				sf::Vector2f newPosition = position + direction * speed * dt;
				if (newPosition.x - body.getLocalBounds().width / 2.f < movableBound.left)
				{
					SetPosition({ newPosition.x = movableBound.left + body.getLocalBounds().width / 2.f, newPosition.y });
					direction = { 0.f, direction.y };
				}
				if (newPosition.x + body.getLocalBounds().width / 2.f > movableBound.left + movableBound.width)
				{
					SetPosition({ newPosition.x = movableBound.left + movableBound.width - body.getLocalBounds().width / 2.f, newPosition.y });
					direction = { 0.f, direction.y };
				}
				if (newPosition.y - body.getLocalBounds().height / 2.f < movableBound.top)
				{
					SetPosition({ newPosition.x, newPosition.y = movableBound.top + body.getLocalBounds().height / 2.f });
					direction = { direction.x,  0.f };
				}
				if (newPosition.y + body.getLocalBounds().height / 2.f > movableBound.top + movableBound.height)
				{
					SetPosition({ newPosition.x, newPosition.y = movableBound.top + movableBound.height - body.getLocalBounds().height / 2.f });
					direction = { direction.x, 0.f };
				}
				SetPosition(newPosition);
			}
			else
			{
				isBeforeAttack = true;
				speed = 30.f;
				SetState(GhostState::Idle);
			}
		}
	}
}

void Ghost::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}