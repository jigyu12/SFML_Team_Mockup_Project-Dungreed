#include "stdafx.h"
#include "HandCrossbow.h"
#include "Room.h"

HandCrossbow::HandCrossbow(const std::string& name)
	: Weapon(name)
{
}

void HandCrossbow::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
		origin = Utils::SetOrigin(sprite, originPreset);
	}
	else
	{
		std::cerr << "Cannot assign Origins::Custom" << std::endl;
	}
}

void HandCrossbow::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void HandCrossbow::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void HandCrossbow::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void HandCrossbow::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void HandCrossbow::Init()
{
	sortingLayer = SortingLayers::Foreground;

	weaponType = WeaponType::Ranged;
}

void HandCrossbow::Reset()
{
	SetTextureId("graphics/weapon/Crossbow.png");
	sprite.setTexture(TEXTURE_MGR.Get(textureId));

	originalDamageMin = 7;
	originalDamageMax = 9;

	attackSpeedDelayTime = 1.f;
	attackSpeedAccumTime = attackSpeedDelayTime;

	Utils::SetOrigin(sprite, Origins::ML);
}

void HandCrossbow::Update(float dt)
{
	if (!owner)
	{
		auto player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
		if (InputMgr::GetKeyDown(sf::Keyboard::F) && sprite.getGlobalBounds().intersects(player->GetGlobalBounds()) && player->GetCurrentHp() > 0)
		{
			SetOwnerPlayer(player);
		}
	}
	else
	{
		attackSpeedAccumTime += dt;
		if (attackSpeedAccumTime > attackSpeedDelayTime && InputMgr::GetMouseButton(sf::Mouse::Left) && isCurrentWeapon && owner->GetCurrentHp() > 0)
		{
			attackSpeedAccumTime = 0.f;

			Shoot();
		}
	}
	
	hitbox.UpdateTr(sprite, GetLocalBounds());
}

void HandCrossbow::LateUpdate(float dt)
{
	if (!owner)
	{
		sortingOrder = 100;

		isOnGround = false;

		auto globalBounds = hitbox.rect.getGlobalBounds();
		const auto& roomHitBoxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		for (auto& roomHitBox : roomHitBoxes)
		{
			if (Utils::CheckCollision(*roomHitBox.first, hitbox))
			{
				Weapon::CollisionState state = GetCollsionState(globalBounds, roomHitBox.first->rect.getGlobalBounds());

				if (state.Up)
				{
				}

				if (state.Down)
				{
					isOnGround = true;
				}

				if (state.Left)
				{
				}

				if (state.Right)
				{
				}
			}
		}

		if (isOnGround)
			SetPosition({ position.x, position.y += gravity * dt * 0.f });
		else
			SetPosition({ position.x, position.y += gravity * dt });
	}
	else
	{
		if (owner->GetCurrentHp() <= 0)
			return;

		sortingOrder = owner->sortingOrder + 1;

		SetPosition({ owner->GetPosition().x , owner->GetPosition().y - 9 });

		sf::Vector2i mousePos = InputMgr::GetMousePosition();
		sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
		look = Utils::GetNormal(mouseworldPos - sprite.getPosition());
		SetRotation(Utils::Angle(look));
	}
}

void HandCrossbow::Draw(sf::RenderWindow& window)
{
	if (owner)
	{
		if (isCurrentWeapon)
			window.draw(sprite);
	}
	else
	{
		window.draw(sprite);
		hitbox.Draw(window);
	}
}

void HandCrossbow::Release()
{
}

void HandCrossbow::Shoot()
{
	Arrow* arrow = arrowPool.Take();
	SCENE_MGR.GetCurrentScene()->AddGo(arrow);

	arrow->Fire(position, look, 300.f, GetAttackDamage());
}