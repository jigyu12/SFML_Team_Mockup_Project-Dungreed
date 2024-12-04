#include "stdafx.h"
#include "UiEditTile.h"
#include "TileMap.h"
#include "SceneMapEdit.h"
#include "UiEditor.h"
#include "Button.h"

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
	selectedTileText->SetPosition(transform.transformPoint(120.f, 30.f));

	for (int i = 0;i < upButtons.size();++i)
	{
		upButtons[i]->SetPosition(transform.transformPoint(250.f, 100.f + i * 60.f));
	}
	for (int i = 0;i < downButtons.size();++i)
	{
		downButtons[i]->SetPosition(transform.transformPoint(350.f, 100.f + i * 60.f));
	}
	for (int i = 0;i < paramTexts.size();++i)
	{
		paramTexts[i]->SetPosition(transform.transformPoint(50.f, 100.f + i * 60.f));
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

	selectedTileText = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
	selectedTileText->Init();

	upButtons.resize((int)UiType::TypeCount);
	paramTexts.resize((int)UiType::TypeCount);
	downButtons.resize((int)UiType::TypeCount);
	for (int i = 0; i < (int)UiType::TypeCount;++i)
	{
		upButtons[i] = new Button();
		upButtons[i]->Init();
		paramTexts[i] = new TextGo(RESOURCEID_TABLE->Get("Font", "French"));
		paramTexts[i]->Init();
		downButtons[i] = new Button();
		downButtons[i]->Init();
	}
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

	selectedTileText->Release();
	delete selectedTileText;

	for (int i = 0; i < (int)UiType::TypeCount;++i)
	{
		upButtons[i]->Release();
		delete upButtons[i];
		paramTexts[i]->Release();
		delete paramTexts[i];
		downButtons[i]->Release();
		delete downButtons[i];
	}
	upButtons.clear();
	paramTexts.clear();
	downButtons.clear();
}

void UiEditTile::Reset()
{
	for (int i = 0; i < tileList.size();++i)
	{
		tileList[i]->Reset();
	}
	boxWindow.setFillColor({ 120,120,120 });

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


	selectedTileText->Reset();
	selectedTileText->Set(30);
	selectedTileText->SetString("Selected Tile", true);

	for (int i = 0; i < (int)UiType::TypeCount;++i)
	{
		upButtons[i]->Reset();
		upButtons[i]->Set({ 90.f,45.f }, 20);
		upButtons[i]->SetString("Up", true);

		downButtons[i]->Reset();
		downButtons[i]->Set({ 90.f,45.f }, 20);
		downButtons[i]->SetString("Down", true);
		switch (i)
		{
		case (int)UiType::Layer:
			upButtons[i]->SetClickedEvent([this]()
				{
					uieditor->SetSelectedTileLayer(Utils::Clamp(uieditor->GetSelectedTileLayer() + 1, -1, MapData::TileMapCount - 1));
					editingTileMap = uieditor->GetSelectedTileMap();
				});
			downButtons[i]->SetClickedEvent([this]()
				{
					uieditor->SetSelectedTileLayer(Utils::Clamp(uieditor->GetSelectedTileLayer() - 1, -1, MapData::TileMapCount - 1));
					editingTileMap = uieditor->GetSelectedTileMap();
				});
			break;
		case (int)UiType::Page:
			upButtons[i]->SetClickedEvent([this]()
				{
					currentPage = Utils::Clamp(++currentPage, 0, pagecount - 1);
				});
			downButtons[i]->SetClickedEvent([this]()
				{
					currentPage = Utils::Clamp(--currentPage, 0, pagecount);
				});
			break;
		case (int)UiType::CountX:
			upButtons[i]->SetClickedEvent([this]()
				{
					if (editingTileMap == nullptr)
					{
						return;
					}
					sf::Vector2i count = editingTileMap->GetCellCount();
					uieditor->ResizeTileMaps({ count.x + 1, count.y });
				});
			downButtons[i]->SetClickedEvent([this]()
				{
					if (editingTileMap == nullptr)
					{
						return;
					}
					sf::Vector2i count = editingTileMap->GetCellCount();
					uieditor->ResizeTileMaps({std::max(1, count.x - 1), count.y });
				});
			break;
		case (int)UiType::CountY:
			upButtons[i]->SetClickedEvent([this]()
				{
					if (editingTileMap == nullptr)
					{
						return;
					}
					sf::Vector2i count = editingTileMap->GetCellCount();
					uieditor->ResizeTileMaps({ count.x, count.y + 1 });
				});
			downButtons[i]->SetClickedEvent([this]()
				{
					if (editingTileMap == nullptr)
					{
						return;
					}
					sf::Vector2i count = editingTileMap->GetCellCount();
					uieditor->ResizeTileMaps({ count.x , std::max(1,count.y - 1) });
				});
			break;
		}

		paramTexts[i]->Reset();
		paramTexts[i]->Set(30);
	}
}

void UiEditTile::Update(float dt)
{
	for (int i = 0; i < (int)UiType::TypeCount;++i)
	{
		upButtons[i]->Update(dt);
		downButtons[i]->Update(dt);
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
		if (tileList[currentPage]->GetGlobalBounds().contains(mousepos))
		{
			selectedPage = currentPage;
			selectedTileIndex = tileList[selectedPage]->GetTileIndex(mousepos);
			selectedTile->SetTile(sf::Vector2i(0, 0), TILE_TABLE->Get(selectedTileIndex));
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

	paramTexts[(int)UiType::Page]->SetString("Page", std::to_wstring(currentPage));
	paramTexts[(int)UiType::Layer]->SetString("Layer", (uieditor->GetSelectedTileLayer() == -1 ? L"ALL" : std::to_wstring(uieditor->GetSelectedTileLayer())));

	if (uieditor != nullptr)
	{
		sf::Vector2i count = uieditor->GetSelectedTileMap()->GetCellCount();

		paramTexts[(int)UiType::CountX]->SetString("CountX" , std::to_wstring(count.x));
		paramTexts[(int)UiType::CountY]->SetString("CountY" , std::to_wstring(count.y));
	}
}

void UiEditTile::Draw(sf::RenderWindow& window)
{
	window.draw(boxWindow);

	for (int i = 0; i < (int)UiType::TypeCount;++i)
	{
		upButtons[i]->Draw(window);
		downButtons[i]->Draw(window);
		paramTexts[i]->Draw(window);
	}

	tileList[currentPage]->Draw(window);
	selectedTile->Draw(window);
	selectedTileText->Draw(window);
}
