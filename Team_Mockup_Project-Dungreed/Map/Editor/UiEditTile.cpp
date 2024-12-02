#include "stdafx.h"
#include "UiEditTile.h"
#include "TileMap.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"

UiEditTile::UiEditTile(const std::string& name)
	: GameObject(name)
{
}

void UiEditTile::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	boxWindow.setPosition(position);

	sf::Transform transform = boxWindow.getTransform();

	for (int k = 0; k < tileList.size();++k)
	{
		tileList[k]->SetPosition(transform.transformPoint(40.f, 360.f));
	}

	selectedTile->SetPosition(transform.transformPoint(50.f, 30.f));
	selectedTileText.setPosition(transform.transformPoint(120.f, 30.f));

	for (int i = 0;i < upButtons.size();++i)
	{
		upButtons[i].setPosition(transform.transformPoint(250.f, 100.f + i * 60.f));
	}
	for (int i = 0;i < downButtons.size();++i)
	{
		downButtons[i].setPosition(transform.transformPoint(350.f, 100.f + i * 60.f));
	}
	for (int i = 0;i < buttonTexts.size();++i)
	{
		buttonTexts[i].setPosition(transform.transformPoint(50.f, 100.f + i * 60.f));
	}
}

void UiEditTile::SetRotation(float angle)
{
	rotation = angle;
	boxWindow.setRotation(rotation);
}

void UiEditTile::SetScale(const sf::Vector2f& s)
{
	scale = s;
	boxWindow.setScale(scale);
}

void UiEditTile::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(boxWindow, originPreset);
	}
}

void UiEditTile::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	boxWindow.setOrigin(origin);
}

void UiEditTile::SetSize(const sf::Vector2f& size)
{
	boxWindow.setSize(size);
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

	upButtons.resize((int)UiType::TypeCount);
	buttonTexts.resize((int)UiType::TypeCount);
	downButtons.resize((int)UiType::TypeCount);
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
	boxWindow.setFillColor({ 150,150,150,255 });

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
		tileList[k]->SetShowGridLine(true);
	}

	selectedTile->SetTexture("graphics/map/Map.png");
	selectedTile->Set({ 1,1 }, { 16.f,16.f }, std::vector<std::vector<int>>(1, std::vector<int>(1, 0)));
	selectedTile->SetScale({ 3.f, 3.f });
	selectedTile->SetShowGridLine(true);

	for (auto& upButton : upButtons)
	{
		upButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttonup.png"));
	}
	for (auto& downButton : downButtons)
	{
		downButton.setTexture(TEXTURE_MGR.Get("graphics/ui/mapeditor/uibuttondown.png"));
	}
	for (auto& text : buttonTexts)
	{
		text.setFont(FONT_MGR.Get("fonts/french.ttf"));
	}

	selectedTileText.setFont(FONT_MGR.Get("fonts/french.ttf"));
	selectedTileText.setString("SELECTED TILE");
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
			if (upButtons[(int)UiType::Page].getGlobalBounds().contains(mousepos))
			{
				if (++currentPage == pagecount)
				{
					currentPage = 0;
				}
			}
			else if (downButtons[(int)UiType::Page].getGlobalBounds().contains(mousepos))
			{
				if (--currentPage < 0)
				{
					currentPage = pagecount - 1;
				}
			}
			else if (upButtons[(int)UiType::CountX].getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x + 1, count.y });
			}
			else if (downButtons[(int)UiType::CountX].getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x - 1, count.y });
			}
			else if (upButtons[(int)UiType::CountY].getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x, count.y + 1 });
			}
			else if (downButtons[(int)UiType::CountY].getGlobalBounds().contains(mousepos))
			{
				sf::Vector2i count = editingTileMap->GetCellCount();
				uieditor->ResizeTileMaps({ count.x , count.y - 1 });
			}
		}
		if (upButtons[(int)UiType::Layer].getGlobalBounds().contains(mousepos))
		{
			uieditor->SetSelectedTileLayer(Utils::Clamp(uieditor->GetSelectedTileLayer() + 1, -1, MapData::TileMapCount - 1));
			editingTileMap = uieditor->GetSelectedTileMap();
		}
		else if (downButtons[(int)UiType::Layer].getGlobalBounds().contains(mousepos))
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


	buttonTexts[(int)UiType::Page].setString("PAGE : " + std::to_string(currentPage));
	buttonTexts[(int)UiType::Layer].setString("LAYER : " + (uieditor->GetSelectedTileLayer() == -1 ? "ALL" : std::to_string(uieditor->GetSelectedTileLayer())));

	if (uieditor != nullptr)
	{
		sf::Vector2i count = uieditor->GetSelectedTileMap()->GetCellCount();


		buttonTexts[(int)UiType::CountX].setString("COUNT X : " + std::to_string(count.x));
		buttonTexts[(int)UiType::CountY].setString("COUNT Y : " + std::to_string(count.y));
	}
}

void UiEditTile::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);


	for (auto& upButton : upButtons)
	{
		window.draw(upButton);
	}
	for (auto& downButton : downButtons)
	{
		window.draw(downButton);
	}
	for (auto& text : buttonTexts)
	{
		window.draw(text);
	}

	tileList[currentPage]->Draw(window);
	selectedTile->Draw(window);
	window.draw(selectedTileText);
}
