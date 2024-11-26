#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& name)
	:Character(name)
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
	body.setRotation(rotation);
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

}

void Player::Release()
{

}

void Player::Reset()
{

}

void Player::Update(float dt)
{

}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
