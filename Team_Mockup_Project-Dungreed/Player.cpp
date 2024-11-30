#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneDev3.h"
#include "Room.h"
#include "Weapon.h"

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
	jumpForce = 200.f;
	gravity = 300.f;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 2;
}

void Player::Release()
{

}

void Player::Reset()
{


	animator.SetTarget(&body);
	animator.Play("animations/player Idle.csv");
	/*body.setTexture(TEXTURE_MGR.Get(playerId));*/

	hitbox.SetColor(sf::Color::Blue);

	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::BC);
	SetRotation(0.f);

	
}

void Player::SetStatus(Status status)
{
	this->status = (status);
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	switch (status)
	{
	case Player::Status::Ground:
		break;
	case Player::Status::Jump:
		break;
	case Player::Status::Dash:
		velocity = look * dashSpeed;
		dashTimer = 0.f;
		dashCoolTimer = 0.f;
		animator.Play("animations/player Dash.csv");
		break;
	case Player::Status::DownJump:
		velocity.y = downSpeed;
		break;
	
	default:
		break;

	}
}

void Player::Update(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SwitchWeaponSlot(sf::Keyboard::Num1);
	}
	else if(InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SwitchWeaponSlot(sf::Keyboard::Num2);
	}
	

	animator.Update(dt);

	dashCoolTimer += dt;


	switch (status)
	{
	case Player::Status::Ground:
		UpdateGrounded(dt);
		break;
	case Player::Status::Jump:
		UpdateJump(dt);
		break;
	case Player::Status::Dash:
		UpdateDash(dt);
		break;
	case Player::Status::DownJump:
		UpdateDownJump(dt);
		break;
	default:
		break;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::LShift)&& dashCoolTimer >= 1.f)
	{
		SetStatus(Player::Status::Dash);
	}
	if (InputMgr::GetKey(sf::Keyboard::S) && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetStatus(Player::Status::DownJump);
	}
	


	SetPosition(position + velocity * dt);


	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseworldPos - position);
	SetOrigin(Origins::BC);


	hitbox.UpdateTr(body, { 8,12,16,21 });
	
}

void Player::LateUpdate(float dt)
{
	auto playerGlobalBounds = hitbox.rect.getGlobalBounds();
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	auto hitboxBounds = dynamic_cast<Room*>(SCENE_MGR.GetCurrentScene()->FindGo("tilemap"))->GetHitBoxes();
	bool collided = false;
	for (auto& startHitBox : hitboxBounds)
	{
		if (Utils::CheckCollision(*startHitBox.first, hitbox))
		{
			
			Player::CollisionState state = GetCollsionState(playerGlobalBounds, startHitBox.first->rect.getGlobalBounds());
			if (state.Up)
			{
				switch (startHitBox.second.type)
				{
				case HitBoxData::Type::Immovable:
					if (velocity.y < 0)
						velocity.y = 50.f;
					collided = true;
					break;
				case HitBoxData::Type::Downable:
					collided = true;
						break;
				case HitBoxData::Type::PortalDown:
					
				default:
					break;
				}
			}
			if ((state.Down) && velocity.y >= 0.f && startHitBox.first->rect.getGlobalBounds().contains(position))
			{
				switch (startHitBox.second.type)
				{
				case HitBoxData::Type::Immovable:
					SetStatus(Status::Ground);
					position.y = startHitBox.first->rect.getGlobalBounds().top;
					SetPosition(position);
					velocity.y = 0.f;
					collided = true;
					break;
				case HitBoxData::Type::Downable:
					if (status != Status::Dash &&(status != Status::DownJump || startHitBox.first != DownPlatform))
					{
						SetStatus(Status::Ground);
						position.y = startHitBox.first->rect.getGlobalBounds().top;
						SetPosition(position);
						DownPlatform = startHitBox.first;
						collided = true;
					}
					break;
				default:
					break;
				}
				
			}
			if (state.Right && (status == Status::Jump || (state.Down==false)))
			{
				switch (startHitBox.second.type)
				{
				case HitBoxData::Type::Immovable:
					if (horizontalInput>0)
					{
						position.x = startHitBox.first->rect.getGlobalBounds().left- hitbox.rect.getGlobalBounds().width / 2;
						SetPosition(position);
						collided = true;
					}	
					break;
				case HitBoxData::Type::Downable:
					break;
				default:
					break;
				}
			}
			if (state.Left && (status == Status::Jump || (state.Down == false)))
			{
				switch (startHitBox.second.type)
				{
				case HitBoxData::Type::Immovable:
					if (horizontalInput < 0)
					{
						position.x = startHitBox.first->rect.getGlobalBounds().left + startHitBox.first->rect.getGlobalBounds().width
							+ hitbox.rect.getGlobalBounds().width / 2;
						SetPosition(position);
						collided = true;
					}
					break;
				case HitBoxData::Type::Downable:
					break;
				default:
					break;
				}
			}
		}
	}
	if (!collided && status == Status::Ground)
	{
		SetStatus( Status::Jump);
	}
}





void Player::UpdateGrounded(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		Jump();
	}
	if (horizontalInput == 0&&animator.GetCurrentClipId() != "Idle")
	{
		animator.Play("animations/player Idle.csv");
	}

	if (horizontalInput != 0 && animator.GetCurrentClipId() != "Walk")
	{
		animator.Play("animations/player Walk.csv");
	}
}

void Player::UpdateJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	jumpTimer += dt;

	if (jumpTimer > 1.f || !InputMgr::GetKey(sf::Keyboard::Space) || velocity.y > 0.f)
	{
		velocity.y += gravity * dt;
	}
}

void Player::UpdateDownJump(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	if (velocity.y > 0.f)
	{
		velocity.y += gravity * dt;
	}
}

void Player::UpdateDash(float dt)
{
	dashTimer += dt;
	if (dashTimer > 0.3f )
	{
		velocity = { 0.f,0.f };
		SetStatus(Status::Jump);
	}
}

void Player::Jump()
{
	velocity.y = -jumpForce;
	jumpTimer = 0.f;
	SetStatus(Status::Jump);
}

void Player::SetWeaponToWeaponSlot1(Weapon* weapon, bool isCurrentWeapon)
{
	weaponSlot1 = weapon;
	weaponSlot1->SetIsCurrentWeapon(isCurrentWeapon);
}

void Player::SetWeaponToWeaponSlot2(Weapon* weapon, bool isCurrentWeapon)
{
	weaponSlot2 = weapon;
	weaponSlot2->SetIsCurrentWeapon(isCurrentWeapon);
}

void Player::SwitchWeaponSlot(sf::Keyboard::Key key)
{
	if (key != sf::Keyboard::Num1 && key != sf::Keyboard::Num2)
		return;

	if (key == sf::Keyboard::Num1)
	{
		weaponSlot1->SetIsCurrentWeapon(true);
		weaponSlot2->SetIsCurrentWeapon(false);
	}
	else
	{
		weaponSlot1->SetIsCurrentWeapon(false);
		weaponSlot2->SetIsCurrentWeapon(true);
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}