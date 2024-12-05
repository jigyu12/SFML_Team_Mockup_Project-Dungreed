#include "stdafx.h"
#include "SkeletonDog.h"
#include "Room.h"

SkeletonDog::SkeletonDog(const std::string& name)
	: Monster(name)
{
}

void SkeletonDog::SetOrigin(Origins preset)
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

void SkeletonDog::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkeletonDog::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void SkeletonDog::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkeletonDog::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkeletonDog::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::SkeletonDog;
}

void SkeletonDog::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;

	maxhp = 20;
	hp = maxhp;
	speed = 50.f;
	originalDamage = 5;
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

	attackSpeedDelay = 1.f;
	attackMoveTimeDelay = 2.f;

	hitTimeAccum = 0.f;
	hitTimeDelay = 0.1f;

	idleTimeDelay = 2.f;

	moveTimeDelay = 4.f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(SkeletonDogState::Idle);

	{
		detectionRange.setFillColor(sf::Color::Transparent);
		detectionRange.setOutlineColor(sf::Color::Blue);
		detectionRange.setOutlineThickness(1.f);
		detectionRange.setPosition(body.getPosition());
		detectionRange.setSize({120.f, 40.f});
		detectionRange.setOrigin({  detectionRange.getSize().x / 2, detectionRange.getSize().y });
	}

	{
		detectionLine.setFillColor(sf::Color::Magenta);
		detectionLine.setPosition({ body.getPosition().x + direction.x * 12.f ,body.getPosition().y - 8.f });
		detectionLine.setSize({ 1.f, 40.f - 8.f });
		detectionLine.setOrigin({ detectionLine.getLocalBounds().width / 2.f, detectionLine.getLocalBounds().height });
	}

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);
}

void SkeletonDog::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());
	detectionLineHitbox.UpdateTr(detectionLine, detectionLine.getLocalBounds());

	switch (state)
	{
	case SkeletonDog::SkeletonDogState::Idle: 
	{
		UpdateIdle(dt);
	}
		break;
	case SkeletonDog::SkeletonDogState::Move:
	{
		UpdateMove(dt);
	}
		break;
	case SkeletonDog::SkeletonDogState::Attack:
	{
		UpdateAttack(dt);
	}
		break;
	case SkeletonDog::SkeletonDogState::Death:
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
		if(!isDead)
			SetState(SkeletonDogState::Death);
	}

	detectionRange.setPosition(body.getPosition());
	detectionLine.setPosition({ body.getPosition().x + direction.x * 12.f , body.getPosition().y - 8.f });

	animator.Update(dt);
}

void SkeletonDog::LateUpdate(float dt)
{
	if (isDead)
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	else
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });
}

void SkeletonDog::Draw(sf::RenderWindow& window)
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
		window.draw(detectionLine);
	}

	hitbox.Draw(window);
	detectionLineHitbox.Draw(window);
}

void SkeletonDog::Release()
{
}

void SkeletonDog::SetState(SkeletonDogState state)
{
	this->state = state;

	switch (state)
	{
	case SkeletonDog::SkeletonDogState::Idle:
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/SkeletonDog Idle.csv");
	}
		break;
	case SkeletonDog::SkeletonDogState::Move:
	{
		moveTimeAccum = 0.f;

		animator.Play("animations/SkeletonDog Move.csv");
	}
		break;
	case SkeletonDog::SkeletonDogState::Attack:
	{
		attackMoveTimeAccum = 0.f;
		attackAccumSpeed = 0.f;
		isAttackMove = true;

		speed = 80.f;

		velocityY = 0.f;

		if (animator.GetCurrentClipId() != "Move")
		{
			animator.Play("animations/SkeletonDog Move.csv");
		}
	}
		break;
	case SkeletonDog::SkeletonDogState::Death:
	{
		deathTimeAccum = 0.f;

		isDead = true;

		SetPosition({ position.x, position.y - GetLocalBounds().height / 2.f });

		animator.Play("animations/Monster Die.csv");
	}
		break;
	}
}

