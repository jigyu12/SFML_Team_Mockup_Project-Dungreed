#include "stdafx.h"
#include "SkelSword.h"
#include "Room.h"

SkelSword::SkelSword(const std::string& name)
	: Monster(name)
{
}

void SkelSword::SetOrigin(Origins preset)
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

void SkelSword::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void SkelSword::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void SkelSword::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void SkelSword::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void SkelSword::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::SkelSword;
}

void SkelSword::Reset()
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
		sword.setScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
		sword.setScale({ -1.f, 1.f });
	}
	gravity = 98.f;

	hitTimeAccum = 0.f;
	hitTimeDelay = 0.1f;

	idleTimeDelay = 2.f;

	moveTimeDelay = 5.f;

	beforeAttackTimeDelay = 3.4f;

	attackTimeDelay = 0.6f;

	deathTimeDelay = 1.f;

	isDamaged = false;
	isDead = false;
	isBeforeAttack = true;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	SetState(SkelSwordState::Idle);

	{
		detectionRange.setFillColor(sf::Color::Transparent);
		detectionRange.setOutlineColor(sf::Color::Blue);
		detectionRange.setOutlineThickness(1.f);
		detectionRange.setPosition(body.getPosition());
		detectionRange.setSize({ 120.f, 40.f });
		detectionRange.setOrigin({ detectionRange.getSize().x / 2, detectionRange.getSize().y });
	}
	{
		detectionLine.setFillColor(sf::Color::Magenta);
		detectionLine.setPosition({ body.getPosition().x + direction.x * 15.f ,body.getPosition().y - 8.f });
		detectionLine.setSize({ 1.f, 40.f - 8.f });
		detectionLine.setOrigin({ detectionLine.getLocalBounds().width / 2.f, detectionLine.getLocalBounds().height });
	}

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);

	animatorSword.SetTarget(&sword);
	animatorSword.Play("animations/Skel Sword Idle.csv");
	sword.setOrigin({ sword.getLocalBounds().left , sword.getLocalBounds().height });
	sword.setPosition({ position.x , position.y - 3.f });

	handTexture.loadFromFile("graphics/monster/Skels/SkelHand.png");
	hand1.setTexture(handTexture);
	hand2.setTexture(handTexture);

	attackBound.setSize({ 20.f, 30.f });
	attackBound.setOrigin({ attackBound.getSize().x / 2.f, attackBound.getSize().y / 2.f });
}

void SkelSword::Update(float dt)
{
	hitbox.UpdateTr(body, GetLocalBounds());
	detectionLineHitbox.UpdateTr(detectionLine, detectionLine.getLocalBounds());

	sword.setPosition({ position.x , position.y - 3.f });

	switch (state)
	{
	case SkelSword::SkelSwordState::Idle:
	{
		UpdateIdle(dt);
	}
	break;
	case SkelSword::SkelSwordState::Move:
	{
		UpdateMove(dt);
	}
	break;
	case SkelSword::SkelSwordState::Attack:
	{
		UpdateAttack(dt);
	}
	break;
	case SkelSword::SkelSwordState::Death:
	{
		UpdateDeath(dt);
	}
	break;
	}

	if (direction.x > 0)
	{
		SetScale({ 1.f, 1.f });
		sword.setScale({ 1.f, 1.f });
	}
	else
	{
		SetScale({ -1.f, 1.f });
		sword.setScale({ -1.f, 1.f });
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
			SetState(SkelSwordState::Death);
	}

	detectionRange.setPosition(body.getPosition());
	detectionLine.setPosition({ body.getPosition().x + direction.x * 15.f , body.getPosition().y - 8.f });

	animator.Update(dt);
	animatorSword.Update(dt);
}

void SkelSword::LateUpdate(float dt)
{
	if (isDead)
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
	else
		SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	sword.setOrigin({ sword.getLocalBounds().left , sword.getLocalBounds().height });

	attackBound.setPosition({ GetPosition().x + 20.f * direction.x, GetPosition().y - 15.f });
	attackBoundHitbox.UpdateTr(attackBound, attackBound.getLocalBounds());
	attackBoundHitbox.SetColor(sf::Color::Black);
}

