#include "stdafx.h"
#include "UiEditTile.h"
#include "TileMap.h"
#include "MapData.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"

UiEditTile::UiEditTile(const std::string& name)
	: GameObject(name)
{
}

void UiEditTile::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiEditTile::SetRotation(float angle)
{
	rotation = angle;
}

void UiEditTile::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiEditTile::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiEditTile::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiEditTile::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;

	pagecount = 2;

	for (int i = 0; i < pagecount;++i)
	{
		TileMap* tmpTileMap = new TileMap();
		tmpTileMap->Init();
		tileList.push_back(tmpTileMap);
	}

	selectedTile = new TileMap();
	selectedTile->Init();
}

void UiEditTile::Release()
{
	for (int i = 0; i < tileList.size();++i)
	{
		tileList[i]->Release();
		delete tileList[i];
	}
	tileList.clear();

	selectedTile->Release();
	delete selectedTile;
}

void UiEditTile::Reset()
{
	for (int i = 0; i < tileList.size();++i)
	{
		tileList[i]->Reset();
	}

	uieditor = dynamic_cast<UiEditor*>(SCENE_MGR.GetCurrentScene()->FindGo("uieditor"));

	selectedPage = 0;
	currentPage = 0;

	std::vector<sf::Vector2f> vectiledata;

	const std::unordered_map<int, TileDatum>& tiletable = TILE_TABLE->GetTable();


	std::vector<std::vector<std::vector<int>>> tileIndexes(pagecount, std::vector<std::vector<int>>(10, std::vector<int>(10, -1)));
	int cnt = 0;
	for (auto& tiledata : tiletable)
	{
		int k = tiledata.first / (tileIndexes[0].size() * tileIndexes[0][0].size());
		if (tiledata.first % (tileIndexes[0].size() * tileIndexes[0][0].size()) == 0)
		{
			cnt = 0;
		}

		int j = cnt / tileIndexes[0][0].size();
		int i = cnt % tileIndexes[0][0].size();

		if (++cnt > tileIndexes.size() * tileIndexes[0].size() * tileIndexes[0][0].size()
			|| k >= pagecount)
		{
			break;
		}
		tileIndexes[k][j][i] = tiledata.first;
	}

	for (int k = 0; k < tileList.size();++k)
	{
		tileList[k]->SetTexture("graphics/map/Map.png");
		tileList[k]->Set({ (int)tileIndexes[0][0].size(),(int)tileIndexes[0].size() }, { 16.f,16.f }, tileIndexes[k]);
		tileList[k]->SetScale({ 2.5f, 2.5f });
		tileList[k]->SetPosition({ 40.f,560.f });
		tileList[k]->SetShowGridLine(true);
	}

	selectedTile->SetTexture("graphics/map/Map.png");
	selectedTile->Set({ 1,1 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, 0)));
	selectedTile->SetScale({ 3.f, 3.f });
	selectedTile->SetPosition({ 50.f, 100.f });
	selectedTile->SetShowGridLine(true);

	layerUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	layerUpButton.setPosition(50.f, 200.f);

	layerDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	layerDownButton.setPosition(50.f, 320.f);

	layerText.setFont(FONT_MGR.Get("fonts/french.ttf"));
	layerText.setPosition(50.f, 260.f);

	pageUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	pageUpButton.setPosition(150.f, 200.f);

	pageDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	pageDownButton.setPosition(150.f, 320.f);

	pageText.setFont(FONT_MGR.Get("fonts/french.ttf"));
	pageText.setPosition(150.f, 260.f);

	xUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	xUpButton.setPosition(250.f, 200.f);

	xDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	xDownButton.setPosition(250.f, 320.f);

	countxText.setFont(FONT_MGR.Get("fonts/french.ttf"));
	countxText.setPosition(250.f, 260.f);

	yUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	yUpButton.setPosition(350.f, 200.f);

	yDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	yDownButton.setPosition(350.f, 320.f);

	countyText.setFont(FONT_MGR.Get("fonts/french.ttf"));
	countyText.setPosition(350.f, 260.f);
}

void UiEditTile::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
		if (tileList[currentPage]->GetGlobalBounds().contains(mousepos))
		{
			selectedPage = currentPage;
			selectedTileIndex = tileList[selectedPage]->GetTileIndex(mousepos);
			selectedTile->SetTile(sf::Vector2i(0, 0), TILE_TABLE->Get(selectedTileIndex));
		}
		if (editingTileMap != nullptr)
		{
			if (pageUpButton.getGlobalBounds().contains(mousepos))
			{
				if (++currentPage == pagecount)
				{
					currentPage = 0;
				}
			}
			else if (pageDownButton.getGlobalBounds().contains(mousepos))
			{
				if (--currentPage < 0)
				{
					currentPage = pagecount - 1;
				}
			}
			else if (xUpButton.getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x + 1, count.y });
			}
			else if (xDownButton.getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x - 1, count.y });
			}
			else if (yUpButton.getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x, count.y + 1 });
			}
			else if (yDownButton.getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x , count.y - 1 });
			}
		}
		if (layerUpButton.getGlobalBounds().contains(mousepos))
		{
			uieditor->SetSelectedTileLayer(Utils::Clamp(uieditor->GetSelectedTileLayer() + 1, -1, MapData::TileMapCount - 1));
			editingTileMap = uieditor->GetSelectedTileMap();
		}
		else if (layerDownButton.getGlobalBounds().contains(mousepos))
		{
			uieditor->SetSelectedTileLayer(Utils::Clamp(uieditor->GetSelectedTileLayer() - 1, -1, MapData::TileMapCount - 1));
			editingTileMap = uieditor->GetSelectedTileMap();
		}
	}
	if (InputMgr::GetMouseButton(sf::Mouse::Left)
		&& InputMgr::GetMousePosition().x > 480.f)
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(InputMgr::GetMousePosition());
		if (editingTileMap != nullptr && editingTileMap->GetGlobalBounds().contains(mousepos))
		{
			editingTileMap->SetTile(mousepos, TILE_TABLE->Get(selectedTileIndex));
		}
	}
	pageText.setString(std::to_string(currentPage));
	layerText.setString(std::to_string(uieditor->GetSelectedTileLayer()));

	if (editingTileMap != nullptr)
	{
		sf::Vector2i count = editingTileMap->GetCellCount();

		countxText.setString(std::to_string(count.x));
		countyText.setString(std::to_string(count.y));
	}
}

void UiEditTile::Draw(sf::RenderWindow& window)
{
	window.draw(pageUpButton);
	window.draw(pageDownButton);
	window.draw(pageText);
	window.draw(layerUpButton);
	window.draw(layerDownButton);
	window.draw(layerText);
	window.draw(xUpButton);
	window.draw(xDownButton);
	window.draw(countxText);
	window.draw(yUpButton);
	window.draw(yDownButton);
	window.draw(countyText);
	tileList[currentPage]->Draw(window);
	selectedTile->Draw(window);
}
