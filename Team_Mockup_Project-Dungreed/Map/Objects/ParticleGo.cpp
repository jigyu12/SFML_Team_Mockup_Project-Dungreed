#include "stdafx.h"
#include "ParticleGo.h"
#include "Room.h"

ParticleGo::ParticleGo(const std::string& name)
	: GameObject(name)
{
}

void ParticleGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void ParticleGo::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void ParticleGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void ParticleGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ParticleGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ParticleGo::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 20;
}

void ParticleGo::Release()
{
	returnThis();
}

void ParticleGo::Reset()
{
}

void ParticleGo::Update(float dt)
{
	timer += dt;
	if (timer > 3.f && returnThis)
	{
		returnThis();
	}
	SetRotation(rotation + rotationSpeed + dt);
	velocity += gravity * dt;
	SetPosition(position + velocity * dt);
	hitbox.UpdateTr(body, body.getLocalBounds());
}

void ParticleGo::LateUpdate(float dt)
{
	auto hitboxBounds = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
	for (auto& startHitBox : hitboxBounds)
	{
		if (startHitBox.second.type == HitBoxData::Type::Immovable)
		{
			CollisionState state;
			if (Utils::CheckCollision(hitbox, *startHitBox.first, state))
			{
				if (state.Up && velocity.y < 0.f)
				{
					velocity.y *= -0.5f;
				}
				if (state.Down)
				{
					position.y = std::min(position.y, state.area.top);
					if (velocity.y > 0.f)
					{
						velocity.y *= -0.25f;
					}
					float multiflier = powf(0.368f, dt);
					velocity.x *= multiflier;
					rotationSpeed *= multiflier;
				}
				if (state.Right)
				{
					position.x = std::min(position.x, state.area.left);
					if (velocity.x > 0.f)
					{
						velocity.x *= -0.25f;
					}
				}
				if (state.Left)
				{
					position.x = std::max(position.x, state.area.left + state.area.width);
					if (velocity.x < 0.f)
					{
						velocity.x *= -0.25f;
					}
				}
			}
		}
	}
}

void ParticleGo::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}

void ParticleGo::Start(const std::string& name, const sf::Vector2f& position)
{
	body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", name)));
	SetPosition(position);
	velocity = Utils::RandomInUnitCircle() * 100.f;
	rotationSpeed = Utils::RandomRange(-10.f, 10.f);
	gravity.y = 50.f;
	timer = 0.f;
	SetOrigin(Origins::MC);
}
