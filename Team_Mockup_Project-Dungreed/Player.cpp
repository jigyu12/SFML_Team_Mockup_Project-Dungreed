#include "stdafx.h"

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

void Player::Init()
{
	speed = 100.f;
	jumpForce = 200.f;
}

void Player::Release()
{

}

void Player::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(playerId));

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
	//if (horizontalInput != 0.f)
	//{
	//	if (horizontalInput > 0.f)
	//	{
	//		SetScale(sf::Vector2f(scale.x, scale.y));
	//	}
	//}
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseworldPos - position);
	SetRotation(Utils::Angle(look));


	SetPosition(position + velocity * dt);

	if (position.y > 0.f)
	{
		position.y = 0.f;
		SetStatus(Status::Ground);
	}

	SetOrigin(Origins::BC);
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

		break;
	default:
		break;

	}
}

void Player::UpdateGrounded(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	if (horizontalInput != 0.f)
	{
		SetScale(horizontalInput > 0.f ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetStatus(Player::Status::Jump);
	}
	else
	{
		direction.x = horizontalInput;
		velocity.x = direction.x * speed;
	}
}

void Player::UpdateJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	velocity += gravity * dt;


	if (InputMgr::GetKeyUp(sf::Keyboard::Space))
	{

		//velocity.y = jumpForce;


	}



}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}