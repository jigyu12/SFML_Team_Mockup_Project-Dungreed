#include "stdafx.h"
#include "Scene.h"
#include "Room.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Bat.h"
#include "SkeletonDog.h"
#include "ParticleGo.h"
#include "TorchMo.h"
#include "DoorMo.h"
#include "SealStoneMo.h"
#include "BreakableMo.h"
#include "BackgroundMo.h"
#include "SkellBoss.h"
#include "SkellBossLeftHand.h"
#include "Banshee.h"
#include "Ghost.h"

Room::Room(const std::string& name)
	: GameObject(name)
{
}

void Room::SetActive(bool active)
{
	GameObject::SetActive(active);
	for (auto tileMap : tileMaps)
	{
		tileMap->SetActive(active);
	}
	for (auto& obj : objects)
	{
		obj.first->SetActive(active);
	}
}

void Room::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	for (auto tileMap : tileMaps)
	{
		tileMap->SetPosition(position);
	}
	sf::Transform transform = tileMaps[0]->GetTransform();
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setPosition(transform.transformPoint(hitBox.second.position));
	}
	for (auto& obj : objects)
	{
		obj.first->SetPosition(transform.transformPoint(obj.second.position));
	}

	SetViewBounds();
}

void Room::SetRotation(float angle)
{
	rotation = angle;

	for (auto tileMap : tileMaps)
	{
		tileMap->SetRotation(rotation);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setRotation(rotation + hitBox.second.rotation);
	}
	for (auto& obj : objects)
	{
		obj.first->SetRotation(rotation);
	}
}

void Room::SetScale(const sf::Vector2f& s)
{
	scale = s;

	for (auto tileMap : tileMaps)
	{
		tileMap->SetScale(scale);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setScale(scale);
	}
	for (auto& obj : objects)
	{
		obj.first->SetScale(scale);
	}
}

void Room::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(subBackground, originPreset);
		for (auto tileMap : tileMaps)
		{
			tileMap->SetOrigin(originPreset);
		}
	}
}

void Room::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	for (auto tileMap : tileMaps)
	{
		tileMap->SetOrigin(origin);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setOrigin(origin);
	}
}

sf::FloatRect Room::GetLocalBounds() const
{
	return tileMaps[0]->GetLocalBounds();
}

sf::FloatRect Room::GetGlobalBounds() const
{
	return tileMaps[0]->GetGlobalBounds();
}

void Room::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 0;

	for (int i = 0; i < MapData::TileMapCount; ++i)
	{
		TileMap* tileMap = new TileMap("tileMap" + std::to_string(i));
		tileMaps.push_back(tileMap);
	}
}

void Room::Release()
{
	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	for (auto& obj : objects)
	{
		obj.first->Release();
		delete obj.first;
	}
	objects.clear();

	for (auto tileMap : tileMaps)
	{
		tileMap->Release();
		delete tileMap;
	}
	tileMaps.clear();
}

void Room::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	scene = SCENE_MGR.GetCurrentScene();
	subBackground.setTexture(TEXTURE_MGR.Get("graphics/map/SubBG.png"));
	SetOrigin(Origins::MC);
	Scene* scene = SCENE_MGR.GetCurrentScene();
}

void Room::Update(float dt)
{
	for (auto& obj : objects)
	{
		obj.first->Update(dt);
	}
}

