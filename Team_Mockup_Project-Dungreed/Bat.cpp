#include "stdafx.h"
#include "Bat.h"
#include "Room.h"

Bat::Bat(const std::string& name)
	: Monster(name)
{
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

void Bat::Init()
{
	sortingLayer = SortingLayers::Foreground;

	monsterType = MonsterType::Bat;
}

void Bat::Reset()
{
	if (!(target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"))))
	{
		std::cerr << "target player was nullptr" << std::endl;

		return;
	}
	sortingOrder = target->sortingOrder - 1;
	direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y - target->GetLocalBounds().height / 2.f });


	hp = 6;
	speed = 30.f;
	originalDamage = 5;

	attackAccumSpeed = 0.f;
	attackSpeedDelay = 1.f;

	idleAccumTime = 0.f;
	idleTimeDelay = 1.5f;
	idleRandMoveAccumTime = 0.f;
	idleRandMoveTimeDelay = 2.f;
	isRandMoving = false;

	hitAccumTime = 0.f;
	hitTimeDelay = 0.1f;
	isDamaged = false;

	deathAccumTime = 0.f;
	deathTimeDelay = 1.f;
	isDead = false;

	animator.SetTarget(&body);
	animator.Play("animations/Bat Idle.csv");

	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	state = BatState::Idle;

	movableBound = { -FRAMEWORK.GetWindowBounds().width / 2.f / 6.f, -FRAMEWORK.GetWindowBounds().height / 2.f / 6.f,FRAMEWORK.GetWindowBounds().width / 6.f, FRAMEWORK.GetWindowBounds().height / 6.f };

	detectionRange.setFillColor(sf::Color::Transparent);
	detectionRange.setOutlineColor(sf::Color::Blue);
	detectionRange.setOutlineThickness(1.f);
	detectionRange.setPosition(body.getPosition());
	detectionRange.setRadius(20.f);
	detectionRange.setOrigin({ detectionRange.getLocalBounds().width / 2.f, detectionRange.getLocalBounds().height / 2.f });

	shader.loadFromFile("shader/red.frag", sf::Shader::Type::Fragment);
}

void Bat::Update(float dt)
{
	detectionRange.setPosition(body.getPosition());

	switch (state)
	{
	case Bat::BatState::Idle:
	{
		idleAccumTime += dt;

		if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) < target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
		{
			idleAccumTime = 0.f;
			idleRandMoveAccumTime = 0.f;
			isRandMoving = false;
			state = BatState::Move;
		}

		if (idleAccumTime > idleTimeDelay)
		{
			if (!isRandMoving)
			{
				isRandMoving = true;

				float dirX;
				if ((std::fabs(dirX = (float)Utils::RandomRange(-1, 1))) < EPSILON)
				{
					dirX = 0.f;
				}
				else
				{
					dirX = dirX < EPSILON ? -1 : 1;
				}
				float dirY;
				if (std::fabs(dirX) < EPSILON)
				{
					while (true)
					{
						dirY = (float)Utils::RandomRange(-1, 1);
						if (std::fabs(dirY) > EPSILON)
						{
							break;
						}
					}
					dirY = dirY < EPSILON ? -1 : 1;
				}
				else
				{
					if (std::fabs(dirY = (float)Utils::RandomRange(-1, 1)) < EPSILON)
					{
						dirY = 0.f;
					}
					else
					{
						dirY = dirY < EPSILON ? -1 : 1;
					}
				}
				direction = { dirX, dirY };
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
			}
			else
			{
				idleRandMoveAccumTime += dt;
				if (idleRandMoveAccumTime < idleRandMoveTimeDelay)
				{
					sf::Vector2f oldPos = position;

					sf::Vector2f newPosition = position + direction * speed * dt;
					SetPosition(newPosition);
					hitbox.UpdateTr(body, GetLocalBounds());

					auto& roomHitBoxes = dynamic_cast<Room*>(SCENE_MGR.GetCurrentScene()->FindGo("tilemap"))->GetHitBoxes();
					std::vector<std::pair<HitBox*, HitBoxData>> walls;
					for (auto& hitBox : roomHitBoxes)
					{
						if (hitBox.second.type != HitBoxData::Type::Downable)
						{
							walls.push_back(hitBox);
						}
					}

					for (const auto& wall : walls)
					{
						if (Utils::CheckCollision(hitbox, *wall.first))
						{
							direction = { -direction.x, -direction.y };

							if (direction.x > 0)
							{
								SetScale({ 1.f, 1.f });
							}
							else
							{
								SetScale({ -1.f, 1.f });
							}

							oldPos = oldPos + direction * speed * dt;
							SetPosition(oldPos);
							hitbox.UpdateTr(body, GetLocalBounds());

							isRandMoving = false;

							return;
						}
					}
					/*if (newPosition.x - body.getLocalBounds().width / 2.f < movableBound.left )
					{
						SetPosition({newPosition.x = movableBound.left + body.getLocalBounds().width / 2.f, newPosition.y});
						direction = {-direction.x, -direction.y};
						isRandMoving = false;
					}
					if (newPosition.x + body.getLocalBounds().width / 2.f > movableBound.left + movableBound.width)
					{
						SetPosition({ newPosition.x = movableBound.left + movableBound.width - body.getLocalBounds().width / 2.f, newPosition.y });
						direction = { -direction.x, -direction.y };
						isRandMoving = false;
					}
					if (newPosition.y - body.getLocalBounds().height / 2.f < movableBound.top)
					{
						SetPosition({ newPosition.x, newPosition.y = movableBound.top + body.getLocalBounds().height / 2.f });
						direction = { -direction.x, -direction.y };
						isRandMoving = false;
					}
					if (newPosition.y + body.getLocalBounds().height / 2.f > movableBound.top + movableBound.height)
					{
						SetPosition({ newPosition.x, newPosition.y = movableBound.top + movableBound.height - body.getLocalBounds().height / 2.f });
						direction = { -direction.x, -direction.y };
						isRandMoving = false;
					}*/

					SetPosition(newPosition);
				}
				else
				{
					idleAccumTime = 0.f;
					idleRandMoveAccumTime = 0.f;
					isRandMoving = false;
				}
			}
		}
	}
	break;
	case Bat::BatState::Move:
	{
		if (Utils::Distance(detectionRange.getPosition(), { target->GetPosition().x,  target->GetPosition().y - target->GetLocalBounds().height / 2.f }) > target->GetLocalBounds().height / 2.f + detectionRange.getRadius())
		{
			idleAccumTime = idleTimeDelay;
			idleRandMoveAccumTime = 0.f;
			isRandMoving = false;
			state = BatState::Idle;
		}

		direction = Utils::GetNormal({ target->GetPosition().x - position.x , target->GetPosition().y - position.y - target->GetLocalBounds().height / 2.f });
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
	break;
	case Bat::BatState::Death:
	{
		deathAccumTime += dt;
		if (deathAccumTime > deathTimeDelay)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		}
	}
	break;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
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
			state = BatState::Death;
			animator.Play("animations/Monster Die.csv");
			isDead = true;
		}
	}


	animator.Update(dt);

	hitbox.UpdateTr(body, GetLocalBounds());
}

void Bat::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
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
		window.draw(detectionRange);

	hitbox.Draw(window);
}

void Bat::Release()
{
}

void Bat::OnDamaged()
{
	hp -= 3; /////////// 임시 코드
	isDamaged = true;
}