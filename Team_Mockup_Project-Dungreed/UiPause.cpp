#include "stdafx.h"
#include "UiPause.h"
#include "Player.h"

UiPause::UiPause(const std::string& name)
	: GameObject(name)
{
}

void UiPause::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiPause::SetRotation(float angle)
{
	rotation = angle;
}

void UiPause::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiPause::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiPause::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiPause::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 200;

	buttonRun.Init();
}

void UiPause::Release()
{
}

void UiPause::Reset()
{
	buttonRun.Reset();

	const sf::View& uiview = SCENE_MGR.GetCurrentScene()->GetUiView();

	renderTexture.create(uiview.getSize().x, uiview.getSize().y);

	line.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "MenuLine")));
	line.setPosition(uiview.getSize() * 0.5f);
	line.setScale(6.f, 6.f);
	Utils::SetOrigin(line, Origins::MC);

	buttonRun.SetPosition(uiview.getSize() * 0.5f);
	buttonRun.Set({ 180,126 }, RESOURCEID_TABLE->Get("Graphic", "RunIconOff"));
	buttonRun.SetOrigin(Origins::MC);
	buttonRun.SetClickedEvent([this]()
		{
			SCENE_MGR.ChangeScene(SceneIds::MainTitle);
			FRAMEWORK.SetTimeScale(1.f);
			Player* player =dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
			if (player != nullptr)
			{
				player->SaveLastData();
			}
		});
}

void UiPause::Update(float dt)
{
	buttonRun.Update(dt);

	bool mouseOn = buttonRun.isMouseOn();

	if (buttonOn != mouseOn)
	{
		buttonOn = mouseOn;
		if (buttonOn)
		{
			buttonRun.SetTexture(RESOURCEID_TABLE->Get("Graphic", "RunIconOn"));
		}
		else
		{
			buttonRun.SetTexture(RESOURCEID_TABLE->Get("Graphic", "RunIconOff"));
		}
	}

	buttonRun.SetButtonFillColor(sf::Color::White);
	buttonRun.SetButtonOutlineColor(sf::Color::Transparent);
}

void UiPause::Draw(sf::RenderWindow& window)
{
	renderTexture.clear({ 46,43,63 });
	renderTexture.draw(line);
	buttonRun.Draw(renderTexture);
	renderTexture.display();
	drawWindow.setTexture(renderTexture.getTexture());
	window.draw(drawWindow);
}
