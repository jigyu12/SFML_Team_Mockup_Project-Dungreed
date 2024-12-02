#include "stdafx.h"
#include "UiEditMonster.h"

UiEditMonster::UiEditMonster(const std::string& name)
	: GameObject(name)
{
}

void UiEditMonster::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	boxWindow.setPosition(position);

	sf::Transform transform = boxWindow.getTransform();

	for (int i = 0;i < monsterList.size();++i)
	{
		monsterList[i].setPosition(transform.transformPoint(50.f + (i % 4) * 100.f, 100.f + (i / 4) * 100.f));
	}

	waveText.setPosition(transform.transformPoint(50.f, 40.f));
	waveUpButton.setPosition(transform.transformPoint(250.f, 40.f));
	waveDownButton.setPosition(transform.transformPoint(350.f, 40.f));
}

void UiEditMonster::SetRotation(float angle)
{
	rotation = angle;
	boxWindow.setRotation(rotation);

	boxWindow.setPosition(position);
}

void UiEditMonster::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
	boxWindow.setPosition(position);
}

void UiEditMonster::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(boxWindow, originPreset);
	}
	boxWindow.setPosition(position);
}

void UiEditMonster::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	boxWindow.setOrigin(origin);
	boxWindow.setPosition(position);
}

void UiEditMonster::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
}

void UiEditMonster::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiEditMonster::Release()
{
	ClearSpawnData();
}

void UiEditMonster::Reset()
{
	boxWindow.setFillColor({ 150,150,150,255 });

	monsterList.resize((int)Monster::MonsterType::Count);

	monsterList[(int)Monster::MonsterType::SkeletonDog].setTexture(&TEXTURE_MGR.Get("graphics/monster/SkelDogIdle0.png"));
	monsterList[(int)Monster::MonsterType::SkeletonDog].setSize({ 90.f,90.f });
	monsterList[(int)Monster::MonsterType::SkeletonDog].setOutlineColor(sf::Color::White);
	monsterList[(int)Monster::MonsterType::SkeletonDog].setOutlineThickness(1.f);

	monsterList[(int)Monster::MonsterType::Bat].setTexture(&TEXTURE_MGR.Get("graphics/monster/Bat0.png"));
	monsterList[(int)Monster::MonsterType::Bat].setSize({ 90.f,90.f });
	monsterList[(int)Monster::MonsterType::Bat].setOutlineColor(sf::Color::White);
	monsterList[(int)Monster::MonsterType::Bat].setOutlineThickness(1.f);


	waveUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	waveDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	waveText.setFont(FONT_MGR.Get("fonts/french.ttf"));
}

