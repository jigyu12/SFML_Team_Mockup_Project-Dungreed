#include "stdafx.h"
#include "TileMap.h"
#include "MapData.h"

TileMap::TileMap(const std::string& name)
	: GameObject(name)
{
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	UpdateTransform();
}

void TileMap::SetRotation(float angle)
{
	rotation = angle;
	UpdateTransform();
}

void TileMap::SetScale(const sf::Vector2f& s)
{
	scale = s;
	UpdateTransform();
}

void TileMap::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		sf::FloatRect rect = GetLocalBounds();
		origin.x = rect.width * ((int)preset % 3) * 0.5f;
		origin.y = rect.height * ((int)preset / 3) * 0.5f;
	}
	UpdateTransform();
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	UpdateTransform();
}

sf::FloatRect TileMap::GetLocalBounds() const
{
	return { 0.f, 0.f, cellCount.x * cellSize.x, cellCount.y * cellSize.y };
}

sf::FloatRect TileMap::GetGlobalBounds() const
{
	sf::FloatRect bounds = GetLocalBounds();
	return transform.transformRect(bounds);
}

void TileMap::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = -1;
}

void TileMap::Release()
{
}

void TileMap::Reset()
{

	SetOrigin(Origins::MC);
	SetScale({ 1.f, 1.f });
	SetPosition({ 0.f, 0.f });
}

void TileMap::Update(float dt)
{

}

void TileMap::Draw(sf::RenderWindow& window)
{
	sf::RenderStates state;
	if (texture != nullptr)
	{
		state.texture = texture;
	}
	state.transform = transform;
	window.draw(va, state);
}

void TileMap::Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<std::vector<int>>& tileIndex)
{
	cellCount = count;
	cellSize = size;
	this->tileIndex = tileIndex;
	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);

	this->tileIndex.resize(count.y);
	for (int i = 0;i < this->tileIndex.size();++i)
	{
		this->tileIndex[i].resize(count.x, -1);
	}

	sf::Vector2f posOffset[4] =
	{
		{ 0.f, 0.f },
		{ size.x, 0.f },
		{ size.x, size.y },
		{ 0.f, size.y },
	};

	for (int i = 0; i < count.y; ++i)
	{
		for (int j = 0; j < count.x; ++j)
		{

			int quadIndex = i * count.x + j;
			sf::Vector2f quadPos(j * size.x, i * size.y);

			for (int k = 0; k < 4; ++k)
			{
				int vertexIndex = quadIndex * 4 + k;
				va[vertexIndex].position = quadPos + posOffset[k];
				if (i < this->tileIndex.size() && j < this->tileIndex[0].size())
				{
					va[vertexIndex].texCoords = TILE_TABLE->Get(this->tileIndex[i][j]).startpos + posOffset[k];
				}
				else
				{
					va[vertexIndex].texCoords = TILE_TABLE->Undefined.startpos + posOffset[k];
				}
			}
		}
	}
}

void TileMap::Set(const TileMapData& tileMapData)
{
	SetTexture(tileMapData.texId);
	Set(tileMapData.cellcount, tileMapData.cellsize, tileMapData.tileIndex);
}

void TileMap::SetTexture(const std::string& texId)
{
	this->texId = texId;
	texture = &TEXTURE_MGR.Get(this->texId);
}

void TileMap::SetTile(const sf::Vector2f& mousepos, const TileDatum& tile)
{
	sf::Vector2i cellpos = GetTileIndex(mousepos);

	tileIndex[cellpos.y][cellpos.x] = tile.index;

	int quadindex = (cellpos.y * cellCount.x + cellpos.x) * 4;
	va[quadindex + 0].texCoords = { tile.startpos.x,tile.startpos.y };
	va[quadindex + 1].texCoords = { tile.startpos.x + cellSize.x,tile.startpos.y };
	va[quadindex + 2].texCoords = { tile.startpos.x + cellSize.x,tile.startpos.y + cellSize.y };
	va[quadindex + 3].texCoords = { tile.startpos.x,tile.startpos.y + cellSize.y };
}

TileMapData TileMap::GetTileMapData()
{
	TileMapData data;
	data.name = this->name;
	data.cellcount = this->cellCount;
	data.cellsize = this->cellSize;
	data.texId = this->texId;
	data.tileIndex = this->tileIndex;

	return data;
}

sf::Vector2i TileMap::GetTileIndex(const sf::Vector2f& mousepos)
{
	sf::Transform inverse = transform.getInverse();
	sf::Vector2f localPoint = inverse.transformPoint(mousepos);
	return { (int)(localPoint.x / cellSize.x),(int)(localPoint.y / cellSize.y) };
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}
