#include "stdafx.h"
#include "SkeletonDog.h"
#include "Room.h"

SkeletonDog::SkeletonDog(const std::string& name)
	: Monster(name)
{
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
	direction = Utils::GetNormal({ 1.f, 0.f });


	hp = 20;
	speed = 50.f;
	originalDamage = 8;

	idleAccumTime = 0.f;
	idleTimeDelay = 1.5f;
	isIdle = false;

	moveAccumTime = 0.f;
	moveTimeDelay = 4.f;
	isMoving = false;

	attackAccumTime = 0.f;
	attackTimeDelay = 2.f;
	isAttack = false;

	attackMoveAccumTime  = attackMoveTimeDelay;
	attackMoveTimeDelay = 2.f;
	isAttackMove = false;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;
	isDamaged = false;

	deathAccumTime = 0.f;
	deathTimeDelay = 1.f;
	isDead = false;

	animator.SetTarget(&body);

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	state = SkeleDogState::Idle;

	movableBound = { -FRAMEWORK.GetWindowBounds().width / 2.f / 6.f, 0.f,FRAMEWORK.GetWindowBounds().width / 6.f, 0.f };

	detectionRange.setFillColor(sf::Color::Transparent);
	detectionRange.setOutlineColor(sf::Color::Blue);
	detectionRange.setOutlineThickness(1.f);
	detectionRange.setPosition(body.getPosition());
	detectionRange.setSize({160.f, 40.f});
	detectionRange.setOrigin({ detectionRange.getSize().x / 2, detectionRange.getSize().y });

	detectionLine.setFillColor(sf::Color::Magenta);
	detectionLine.setSize({ 1.f, 40.f });
	detectionLine.setOrigin({ detectionLine.getSize().x / 2.f ,detectionLine.getSize().y});
	detectionLine.setPosition({ body.getPosition().x + direction.x * 10.f ,body.getPosition().y});
	
	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);

	velocityY = 0.f;
	gravity = 98.f * 2.f;
	jumpSpeed = -90.0f;
	isOnGround = true;
}

void SkeletonDog::Update(float dt)
{
	detectionRange.setPosition(body.getPosition());
	detectionLine.setPosition({ body.getPosition().x + direction.x * 10.f ,body.getPosition().y });

	switch (state)
	{
	case SkeletonDog::SkeleDogState::Idle:
	{
		idleAccumTime += dt;

		if (!isIdle)
		{
			animator.Play("animations/SkeletonDog Idle.csv");
			isIdle = true;
		}
		
		if (detectionRange.getGlobalBounds().intersects(target->GetGlobalBounds()))
		{
			idleAccumTime = 0.f;
			isIdle = false;
			state = SkeleDogState::Attack;
		}

		if (idleAccumTime > idleTimeDelay)
		{
			idleAccumTime = 0.f;
			isIdle = false;
			state = SkeleDogState::Move;
		}
	}
		break;
	case SkeletonDog::SkeleDogState::Move:
	{
		if (detectionRange.getGlobalBounds().intersects(target->GetGlobalBounds()))
		{
			isAttackMove = true;
			isMoving = true;

			attackMoveAccumTime += dt;

			if (attackMoveAccumTime > attackMoveTimeDelay)
			{
				moveAccumTime = 0.f;
				attackMoveAccumTime = 0.f;
				isMoving = false;
				isAttackMove = false;
				state = SkeleDogState::Attack;
			}
		}
		
		moveAccumTime += dt;
		if (moveAccumTime < moveTimeDelay)
		{
			if (!isMoving)
			{
				animator.Play("animations/SkeletonDog Move.csv");
				isMoving = true;
			}

			if (detectionLine.getGlobalBounds().getPosition().x < movableBound.left || detectionLine.getGlobalBounds().getPosition().x > movableBound.left + movableBound.width)
			{
				direction.x = -direction.x;
			}
			else if(detectionRange.getGlobalBounds().intersects(target->GetGlobalBounds()))
			{
				direction = Utils::GetNormal({ target->GetPosition().x - position.x , 0.f });
			}

			float mag = Utils::Magnitude(direction);
			if (mag > 1.f)
			{
				Utils::Normailize(direction);
			}
			if (direction.x > 0)
			{
				SetScale({ 1.f, 1.f });
			}
			else
			{
				SetScale({ -1.f, 1.f });
			}
			sf::Vector2f newPosition = position + direction * speed * dt;
			SetPosition(newPosition);
		}
		else
		{
			moveAccumTime = 0.f;
			isMoving = false;
			state = SkeleDogState::Idle;
		}
	}
		break;
	case SkeletonDog::SkeleDogState::Attack:
	{
		if (!isAttack && isOnGround)
		{
			direction = Utils::GetNormal({ target->GetPosition().x - position.x , 0.f });
			velocityY = jumpSpeed;
			isAttack = true;
			isOnGround = false;
			animator.Play("animations/SkeletonDog Attack.csv");
		}
		else
		{
			velocityY += gravity * dt;

			float mag = Utils::Magnitude(direction);
			if (mag > 1.f)
			{
				Utils::Normailize(direction);
			}
			if (direction.x > 0)
			{
				SetScale({ 1.f, 1.f });
			}
			else
			{
				SetScale({ -1.f, 1.f });
			}
			sf::Vector2f newPosition = { position.x + direction.x * (speed * 2) * dt, position.y + direction.y * speed * dt };
			newPosition = { Utils::Clamp(newPosition.x, movableBound.left + body.getGlobalBounds().width / 2.f , movableBound.left + movableBound.width - body.getGlobalBounds().width / 2.f), newPosition.y};
			SetPosition({ newPosition.x , newPosition.y + velocityY * dt });

			if (body.getPosition().y > movableBound.top - movableBound.height)
			{
				body.setPosition(body.getPosition().x, movableBound.top - movableBound.height);
				velocityY = 0.0f;
				isAttack = false;
				isOnGround = true;
				animator.Play("animations/SkeletonDog Move.csv");
				state = SkeleDogState::Move;
			}
		}

	}
		break;
	case SkeletonDog::SkeleDogState::Death:
	{
		deathAccumTime += dt;
		if (deathAccumTime > deathTimeDelay)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		}
	}
		break;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		OnDamaged();
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
		if (!isDead)
		{
			state = SkeleDogState::Death;
			SetPosition({ position.x, position.y - GetLocalBounds().height / 2.f });
			animator.Play("animations/Monster Die.csv");
			isDead = true;
		}
	}

	animator.Update(dt);

	hitbox.UpdateTr(body, GetLocalBounds());
}

void SkeletonDog::LateUpdate(float dt)
{
	if(isDead)
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
}

void SkeletonDog::Release()
{
}

void SkeletonDog::OnDamaged()
{
	hp -= 10; /////////// 임시 코드
	isDamaged = true;
}

void SkeletonDog::Jump()
{
}