void Room::LateUpdate(float dt)
{
	if (player != nullptr)
	{
		subBGCenter = player->GetPosition();
		subBGCenter.x = Utils::Clamp(subBGCenter.x, viewbounds.left, viewbounds.left + viewbounds.width);
		subBGCenter.y = Utils::Clamp(subBGCenter.y, viewbounds.top, viewbounds.top + viewbounds.height);

		subBackground.setPosition(subBGCenter);
		for (auto& hitbox : hitBoxes)
		{
			switch (hitbox.second.type)
			{
			case HitBoxData::Type::PortalRight:
			case HitBoxData::Type::PortalLeft:
			case HitBoxData::Type::PortalUp:
			case HitBoxData::Type::PortalDown:
				if ((cleared || wave == -2)
					&& Utils::PointInTransformBounds(hitbox.first->rect, hitbox.first->rect.getLocalBounds(), player->GetHitBox().GetCenter()))
				{
					if (ROOM_MGR.RoomChange(hitbox.second.type))
					{
						for (std::pair<Monster*, SpawnData> monster : monsters)
						{
							monster.first->SetActive(false);
						}
					}
				}
				break;
			case HitBoxData::Type::SpawnTrigger:
				if (Utils::PointInTransformBounds(hitbox.first->rect, hitbox.first->rect.getLocalBounds(), player->GetHitBox().GetCenter())
					&& wave == -2
					&& !cleared)
				{
					++wave;
					for (const auto& object : objects)
					{
						if (object.second.type == ObjectData::Type::SealStone)
						{
							object.first->SetStatus(MapObject::Status::Close);
						}
					}
				}
				break;
			case HitBoxData::Type::Spike:
				if (Utils::CheckCollision(player->GetHitBox(), *hitbox.first))
				{
					player->OnDamage(5);
				}
				break;
			}
		}

		if (ROOM_MGR.GetCurrentRoom() == this
			&& !tileMaps[0]->GetGlobalBounds().contains(player->GetHitBox().GetCenter()))
		{
			player->SetPosition(tileMaps[0]->GetTransform().transformPoint(mapData.playerStartPoint[(int)enteredPortal]));
			player->SetStatus(Player::Status::Ground);
		}
	}
	int count = 0;
	int wavecount = 0;
	for (const std::pair<Monster*, SpawnData>& monster : monsters)
	{
		if (monster.first->IsDead())
		{
			++count;
		}
		if (wave == monster.second.wave && !monster.first->IsDead())
		{
			++wavecount;
		}
	}
	if (monsters.size() == count
		&& !cleared)
	{
		cleared = true;
		wave = 100;
		for (const auto& object : objects)
		{
			if (object.second.type == ObjectData::Type::SealStone)
			{
				object.first->SetStatus(MapObject::Status::Open);
			}
		}
	}
	else if (wavecount == 0 && wave != -2)
	{
		++wave;
		for (const std::pair<Monster*, SpawnData>& monster : monsters)
		{
			if (wave == monster.second.wave && !monster.first->IsDead())
			{
				monster.first->SetActive(true);
			}
		}
	}

	for (auto& monster : monsters)
	{
		if (ROOM_MGR.GetCurrentRoom() == this
			&& !tileMaps[0]->GetGlobalBounds().contains(monster.first->GetPosition()))
		{
			monster.first->SetPosition(tileMaps[0]->GetTransform().transformPoint(monster.second.position));
		}
	}
}

void Room::Draw(sf::RenderWindow& window)
{
	window.draw(subBackground);
	for (auto tileMap : tileMaps)
	{
		tileMap->Draw(window);
	}

	for (auto& obj : objects)
	{
		if (obj.first->IsActive())
		{
			obj.first->Draw(window);
		}
	}

	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->Draw(window);
	}
}

void Room::LoadMapData(const std::string& path)
{
	SetMapData(MapDataLoader::Load(path));
}

