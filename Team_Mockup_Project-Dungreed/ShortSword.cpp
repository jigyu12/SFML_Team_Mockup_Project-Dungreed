#include "stdafx.h"
#include "ShortSword.h"
#include "Room.h"

ShortSword::ShortSword(const std::string& name)
	: Weapon(name)
{
}

void ShortSword::SetOrigin(Origins preset)
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

void ShortSword::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void ShortSword::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void ShortSword::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void ShortSword::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void ShortSword::Init()
{
	SetTextureId("graphics/weapon/Sword.png");
	sprite.setTexture(TEXTURE_MGR.Get(textureId));

	sortingLayer = SortingLayers::Foreground;

	weaponType = WeaponType::Melee;
}

void ShortSword::Reset()
{
	originalDamageMin = 8;
	originalDamageMax = 10;

	attackSpeedDelayTime = 3.03f;
	attackSpeedAccumTime = attackSpeedDelayTime;
	
	Utils::SetOrigin(sprite, Origins::BC);
}

void ShortSword::Update(float dt)
{
	if (!owner)
	{
		auto player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
		if (InputMgr::GetKeyDown(sf::Keyboard::F) && sprite.getGlobalBounds().intersects(player->GetGlobalBounds()))
		{
			SetOwnerPlayer(player);
		}
	}
	
	hitbox.UpdateTr(sprite, GetLocalBounds());
}

void ShortSword::LateUpdate(float dt)
{
	if (!owner)
	{
		sortingOrder = 100;

		isOnGround = false;

		auto skelDogGlobalBounds = hitbox.rect.getGlobalBounds();
		auto& roomHitBoxes = dynamic_cast<Room*>(SCENE_MGR.GetCurrentScene()->FindGo("tilemap"))->GetHitBoxes();
		for (auto& roomHitBox : roomHitBoxes)
		{
			if (Utils::CheckCollision(*roomHitBox.first, hitbox))
			{
				Weapon::CollisionState state = GetCollsionState(skelDogGlobalBounds, roomHitBox.first->rect.getGlobalBounds());

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
		sortingOrder = owner->sortingOrder + 1;

		SetPosition({ owner->GetPosition().x , owner->GetPosition().y - 9 });

		sf::Vector2i mousePos = InputMgr::GetMousePosition();
		sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
		look = Utils::GetNormal(mouseworldPos - sprite.getPosition());
		SetRotation(Utils::Angle(look) + 90);
	}
}

void ShortSword::Draw(sf::RenderWindow& window)
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

void ShortSword::Release()
{
}