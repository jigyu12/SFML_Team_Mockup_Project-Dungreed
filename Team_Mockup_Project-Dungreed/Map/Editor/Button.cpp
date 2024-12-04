#include "stdafx.h"
#include "Button.h"
#include "TextGo.h"

Button::Button(const std::string& name)
	: GameObject(name)
{
}

void Button::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	buttonBackground.setPosition(position);
	buttonText->SetPosition(buttonBackground.getTransform().transformPoint(Utils::GetCenter(buttonBackground.getLocalBounds())));
}

void Button::SetRotation(float angle)
{
	rotation = angle;
	buttonBackground.setRotation(rotation);
	buttonText->SetRotation(rotation);
}

void Button::SetScale(const sf::Vector2f& s)
{
	scale = s;
	buttonBackground.setScale(scale);
}

void Button::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(buttonBackground, originPreset);
		buttonText->SetOrigin(Origins::MC);
	}
}

void Button::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	buttonBackground.setOrigin(origin);
	buttonText->SetOrigin(Origins::MC);
}

void Button::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 450;

	buttonText = new TextGo(RESOURCEID_TABLE->Get("Font", "French"), "text");
	buttonText->Init();
}

void Button::Release()
{
	buttonText->Release();
	delete buttonText;
}

void Button::Reset()
{
	buttonText->Reset();

	buttonBackground.setFillColor({ 150,150,150 });
	buttonBackground.setOutlineColor(sf::Color::Black);
	buttonBackground.setOutlineThickness(1.f);

	SetOrigin(Origins::TL);
}

void Button::Update(float dt)
{
	sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
	mouseon = buttonBackground.getGlobalBounds().contains(mousepos);
	if (mouseon)
	{
		buttonBackground.setFillColor({ 200,200,200 });

		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) && Clicked)
		{
			Clicked();
		}
	}
	else
	{
		SetPressed(pressed);
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(buttonBackground);
	buttonText->Draw(window);
}

void Button::SetPressed(bool pressed)
{
	this->pressed = pressed;
	if (this->pressed)
	{
		buttonBackground.setFillColor({ 100,100,100 });
	}
	else
	{
		buttonBackground.setFillColor({ 150,150,150 });
	}
}

void Button::Set(const sf::Vector2f& buttonSize, int charSize, const sf::Color& charColor)
{
	SetButtonSize(buttonSize);
	buttonText->Set(charSize, charColor);
}

void Button::SetString(const std::string& text, bool usetable)
{
	buttonText->SetString(text, usetable);
}

void Button::SetString(const std::string& textid, const std::wstring& text)
{
	buttonText->SetString(textid, text);
}

void Button::SetCharSize(float size)
{
	buttonText->SetCharacterSize(size);
}

void Button::SetButtonSize(const sf::Vector2f& size)
{
	buttonBackground.setSize(size);
}

void Button::SetClickedEvent(const std::function<void()>& event)
{
	Clicked = event;
}

