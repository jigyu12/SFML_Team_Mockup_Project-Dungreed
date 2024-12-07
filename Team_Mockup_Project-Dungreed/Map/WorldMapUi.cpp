#include "stdafx.h"
#include "WorldMapUi.h"
#include "TextGo.h"

WorldMapUi::WorldMapUi(const std::string& name)
	: GameObject(name)
{
}

void WorldMapUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void WorldMapUi::SetRotation(float angle)
{
	rotation = angle;
}

void WorldMapUi::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void WorldMapUi::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void WorldMapUi::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void WorldMapUi::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
	timer = 0.f;

	floorText = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
	floorText->Init();
}

void WorldMapUi::Release()
{
	floorText->Release();
	delete floorText;
}

void WorldMapUi::Reset()
{
	floorText->Reset();
	floorText->SetOrigin(Origins::BR);

	playerRect.setSize({ 144.f, 144.f });
	playerRect.setFillColor(sf::Color::Cyan);
	Utils::SetOrigin(playerRect, Origins::MC);

	const sf::View& uiview = SCENE_MGR.GetCurrentScene()->GetUiView();

	renderTexture.create(uiview.getSize().x * 0.75f, uiview.getSize().y * 0.75f);

	renderSprite.setPosition(uiview.getSize() * 0.5f);
	renderSprite.setOrigin((sf::Vector2f)renderTexture.getSize() * 0.5f);
	floorText->SetPosition((sf::Vector2f)renderTexture.getSize() * 0.9f);
}

void WorldMapUi::Update(float dt)
{
	timer += dt;
	const sf::Vector2f& center = ((sf::Vector2f)renderTexture.getSize()) * 0.5f;
	const sf::Vector2i& currentRoomCoord = ROOM_MGR.GetCurrentRoomCoord();
	playerRect.setPosition(center);
	if (timer > 1.f)
	{
		timer = 0.f;
	}
	for (int i = 0;i < rooms.size();++i)
	{
		rooms[i].roomIcon.setPosition((sf::Vector2f)(rooms[i].coord - currentRoomCoord) * 156.f + center);
		for (int j = 0;j < rooms[i].connectors.size();++j)
		{
			sf::Vector2f pos = (sf::Vector2f)(rooms[i].coord - currentRoomCoord) * 156.f + center;
			switch (rooms[i].connectors[j].first)
			{
			case (int)HitBoxData::Type::PortalUp:
				pos.y -= 78.f;
				break;
			case (int)HitBoxData::Type::PortalDown:
				pos.y += 78.f;
				break;
			case (int)HitBoxData::Type::PortalLeft:
				pos.x -= 78.f;
				break;
			case (int)HitBoxData::Type::PortalRight:
				pos.x += 78.f;
				break;
			}
			rooms[i].connectors[j].second.setPosition(pos);
		}
		if (rooms[i].type != RoomData::Type::Normal)
			rooms[i].typeIcon.setPosition((sf::Vector2f)(rooms[i].coord - currentRoomCoord) * 156.f + center);
	}
	floorText->SetString("CurrentFloor", std::to_wstring(ROOM_MGR.GetCurrentFloor()));
}

void WorldMapUi::Draw(sf::RenderWindow& window)
{
	if (InputMgr::GetKey(sf::Keyboard::Tab))
	{
		renderTexture.clear({ 0, 0, 0, 100 });

		if (timer < 0.5f)
		{
			renderTexture.draw(playerRect);
		}
		for (int i = 0;i < rooms.size();++i)
		{
			renderTexture.draw(rooms[i].roomIcon);
			for (int j = 0;j < rooms[i].connectors.size();++j)
			{
				renderTexture.draw(rooms[i].connectors[j].second);
			}
			if (rooms[i].type != RoomData::Type::Normal)
			{
				renderTexture.draw(rooms[i].typeIcon);
			}
		}
		floorText->Draw(renderTexture);
		renderTexture.display();
		renderSprite.setTexture(renderTexture.getTexture());
		window.draw(renderSprite);
	}
}

void WorldMapUi::RefreshData()
{
	rooms.clear();

	const std::unordered_map<sf::Vector2i, RoomData, Vector2iHash >& floordata = ROOM_MGR.GetFloorData();

	for (const auto& room : floordata)
	{
		RoomIcons roomdatum;
		roomdatum.coord = room.first;
		roomdatum.type = room.second.type;

		roomdatum.roomIcon.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "RoomIcon")));
		roomdatum.roomIcon.setScale(6.f, 6.f);
		Utils::SetOrigin(roomdatum.roomIcon, Origins::MC);

		for (int i = 0; i < room.second.connection.size();++i)
		{
			if (room.second.connection[i])
			{
				sf::RectangleShape shape;
				shape.setFillColor(sf::Color::White);
				switch (i)
				{
				case (int)HitBoxData::Type::PortalUp:
					shape.setSize({ 12.f,36.f });
					break;
				case (int)HitBoxData::Type::PortalDown:
					shape.setSize({ 12.f,36.f });
					break;
				case (int)HitBoxData::Type::PortalLeft:
					shape.setSize({ 36.f,12.f });
					break;
				case (int)HitBoxData::Type::PortalRight:
					shape.setSize({ 36.f,12.f });
					break;
				}
				Utils::SetOrigin(shape, Origins::MC);
				roomdatum.connectors.push_back({ i, shape });
			}
		}


		switch (room.second.type)
		{
		case RoomData::Type::Enter:
		{
			roomdatum.typeIcon.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "EnteranceIcon")));
			roomdatum.typeIcon.setScale(4.f, 4.f);
			Utils::SetOrigin(roomdatum.typeIcon, Origins::MC);
			break;
		}
		case RoomData::Type::Exit:
			roomdatum.typeIcon.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "ExitIcon")));
			roomdatum.typeIcon.setScale(4.f, 4.f);
			Utils::SetOrigin(roomdatum.typeIcon, Origins::MC);
			break;
		}

		rooms.push_back(roomdatum);
	}
}
