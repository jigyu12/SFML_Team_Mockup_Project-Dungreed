#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneDev3.h"
#include "Room.h"
#include "Weapon.h"
#include "PlayerUi.h"
#include "Monster.h"
#include "SkellBossSword.h"

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
	jumpForce = 100.f;
	gravity = 300.f;
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 2;
	maxhp = hp = 90;
}

void Player::Release()
{

}

void Player::Reset()
{

	playerui = dynamic_cast<PlayerUi*>(SCENE_MGR.GetCurrentScene()->FindGo("playerUi"));

	playerStatus.attackDamage = 4;
	playerStatus.level = 1;
	playerStatus.armor = 0;
	playerStatus.armorPercent = 0;
	playerStatus.movementSpeed = 1;
	playerStatus.exp = 0;
	//float attackDamage;
	//int level;
	//float criticalDamage;
	//float exp;
	//float armor;
	//float armorPercent;
	//float movementSpeed;
	//float criticalPercent;
	//float dashDamage;

	animator.SetTarget(&body);
	animator.Play("animations/player Idle.csv");
	/*body.setTexture(TEXTURE_MGR.Get(playerId));*/
	animator.ClearEvent();
	animator.AddEvent("TP", 0, [this]() {SetOrigin(originPreset); });
	animator.AddEvent("TP", 1, [this]() {SetOrigin(originPreset); });
	animator.AddEvent("TP", 2, [this]() {SetOrigin(originPreset); });
	animator.AddEvent("TP", 6, [this]() {SetOrigin(originPreset); });
	animator.AddEvent("TP", 7, [this]() {
		//hp = maxhp;
		//isDead = false;
		//playerui->SetHp(90, 90);
		//animator.Play("animations/player Idle.csv");
		//SetOrigin(originPreset);
		//SetStatus(Status::Ground);
		SCENE_MGR.ChangeScene(SceneIds::MainTitle);
		});
	
	originalPlayerColor = sf::Color::White;

	hitbox.SetColor(sf::Color::Blue);

	SetPosition({ 0.f,0.f });
	SetOrigin(Origins::BC);
	SetRotation(0.f);
	SetStatus(Status::Ground);
	dashCoolTimer = 0.f;
	hp = maxhp;
	SwitchWeaponSlot(sf::Keyboard::Num2);
	SwitchWeaponSlot(sf::Keyboard::Num1);
	LoadFile();
}

void Player::SetStatus(Status status)
{
	this->status = (status);
	switch (status)
	{
	case Player::Status::Ground:
		velocity.y = 0.f;
		break;
	case Player::Status::Jump:
		break;
	case Player::Status::Dash:
		DamagedMonster.clear();
		velocity = look * dashSpeed;
		dashTimer = 0.f;
		dashCoolTimer -= 1.f;
		animator.Play("animations/player Dash.csv");
		break;
	case Player::Status::DownJump:
		velocity.y = downSpeed;
		break;
	case Player::Status::Dead:
		animator.Play("animations/player Dead.csv");
		animator.PlayQueue("animations/player TelePort.csv");
		isDead = true;
		break;
	default:
		break;

	}
}

void Player::Update(float dt)
{

	if (InputMgr::GetKeyDown(sf::Keyboard::M))
	{
		SaveLastData();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::N))
	{
		LoadFile();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::K))
	{
		LevelUpData();
	}
	if (playerStatus.exp >= 8)
	{
		LevelUpData();
		playerStatus.exp = 0;
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SwitchWeaponSlot(sf::Keyboard::Num1);
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SwitchWeaponSlot(sf::Keyboard::Num2);
	}



	animator.Update(dt);

	dashCoolTimer += dt;
	//dashCoolTimer = std::min(dashCoolTimer, 2.f);
	//dashCoolTimer = std::max(dashCoolTimer, 0.f);

	dashCoolTimer = Utils::Clamp(dashCoolTimer, 0.f, 2.f);



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
	case Player::Status::Dead:
		return;
	default:
		break;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::LShift) && dashCoolTimer >= 1.f)
	{
		SetStatus(Player::Status::Dash);
	}




	SetPosition(position + velocity * dt);


	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseworldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseworldPos - position);

	if ((look.x > 0.f && scale.x < 0.f)
		|| (look.x < 0.f && scale.x >0.f))
	{
		SetScale({ -scale.x,scale.y });
	}

	//Utils::Angle(look);
	SetOrigin(Origins::BC);


	hitbox.UpdateTr(body, { 8,12,16,21 });

}

