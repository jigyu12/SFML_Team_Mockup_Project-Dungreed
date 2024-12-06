#include "stdafx.h"
#include "UiEditHitBox.h"
#include "TileMap.h"
#include "UiEditor.h"
#include "Button.h"
#include "TextGo.h"

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
		editStatusButtons[i]->SetPosition(transform.transformPoint(50.f + i * 100.f, 30.f));
	}
	for (int i = 0; i < dirButtons.size();++i)
	{
		dirButtons[i]->SetPosition(transform.transformPoint(50.f, 90.f + i * 60.f));
	}
	for (int i = 0; i < positionTexts.size();++i)
	{
		positionTexts[i]->SetPosition(transform.transformPoint(150.f, 90.f + i * 60.f));
	}
	for (int i = 0; i < hitBoxTypeButtons.size();++i)
	{
		hitBoxTypeButtons[i]->SetPosition(transform.transformPoint(50.f + (i % 4) * 100.f, 150.f + (i / 4) * 60.f));
	}
	for (int i = 0; i < roomTypeButtons.size();++i)
	{
		roomTypeButtons[i]->SetPosition(transform.transformPoint(50.f + 100 * i, 90.f));
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
	hitBoxTypeButtons.resize((int)HitBoxData::Type::Count);
	dirButtons.resize(4);
	positionTexts.resize(4);
	startPositions.resize(4);
	roomTypeButtons.resize((int)RoomData::Type::Count);

	for (int i = 0;i < editStatusButtons.size();++i)
	{
		editStatusButtons[i] = new Button();
		editStatusButtons[i]->Init();
	}
	for (int i = 0;i < hitBoxTypeButtons.size();++i)
	{
		hitBoxTypeButtons[i] = new Button();
		hitBoxTypeButtons[i]->Init();
	}
	for (int i = 0;i < dirButtons.size();++i)
	{
		dirButtons[i] = new Button();
		dirButtons[i]->Init();
		positionTexts[i] = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
		positionTexts[i]->Init();
	}
	for (int i = 0; i < roomTypeButtons.size();++i)
	{
		roomTypeButtons[i] = new Button();
		roomTypeButtons[i]->Init();
	}
}

void UiEditHitBox::Release()
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}

	hitboxes.clear();

	for (int i = 0;i < editStatusButtons.size();++i)
	{
		editStatusButtons[i]->Release();
		delete editStatusButtons[i];
	}
	editStatusButtons.clear();
	for (int i = 0;i < hitBoxTypeButtons.size();++i)
	{
		hitBoxTypeButtons[i]->Release();
		delete hitBoxTypeButtons[i];
	}
	hitBoxTypeButtons.clear();
	for (int i = 0;i < dirButtons.size();++i)
	{
		dirButtons[i]->Release();
		delete dirButtons[i];
		positionTexts[i]->Release();
		delete positionTexts[i];
	}
	dirButtons.clear();
	positionTexts.clear();
	for (int i = 0; i < roomTypeButtons.size();++i)
	{
		roomTypeButtons[i]->Release();
		delete roomTypeButtons[i];
	}
	roomTypeButtons.clear();
}

