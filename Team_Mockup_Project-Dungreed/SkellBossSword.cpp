#include "stdafx.h"
#include "SkellBossSword.h"
#include "SkellBoss.h"
#include "Player.h"
#include "Room.h"

SkellBossSword::SkellBossSword(const std::string& name)
	: SpriteGo(name)
{
}

void SkellBossSword::SetOrigin(Origins preset)
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

void SkellBossSword::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SkellBossSword::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void SkellBossSword::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void SkellBossSword::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void SkellBossSword::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void SkellBossSword::Reset()
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	speed = 350.f;
	damage = 8;

	owner = dynamic_cast<SkellBoss*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBoss"));
	sortingOrder = owner->sortingOrder - 1;

	target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	idleTimeDelay = 1.5f;

	attackTimeDelay = Utils::RandomRange(0.05f, 0.2f);

	destroyTimeDealy = 1.f;

	isFire = false;

	SetState(SkellBossSwordState::Idle);
}

void SkellBossSword::Update(float dt)
{
	if (!isFire)
		return;

	switch (state)
	{
	case SkellBossSwordState::Idle:
	{
		UpdateIdle(dt);
	}
	break;
	case SkellBossSwordState::Attack:
	{
		UpdateAttack(dt);
	}
	break;
	case SkellBossSwordState::Destroy:
	{
		UpdateDestroy(dt);
	}
	}
}

void SkellBossSword::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	if (Utils::CheckCollision(hitbox, target->GetHitBox()) && !target->IsDead())
	{
		target->OnDamage(damage);
	}
}

void SkellBossSword::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	hitbox.Draw(window);
}

void SkellBossSword::Release()
{
}

void SkellBossSword::SetState(SkellBossSwordState state)
{
	this->state = state;

	switch (state)
	{
	case SkellBossSwordState::Idle:
	{
		SetTextureId("graphics/boss/BossAttackSword/BossSword.png");
		sprite.setTexture(TEXTURE_MGR.Get(textureId));

		hitbox.UpdateTr(sprite, { 0.f,0.f,0.f,0.f });
		idleTimeAccum = 0.f;
	}
	break;
	case SkellBossSwordState::Attack:
	{
		SetTextureId("graphics/boss/BossAttackSword/BossSwordFX.png");
		sprite.setTexture(TEXTURE_MGR.Get(textureId));

		hitbox.UpdateTr(sprite, GetLocalBounds());
		attackTimeAccum = 0.f;
	}
	break;
	case SkellBossSwordState::Destroy:
	{
		SetTextureId("graphics/boss/BossAttackSword/BossSword.png");
		sprite.setTexture(TEXTURE_MGR.Get(textureId));

		hitbox.UpdateTr(sprite, { 0.f,0.f,0.f,0.f });
		destroyTimeAccum = 0.f;
	}
	break;
	}
}

void SkellBossSword::UpdateIdle(float dt)
{
	direction = Utils::GetNormal(target->GetPosition() - position);
	SetRotation(Utils::Angle(direction) + 90);
	idleTimeAccum += dt;
	if (idleTimeAccum >= idleTimeDelay)
	{
		SetState(SkellBossSwordState::Attack);
	}
}

void SkellBossSword::UpdateAttack(float dt)
{
	SetPosition(position + direction * speed * dt);
	hitbox.UpdateTr(sprite, GetLocalBounds());

	auto hitboxBounds = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
	bool collided = false;
	for (auto& startHitBox : hitboxBounds)
	{
		if (Utils::CheckCollision(hitbox, *startHitBox.first))
		{
			if (startHitBox.second.type != HitBoxData::Type::Downable)
				collided = true;
		}
	}

	if (collided)
	{
		attackTimeAccum += dt;
		if (attackTimeAccum >= attackTimeDelay)
		{
			auto& swords = owner->GetSwords();
			for (auto it = swords.begin(); it != swords.end(); it++)
			{
				if (*it == this)
				{
					swords.erase(it);

					break;
				}
			}
			SetState(SkellBossSwordState::Destroy);
		}
	}
}

void SkellBossSword::UpdateDestroy(float dt)
{
	destroyTimeAccum += dt;
	if (destroyTimeAccum >= destroyTimeDealy)
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
}

void SkellBossSword::Fire(const sf::Vector2f& pos)
{
	isFire = true;

	SetPosition(pos);
}