void SkelSword::Draw(sf::RenderWindow& window)
{
	if (!isDamaged && hp > 0)
	{
		window.draw(body);
		window.draw(sword);
		attackBoundHitbox.Draw(window);
	}
	else if (isDamaged && hp <= 0)
	{
		window.draw(body);
	}
	else
	{
		window.draw(body, &shader);
		window.draw(sword);
		attackBoundHitbox.Draw(window);
	}

	if (Variables::isDrawHitBox)
	{
		window.draw(detectionRange);
		window.draw(detectionLine);
	}

	hitbox.Draw(window);
	detectionLineHitbox.Draw(window);
}

void SkelSword::Release()
{
}

void SkelSword::SetState(SkelSwordState state)
{
	this->state = state;

	switch (state)
	{
	case SkelSword::SkelSwordState::Idle:
	{
		idleTimeAccum = 0.f;

		animator.Play("animations/Skel Basic Idle.csv");
	}
	break;
	case SkelSword::SkelSwordState::Move:
	{
		moveTimeAccum = 0.f;

		animator.Play("animations/Skel Basic Move.csv");
	}
	break;
	case SkelSword::SkelSwordState::Attack:
	{
		beforeAttackTimeAccum = 0.f;
		attackTimeAccum = 0.f;

		isBeforeAttack = true;
		isAttack = false;

		speed = 90.f;

		if (animator.GetCurrentClipId() != "Move")
		{
			animator.Play("animations/Skel Basic Move.csv");
		}
	}
	break;
	case SkelSword::SkelSwordState::Death:
	{
		deathTimeAccum = 0.f;

		isDead = true;

		animator.Play("animations/Monster Die.csv");

		SetPosition({ position.x, position.y - 8.f });

		target->AddCurrentExp(1);
	}
	break;
	}
}

void SkelSword::UpdateIdle(float dt)
{
	if (detectionRange.getGlobalBounds().intersects(target->GetHitBox().rect.getGlobalBounds()) && idleTimeAccum >= idleTimeDelay / 2.f)
	{
		SetState(SkelSwordState::Attack);

		return;
	}

	idleTimeAccum += dt;

	if (idleTimeAccum >= idleTimeDelay)
	{
		SetState(SkelSwordState::Move);

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

void SkelSword::UpdateMove(float dt)
{
	if (detectionRange.getGlobalBounds().intersects(target->GetHitBox().rect.getGlobalBounds()))
	{
		SetState(SkelSwordState::Attack);

		return;
	}

	moveTimeAccum += dt;

	if (moveTimeAccum >= moveTimeDelay)
	{
		SetState(SkelSwordState::Idle);

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

void SkelSword::UpdateAttack(float dt)
{
	beforeAttackTimeAccum += dt;

	if (beforeAttackTimeAccum < beforeAttackTimeDelay - 0.4f)
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
		if (isBeforeAttack)
		{
			isBeforeAttack = false;
			speed = 0.f;
			animatorSword.Play("animations/Skel Sword Attack.csv");
		}
		else
		{
			if (!isAttack && beforeAttackTimeAccum >= beforeAttackTimeDelay)
			{
				isAttack = true;

				Attack();
			}

			attackTimeAccum += dt;

			if (attackTimeAccum >= attackTimeDelay)
			{
				speed = 30.f;
				isBeforeAttack = true;
				SetState(SkelSwordState::Idle);
			}
		}
	}
}

void SkelSword::UpdateDeath(float dt)
{
	deathTimeAccum += dt;
	if (deathTimeAccum >= deathTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}

void SkelSword::Attack()
{
	if (Utils::CheckCollision(target->GetHitBox(), attackBoundHitbox))
	{
		target->OnDamage(10);
	}
}