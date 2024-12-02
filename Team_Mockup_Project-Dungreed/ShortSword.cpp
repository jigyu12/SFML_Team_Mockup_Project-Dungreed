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
	sortingLayer = SortingLayers::Foreground;

	weaponType = WeaponType::Melee;
}

void ShortSword::Reset()
{
	SetTextureId("graphics/weapon/Sword.png");
	sprite.setTexture(TEXTURE_MGR.Get(textureId));

	originalDamageMin = 8;
	originalDamageMax = 10;

	attackSpeedDelayTime = 0.4f;
	attackSpeedAccumTime = attackSpeedDelayTime;

	isSwing = false;
	swingTimeAccum = 0.f;
	swingTimeDelay = 0.35f;

	isUp = true;

	attackBound.setSize({40.f, 30.f});
	attackBound.setOrigin({attackBound.getSize().x / 2.f, attackBound.getSize().y / 2.f});

	animatorFx.SetTarget(&swordSwingFx);

	Utils::SetOrigin(sprite, Origins::BC);
}

void ShortSword::Update(float dt)
{
	if (!owner)
	{
		auto player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
		if (InputMgr::GetKeyDown(sf::Keyboard::F) && sprite.getGlobalBounds().intersects(player->GetGlobalBounds()) && player->GetHp() > 0)
		{
			SetOwnerPlayer(player);
		}
	}
	
	animatorFx.Update(dt);

	hitbox.UpdateTr(sprite, GetLocalBounds());
}

void ShortSword::LateUpdate(float dt)
{
	if (!owner)
	{
		sortingOrder = 100;

		isOnGround = false;

		auto globalBounds = hitbox.rect.getGlobalBounds();
		auto& roomHitBoxes = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
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
		if (isSwing)
		{
			swingTimeAccum += dt;
		}

		if (owner->GetHp() <= 0)
			return;

		sortingOrder = owner->sortingOrder + 1;

		SetPosition({ owner->GetPosition().x , owner->GetPosition().y - 9 });

		sf::Vector2i mousePos = InputMgr::GetMousePosition();
		sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
		look = Utils::GetNormal(mouseworldPos - sprite.getPosition());
		
		attackBound.setPosition({ owner->GetPosition().x + owner->GetPlayerLookNormal().x * 30.f, owner->GetPosition().y - 4.f + owner->GetPlayerLookNormal().y * 30.f });
		attackBound.setRotation(Utils::Angle(look) + 90);
		attackBoundHitbox.UpdateTr(attackBound, attackBound.getLocalBounds());
		attackBoundHitbox.SetColor(sf::Color::Black);

		attackSpeedAccumTime += dt;
		if (attackSpeedAccumTime > attackSpeedDelayTime && InputMgr::GetMouseButtonDown(sf::Mouse::Left) && isCurrentWeapon)
		{
			attackSpeedAccumTime = 0.f;

			Attack();

			isUp = !isUp;
	
			animatorFx.Play("animations/Sword Swing Fx.csv");

			Utils::SetOrigin(swordSwingFx, Origins::MC);
			swordSwingFx.setPosition({ owner->GetPosition().x + owner->GetPlayerLookNormal().x * 30.f,owner->GetPosition().y - 4.f + owner->GetPlayerLookNormal().y * 30.f });
			swordSwingFx.setRotation(Utils::Angle(look) + 90);

			isSwing = true;
		}

		if (isCurrentWeapon && isUp)
		{
			SetRotation(Utils::Angle(look) - 10);
		}
		else if (isCurrentWeapon && !isUp)
		{
			SetRotation(Utils::Angle(look) + 190);
		}
	}
}

void ShortSword::Draw(sf::RenderWindow& window)
{
	if (owner)
	{
		if (isCurrentWeapon)
		{
			window.draw(sprite);
			attackBoundHitbox.Draw(window);
		}

		if (isSwing)
		{
			if (swingTimeAccum > swingTimeDelay)
			{
				swingTimeAccum = 0.f;
				isSwing = false;
			}
			window.draw(swordSwingFx);
		}
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

void ShortSword::Attack()
{
	auto& gameObjects = SCENE_MGR.GetCurrentScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (auto* monster = dynamic_cast<Monster*>(gameObject))
		{
			if (Utils::CheckCollision(monster->GetHitBox(), attackBoundHitbox))
			{
				monster->OnDamaged(Utils::RandomRange(originalDamageMin, originalDamageMax));
			}
		}
	}
}