void Player::LateUpdate(float dt)
{


	if (status != Status::Dead)
	{
		auto playerGlobalBounds = hitbox.rect.getGlobalBounds();
		float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);

		auto hitboxBounds = ROOM_MGR.GetCurrentRoom()->GetHitBoxes();
		bool collided = false;
		for (auto& startHitBox : hitboxBounds)
		{
			CollisionState state;
			if (Utils::CheckCollision(hitbox, *startHitBox.first, state))
			{
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
						break;
					case HitBoxData::Type::PortalDown:
						break;
					default:
						break;
					}
				}
				if (state.Down)
				{
					if (velocity.y >= 0.f && state.area.height < 5.f
						&& (state.AspectRatio() < 1.f || (startHitBox.first->rect.getRotation() != 0.f || startHitBox.first->rect.getRotation() != 360.f)))
					{
						switch (startHitBox.second.type)
						{
						case HitBoxData::Type::Immovable:
							SetStatus(Status::Ground);
							position.y = std::min(position.y, state.area.top);
							velocity.y = 0.f;
							SetPosition(position);
							collided = true;
							break;
						case HitBoxData::Type::Downable:
							if (status != Status::Dash
								&& (status != Status::DownJump || startHitBox.first != DownPlatform))
							{
								SetStatus(Status::Ground);
								position.y = std::min(position.y, state.area.top);
								velocity.y = 0.f;
								SetPosition(position);
								DownPlatform = startHitBox.first;
								collided = true;
							}
							break;
						}
					}
				}
				if (state.Right)
				{
					if (status == Status::Dash)
					{
						SetStatus(Status::Jump);
					}

					if (startHitBox.second.type == HitBoxData::Type::Immovable
						&& state.area.height > 5.f
						&& (status == Status::Jump || (state.AspectRatio() > 2.f && state.area.width < 5.f)))
					{
						if (horizontalInput >= 0)
						{
							position.x = std::min(position.x, position.x - state.area.width);
							SetPosition(position);
							collided = true;
						}
					}
				}
				if (state.Left)
				{
					if (status == Status::Dash)
					{
						SetStatus(Status::Jump);
					}
					if (startHitBox.second.type == HitBoxData::Type::Immovable
						&& state.area.height > 5.f
						&& (status == Status::Jump || (state.AspectRatio() > 2.f && state.area.width < 5.f)))
					{
						if (horizontalInput <= 0)
						{
							position.x = std::max(position.x, position.x + state.area.width);
							SetPosition(position);
							collided = true;
						}
					}
				}
			}
		}
		if (!collided && status == Status::Ground)
		{
			SetStatus(Status::Jump);
		}

		const auto& monsters = ROOM_MGR.GetCurrentRoom()->GetMonsters();
		for (auto& monster : monsters)
		{



			auto it = std::find(DamagedMonster.begin(), DamagedMonster.end(), monster);
			if (it != DamagedMonster.end())
				continue;


			if (Utils::CheckCollision(monster->GetHitBox(), hitbox))
			{
				if (!monster->IsDead() && !isDead)
				{
					if (monster->GetOriginalDamage() != 0)
					{
						OnDamage(monster->GetOriginalDamage());
					}
					if (status == Status::Dash)
					{
						if (GetCurrentWeapon() == weaponSlot1)
						{
							int swordDashDamage = CalculationDamage(weaponSlot1->GetAttackDamage() / 2);
							DamagedMonster.push_back(monster);
							monster->OnDamaged(swordDashDamage);
						}
						else
						{
							int bowDashDamage = CalculationDamage(weaponSlot2->GetAttackDamage() / 2);
							DamagedMonster.push_back(monster);
							monster->OnDamaged(bowDashDamage);
						}
					}
				}
			}
		}


		/*const auto& bossSwords = dynamic_cast<SkellBossSword*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBossSword"));*/
		if (isDamaged)
		{
			invincibilityTimer += dt;
			if (invincibilityTimer > 1.f)
			{
				invincibilityTimer = 0.f;
				isDamaged = false;
				body.setColor(originalPlayerColor);
			}
		}

		if (hp <= 0)
		{
			SetStatus(Status::Dead);
		}
	}
}





