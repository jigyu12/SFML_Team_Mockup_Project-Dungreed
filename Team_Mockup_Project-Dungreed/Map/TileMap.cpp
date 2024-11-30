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
	tileIndexes.resize(1);
	texture = nullptr;
	showGridLine = false;

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
	if (showGridLine)
	{
		sf::RenderStates gridstate;
		gridstate.transform = transform;
		window.draw(gridLine, gridstate);
	}
}

void TileMap::Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<std::vector<int>>& tileIndex, bool clear)
{
	if (clear)
	{
		this->tileIndexes.clear();
	}

	cellCount = count;
	cellSize = size;
	this->tileIndexes = tileIndex;
	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);
	gridLine.clear();
	gridLine.resize((count.x + count.y + 2) * 2);
	gridLine.setPrimitiveType(sf::PrimitiveType::Lines);

	if (this->tileIndexes.size() != count.y)
	{
		this->tileIndexes.resize(count.y);
	}
	for (int i = 0;i < this->tileIndexes.size();++i)
	{
		if (this->tileIndexes[i].size() != count.x)
		{
			this->tileIndexes[i].resize(count.x, 0);
		}
	}

	sf::Vector2f posOffset[4] =
	{
		{ 0.f, 0.f },
		{ size.x, 0.f },
		{ size.x, size.y },
		{ 0.f, size.y },
	};

	for (int i = 0;i <= count.x;++i)
	{
		gridLine[2 * i].color = sf::Color::White;
		gridLine[2 * i].position = { size.x * i,0.f };
		gridLine[2 * i + 1].color = sf::Color::White;
		gridLine[2 * i + 1].position = { size.x * i,size.y * count.y };
	}
	for (int i = 0;i <= count.y;++i)
	{
		gridLine[2 * (count.x + 1 + i)].color = sf::Color::White;
		gridLine[2 * (count.x + 1 + i)].position = { 0.f,size.y * i };
		gridLine[2 * (count.x + 1 + i) + 1].color = sf::Color::White;
		gridLine[2 * (count.x + 1 + i) + 1].position = { size.x * count.x,size.y * i };
	}

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
				if (i < this->tileIndexes.size() && j < this->tileIndexes[0].size())
				{
					va[vertexIndex].texCoords = TILE_TABLE->Get(this->tileIndexes[i][j]).startpos + posOffset[k];
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
	Set(tileMapData.cellcount, tileMapData.cellsize, tileMapData.tileIndexes, true);
}

void TileMap::SetTexture(const std::string& texId)
{
	this->texId = texId;
	texture = &TEXTURE_MGR.Get(this->texId);
}

void TileMap::SetTile(const sf::Vector2f& mousepos, const TileDatum& tile)
{
	sf::Vector2i cellpos = GetTilePosition(mousepos);
	SetTile(cellpos, tile);
}

void TileMap::SetTile(const sf::Vector2i& cellpos, const TileDatum& tile)
{
	tileIndexes[cellpos.y][cellpos.x] = tile.index;

	int quadindex = (cellpos.y * cellCount.x + cellpos.x) * 4;
	va[quadindex + 0].texCoords = { tile.startpos.x,tile.startpos.y };
	va[quadindex + 1].texCoords = { tile.startpos.x + cellSize.x,tile.startpos.y };
	va[quadindex + 2].texCoords = { tile.startpos.x + cellSize.x,tile.startpos.y + cellSize.y };
	va[quadindex + 3].texCoords = { tile.startpos.x,tile.startpos.y + cellSize.y };
}

TileMapData TileMap::GetTileMapData() const
{
	TileMapData data;
	data.name = this->name;
	data.cellcount = this->cellCount;
	data.cellsize = this->cellSize;
	data.texId = this->texId;
	data.tileIndexes = this->tileIndexes;

	return data;
}

sf::Vector2i TileMap::GetTilePosition(const sf::Vector2f& mousepos) const
{
	sf::Transform inverse = transform.getInverse();
	sf::Vector2f localPoint = inverse.transformPoint(mousepos);
	return { (int)(localPoint.x / cellSize.x),(int)(localPoint.y / cellSize.y) };
}

int TileMap::GetTileIndex(const sf::Vector2f& mousepos) const
{
	sf::Vector2i tilepos = GetTilePosition(mousepos);
	if (tilepos.y < tileIndexes.size() && tilepos.x < tileIndexes[0].size())
	{
		return tileIndexes[tilepos.y][tilepos.x];
	}
	return -1;
}

void TileMap::Resize(const sf::Vector2i& count)
{
	std::vector<std::vector<int>> oldData = tileIndexes;
	int minx = std::min((int)oldData[0].size(), count.x);
	int miny = std::min((int)oldData.size(), count.y);
	tileIndexes.resize(count.y);
	for (int i = 0;i < tileIndexes.size();++i)
	{
		tileIndexes[i].resize(count.x, -1);
	}

	for (int j = 0;j < miny;++j)
	{
		for (int i = 0;i < minx;++i)
		{
			tileIndexes[j][i] = oldData[j][i];
		}
	}
	Set(count, cellSize, tileIndexes);
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}
