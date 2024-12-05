#include "stdafx.h"
#include "BirdGo.h"

BirdGo::BirdGo(const std::string& name)
	:GameObject(name)
{
}

void BirdGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void BirdGo::SetRotation(float angle)
{
	rotation = angle;
}

void BirdGo::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void BirdGo::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void BirdGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}


void BirdGo::SetStatus(Status status)
{
	position.y = Utils::RandomRange(300.f, 700.f);

	this->status = status;
	switch (status)
	{
	case BirdGo::Status::Left:
		body.setScale({ 7.f,7.f });
		SetPosition({ 0.f, position.y });
		break;
	case BirdGo::Status::Right:
		body.setScale({ -7.f,7.f });
		SetPosition({ 1980.f,position.y });
		break;
	default:
		break;
	}
}

void BirdGo::Init()
{
}

void BirdGo::Release()
{
}

void BirdGo::Reset()
{
	animator.SetTarget(&body);
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	animator.Play("animations/Bird Fly.csv");
	speed = 200.f;
if (Utils::RandomRange(0, 1) == 0)
{
	SetStatus(Status::Left);
}
else
{
	SetStatus(Status::Right);
}
}

void BirdGo::Update(float dt)
{
	animator.Update(dt);
	switch (status)
	{
	case BirdGo::Status::Left:
		UpdateLeft(dt);
		break;
	case BirdGo::Status::Right:
		UpdateRight(dt);
		break;
	default:
		break;
	}

}

void BirdGo::UpdateLeft(float dt)
{
	speed = Utils::RandomRange(100, 500);
	position.x += speed * dt;
	SetPosition(position);

	if (position.x > 1980.f)
	{
		if (Utils::RandomRange(0, 1) == 0)
		{
			SetStatus(Status::Left);
		}
		else
		{
			SetStatus(Status::Right);
		}
	}
}

void BirdGo::UpdateRight(float dt)
{
	speed = Utils::RandomRange(100, 700);
	position.x -= speed * dt;
	SetPosition(position);

	if (position.x < 0.f)
	{
		if (Utils::RandomRange(0, 1)== 0)
		{
			SetStatus(Status::Left);
		}
		else
		{
			SetStatus(Status::Right);
		}
	}
}

void BirdGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
