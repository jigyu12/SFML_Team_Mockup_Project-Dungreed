#include "stdafx.h"
#include "Scene.h"
#include "Room.h"
#include "TileMap.h"
#include "Player.h"
#include "MapObject.h"
#include "Monster.h"
#include "Bat.h"
#include "SkeletonDog.h"

Room::Room(const std::string& name)
	: GameObject(name)
{
}

void Room::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	for (auto tileMap : tileMaps)
	{
		tileMap->SetPosition(position);
	}
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first->rect.setPosition(tileMaps[0]->GetTransform().transformPoint(hitBox.second.origin));
	}
	for (auto& obj : objects)
	{
		obj.first->SetPosition(position);
	}
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
		hitBox.first->rect.setRotation(rotation);
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
		origin = tileMaps[0]->GetOrigin();
		//for (auto& hitBox : hitBoxes)
		//{
		//	hitBox.first->rect.setOrigin(origin);
		//}
		for (auto& obj : objects)
		{
			obj.first->SetOrigin(origin);
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
		delete obj.first;
	}
	objects.clear();

	for (auto tileMap : tileMaps)
	{
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
}

void Room::Update(float dt)
{
	if (player != nullptr)
	{
		subBGCenter = player->GetPosition();
		subBGCenter.x = Utils::Clamp(subBGCenter.x, viewbounds.left, viewbounds.left + viewbounds.width);
		subBGCenter.y = Utils::Clamp(subBGCenter.y, viewbounds.top, viewbounds.top + viewbounds.height);

		subBackground.setPosition(subBGCenter);
		for (auto& hitbox : hitBoxes)
		{
			if (!cleared || hitbox.second.type > HitBoxData::Type::PortalRight)
			{
				continue;
			}
			if (Utils::PointInTransformBounds(hitbox.first->rect, hitbox.first->rect.getLocalBounds(), player->GetHitBox().GetCenter()))
			{
				if (ROOM_MGR.RoomChange(hitbox.second.type))
				{
					for (std::pair<Monster*, SpawnData> monster : monsters)
					{
						if (monster.second.wave == this->wave)
						{
							monster.first->SetActive(false);
						}
					}
				}
				break;
			}
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
	if (monsters.size() == count)
	{
		cleared = true;
	}
	else if (wavecount == 0)
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
		obj.first->Draw(window);
	}
	if (Variables::isDrawHitBox)
	{
		for (auto& hitBox : hitBoxes)
		{
			window.draw(hitBox.first->rect);
		}
	}
}

void Room::LoadMapData(const std::string& path)
{
	sf::Vector2f worldViewSize = SCENE_MGR.GetCurrentScene()->GetWorldView().getSize();

	monsters.clear();

	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	mapData = MapDataLoader::Load(path);

	for (int i = 0; i < tileMaps.size(); ++i)
	{
		tileMaps[i]->Set(mapData.tileMapData[i]);
	}

	for (const ObjectData& objData : mapData.objectData)
	{
		MapObject* obj = new MapObject();

		obj->Init();
		obj->Reset();
		obj->Set(objData.type);

		objects.push_back({ obj,objData });
	}

	for (const HitBoxData& hitBoxDatum : mapData.hitBoxData)
	{
		HitBox* hitbox = new HitBox();

		hitbox->rect.setSize(hitBoxDatum.size);
		hitbox->rect.setPosition(hitBoxDatum.origin);
		//hitbox->rect.setOrigin(-hitBoxDatum.origin);
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
		default:
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
		}
	}
	if (mapData.monsterSpawnData.size() == 0)
	{
		cleared = true;
	}

	viewbounds = tileMaps[0]->GetGlobalBounds();

	viewbounds.left += worldViewSize.x * 0.5f;
	viewbounds.top += worldViewSize.y * 0.5f;
	viewbounds.width -= worldViewSize.x;
	viewbounds.height -= worldViewSize.y;
}

const std::vector<std::pair<HitBox*, HitBoxData>>& Room::GetHitBoxes() const
{
	return hitBoxes;
}

void Room::EnterRoom(HitBoxData::Type connection)
{
	if (player != nullptr)
	{
		player->SetPosition(mapData.playerStartPoint[(int)connection]);
	}

	if (cleared)
	{
		return;
	}
	for (std::pair<Monster*, SpawnData>& monster : monsters)
	{
		if (monster.second.wave == 0)
		{
			monster.first->SetActive(true);
		}
	}
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
