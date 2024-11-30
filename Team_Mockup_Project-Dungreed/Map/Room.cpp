#include "stdafx.h"
#include "Room.h"
#include "TileMap.h"
#include "Player.h"
#include "MapObject.h"

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
		hitBox.first->rect.setPosition(position);
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
		for (auto& hitBox : hitBoxes)
		{
			hitBox.first->rect.setOrigin(origin - hitBox.second.origin);
		}
		for (auto& obj : objects)
		{
			obj.first->SetOrigin(origin - obj.second.origin);
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

	for (int i = 0;i < MapData::TileMapCount;++i)
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
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));

	subBackground.setTexture(TEXTURE_MGR.Get("graphics/map/SubBG.png"));
	SetOrigin(Origins::MC);
}

void Room::Update(float dt)
{
	if (player != nullptr)
	{
		for (auto& hitbox : hitBoxes)
		{
			if (hitbox.second.type > HitBoxData::Type::PortalRight)
			{
				continue;
			}
			if (Utils::PointInTransformBounds(hitbox.first->rect, hitbox.first->rect.getLocalBounds(), player->GetHitBox().GetCenter()))
			{
				ROOM_MGR.RoomChange(hitbox.second.type);
				break;
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
	for (auto& hitbox : hitBoxes)
	{
		delete hitbox.first;
	}
	hitBoxes.clear();

	mapData = MapDataLoader::Load(path);

	for (int i = 0; i < tileMaps.size();++i)
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
		hitbox->rect.setOrigin(hitBoxDatum.origin);
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
	SetOrigin(Origins::MC);
}

void Room::SaveMapData(const std::string& path)
{
	for (int i = 0; i < tileMaps.size();++i)
	{
		mapData.tileMapData[i].texId = "graphics/map/Map.png";
		if (path == "maps/1fenter.json")
		{
			mapData.playerStartPoint[(int)MapData::Direction::Down] = { 5 * 16.f,7.f * 16.f };

			HitBoxData hitBoxData;
			hitBoxData.origin = { 0.f,0.f };
			hitBoxData.size = { 18 * 16.f,1 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 0.f,0.f };
			hitBoxData.size = { 1 * 16.f,9 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 17 * 16.f,0.f };
			hitBoxData.size = { 2 * 16.f,4 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 0 * 16.f,8 * 16.f };
			hitBoxData.size = { 19 * 16.f,1 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;


			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 18 * 16.f ,4 * 16.f };
			hitBoxData.size = { 1 * 16.f,4 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::PortalRight;

			mapData.hitBoxData.push_back(hitBoxData);

			mapData.tileMapData[i].name = "1fEnter";
			mapData.tileMapData[i].cellcount = { 19,9 };
			mapData.tileMapData[i].cellsize = { 16.f,16.f };
			if (i == 1)
			{
				mapData.tileMapData[i].tileIndexes = { {35, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 36, 8},
		{9, 148, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 151, 7, 8},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 145, 7, 8},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 145, 19, 20},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 158, 149, 151},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 145},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 145},
		{9, 146, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 145},
		{37, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, };
			}
		}
		else if (path == "maps/1froom1.json")
		{


			mapData.playerStartPoint[(int)MapData::Direction::Down] = { 5 * 16.f,7.f * 16.f };

			ObjectData objData;

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 64.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 80.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 96.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 128.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 144.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 160.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 176.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 208.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 224.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 240.f,80.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 144.f,128.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 160.f,128.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 128.f,160.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 144.f,160.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 160.f,160.f };
			mapData.objectData.push_back(objData);

			objData.type = ObjectData::Type::Platform;
			objData.origin = { 176.f,160.f };
			mapData.objectData.push_back(objData);

			HitBoxData hitBoxData;
			hitBoxData.origin = { 0.f,0.f };
			hitBoxData.size = { 8 * 16.f,2 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 0.f,0.f };
			hitBoxData.size = { 2 * 16.f,8 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 12 * 16.f,0.f };
			hitBoxData.size = { 7 * 16.f,2 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 0 * 16.f,12 * 16.f };
			hitBoxData.size = { 19 * 16.f,1 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 18 * 16.f,0 * 16.f };
			hitBoxData.size = { 1 * 16.f,13 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Immovable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 4 * 16.f,5 * 16.f };
			hitBoxData.size = { 3 * 16.f,0.25f * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Downable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 8 * 16.f,5 * 16.f };
			hitBoxData.size = { 4 * 16.f,0.25f * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Downable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 13 * 16.f,5 * 16.f };
			hitBoxData.size = { 3 * 16.f,0.25f * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Downable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 9 * 16.f,8 * 16.f };
			hitBoxData.size = { 2 * 16.f,0.25f * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Downable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 8 * 16.f,10 * 16.f };
			hitBoxData.size = { 4 * 16.f,0.25f * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::Downable;
			mapData.hitBoxData.push_back(hitBoxData);

			hitBoxData.origin = { 0 * 16.f,8 * 16.f };
			hitBoxData.size = { 1 * 16.f,4 * 16.f };
			hitBoxData.rotation = 0.f;
			hitBoxData.type = HitBoxData::Type::PortalLeft;
			mapData.hitBoxData.push_back(hitBoxData);

			mapData.tileMapData[i].name = "1froom1";
			mapData.tileMapData[i].cellcount = { 19,13 };
			mapData.tileMapData[i].cellsize = { 16.f,16.f };
			if (i == 1)
			{
				mapData.tileMapData[i].tileIndexes = { {8, 8, 8, 8, 8, 8, 8, 9, 146, 160, 160, 145, 7, 8, 8, 8, 8, 8, 8},
		{8, 35, 20, 20, 20, 20, 20, 21, 146, 160, 160, 145, 19, 20, 20, 20, 20, 20, 36},
		{8, 9, 148, 149, 149, 149, 149, 149, 156, 160, 160, 158, 149, 149, 149, 149, 149, 151, 7},
		{8, 9, 146, 160, 161, 160, 160, 160, 160, 160, 160, 163, 160, 161, 160, 160, 160, 145, 7},
		{8, 9, 146, 160, 160, 160, 163, 160, 161, 160, 160, 160, 160, 160, 160, 160, 161, 145, 7},
		{8, 9, 146, 145, 0, 0, 0, 157, 0, 0, 0, 0, 157, 0, 0, 0, 146, 145, 7},
		{8, 9, 146, 145, 0, 0, 0, 157, 0, 0, 0, 0, 157, 0, 0, 0, 146, 145, 7},
		{20, 21, 146, 145, 0, 0, 0, 157, 0, 0, 0, 0, 157, 0, 0, 0, 146, 145, 7},
		{149, 149, 156, 145, 0, 0, 0, 157, 0, 0, 0, 0, 157, 0, 0, 0, 146, 145, 7},
		{160, 160, 160, 145, 0, 0, 0, 157, 0, 0, 0, 0, 157, 0, 0, 0, 146, 145, 7},
		{160, 160, 160, 158, 178, 184, 180, 159, 178, 179, 179, 180, 159, 178, 179, 180, 156, 145, 7 },
		{160, 160, 160, 163, 160, 161, 160, 160, 160, 174, 175, 160, 161, 160, 160, 160, 160, 145, 7 },
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 38},
				};
			}
		}
	}

	MapDataLoader::Save(mapData, path);
}


const std::vector<std::pair<HitBox*, HitBoxData>>& Room::GetHitBoxes() const
{
	return hitBoxes;
}

void Room::EnterRoom(HitBoxData::Type connection)
{
	player->SetPosition(mapData.playerStartPoint[(int)connection]);
}