void UiEditMonster::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());

			for (int i = 0;i < monsterList.size();++i)
			{
				if (monsterList[i].getGlobalBounds().contains(mousepos))
				{
					monsterList[i].setOutlineColor(sf::Color::Red);
					selectedType = (Monster::MonsterType)i;
				}
				else
				{
					monsterList[i].setOutlineColor(sf::Color::White);
				}
			}

			if (waveUpButton.getGlobalBounds().contains(mousepos)
				&& selectedMonster != nullptr)
			{
				spawnData[selectedMonster].wave = Utils::Clamp(++spawnData[selectedMonster].wave, 0, 100);
			}
			if (waveDownButton.getGlobalBounds().contains(mousepos)
				&& selectedMonster != nullptr)
			{
				spawnData[selectedMonster].wave = Utils::Clamp(--spawnData[selectedMonster].wave, 0, 100);
			}
		}

	}
	else
	{
		sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			if (selectedMonster != nullptr)
			{
				selectedMonster->setOutlineColor(sf::Color::Green);
			}
			selectedMonster = nullptr;
			for (auto& spawnDatum : spawnData)
			{
				if (Utils::PointInTransformBounds(*spawnDatum.first, spawnDatum.first->getLocalBounds(), worldMousePos))
				{
					status = EditStatus::MoveOld;
					selectedMonster = spawnDatum.first;
					selectedMonster->setOutlineColor(sf::Color::Red);
					startPos = worldMousePos - selectedMonster->getPosition();
				}
			}
			if (selectedMonster == nullptr)
			{
				startPos = worldMousePos;

				SpawnData spawnDatum;
				spawnDatum.position = startPos;
				spawnDatum.type = selectedType;
				spawnDatum.wave = 0;

				status = EditStatus::Create;
				sf::RectangleShape* shape = new sf::RectangleShape();
				switch (selectedType)
				{
				case Monster::MonsterType::SkeletonDog:
					shape->setTexture(&TEXTURE_MGR.Get("graphics/monster/SkelDogIdle0.png"));
					shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
					Utils::SetOrigin(*shape, Origins::BC);
					break;
				case Monster::MonsterType::Bat:
					shape->setTexture(&TEXTURE_MGR.Get("graphics/monster/Bat0.png"));
					shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
					Utils::SetOrigin(*shape, Origins::MC);
					break;
				}

				shape->setOutlineColor(sf::Color::Red);
				shape->setOutlineThickness(1.f);
				shape->setPosition(startPos);
				selectedMonster = shape;
				spawnData.insert({ shape,spawnDatum });
			}
		}
		else if (InputMgr::GetMouseButton(sf::Mouse::Left)
			&& selectedMonster != nullptr)
		{
			sf::Vector2f worldMousePos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());

			switch (status)
			{
			case EditStatus::MoveOld:
				selectedMonster->setPosition(worldMousePos - startPos);
				break;
			}
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Right)
			&& selectedMonster != nullptr)
		{
			auto found = spawnData.find(selectedMonster);
			if (found != spawnData.end())
			{
				spawnData.erase(found->first);
			}
		}
	}

	waveText.setString("SPAWN WAVE : " + (selectedMonster != nullptr ? std::to_string(spawnData[selectedMonster].wave) : ""));
}

void UiEditMonster::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);

	for (int i = 0; i < monsterList.size();++i)
	{
		window.draw(monsterList[i]);
	}

	window.draw(waveText);
	window.draw(waveUpButton);
	window.draw(waveDownButton);

	sf::View prev = window.getView();
	sf::View world = SCENE_MGR.GetCurrentScene()->GetWorldView();
	sf::Vector2f size = world.getSize();
	world.move(size.x * 0.125f, 0.f);
	world.setSize(size.x * 0.75f, size.y);
	world.setViewport({ 0.25f,0.f,0.75f,1.f });
	window.setView(world);
	for (auto& datum : spawnData)
	{
		window.draw(*datum.first);
	}
	window.setView(prev);
}

std::vector<SpawnData> UiEditMonster::GetSpawnData() const
{
	std::vector<SpawnData> data;
	for (auto& spawndatum : spawnData)
	{
		data.push_back(spawndatum.second);
	}
	return data;
}

void UiEditMonster::SetSpawnData(const std::vector<SpawnData>& data)
{
	ClearSpawnData();

	for (const SpawnData& datum : data)
	{
		sf::RectangleShape* shape = new sf::RectangleShape();
		switch (datum.type)
		{
		case Monster::MonsterType::SkeletonDog:
			shape->setTexture(&TEXTURE_MGR.Get("graphics/monster/SkelDogIdle0.png"));
			shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
			Utils::SetOrigin(*shape, Origins::BC);
			break;
		case Monster::MonsterType::Bat:
			shape->setTexture(&TEXTURE_MGR.Get("graphics/monster/Bat0.png"));
			shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
			Utils::SetOrigin(*shape, Origins::MC);
			break;
		}

		shape->setOutlineColor(sf::Color::Red);
		shape->setOutlineThickness(1.f);
		shape->setPosition(datum.position);
		selectedMonster = shape;
		spawnData.insert({ shape,datum });
	}
}

void UiEditMonster::ClearSpawnData()
{
	for (auto& spawndatum : spawnData)
	{
		delete spawndatum.first;
	}
	spawnData.clear();
}

void UiEditMonster::SetMonsterType(const Monster::MonsterType& type)
{
	selectedType = type;
}
