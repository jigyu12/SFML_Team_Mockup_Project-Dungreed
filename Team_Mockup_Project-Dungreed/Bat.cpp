#include "stdafx.h"
#include "Bat.h"
#include "Room.h"

Bat::Bat(const std::string& name)
	: Monster(name)
{
}

void Bat::SetOrigin(Origins preset)
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

void Bat::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Bat::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Bat::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Bat::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Bat::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::Bat;
}

void Bat::Reset()
{
	SOUND_MGR.PlaySfx("sound/Sfx/monster/SpawnMonster.wav");
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	maxhp = 6 * ((ROOM_MGR.GetCurrentFloor() + 1) / 2);
	hp = maxhp;
	speed = 30.f;
	originalDamage = 5;
	direction = {Utils::RandomRange(0, 1) < 1 ? -1.f : 1.f , 0.f};
	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
	}

	hitTimeDelay = 0.1f;

	idleTimeDelay = 2.f;

	randMoveTimeDelay = 5.f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(BatState::Idle);

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

void Bat::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());

	switch (state)
	{
	case Bat::BatState::Idle:
	{
		UpdateIdle(dt);
	}
		break;
	case Bat::BatState::RandMove:
	{
		UpdateRandMove(dt);
	}
		break;
	case Bat::BatState::Attack:
	{
		UpdateAttack(dt);
	}
		break;
	case Bat::BatState::Death:
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
			SetState(BatState::Death);
	}

	detectionRange.setPosition(body.getPosition());

	animator.Update(dt);

	Monster::Update(dt);
}

void Bat::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });
}

void Bat::Draw(sf::RenderWindow& window)
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

void Bat::Release()
{
}

void Bat::SetState(BatState state)
{
	this->state = state;

	switch (state)
	{
	case Bat::BatState::Idle: 
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/Bat Idle.csv");
	}
		break;
	case Bat::BatState::RandMove:
	{
		randMoveTimeAccum = 0.f;

		do
		{
			direction = Utils::GetNormal({ Utils::RandomRange(-1.f, 1.f),Utils::RandomRange(-1.f, 1.f) });
		} while (Utils::Magnitude(direction) < EPSILON);

		movableBound = ROOM_MGR.GetCurrentRoom()->GetGlobalBounds();
	}
		break;
	case Bat::BatState::Attack:
	{
		movableBound = ROOM_MGR.GetCurrentRoom()->GetGlobalBounds();
	}
		break;
	case Bat::BatState::Death:
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

void Bat::UpdateIdle(float dt)
{
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
	{
		SetState(BatState::Attack);

		return;
	}

	idleTimeAccum += dt;

	if (idleTimeAccum >= idleTimeDelay)
	{
		SetState(BatState::RandMove);

		return;
	}
}

void Bat::UpdateRandMove(float dt)
{
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
	{
		SetState(BatState::Attack);

		return;
	}

	randMoveTimeAccum += dt;

	if (randMoveTimeAccum >= randMoveTimeDelay)
	{
		SetState(BatState::Idle);

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

void Bat::UpdateAttack(float dt)
{
	if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) >= target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
	{
		SetState(BatState::RandMove);

		return;
	}

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

void Bat::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}