void Room::SetMapData(const MapDataVC& mapData)
{
	this->mapData = mapData;

	monsters.clear();

	for (auto& object : objects)
	{
		delete object.first;
	}
	objects.clear();

	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	// 황규영 빈 맵데이터 입력 확인용
	assert(mapData.tileMapData[0].cellcount.x != 0);

	for (int i = 0; i < tileMaps.size(); ++i)
	{
		tileMaps[i]->Set(mapData.tileMapData[i]);
	}

	for (const ObjectData& objData : mapData.objectData)
	{
		MapObject* obj = nullptr;
		switch (objData.type)
		{
		case ObjectData::Type::Torch:
			obj = new TorchMo();
			break;
		case ObjectData::Type::SealStone:
			obj = new SealStoneMo();
			break;
		case ObjectData::Type::BigBox:
		case ObjectData::Type::Box:
		case ObjectData::Type::OakDrum:
		case ObjectData::Type::Table:
		case ObjectData::Type::SkullTable:
			obj = new BreakableMo();
			break;
		case ObjectData::Type::Door:
			obj = new DoorMo();
			break;
		case ObjectData::Type::Cell:
		case ObjectData::Type::BrokenCell:
		case ObjectData::Type::UpperCell0:
		case ObjectData::Type::UpperCell1:
		case ObjectData::Type::Skull0:
		case ObjectData::Type::Skull1:
		case ObjectData::Type::Bone0:
		case ObjectData::Type::Bone1:
			obj = new BackgroundMo();
			break;
		}
		if (obj != nullptr)
		{
			obj->Init();
			obj->Reset();
			obj->Set(objData.type);
			obj->SetRotation(objData.rotation);
			objects.push_back({ obj,objData });
		}
	}

	for (const HitBoxData& hitBoxDatum : mapData.hitBoxData)
	{
		HitBox* hitbox = new HitBox();

		hitbox->rect.setSize(hitBoxDatum.size);
		hitbox->rect.setPosition(hitBoxDatum.position);
		hitbox->rect.setRotation(hitBoxDatum.rotation);
		switch (hitBoxDatum.type)
		{
		case HitBoxData::Type::PortalUp:
		case HitBoxData::Type::PortalDown:
		case HitBoxData::Type::PortalLeft:
		case HitBoxData::Type::PortalRight:
			hitbox->rect.setOutlineColor(sf::Color::Cyan);
			break;
		case HitBoxData::Type::Immovable:
			hitbox->rect.setOutlineColor(sf::Color::Red);
			break;
		case HitBoxData::Type::Downable:
			hitbox->rect.setOutlineColor(sf::Color::Yellow);
			break;
		case HitBoxData::Type::SpawnTrigger:
			wave = -2;
			hitbox->rect.setOutlineColor(sf::Color::White);
			break;
		case HitBoxData::Type::Spike:
			hitbox->rect.setOutlineColor(sf::Color::Black);
			break;
		}
		hitBoxes.push_back({ hitbox,hitBoxDatum });
	}

	SetOrigin(originPreset);
	SetPosition(position);

	Scene* scene = SCENE_MGR.GetCurrentScene();

	for (const SpawnData& spawndatum : mapData.monsterSpawnData)
	{
		switch (spawndatum.type)
		{
		case Monster::MonsterType::Bat:
		{
			Bat* bat = scene->AddGo(new Bat());
			bat->Init();
			bat->Reset();
			bat->SetActive(false);
			bat->SetPosition(tileMaps[0]->GetTransform().transformPoint(spawndatum.position));
			monsters.push_back({ bat,spawndatum });
		}
		break;
		case Monster::MonsterType::SkeletonDog:
		{
			SkeletonDog* skeletonDog = scene->AddGo(new SkeletonDog());
			skeletonDog->Init();
			skeletonDog->Reset();
			skeletonDog->SetActive(false);
			skeletonDog->SetPosition(tileMaps[0]->GetTransform().transformPoint(spawndatum.position));
			monsters.push_back({ skeletonDog ,spawndatum });
		}
		break;
		case Monster::MonsterType::Banshee:
		{
			Banshee* banshee = scene->AddGo(new Banshee());
			banshee->Init();
			banshee->Reset();
			banshee->SetActive(false);
			banshee->SetPosition(tileMaps[0]->GetTransform().transformPoint(spawndatum.position));
			monsters.push_back({ banshee ,spawndatum });
		}
		break;
		case Monster::MonsterType::Ghost:
		{
			Ghost* ghost = scene->AddGo(new Ghost());
			ghost->Init();
			ghost->Reset();
			ghost->SetActive(false);
			ghost->SetPosition(tileMaps[0]->GetTransform().transformPoint(spawndatum.position));
			monsters.push_back({ ghost ,spawndatum });
		}
		break;
		case Monster::MonsterType::SkellBoss:
		{
			SkellBoss* skellBoss = scene->AddGo(new SkellBoss());
			skellBoss->Init();
			skellBoss->Reset();
			skellBoss->SetActive(false);
			skellBoss->SetPosition(tileMaps[0]->GetTransform().transformPoint(spawndatum.position));
			monsters.push_back({ skellBoss ,spawndatum });
		}
		break;
		}
	}
	if (mapData.monsterSpawnData.size() == 0)
	{
		cleared = true;
		wave = 100;
	}
	else
	{
		cleared = false;
	}

	SetViewBounds();
}

