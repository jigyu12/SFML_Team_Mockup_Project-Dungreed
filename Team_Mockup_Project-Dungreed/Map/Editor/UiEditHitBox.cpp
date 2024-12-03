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
	boxWindow.setPosition(position);

	sf::Transform transform = boxWindow.getTransform();

	for (int i = 0; i < editStatusButtons.size();++i)
	{
		editStatusButtons[i].setPosition(transform.transformPoint(50.f + i * 100.f, 30.f));
	}
	for (int i = 0; i < dirButtons.size();++i)
	{
		dirButtons[i].setPosition(transform.transformPoint(50.f, 90.f + i * 60.f));
	}
	for (int i = 0; i < positionTexts.size();++i)
	{
		positionTexts[i].setPosition(transform.transformPoint(150.f, 90.f + i * 60.f));
	}
}

void UiEditHitBox::SetRotation(float angle)
{
	rotation = angle;
	boxWindow.setRotation(rotation);
}

void UiEditHitBox::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
}

void UiEditHitBox::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(boxWindow, originPreset);
	}
}

void UiEditHitBox::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	boxWindow.setOrigin(origin);
}

void UiEditHitBox::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditHitBox::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;

	editStatusButtons.resize((int)EditStatus::Count);
	dirButtons.resize(4);
	positionTexts.resize(4);
	startPositions.resize(4);
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
	boxWindow.setFillColor({ 150,150,150,255 });

	uieditor = dynamic_cast<UiEditor*>(SCENE_MGR.GetCurrentScene()->FindGo("uieditor"));

	for (int i = 0; i < editStatusButtons.size();++i)
	{
		switch ((UiEditHitBox::EditStatus)i)
		{
		case UiEditHitBox::EditStatus::Hitbox:
			editStatusButtons[i].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondraw.png"));
			editStatusButtons[i].setColor({ 150,150,150,255 });
			break;
		case UiEditHitBox::EditStatus::StartPosition:
			editStatusButtons[i].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonstartpos.png"));
			break;
		}
	}
	dirButtons[0].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	dirButtons[0].setColor({ 150,150,150,255 });
	dirButtons[1].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	dirButtons[2].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonleft.png"));
	dirButtons[3].setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonright.png"));

	for (int i = 0; i < positionTexts.size();++i)
	{
		positionTexts[i].setFont(FONT_MGR.Get("fonts/french.ttf"));
	}

	spawnPoint.setTexture(TEXTURE_MGR.Get("graphics/player/CharIdle.png"));
	spawnPoint.setTextureRect({0,0,32,32});
	Utils::SetOrigin(spawnPoint, Origins::BC);

	hitboxStatus = HitBoxEditStatus::Create;
	editStatus = EditStatus::Hitbox;
	startPosType = HitBoxData::Type::PortalUp;
}

void UiEditHitBox::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
	{
		sf::Vector2f uimousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());

		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			for (int i = 0; i < editStatusButtons.size();++i)
			{
				if (editStatusButtons[i].getGlobalBounds().contains(uimousepos))
				{
					editStatusButtons[(int)editStatus].setColor(sf::Color::White);
					editStatusButtons[i].setColor({ 150,150,150,255 });
					editStatus = (EditStatus)i;
				}
			}
			if (editStatus == EditStatus::StartPosition)
			{
				for (int i = 0; i < dirButtons.size();++i)
				{
					if (dirButtons[i].getGlobalBounds().contains(uimousepos))
					{
						dirButtons[(int)startPosType].setColor(sf::Color::White);
						dirButtons[i].setColor({ 150,150,150,255 });
						startPosType = (HitBoxData::Type)i;
					}
				}
			}
		}
	}
	else
	{
		switch (editStatus)
		{
		case UiEditHitBox::EditStatus::Hitbox:
			if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
			{
				sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

				selectedHitBox = nullptr;
				for (auto& hitbox : hitboxes)
				{
					if (Utils::PointInTransformBounds(*hitbox.first, hitbox.first->getLocalBounds(), worldMousePos))
					{
						hitboxStatus = HitBoxEditStatus::Move;
						selectedHitBox = hitbox.first;
						editStartPos = worldMousePos - selectedHitBox->getPosition();
					}
				}
				if (selectedHitBox == nullptr)
				{
					hitboxStatus = HitBoxEditStatus::Create;
					editStartPos = worldMousePos;
					sf::RectangleShape* shape = new sf::RectangleShape();
					shape->setFillColor(sf::Color::Transparent);
					shape->setOutlineColor(sf::Color::Red);
					shape->setOutlineThickness(1.f);
					shape->setPosition(editStartPos);
					selectedHitBox = shape;
					hitboxes.insert({ shape,HitBoxData::Type::Immovable });
				}
			}
			else if (InputMgr::GetMouseButton(sf::Mouse::Left)
				&& selectedHitBox != nullptr)
			{
				sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

				switch (hitboxStatus)
				{
				case UiEditHitBox::HitBoxEditStatus::Create:
					selectedHitBox->setSize(worldMousePos - editStartPos);
					break;
				case UiEditHitBox::HitBoxEditStatus::Move:
					selectedHitBox->setPosition(worldMousePos - editStartPos);
					break;
				}
			}
			if (InputMgr::GetMouseButtonDown(sf::Mouse::Right)
				&& selectedHitBox != nullptr)
			{
				auto found = hitboxes.find(selectedHitBox);
				if (found != hitboxes.end())
				{
					delete found->first;
					hitboxes.erase(found);
					selectedHitBox = nullptr;
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
				selectedHitBox->rotate(-45.f);
			}
			break;
		case UiEditHitBox::EditStatus::StartPosition:
			if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
			{
				sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
				startPositions[(int)startPosType] = worldMousePos;
			}
			break;
		}
	}

	if (editStatus == EditStatus::StartPosition)
	{
		spawnPoint.setPosition(startPositions[(int)startPosType]);
		for (int i = 0; i < positionTexts.size();++i)
		{
			positionTexts[i].setString("POSITION : " + std::to_string(startPositions[i].x).substr(0, 5) + ", " + std::to_string(startPositions[i].y).substr(0, 5));
		}
	}
}

void UiEditHitBox::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);

	for (int i = 0; i < editStatusButtons.size();++i)
	{
		window.draw(editStatusButtons[i]);
	}
	if (editStatus == EditStatus::StartPosition)
	{
		for (int i = 0; i < dirButtons.size();++i)
		{
			window.draw(dirButtons[i]);
		}
		for (int i = 0; i < positionTexts.size();++i)
		{
			window.draw(positionTexts[i]);
		}
	}

	sf::View prev = window.getView();
	sf::View world = SCENE_MGR.GetCurrentScene()->GetWorldView();
	sf::Vector2f size = world.getSize();
	world.move(size.x * 0.125f, 0.f);
	world.setSize(size.x * 0.75f, size.y);
	world.setViewport({ 0.25f,0.f,0.75f,1.f });
	window.setView(world);

	switch (editStatus)
	{
	case UiEditHitBox::EditStatus::Hitbox:
		for (auto& hitbox : hitboxes)
		{
			window.draw(*hitbox.first);
		}
		break;
	case UiEditHitBox::EditStatus::StartPosition:
		window.draw(spawnPoint);
		break;
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

void UiEditHitBox::ClearHitBoxData()
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
}