void SkeletonDog::UpdateIdle(float dt)
{
	if (detectionRange.getGlobalBounds().intersects(target->GetHitBox().rect.getGlobalBounds()) && idleTimeAccum >= idleTimeDelay / 2.f)
	{
		SetState(SkeletonDogState::Attack);

		return;
	}

	idleTimeAccum += dt;

	if (idleTimeAccum >= idleTimeDelay)
	{
		SetState(SkeletonDogState::Move);

		return;
	}
	else
	{
		auto roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		bool lineCollided = false;

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

			if (roomHitbox.second.type == HitBoxData::Type::Immovable)
			{
				if (Utils::CheckCollision(detectionLineHitbox, *roomHitbox.first, state))
				{
					lineCollided = true;
				}
			}
		}

		if (lineCollided)
		{
			direction.x = -direction.x;
		}
	}
}

void SkeletonDog::UpdateMove(float dt)
{
	if (detectionRange.getGlobalBounds().intersects(target->GetHitBox().rect.getGlobalBounds()))
	{
		SetState(SkeletonDogState::Attack);

		return;
	}

	moveTimeAccum += dt;

	if (moveTimeAccum >= moveTimeDelay)
	{
		SetState(SkeletonDogState::Idle);

		return;
	}
	else
	{
		auto roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		bool lineCollided = false;

		sf::Vector2f newPosition = position + direction * speed * dt;
		SetPosition({ newPosition.x, newPosition.y + gravity * dt });

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

			if (roomHitbox.second.type == HitBoxData::Type::Immovable)
			{
				if (Utils::CheckCollision(detectionLineHitbox, *roomHitbox.first, state))
				{
					lineCollided = true;
				}
			}
		}

		if (lineCollided)
		{
			direction.x = -direction.x;
		}
	}
}

void SkeletonDog::UpdateAttack(float dt)
{
	attackMoveTimeAccum += dt;

	if (attackMoveTimeAccum < attackMoveTimeDelay)
	{
		auto roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		bool lineCollided = false;

		direction = Utils::GetNormal({ target->GetPosition().x - position.x, 0.f });
		sf::Vector2f newPosition = position + direction * speed * dt;
		SetPosition({ newPosition.x, newPosition.y + gravity * dt });

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

			if (roomHitbox.second.type == HitBoxData::Type::Immovable)
			{
				if (Utils::CheckCollision(detectionLineHitbox, *roomHitbox.first, state))
				{
					lineCollided = true;
				}
			}
		}

		if (lineCollided)
		{
			direction.x = -direction.x;
		}
	}
	else
	{
		if (isAttackMove)
		{
			isAttackMove = false;
			velocityY = -gravity * 1.f / 2.f;
			speed = 100.f;
			animator.Play("animations/SkeletonDog Attack.csv");
		}
		else
		{
			auto roomHitboxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
			bool collided = false;
			bool lineCollided = false;
			float fixedXpos;

			sf::Vector2f newPosition = position + direction * speed * dt;
			SetPosition({ newPosition.x, newPosition.y + (velocityY += gravity * dt) * dt});

			testA += dt;
			if (testA <= testB)
				return;

			for (auto& roomHitbox : roomHitboxes)
			{
				CollisionState state;

				if (Utils::CheckCollision(hitbox, *roomHitbox.first, state))
				{
					if (roomHitbox.second.type == HitBoxData::Type::Immovable || roomHitbox.second.type == HitBoxData::Type::Downable)
					{
						if (state.Down && velocityY > 0.f)
						{
							position.y = std::min(position.y, state.area.top);
							SetPosition(position);
							collided = true;
						}
					}
				}

				if (roomHitbox.second.type == HitBoxData::Type::Immovable)
				{
					if (Utils::CheckCollision(hitbox/*detectionLineHitbox*/, *roomHitbox.first, state))
					{
						lineCollided = true;
					}
				}
			}

			if (lineCollided)
			{
				SetPosition({ position.x - direction.x * speed * dt , position.y });
			}

			if (collided)
			{
				speed = 50.f;
				testA = 0.f;

				int val = Utils::RandomRange(0, 2);
				if (val == 0 || val == 1)
				{
					SetState(SkeletonDogState::Idle);
				}
				else
				{
					SetState(SkeletonDogState::Move);
				}
			}
		}

	}
}

void SkeletonDog::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}