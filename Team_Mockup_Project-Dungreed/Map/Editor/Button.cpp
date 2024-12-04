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

	buttonBackground.setOutlineThickness(1.f);

	type = Type::TextButton;
}

void Button::Release()
{
	buttonText->Release();
	delete buttonText;
}

void Button::Reset()
{
	buttonText->Reset();

	SetPressed(false);

	SetOrigin(Origins::TL);
}

void Button::Update(float dt)
{
	sf::Vector2f mousepos = SCENE_MGR.GetCurrentScene()->ScreenToUi(InputMgr::GetMousePosition());
	mouseon = buttonBackground.getGlobalBounds().contains(mousepos);
	if (mouseon)
	{
		if (type == Type::TextButton)
		{
			buttonBackground.setFillColor({ 200,200,200 });
		}
		else if (type == Type::IconButton)
		{
			buttonBackground.setOutlineColor({ 255,150,150 });
		}

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
	if (type == Type::TextButton)
	{
		buttonText->Draw(window);
	}
}

void Button::SetPressed(bool pressed)
{
	this->pressed = pressed;
	if (this->pressed)
	{
		if (type == Type::TextButton)
		{
			buttonBackground.setFillColor({ 100,100,100 });
		}
		else if (type == Type::IconButton)
		{
			buttonBackground.setOutlineColor(sf::Color::Red);
		}
	}
	else
	{
		if (type == Type::TextButton)
		{
			buttonBackground.setFillColor({ 150,150,150 });
		}
		else if (type == Type::IconButton)
		{
			buttonBackground.setOutlineColor(sf::Color::White);
		}
	}
}

void Button::Set(const sf::Vector2f& buttonSize, int charSize, const sf::Color& charColor)
{
	type = Type::TextButton;
	SetButtonSize(buttonSize);
	buttonText->Set(charSize, charColor);
	buttonBackground.setOutlineColor(sf::Color::Black);
}

void Button::Set(const sf::Vector2f& buttonSize, const std::string& texId)
{
	type = Type::IconButton;
	SetButtonSize(buttonSize);
	buttonBackground.setFillColor(sf::Color::White);
	buttonBackground.setTexture(&TEXTURE_MGR.Get(texId));
	buttonBackground.setOutlineColor(sf::Color::White);
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

