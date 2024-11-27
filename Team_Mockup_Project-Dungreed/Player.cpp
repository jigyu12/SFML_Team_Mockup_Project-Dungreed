#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneDev3.h"
#include "Room.h"

Player::Player(const std::string& name)
	:Character(name), velocity({ 0.f,0.f })
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	body.setScale(this->scale);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = Utils::SetOrigin(body, originPreset);
}

Player::CollisionState Player::GetCollsionState(const sf::FloatRect& player, const sf::FloatRect& stage)
{
	Player::CollisionState state;
	if (stage.top + stage.height > player.top
		&& player.top + player.height > stage.top + stage.height)
		state.Up = true;
	if (stage.left + stage.width > player.left
		&& player.left + player.width > stage.left + stage.width)
		state.Left = true;
	if (player.left + player.width > stage.left
		&& stage.left > player.left)
		state.Right = true;
	if (player.top + player.height > stage.top
		&& stage.top > player.top)
		state.Down = true;

	return state;
}

void Player::Init()
{
	speed = 100.f;
	jumpForce = 400.f;
	gravity = 300.f;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 2;
}

void Player::Release()
{

}

void Player::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(playerId));
	sword.setTexture(TEXTURE_MGR.Get(swordId));

	sword.setOrigin(Utils::SetOrigin(sword, Origins::BC));


	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::BC);
	SetRotation(0.f);
}

void Player::Update(float dt)
{
	animator.Update(dt);

	switch (status)
	{
	case Player::Status::Ground:
		UpdateGrounded(dt);
		break;
	case Player::Status::Jump:
		UpdateJump(dt);
		break;
	default:
		break;
	}

	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;


	SetPosition(position + velocity * dt);
	sword.setPosition({ body.getPosition().x ,body.getPosition().y - 9 });


	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseworldPos - sword.getPosition());
	sword.setRotation(Utils::Angle(look) + 90);
	SetOrigin(Origins::BC);
	
	hitbox.SetColor(sf::Color::Blue);
	hitbox.UpdateTr(body, GetGlobalBounds());

	auto hitboxBounds = dynamic_cast<Room*>(SCENE_MGR.GetCurrentScene()->FindGo("tilemap"))->GetHitBoxes();
	for (auto& startHitBox : hitboxBounds)
	{
		if (Utils::CheckCollision(*startHitBox.first, hitbox))
		{
			Player::CollisionState state = GetCollsionState(hitbox.rect.getGlobalBounds(), startHitBox.first->rect.getGlobalBounds());
			if (state.Up)
			{
				if (velocity.y < 0)
					velocity.y *= -0.8f;
			}
			if (state.Down)
			{
				Player::Status::Ground;
				position.y = startHitBox.first->rect.getGlobalBounds().top;
				SetPosition(position);
				velocity.y = 0.f;
			}

			
			
		}
	}

}

void Player::LateUpdate(float dt)
{
}



void Player::SetStatus(Status status)
{
	this->status = (status);

	switch (status)
	{
	case Player::Status::Ground:
		break;
	case Player::Status::Jump:
		velocity.y = -jumpForce;
		jumpTimer = 0.f;
		break;
	default:
		break;

	}
}

void Player::UpdateGrounded(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetStatus(Player::Status::Jump);
	}

}

void Player::UpdateJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);

	jumpTimer += dt;

	if (jumpTimer < 1.f && InputMgr::GetKey(sf::Keyboard::Space))
	{
		velocity.y = -jumpForce;
	}
	else
	{
		velocity.y += gravity * dt;
	}



}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	window.draw(sword);
	hitbox.Draw(window);
}