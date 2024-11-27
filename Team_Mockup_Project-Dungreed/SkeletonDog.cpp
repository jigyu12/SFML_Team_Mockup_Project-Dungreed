#include "stdafx.h"
#include "SkeletonDog.h"

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
	direction = Utils::GetNormal({ target->GetPosition().x - position.x , 0.f });


	hp = 20;
	speed = 35.f;
	originalDamage = 8;

	idleAccumTime = 0.f;
	idleTimeDelay = 1.f;
	isIdle = true;

	moveAccumTime = 0.f;
	moveTimeDelay = 1.5f;
	isMoving = false;

	attackAccumTime = 0.f;
	attackTimeDelay = 1.f;
	isAttack = false;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;
	isDamaged = false;

	deathAccumTime = 0.f;
	deathTimeDelay = 1.f;
	isDead = false;

	animator.SetTarget(&body);
	animator.Play("animations/SkeletonDog Idle.csv");

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height });

	state = SkeleDogState::Idle;

	movableBound = { -FRAMEWORK.GetWindowBounds().width / 2.f / 6.f, 0.f,FRAMEWORK.GetWindowBounds().width / 6.f, 0.f };

	detectionRange.setFillColor(sf::Color::Transparent);
	detectionRange.setOutlineColor(sf::Color::Blue);
	detectionRange.setOutlineThickness(1.f);
	detectionRange.setPosition(body.getPosition());
	detectionRange.setSize({160.f, 60.f});
	detectionRange.setOrigin({ detectionRange.getSize().x / 2, detectionRange.getSize().y });
	
	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);
}

void SkeletonDog::Update(float dt)
{
	detectionRange.setPosition(body.getPosition());

	switch (state)
	{
	case SkeletonDog::SkeleDogState::Idle:
	{
		idleAccumTime += dt;

		if (detectionRange.getGlobalBounds().intersects(target->GetGlobalBounds()))
		{
			//idleAccumTime = 0.f;
			isIdle = false;
			//state = SkeleDogState::Attack;
		}

		if (idleAccumTime > idleTimeDelay)
		{
			//idleAccumTime = 0.f;
			isIdle = false;
			//state = SkeleDogState::Move;
		}
	}
		break;
	case SkeletonDog::SkeleDogState::Move:
	{

	}
		break;
	case SkeletonDog::SkeleDogState::Attack:
	{

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
		window.draw(detectionRange);

	hitbox.Draw(window);
}

void SkeletonDog::Release()
{
}

void SkeletonDog::Attack()
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