std::vector<std::pair<HitBox*, HitBoxData>> Room::GetHitBoxes() const
{
	std::vector<std::pair<HitBox*, HitBoxData>> data;

	data = hitBoxes;

	for (const auto& object : objects)
	{
		if (object.second.type == ObjectData::Type::SealStone
			&& object.first->GetStatus() == MapObject::Status::Close)
		{
			HitBoxData hitBoxData;
			hitBoxData.type = HitBoxData::Type::Immovable;

			data.push_back({ &object.first->GetHitBox(),hitBoxData });
		}
	}

	return data;
}

void Room::SetViewBounds()
{
	viewbounds = tileMaps[0]->GetGlobalBounds();
	sf::Vector2f worldViewSize = SCENE_MGR.GetCurrentScene()->GetWorldView().getSize();

	viewbounds.left += worldViewSize.x * 0.5f;
	viewbounds.top += worldViewSize.y * 0.5f;
	viewbounds.width -= worldViewSize.x;
	viewbounds.height -= worldViewSize.y;
}

void Room::EnterRoom(HitBoxData::Type connection)
{
	enteredPortal = connection;
	if (player != nullptr)
	{
		if (mapData.playerStartPoint[(int)connection] != sf::Vector2f(0.f, 0.f))
		{
			player->SetPosition(tileMaps[0]->GetTransform().transformPoint(mapData.playerStartPoint[(int)connection]));
		}
		else
		{
			player->SetPosition(tileMaps[0]->GetTransform().transformPoint(origin));
		}
	}

	for (const auto& object : objects)
	{
		if (object.second.type == ObjectData::Type::Door
			&& object.first->GetStatus() == MapObject::Status::Idle)
		{
			if (mapData.roomData.type == RoomData::Type::Enter)
			{
				player->SetPosition(object.first->GetPosition() + sf::Vector2f(0.f, -5.f));
				object.first->SetStatus(MapObject::Status::Close);
			}
			if (mapData.roomData.type == RoomData::Type::Exit)
			{
				object.first->SetStatus(MapObject::Status::Open);
			}
		}
		if ((object.second.type == ObjectData::Type::Torch)
			&&(object.first->GetStatus() == MapObject::Status::Idle)
			&& (ROOM_MGR.GetCurrentFloor() % 2 == 0))
		{
			object.first->SetStatus(MapObject::Status::Broken);
		}
	}

	if (wave == 0 && !cleared)
	{
		for (const auto& object : objects)
		{
			if (object.second.type == ObjectData::Type::SealStone
				&& object.first->GetStatus() != MapObject::Status::Close)
			{
				object.first->SetStatus(MapObject::Status::Close);
			}
		}

		for (const std::pair<Monster*, SpawnData>& monster : monsters)
		{
			if (wave == monster.second.wave && !monster.first->IsDead())
			{
				monster.first->SetActive(true);
			}
		}
	}
}

void Room::ClearMonsters()
{
	for (auto& monster : monsters)
	{
		monster.first->SetActive(false);
		scene->RemoveGo(monster.first);
		delete monster.first;
	}
	monsters.clear();
}


std::vector<Monster*> Room::GetMonsters() const
{
	std::vector<Monster*> data;

	for (const auto& monster : monsters)
	{
		if (monster.second.wave == this->wave)
		{
			data.push_back(monster.first);
		}
	}
	return data;
}

std::vector<MapObject*> Room::GetBreakableObjects() const
{
	std::vector<MapObject*> vect;

	for (auto& obj : objects)
	{
		switch (obj.second.type)
		{
		case ObjectData::Type::BigBox:
		case ObjectData::Type::Box:
		case ObjectData::Type::OakDrum:
		case ObjectData::Type::Table:
		case ObjectData::Type::SkullTable:
			vect.push_back(obj.first);
			break;
		default:
			break;
		}
	}

	return vect;
}
