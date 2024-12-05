#include "stdafx.h"
#include "UiEditObject.h"
#include "Button.h"

UiEditObject::UiEditObject(const std::string& name)
	: GameObject(name)
{
}

void UiEditObject::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	boxWindow.setPosition(position);

	sf::Transform transform = boxWindow.getTransform();

	for (int i = 0; i < objectList.size(); ++i)
	{
		objectList[i]->SetPosition(transform.transformPoint(50.f + (i % 4) * 100.f, 100.f + (i / 4) * 100.f));
	}
}

void UiEditObject::SetRotation(float angle)
{
	rotation = angle;
	SetPosition(position);
}

void UiEditObject::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
	SetPosition(position);
}

void UiEditObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(boxWindow, originPreset);
		SetPosition(position);
	}
}

void UiEditObject::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	boxWindow.setOrigin(origin);
	SetPosition(position);

}

void UiEditObject::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditObject::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;

	objectList.resize((int)ObjectData::Type::Count);

	for (int i = 0;i < objectList.size();++i)
	{
		objectList[i] = new Button();
		objectList[i]->Init();
	}
}

void UiEditObject::Release()
{
	for (int i = 0; i < objectList.size();++i)
	{
		delete objectList[i];
	}
	objectList.clear();

	ClearObjectData();
}

void UiEditObject::Reset()
{
	boxWindow.setFillColor({ 150,150,150,255 });

	for (int i = 0;i < objectList.size();++i)
	{
		objectList[i]->Reset();
		switch ((ObjectData::Type)i)
		{
		case ObjectData::Type::Torch:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "TorchIcon"));
			break;
		case ObjectData::Type::OakDrum:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "OakDrumIcon"));
			break;
		case ObjectData::Type::BigBox:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "BigBoxIcon"));
			break;
		case ObjectData::Type::Box:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "BoxIcon"));
			break;
		case ObjectData::Type::SealStone:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "SealStoneIcon"));
			break;
		case ObjectData::Type::Door:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "DoorIcon"));
			break;
		case ObjectData::Type::Table:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "TableIcon"));
			break;
		case ObjectData::Type::SkullTable:
			objectList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "SkullTableIcon"));
			break;
		}
		objectList[i]->SetClickedEvent([this, i]() 
			{
				objectList[(int)selectedType]->SetPressed(false);
				selectedType = (ObjectData::Type)i;
				objectList[(int)selectedType]->SetPressed(true);
			});
	}

	selectedObject = nullptr;
}

void UiEditObject::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
	{
		for (int i = 0; i < objectList.size();++i)
		{
			objectList[i]->Update(dt);
		}
	}
	else
	{
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			selectedObject = nullptr;
			for (auto& objectDatum : objectData)
			{
				if (Utils::PointInTransformBounds(*objectDatum.first, objectDatum.first->getLocalBounds(), worldMousePos))
				{
					status = EditStatus::Move;
					selectedObject = objectDatum.first;
					startPos = worldMousePos - selectedObject->getPosition();
				}
			}
			if (selectedObject == nullptr)
			{
				startPos = worldMousePos;

				status = EditStatus::Create;
				sf::RectangleShape* shape = new sf::RectangleShape();
				SetObjectRect(shape, selectedType);
				shape->setPosition(startPos);
				selectedObject = shape;
				objectData.insert({ shape,selectedType });
			}
		}
		else if (InputMgr::GetMouseButton(sf::Mouse::Left)
			&& selectedObject != nullptr)
		{
			sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

			switch (status)
			{
			case EditStatus::Move:
				selectedObject->setPosition(worldMousePos - startPos);
				break;
			}
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Right)
			&& selectedObject != nullptr)
		{
			auto found = objectData.find(selectedObject);
			if (found != objectData.end())
			{
				delete found->first;
				objectData.erase(found);
				selectedObject = nullptr;
			}
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Middle)
			&& selectedObject !=nullptr)
		{
			selectedObject->rotate(45.f);
		}
	}

	for (auto& datum : objectData)
	{
		if (selectedObject == datum.first)
		{
			datum.first->setOutlineColor(sf::Color::Red);
		}
		else
		{
			datum.first->setOutlineColor(sf::Color::Green);
		}
	}
}

void UiEditObject::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);
	for (int i = 0; i < objectList.size();++i)
	{
		objectList[i]->Draw(window);
	}

	sf::View prev = window.getView();
	sf::View world = SCENE_MGR.GetCurrentScene()->GetWorldView();
	sf::Vector2f size = world.getSize();
	world.move(size.x * 0.125f, 0.f);
	world.setSize(size.x * 0.75f, size.y);
	world.setViewport({ 0.25f,0.f,0.75f,1.f });
	window.setView(world);
	for (auto& datum : objectData)
	{
		window.draw(*datum.first);
	}
	window.setView(prev);
}

void UiEditObject::ClearObjectData()
{
	selectedObject = nullptr;
	for (auto& objectdatum : objectData)
	{
		delete objectdatum.first;
	}
	objectData.clear();
}

void UiEditObject::SetObjectRect(sf::RectangleShape* shape, const ObjectData::Type& type)
{
	switch (type)
	{
	case ObjectData::Type::Torch:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "TorchIcon")));
		break;
	case ObjectData::Type::SealStone:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "SealStoneIcon")));
		break;
	case ObjectData::Type::Door:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "DoorIcon")));
		break;
	case ObjectData::Type::BigBox:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BigBoxIcon")));
		break;
	case ObjectData::Type::Box:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BoxIcon")));
		break;
	case ObjectData::Type::OakDrum:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "OakDrumIcon")));
		break;
	case ObjectData::Type::Table:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "TableIcon")));
		break;
	case ObjectData::Type::SkullTable:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "SkullTableIcon")));
		break;
	}
	shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
	Utils::SetOrigin(*shape, Origins::BC);

	shape->setOutlineThickness(1.f);
}

std::vector<ObjectData> UiEditObject::GetObjectData() const
{
	std::vector<ObjectData> data;

	for (const auto& object : objectData)
	{
		ObjectData datum;

		datum.position = object.first->getPosition();
		datum.rotation = object.first->getRotation();
		datum.type = object.second;

		data.push_back(datum);
	}
	return data;
}

void UiEditObject::SetObjectData(const std::vector<ObjectData>& data)
{
	ClearObjectData();

	for (const ObjectData& datum : data)
	{
		sf::RectangleShape* shape = new sf::RectangleShape();
		shape->setPosition(datum.position);
		shape->setRotation(datum.rotation);
		SetObjectRect(shape, datum.type);
		shape->setOutlineThickness(1.f);
		selectedObject = shape;
		objectData.insert({ shape,datum.type });
	}
}