void UiEditHitBox::Reset()
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
	selectedHitBox = nullptr;
	boxWindow.setFillColor({ 120,120,120 });

	uieditor = dynamic_cast<UiEditor*>(SCENE_MGR.GetCurrentScene()->FindGo("uieditor"));

	for (int i = 0; i < editStatusButtons.size();++i)
	{
		editStatusButtons[i]->Reset();
		editStatusButtons[i]->Set({ 90.f,45.f }, 16);
		editStatusButtons[i]->SetClickedEvent([this, i]() { SetEditStatus((EditStatus)i); });
		switch ((UiEditHitBox::EditStatus)i)
		{
		case UiEditHitBox::EditStatus::Hitbox:
			editStatusButtons[i]->SetString("Draw", true);
			break;
		case UiEditHitBox::EditStatus::StartPosition:
			editStatusButtons[i]->SetString("StartPos", true);
			break;
		}
	}

	for (int i = 0; i < dirButtons.size();++i)
	{
		dirButtons[i]->Reset();
		dirButtons[i]->Set({ 90.f,45.f }, 20);
		switch (i)
		{
		case (int)HitBoxData::Type::PortalUp:
			dirButtons[i]->SetString("Up", true);
			break;
		case (int)HitBoxData::Type::PortalDown:
			dirButtons[i]->SetString("Down", true);
			break;
		case (int)HitBoxData::Type::PortalLeft:
			dirButtons[i]->SetString("Left", true);
			break;
		case (int)HitBoxData::Type::PortalRight:
			dirButtons[i]->SetString("Right", true);
			break;
		}
		dirButtons[i]->SetClickedEvent([this, i]() { hitboxType = (HitBoxData::Type)i; });
	}

	for (int i = 0;i < hitBoxTypeButtons.size();++i)
	{
		hitBoxTypeButtons[i]->Reset();
		hitBoxTypeButtons[i]->Set({ 90.f,45.f }, 16);
		switch (i)
		{
		case (int)HitBoxData::Type::PortalUp:
			hitBoxTypeButtons[i]->SetString("PortalUp", true);
			break;
		case (int)HitBoxData::Type::PortalDown:
			hitBoxTypeButtons[i]->SetString("PortalDown", true);
			break;
		case (int)HitBoxData::Type::PortalLeft:
			hitBoxTypeButtons[i]->SetString("PortalLeft", true);
			break;
		case (int)HitBoxData::Type::PortalRight:
			hitBoxTypeButtons[i]->SetString("PortalRight", true);
			break;
		case (int)HitBoxData::Type::Immovable:
			hitBoxTypeButtons[i]->SetString("Immovable", true);
			break;
		case (int)HitBoxData::Type::Downable:
			hitBoxTypeButtons[i]->SetString("Downable", true);
			break;
		case (int)HitBoxData::Type::SpawnTrigger:
			hitBoxTypeButtons[i]->SetString("SpawnTrigger", true);
			break;
		case (int)HitBoxData::Type::Spike:
			hitBoxTypeButtons[i]->SetString("Spike", true);
			break;
		}
		hitBoxTypeButtons[i]->SetClickedEvent([this, i]() { hitboxType = (HitBoxData::Type)i; });

	}

	for (int i = 0; i < roomTypeButtons.size();++i)
	{
		roomTypeButtons[i]->Reset();
		roomTypeButtons[i]->Set({ 90.f,45.f }, 16);
		switch ((RoomData::Type)i)
		{
		case RoomData::Type::Enter:
			roomTypeButtons[i]->SetString("Enter", true);
			break;
		case RoomData::Type::Exit:
			roomTypeButtons[i]->SetString("Exit", true);
			break;
		case RoomData::Type::Normal:
			roomTypeButtons[i]->SetString("Normal", true);
			break;
		}
		roomTypeButtons[i]->SetClickedEvent([this, i]() { selectedRoomType = (RoomData::Type)i; });
	}

	for (int i = 0; i < positionTexts.size();++i)
	{
		positionTexts[i]->Reset();
		positionTexts[i]->Set(30);
		positionTexts[i]->SetString("Position", true);
	}

	spawnPoint.setTexture(TEXTURE_MGR.Get("graphics/player/CharIdle.png"));
	spawnPoint.setTextureRect({ 0,0,32,32 });
	Utils::SetOrigin(spawnPoint, Origins::BC);

	hitboxStatus = HitBoxEditStatus::Create;
	editStatus = EditStatus::Hitbox;
	hitboxType = HitBoxData::Type::Immovable;
	selectedRoomType = RoomData::Type::Normal;
}

