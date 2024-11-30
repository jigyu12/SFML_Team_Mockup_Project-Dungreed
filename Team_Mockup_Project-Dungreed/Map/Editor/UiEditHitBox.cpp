#include "stdafx.h"
#include "UiEditHitBox.h"
#include "TileMap.h"
#include "UiEditor.h"

UiEditHitBox::UiEditHitBox(const std::string& name)
	: GameObject(name)
{
}

void UiEditHitBox::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditHitBox::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditHitBox::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditHitBox::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditHitBox::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditHitBox::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiEditHitBox::Release()
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
}

void UiEditHitBox::Reset()
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
	selectedHitBox = nullptr;

	uieditor = dynamic_cast<UiEditor*>(SCENE_MGR.GetCurrentScene()->FindGo("uieditor"));

	status = EditStatus::Create;
}

void UiEditHitBox::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
		return;
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
		
		selectedHitBox = nullptr;
		for (auto& hitbox : hitboxes)
		{
			if (Utils::PointInTransformBounds(*hitbox.first, hitbox.first->getLocalBounds(), worldMousePos))
			{
				status = EditStatus::MoveOld;
				selectedHitBox = hitbox.first;
				startPos = worldMousePos - selectedHitBox->getPosition();
			}
		}
		if (selectedHitBox == nullptr)
		{
			status = EditStatus::Create;
			startPos = worldMousePos;
			sf::RectangleShape* shape = new sf::RectangleShape();
			shape->setFillColor(sf::Color::Transparent);
			shape->setOutlineColor(sf::Color::Red);
			shape->setOutlineThickness(1.f);
			shape->setPosition(startPos);
			selectedHitBox = shape;
			hitboxes.insert({ shape,HitBoxData::Type::Immovable });
		}
	}
	else if (InputMgr::GetMouseButton(sf::Mouse::Left)
		&& selectedHitBox != nullptr)
	{
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

		switch (status)
		{
		case UiEditHitBox::EditStatus::Create:
			selectedHitBox->setSize(worldMousePos - startPos);
			break;
		case UiEditHitBox::EditStatus::MoveOld:
			selectedHitBox->setPosition(worldMousePos - startPos);
			break;
		case UiEditHitBox::EditStatus::ResizeOld:
			break;
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Right)
		&& selectedHitBox != nullptr)
	{
		auto found = hitboxes.find(selectedHitBox);
		if (found != hitboxes.end())
		{
			hitboxes.erase(found->first);
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Middle)
		&& selectedHitBox != nullptr)
	{
		if (hitboxes[selectedHitBox] == HitBoxData::Type::Immovable)
		{
			hitboxes[selectedHitBox] = HitBoxData::Type::Downable;
			selectedHitBox->setOutlineColor(sf::Color::Yellow);
		}
		else
		{
			sf::FloatRect boxbound = selectedHitBox->getGlobalBounds();

			if (boxbound.left <= 0.f
				&& hitboxes[selectedHitBox] != HitBoxData::Type::PortalLeft)
			{
				hitboxes[selectedHitBox] = HitBoxData::Type::PortalLeft;
				selectedHitBox->setOutlineColor(sf::Color::Cyan);
			}
			else if (boxbound.top <= 0.f
				&& hitboxes[selectedHitBox] != HitBoxData::Type::PortalUp)
			{
				hitboxes[selectedHitBox] = HitBoxData::Type::PortalUp;
				selectedHitBox->setOutlineColor(sf::Color::Cyan);
			}
			else if (uieditor != nullptr && boxbound.left + boxbound.width >= uieditor->GetSelectedTileMap()->GetGlobalBounds().width
				&& hitboxes[selectedHitBox] != HitBoxData::Type::PortalRight)
			{
				hitboxes[selectedHitBox] = HitBoxData::Type::PortalRight;
				selectedHitBox->setOutlineColor(sf::Color::Cyan);
			}
			else if (uieditor != nullptr && boxbound.top + boxbound.height >= uieditor->GetSelectedTileMap()->GetGlobalBounds().height
				&& hitboxes[selectedHitBox] != HitBoxData::Type::PortalDown)
			{
				hitboxes[selectedHitBox] = HitBoxData::Type::PortalDown;
				selectedHitBox->setOutlineColor(sf::Color::Cyan);
			}
			else
			{
				hitboxes[selectedHitBox] = HitBoxData::Type::Immovable;
				selectedHitBox->setOutlineColor(sf::Color::Red);
			}
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::XButton1)
		&& selectedHitBox != nullptr)
	{
		selectedHitBox->rotate(45.f);
	}
}

void UiEditHitBox::Draw(sf::RenderWindow& window)
{
	sf::View prev = window.getView();
	sf::View world = SCENE_MGR.GetCurrentScene()->GetWorldView();
	sf::Vector2f size = world.getSize();
	world.move(size.x * 0.125f, 0.f);
	world.setSize(size.x * 0.75f, size.y);
	world.setViewport({ 0.25f,0.f,0.75f,1.f });
	window.setView(world);
	for (auto& hitbox : hitboxes)
	{
		window.draw(*hitbox.first);
	}
	window.setView(prev);
}

std::vector<HitBoxData> UiEditHitBox::GetHitBoxData() const
{
	std::vector<HitBoxData> data;
	for (auto& hitBox : hitboxes)
	{
		HitBoxData datum;
		datum.origin = hitBox.first->getPosition();
		datum.rotation = hitBox.first->getRotation();
		datum.size = hitBox.first->getSize();
		datum.type = hitBox.second;
		data.push_back(datum);
	}
	return data;
}

void UiEditHitBox::SetHitBoxData(const std::vector<HitBoxData>& data)
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();

	for (auto& datum : data)
	{
		sf::RectangleShape* shape = new sf::RectangleShape();
		shape->setPosition(datum.origin);
		shape->setRotation(datum.rotation);
		shape->setSize(datum.size);
		shape->setFillColor(sf::Color::Transparent);
		shape->setOutlineThickness(1.f);
		switch (datum.type)
		{
		case HitBoxData::Type::PortalUp:
		case HitBoxData::Type::PortalDown:
		case HitBoxData::Type::PortalLeft:
		case HitBoxData::Type::PortalRight:
			shape->setOutlineColor(sf::Color::Cyan);
			break;
		case HitBoxData::Type::Immovable:
			shape->setOutlineColor(sf::Color::Red);
			break;
		case HitBoxData::Type::Downable:
			shape->setOutlineColor(sf::Color::Yellow);
			break;
		}
		hitboxes.insert({ shape ,datum.type });
	}
}
