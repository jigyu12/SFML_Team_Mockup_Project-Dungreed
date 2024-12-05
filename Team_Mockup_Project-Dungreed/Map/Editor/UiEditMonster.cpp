#include "stdafx.h"
#include "UiEditMonster.h"
#include "Button.h"
#include "TextGo.h"

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
		monsterList[i]->SetPosition(transform.transformPoint(50.f + (i % 4) * 100.f, 100.f + (i / 4) * 100.f));
	}

	waveText->SetPosition(transform.transformPoint(50.f, 40.f));
	waveUpButton->SetPosition(transform.transformPoint(250.f, 40.f));
	waveDownButton->SetPosition(transform.transformPoint(350.f, 40.f));
}

void UiEditMonster::SetRotation(float angle)
{
	rotation = angle;
	boxWindow.setRotation(rotation);

	SetPosition(position);
}

void UiEditMonster::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
	SetPosition(position);
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

	waveUpButton = new Button();
	waveUpButton->Init();
	waveDownButton = new Button();
	waveDownButton->Init();
	waveText = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
	waveText->Init();

	monsterList.resize((int)Monster::MonsterType::Count);
	for (int i = 0;i < monsterList.size();++i)
	{
		monsterList[i] = new Button();
		monsterList[i]->Init();
	}
}

void UiEditMonster::Release()
{
	for (int i = 0;i < monsterList.size();++i)
	{
		delete monsterList[i];
	}
	monsterList.clear();

	waveUpButton->Release();
	delete waveUpButton;
	waveDownButton->Release();
	delete waveDownButton;
	waveText->Release();
	delete waveText;

	ClearSpawnData();
}

void UiEditMonster::Reset()
{
	boxWindow.setFillColor({ 120,120,120 });

	for (int i = 0;i < monsterList.size();++i)
	{
		monsterList[i]->Reset();
		switch ((Monster::MonsterType)i)
		{
		case Monster::MonsterType::SkeletonDog:
			monsterList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "SkelDogIcon"));
			break;
		case Monster::MonsterType::Bat:
			monsterList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "BatIcon"));
			break;
		case Monster::MonsterType::SkellBoss:
			monsterList[i]->Set({ 90.f,90.f }, RESOURCEID_TABLE->Get("Graphic", "SkellBossIcon"));
			break;
		}
		monsterList[i]->SetClickedEvent([this,i]() 
			{ 
				monsterList[(int)selectedType]->SetPressed(false);
				selectedType = (Monster::MonsterType)i;
				monsterList[(int)selectedType]->SetPressed(true);
			});
	}

	waveText->Reset();
	waveText->Set(30);
	waveText->SetString("Wave", true);

	waveUpButton->Reset();
	waveUpButton->Set({ 90.f,45.f }, 20);
	waveUpButton->SetString("Up", true);
	waveUpButton->SetClickedEvent([this]() { UpDownSpawnWave(true); });

	waveDownButton->Reset();
	waveDownButton->Set({ 90.f,45.f }, 20);
	waveDownButton->SetString("Down", true);
	waveDownButton->SetClickedEvent([this]() { UpDownSpawnWave(false); });

	selectedMonster = nullptr;
}

void UiEditMonster::Update(float dt)
{
	if (InputMgr::GetMousePosition().x < 480.f)
	{
		waveDownButton->Update(dt);
		waveUpButton->Update(dt);
		for (int i = 0;i < monsterList.size();++i)
		{
			monsterList[i]->Update(dt);
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
					status = EditStatus::Move;
					selectedMonster = spawnDatum.first;
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
				SetSpawnRect(shape, selectedType);
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
			case EditStatus::Move:
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
				delete found->first;
				spawnData.erase(found);
				selectedMonster = nullptr;
			}
		}
	}

	for (auto& datum:spawnData)
	{
		if (selectedMonster == datum.first)
		{
			datum.first->setOutlineColor(sf::Color::Red);
		}
		else
		{
			datum.first->setOutlineColor(sf::Color::Green);
		}
	}

	waveText->SetString("SpawnWave", selectedMonster != nullptr ? std::to_wstring(spawnData[selectedMonster].wave) : L"");
}

void UiEditMonster::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);

	for (int i = 0; i < monsterList.size();++i)
	{
		monsterList[i]->Draw(window);
	}

	waveText->Draw(window);
	waveUpButton->Draw(window);
	waveDownButton->Draw(window);

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
	for (const auto& spawndatum : spawnData)
	{
		SpawnData datum;

		datum.position = spawndatum.first->getPosition();
		datum.type = spawndatum.second.type;
		datum.wave = spawndatum.second.wave;

		data.push_back(datum);
	}
	return data;
}

void UiEditMonster::SetSpawnData(const std::vector<SpawnData>& data)
{
	ClearSpawnData();

	for (const SpawnData& datum : data)
	{
		sf::RectangleShape* shape = new sf::RectangleShape();
		SetSpawnRect(shape, datum.type);

		shape->setOutlineThickness(1.f);
		shape->setPosition(datum.position);
		selectedMonster = shape;
		spawnData.insert({ shape,datum });
	}
}

void UiEditMonster::SetSpawnRect(sf::RectangleShape* shape, const Monster::MonsterType& type)
{
	switch (type)
	{
	case Monster::MonsterType::SkeletonDog:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "SkelDogIcon")));
		shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
		Utils::SetOrigin(*shape, Origins::BC);
		break;
	case Monster::MonsterType::Bat:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BatIcon")));
		shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
		Utils::SetOrigin(*shape, Origins::MC);
		break;
	case Monster::MonsterType::SkellBoss:
		shape->setTexture(&TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "SkellBossIcon")));
		shape->setSize((sf::Vector2f)(shape->getTexture()->getSize()));
		Utils::SetOrigin(*shape, Origins::BC);
		break;
	}
}

void UiEditMonster::ClearSpawnData()
{
	selectedMonster = nullptr;
	for (auto& spawndatum : spawnData)
	{
		delete spawndatum.first;
	}
	spawnData.clear();
}

void UiEditMonster::UpDownSpawnWave(bool up)
{
	int maxWave = -1;
	bool exists = false;
	for (const auto& spawndatum : spawnData)
	{
		maxWave = std::max(maxWave, spawndatum.second.wave);
		if (spawndatum.first != selectedMonster)
		{
			if (spawndatum.second.wave == spawnData[selectedMonster].wave)
			{
				exists = true;
			}
		}
	}
	if (up && exists)
	{
		++spawnData[selectedMonster].wave;
	}
	else if (!up && (exists|| maxWave == spawnData[selectedMonster].wave))
	{
		spawnData[selectedMonster].wave = std::max(0, --spawnData[selectedMonster].wave);
	}
}