void UiEditHitBox::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
	{
		for (int i = 0; i < hitBoxTypeButtons.size();++i)
		{
			hitBoxTypeButtons[i]->SetPressed(i == (int)hitboxType);
		}
		for (int i = 0; i < roomTypeButtons.size();++i)
		{
			roomTypeButtons[i]->SetPressed(i == (int)selectedRoomType);
		}
		for (int i = 0; i < dirButtons.size();++i)
		{
			dirButtons[i]->SetPressed(i == (int)hitboxType);
		}
		for (int i = 0; i < editStatusButtons.size();++i)
		{
			editStatusButtons[i]->Update(dt);
		}
		switch (editStatus)
		{
		case UiEditHitBox::EditStatus::Hitbox:
			for (int i = 0; i < hitBoxTypeButtons.size();++i)
			{
				hitBoxTypeButtons[i]->Update(dt);
			}
			for (int i = 0; i < roomTypeButtons.size();++i)
			{
				roomTypeButtons[i]->Update(dt);
			}
			break;
		case UiEditHitBox::EditStatus::StartPosition:
			for (int i = 0; i < dirButtons.size();++i)
			{
				dirButtons[i]->Update(dt);
			}
			break;
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

				if (selectedHitBox != nullptr)
				{
					selectedHitBox->setFillColor(sf::Color::Transparent);
				}

				selectedHitBox = nullptr;
				for (auto& hitbox : hitboxes)
				{
					if (Utils::PointInTransformBounds(*hitbox.first, hitbox.first->getLocalBounds(), worldMousePos))
					{
						hitboxStatus = HitBoxEditStatus::Move;
						selectedHitBox = hitbox.first;
						selectedHitBox->setFillColor({ 100,100,100,100 });
						editStartPos = worldMousePos - selectedHitBox->getPosition();
						hitBoxTypeButtons[(int)hitboxType]->SetPressed(false);
						hitBoxTypeButtons[(int)hitbox.second]->SetPressed(true);
						hitboxType = hitbox.second;
					}
				}
				if (selectedHitBox == nullptr)
				{
					for (auto& hitbox : hitboxes)
					{
						if (hitboxType != HitBoxData::Type::Downable
							&& hitboxType != HitBoxData::Type::Immovable
							&& hitboxType == hitbox.second)
						{
							selectedHitBox = hitbox.first;
							selectedHitBox->setFillColor({ 100,100,100,100 });
							if (Utils::PointInTransformBounds(*hitbox.first, hitbox.first->getLocalBounds(), worldMousePos))
							{
								hitboxStatus = HitBoxEditStatus::Move;
								editStartPos = worldMousePos - selectedHitBox->getPosition();
							}
							else
							{
								hitboxStatus = HitBoxEditStatus::Create;
								selectedHitBox->setSize({ 0.f,0.f });
								editStartPos = worldMousePos;
								selectedHitBox->setPosition(editStartPos);
							}
						}
					}
				}

				if (selectedHitBox == nullptr)
				{
					hitboxStatus = HitBoxEditStatus::Create;
					editStartPos = worldMousePos;
					sf::RectangleShape* shape = new sf::RectangleShape();
					shape->setFillColor({ 100,100,100,100 });
					switch (hitboxType)
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
					case HitBoxData::Type::SpawnTrigger:
						shape->setOutlineColor(sf::Color::White);
						break;
					case HitBoxData::Type::Spike:
						shape->setOutlineColor(sf::Color::Black);
						break;
					}
					shape->setOutlineThickness(1.f);
					shape->setPosition(editStartPos);
					selectedHitBox = shape;
					hitboxes.insert({ shape,hitboxType });
				}
			}
			if (selectedHitBox != nullptr)
			{
				if (InputMgr::GetMouseButton(sf::Mouse::Left))
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
				if ((InputMgr::GetMouseButtonDown(sf::Mouse::Right))
					|| (InputMgr::GetMouseButtonUp(sf::Mouse::Left)
						&& std::fabs(selectedHitBox->getSize().x) < 16.f
						&& std::fabs(selectedHitBox->getSize().y) < 16.f))
				{
					auto found = hitboxes.find(selectedHitBox);
					if (found != hitboxes.end())
					{
						delete found->first;
						hitboxes.erase(found);
						selectedHitBox = nullptr;
					}
				}
				if (InputMgr::GetMouseButtonDown(sf::Mouse::Middle))
				{
					selectedHitBox->rotate(45.f);
				}
				if (InputMgr::GetMouseButtonDown(sf::Mouse::XButton1))
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
			}
			break;
		case UiEditHitBox::EditStatus::StartPosition:
			if (InputMgr::GetMouseButtonDown(sf::Mouse::Left)
				&& hitboxType < (HitBoxData::Type)4)
			{
				sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
				startPositions[(int)hitboxType] = worldMousePos;
				spawnPoint.setPosition(startPositions[(int)hitboxType]);
				positionTexts[(int)hitboxType]->SetString("Position"
					, std::to_wstring(startPositions[(int)hitboxType].x).substr(0, 5)
					+ L", "
					+ std::to_wstring(startPositions[(int)hitboxType].y).substr(0, 5));
			}
			break;
		}
	}

}