void Player::UpdateGrounded(float dt)
{
	float horizontalInput = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.x = horizontalInput;
	velocity.x = direction.x * speed;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		if (InputMgr::GetKey(sf::Keyboard::S))
		{
			SetStatus(Player::Status::DownJump);

		}
		else
			Jump();
	}
	if (horizontalInput == 0 && animator.GetCurrentClipId() != "Idle")
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

	if (jumpTimer > 0.5f || !InputMgr::GetKey(sf::Keyboard::Space) || velocity.y > 0.f)
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
	if (dashTimer > 0.2f)
	{
		velocity = { 0.f,0.f };
		SetStatus(Status::Jump);
	}
}

void Player::UpdateDead(float dt)
{
	deadAniTimer += dt;
}

void Player::Jump()
{
	velocity.y = -jumpForce;
	jumpTimer = 0.f;
	SetStatus(Status::Jump);
}

void Player::OnDamage(int monsterDamage)
{
	if (isDamaged || status == Status::Dash)
	{
		return;
	}

	isDamaged = true;
	invincibilityTimer = 0.f;
	sf::Color currColor = body.getColor();
	body.setColor({ currColor.r, currColor.g, currColor.b, 80 });

	hp = Utils::Clamp(hp - monsterDamage, 0, maxhp);

	if (playerui != nullptr)
		playerui->SetHp(hp, maxhp);
}




void Player::SaveLastData()
{
	SaveDataVC saveStatus;
	saveStatus.status = playerStatus;
	saveStatus.status.lastFloor = ROOM_MGR.GetCurrentFloor();
	if (saveStatus.status.lastFloor % 2 == 0)
	{
		--saveStatus.status.lastFloor;
	}
	SAVELOAD_MGR.Save(saveStatus);
}

void Player::LoadFile()
{
	SaveDataVC saveStatus = SAVELOAD_MGR.Load();
	playerStatus = saveStatus.status;
	
	SwitchWeaponSlot(sf::Keyboard::Num1);
}

int Player::GetRealDamage()
{
	if (weaponSlot1->IsCurrentWeapon())
	{
		return (playerStatus.attackDamage + weaponSlot1->GetOriginalDamageMax()) * playerStatus.criticalDamage;
	}
	else
	{
		return (playerStatus.attackDamage + weaponSlot2->GetOriginalDamageMax()) * playerStatus.criticalDamage;
	}
}

int Player::GetMinDamage()
{
	if (weaponSlot1->IsCurrentWeapon())
	{
		return (weaponSlot1->GetOriginalDamageMin());
	}
	else
	{
		return (weaponSlot2->GetOriginalDamageMin());
	}
}

void Player::LevelUpData()
{
	playerStatus.level += 1;
	playerStatus.attackDamage += 1;

}


int Player::CalculationDamage(int damage)
{
	damage += Utils::RandomRange(0.f, playerStatus.attackDamage);
	if (playerStatus.criticalPercent > Utils::RandomRange(0.f, 100.f))
	{
		damage *= playerStatus.criticalDamage;

	}
	return damage;
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
		if (!weaponSlot1->IsCurrentWeapon())
		{
			weaponSlot1->SetIsCurrentWeapon(true);
			weaponSlot1->SetAttackSpeedAccumTime(weaponSlot1->GetAttackSpeed() - 0.5f);
			weaponSlot2->SetIsCurrentWeapon(false);
			playerStatus.criticalPercent = 30;
			playerStatus.criticalDamage = 2;
			playerStatus.attackSpeed = weaponSlot1->GetAttackSpeed();
			playerStatus.dashDamage = 50;
		}
	}
	else
	{
		if (!weaponSlot2->IsCurrentWeapon())
		{
			weaponSlot2->SetIsCurrentWeapon(true);
			weaponSlot2->SetAttackSpeedAccumTime(weaponSlot2->GetAttackSpeed() - 0.5f);
			weaponSlot1->SetIsCurrentWeapon(false);
			playerStatus.criticalPercent = 20;
			playerStatus.criticalDamage = 3;
			playerStatus.attackSpeed = weaponSlot2->GetAttackSpeed();
			playerStatus.dashDamage = 50;
		}

	}
}

Weapon* Player::GetCurrentWeapon() const
{
	if (weaponSlot1->IsCurrentWeapon())
	{
		return weaponSlot1;
	}
	else
	{
		return weaponSlot2;
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}