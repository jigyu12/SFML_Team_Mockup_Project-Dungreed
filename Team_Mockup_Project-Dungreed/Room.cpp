#include "stdafx.h"
#include "Room.h"
#include "TileMap.h"

Room::Room(const std::string& name)
	: GameObject(name)
{
}

void Room::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	tileMap->SetPosition(position);
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first.rect.setPosition(position);
	}
}

void Room::SetRotation(float angle)
{
	rotation = angle;
	tileMap->SetRotation(rotation);
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first.rect.setRotation(rotation);
	}
}

void Room::SetScale(const sf::Vector2f& s)
{
	scale = s;
	tileMap->SetScale(scale);
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first.rect.setScale(scale);
	}
}

void Room::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		tileMap->SetOrigin(originPreset);
		origin = tileMap->GetOrigin();
		for (auto& hitBox : hitBoxes)
		{
			hitBox.first.rect.setOrigin(-hitBox.second.origin + origin);
		}
	}
}

void Room::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	tileMap->SetOrigin(origin);
	for (auto& hitBox : hitBoxes)
	{
		hitBox.first.rect.setOrigin(origin);
	}
}

void Room::Init()
{
	tileMap = new TileMap("");
}

void Room::Release()
{
	delete tileMap;
}

void Room::Reset()
{
}

void Room::Update(float dt)
{
}

void Room::Draw(sf::RenderWindow& window)
{
	tileMap->Draw(window);

	if (Variables::isDrawHitBox)
	{
		for (auto& hitBox : hitBoxes)
		{
			window.draw(hitBox.first.rect);
		}
	}
}

void Room::LoadMapData(const std::string& path)
{
	MapDataLoader loader;
	loader.Load(path);
	mapData = loader.Get();

	const MapDataVC& mapData = loader.Get();
	tileMap->SetTexture(mapData.tileMapData.texId);
	tileMap->Set(mapData.tileMapData.cellcount, mapData.tileMapData.cellsize, mapData.tileMapData.tile);

	for (const HitBoxData& hitBoxDatum : mapData.hitBoxData)
	{
		HitBox hitbox;

		hitbox.rect.setSize(hitBoxDatum.size);
		hitbox.rect.setOrigin(hitBoxDatum.origin);
		hitbox.rect.setRotation(hitBoxDatum.rotation);
		hitBoxes.push_back({ hitbox,hitBoxDatum });
	}
	SetOrigin(Origins::MC);
}

void Room::SaveMapData(const std::string& path)
{
	MapDataLoader loader;

	MapDataVC& mapData = loader.Get();
	mapData = this->mapData;

	HitBoxData hitBoxData;
	hitBoxData.origin = { 0.f,0.f };
	hitBoxData.size = { 18 * 16.f,1 * 16.f };
	hitBoxData.rotation = 0.f;
	hitBoxData.type = (int)HitboxAttribute::Immovable;
	mapData.hitBoxData.push_back(hitBoxData);

	hitBoxData.origin = { 0.f,0.f };
	hitBoxData.size = { 1 * 16.f,9 * 16.f };
	hitBoxData.rotation = 0.f;
	hitBoxData.type = (int)HitboxAttribute::Immovable;
	mapData.hitBoxData.push_back(hitBoxData);

	hitBoxData.origin = { 17 * 16.f,0.f };
	hitBoxData.size = { 2 * 16.f,4 * 16.f };
	hitBoxData.rotation = 0.f;
	hitBoxData.type = (int)HitboxAttribute::Immovable;
	mapData.hitBoxData.push_back(hitBoxData);

	hitBoxData.origin = { 0 * 16.f,8 * 16.f };
	hitBoxData.size = { 19 * 16.f,1 * 16.f };
	hitBoxData.rotation = 0.f;
	hitBoxData.type = (int)HitboxAttribute::Immovable;
	mapData.hitBoxData.push_back(hitBoxData);

	mapData.tileMapData.texId = "graphics/map/Map.png";
	mapData.tileMapData.name = "1fEnter";
	mapData.tileMapData.cellcount = { 19,9 };
	mapData.tileMapData.cellsize = { 16.f,16.f };
	mapData.tileMapData.tile = { {48.f,160.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{128.f,32.f},
{64.f,160.f},
{0.f,64.f},

{32.f,176.f},
{128.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{144.f,336.f},
{176.f,336.f},
{0.f,176.f},
{0.f,64.f},

{32.f,176.f},
{96.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{80.f,336.f},
{0.f,176.f},
{0.f,64.f},

{32.f,176.f},
{96.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{80.f,336.f},
{112.f,32.f},
{128.f,32.f},

{32.f,176.f},
{96.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{176.f,352.f},
{144.f,336.f},
{176.f,336.f},

{32.f,176.f},
{96.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{0.f,336.f},
{80.f,336.f},

{ 32.f,176.f },
{ 96.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 80.f,336.f },

{ 32.f,176.f },
{ 96.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 0.f,336.f },
{ 80.f,336.f },

{ 48.f,176.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
{ 16.f,144.f },
	};

	loader.Save("map.json");
}