void UiEditHitBox::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);

	for (int i = 0; i < editStatusButtons.size();++i)
	{
		editStatusButtons[i]->Draw(window);
	}
	if (editStatus == EditStatus::Hitbox)
	{
		for (int i = 0; i < hitBoxTypeButtons.size();++i)
		{
			hitBoxTypeButtons[i]->Draw(window);
		}
		for (int i = 0; i < roomTypeButtons.size();++i)
		{
			roomTypeButtons[i]->Draw(window);
		}
	}
	else if (editStatus == EditStatus::StartPosition)
	{
		for (int i = 0; i < dirButtons.size();++i)
		{
			dirButtons[i]->Draw(window);
		}
		for (int i = 0; i < positionTexts.size();++i)
		{
			positionTexts[i]->Draw(window);
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
		datum.position = hitBox.first->getPosition();
		datum.rotation = hitBox.first->getRotation();
		datum.size = hitBox.first->getSize();
		datum.type = hitBox.second;
		data.push_back(datum);
	}
	return data;
}

RoomData UiEditHitBox::GetRoomData() const
{
	RoomData roomData;

	roomData.type = selectedRoomType;

	for (const auto& hitBox : hitboxes)
	{
		if ((int)hitBox.second < 4)
		{
			roomData.connection[(int)hitBox.second] = true;
		}
	}

	return roomData;
}

void UiEditHitBox::SetHitBoxData(const std::vector<HitBoxData>& data)
{
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
	selectedHitBox = nullptr;
	for (auto& datum : data)
	{
		sf::RectangleShape* shape = new sf::RectangleShape();
		shape->setPosition(datum.position);
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
		case HitBoxData::Type::SpawnTrigger:
			shape->setOutlineColor(sf::Color::White);
			break;
		}
		hitboxes.insert({ shape ,datum.type });
	}
}

void UiEditHitBox::SetRoomData(const RoomData& data)
{
	selectedRoomType = data.type;
}

void UiEditHitBox::SetEditStatus(const EditStatus& status)
{
	editStatusButtons[(int)editStatus]->SetPressed(false);
	editStatusButtons[(int)status]->SetPressed(true);
	switch (status)
	{
	case EditStatus::Hitbox:
		dirButtons[(int)hitboxType]->SetPressed(false);
		editStatus = status;
		hitboxType = HitBoxData::Type::Immovable;
		hitBoxTypeButtons[(int)hitboxType]->SetPressed(true);
		break;
	case EditStatus::StartPosition:
		hitBoxTypeButtons[(int)hitboxType]->SetPressed(false);
		editStatus = status;
		hitboxType = HitBoxData::Type::PortalUp;
		dirButtons[(int)hitboxType]->SetPressed(true);
		for (int i = 0;i < startPositions.size();++i)
		{
			positionTexts[i]->SetString("Position"
				, std::to_wstring(startPositions[i].x).substr(0, 5)
				+ L", "
				+ std::to_wstring(startPositions[i].y).substr(0, 5));
		}
		break;
	default:
		break;
	}
}

void UiEditHitBox::ClearHitBoxData()
{
	selectedHitBox = nullptr;
	for (auto& hitbox : hitboxes)
	{
		delete hitbox.first;
	}
	hitboxes.clear();
}
