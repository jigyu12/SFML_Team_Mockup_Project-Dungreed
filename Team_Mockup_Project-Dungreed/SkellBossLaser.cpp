#include "stdafx.h"
#include "SkellBossLaser.h"

SkellBossLaser::SkellBossLaser(const std::string& name)
	: SpriteGo(name)
{
}

void SkellBossLaser::SetOrigin(Origins preset)
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

void SkellBossLaser::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SkellBossLaser::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(position);
}

void SkellBossLaser::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(rotation);
}

void SkellBossLaser::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	sprite.setScale(this->scale);
}

void SkellBossLaser::Init()
{
	sortingLayer = SortingLayers::Foreground;
}

void SkellBossLaser::Reset()
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	damage = 9;

	target = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sortingOrder = target->sortingOrder + 2;

	idleTimeAccum = 0.f;
	idleTimeDelay = 0.7f;

	numberOfBodys = 10;

	for (int i = 0; i < numberOfBodys; ++i)
	{
		bodys.push_back(new sf::Sprite());
		bodys[i]->setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

		bodyAnimators.push_back(new Animator());
	}

	animator.SetTarget(&sprite);
	animator.Play("animations/Boss/SkellBoss LaserHead Idle.csv");
	for (int i = 0; i < numberOfBodys; ++i)
	{
		bodyAnimators[i]->SetTarget(bodys[i]);
		bodyAnimators[i]->Play("animations/Boss/SkellBoss LaserBody Idle.csv");
	}

	for (int i = 0; i < numberOfBodys; ++i)
	{
		bodyHitBoxes.push_back(new HitBox());
	}

	isFire = false;
}

void SkellBossLaser::Update(float dt)
{
	if (!isFire)
		return;

	idleTimeAccum += dt;

	if (idleTimeAccum >= idleTimeDelay)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);

		return;
	}

	animator.Update(dt);
	for (int i = 0; i < numberOfBodys; ++i)
	{
		bodyAnimators[i]->Update(dt);
	}

	hitbox.UpdateTr(sprite, GetLocalBounds());
	for (int i = 0; i < numberOfBodys; ++i)
	{
		(*bodyHitBoxes[i]).UpdateTr(*bodys[i], GetLocalBounds());
	}
}

void SkellBossLaser::LateUpdate(float dt)
{
	SetOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });

	if (Utils::CheckCollision(hitbox, target->GetHitBox()) && !target->IsDead())
	{
		target->OnDamage(damage);
	}
	for (int i = 0; i < numberOfBodys; ++i)
	{
		bodys[i]->setOrigin({ GetLocalBounds().width / 2.f , GetLocalBounds().height / 2.f });
		
		if ((*bodys[i]).getGlobalBounds().intersects(target->GetGlobalBounds()) && !target->IsDead())
		{
			target->OnDamage(damage);
		}
	}
}

void SkellBossLaser::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	for (int i = 0; i < numberOfBodys; ++i)
	{
		window.draw(*bodys[i]);
	}

	hitbox.Draw(window);
	for (int i = 0; i < numberOfBodys; ++i)
	{
		(*bodyHitBoxes[i]).Draw(window);
	}
}

void SkellBossLaser::Release()
{
	for (auto& body : bodys)
	{
		delete body;
	}

	for (auto& bodyAnimator : bodyAnimators)
	{
		delete bodyAnimator;
	}

	for (auto& bodyHitBox : bodyHitBoxes)
	{
		delete bodyHitBox;
	}
}

void SkellBossLaser::Fire(sf::Vector2f pos, const sf::Vector2f& s)
{
	isFire = true;

	float dir;

	if (s.x < 0)
		dir = -1.f;
	else
		dir = 1.f;

	pos = {pos.x + 5.f * dir, pos.y};

	SetPosition(pos);
	for (int i = 0; i < numberOfBodys; ++i)
	{
		if(i == 0)
			(*bodys[i]).setPosition({ pos.x += (*bodys[i]).getLocalBounds().width * 5.f / 6.f * dir , pos.y -= 3.f});
		else
			(*bodys[i]).setPosition({ pos.x += (*bodys[i]).getLocalBounds().width  * dir , pos.y });
	}

	SetScale(s);
	for (int i = 0; i < numberOfBodys; ++i)
	{
		(*bodys[i]).setScale(s);
	}
}