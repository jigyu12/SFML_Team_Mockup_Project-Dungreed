#include "stdafx.h"
#include "UiEditTile.h"
#include "TileMap.h"
#include "MapData.h"
#include "SceneMapEdit.h"

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
	pagecount = 2;

	for (int i = 0; i < pagecount;++i)
	{
		TileMap* tmpTileMap = new TileMap();
		tmpTileMap->Init();
		tileList.push_back(tmpTileMap);
	}

	selectedTiles = new TileMap();
	selectedTiles->Init();
}

void UiEditTile::Release()
{
	for (int i = 0; i < tileList.size();++i)
	{
		tileList[i]->Release();
		delete tileList[i];
	}
	tileList.clear();

	selectedTiles->Release();
	delete selectedTiles;
}

void UiEditTile::Reset()
{
	for (int i = 0; i < tileList.size();++i)
	{
		tileList[i]->Reset();
	}

	editingTileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("tileMap"));

	selectedPage = 0;
	currentPage = 0;

	std::vector<sf::Vector2f> vectiledata;

	const std::unordered_map<int, TileDatum>& tiletable = TILE_TABLE->GetTable();


	std::vector<std::vector<std::vector<int>>> tileIndexes(pagecount, std::vector<std::vector<int>>(20, std::vector<int>(5, -1)));
	int cnt = 0;
	for (auto& tiledata : tiletable)
	{
		int k = tiledata.first / (tileIndexes[0].size() * tileIndexes[0][0].size());
		if (tiledata.first % (tileIndexes[0].size() * tileIndexes[0][0].size())==0)
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
		tileList[k]->Set({ 5,20 }, { 16.f,16.f }, tileIndexes[k]);
		tileList[k]->SetScale({ 3.f, 3.f });
		tileList[k]->SetPosition({ 50.f,200.f });
		tileList[k]->SetShowGridLine(true);
	}

	selectedTiles->SetTexture("graphics/map/Map.png");
	selectedTiles->Set({ 1,1 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, -1)));
	selectedTiles->SetScale({ 3.f, 3.f });
	selectedTiles->SetPosition({ 50.f, 100.f });
	selectedTiles->SetShowGridLine(true);

	xUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	xUpButton.setPosition(50.f, 150.f);
	xUpButton.setScale(0.25f, 0.25f);

	xDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	xDownButton.setPosition(150.f, 150.f);
	xDownButton.setScale(0.25f, 0.25f);

	yUpButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	yUpButton.setPosition(250.f, 150.f);
	yUpButton.setScale(0.25f, 0.25f);

	yDownButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	yDownButton.setPosition(350.f, 150.f);
	yDownButton.setScale(0.25f, 0.25f);
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
			selectedTiles->SetTile(sf::Vector2i(0, 0), TILE_TABLE->Get(selectedTileIndex));
		}
		else if (xUpButton.getGlobalBounds().contains(mousepos))
		{
			if (editingTileMap != nullptr)
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				editingTileMap->Resize({ count.x + 1, count.y });
			}
		}
		else if (xDownButton.getGlobalBounds().contains(mousepos))
		{
			sf::Vector2i count = editingTileMap->GetCellCount();
			editingTileMap->Resize({ count.x - 1, count.y });
		}
		else if (yUpButton.getGlobalBounds().contains(mousepos))
		{
			sf::Vector2i count = editingTileMap->GetCellCount();
			editingTileMap->Resize({ count.x, count.y + 1 });
		}
		else if (yDownButton.getGlobalBounds().contains(mousepos))
		{
			sf::Vector2i count = editingTileMap->GetCellCount();
			editingTileMap->Resize({ count.x , count.y - 1 });
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num8))
	{
		if (++currentPage == pagecount)
		{
			currentPage = 0;
		}
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num9))
	{
		if (--currentPage < 0)
		{
			currentPage = pagecount - 1;
		}
	}
}

void UiEditTile::Draw(sf::RenderWindow& window)
{
	window.draw(xUpButton);
	window.draw(xDownButton);
	window.draw(yUpButton);
	window.draw(yDownButton);
	tileList[currentPage]->Draw(window);
	selectedTiles->Draw(window);
}
