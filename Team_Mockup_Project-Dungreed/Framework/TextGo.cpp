#include "stdafx.h"
#include "TextGo.h"

TextGo::TextGo(const std::string& fontId, const std::string& name)
	: GameObject(name), fontId(fontId)
{
}

void TextGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = Utils::SetOrigin(text, originPreset);
}

void TextGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	text.setOrigin(origin);
}

void TextGo::Reset()
{
	text.setFont(FONT_MGR.Get(fontId));
	SetString(stringId);
	//SetOrigin(originPreset);
}

void TextGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	text.setPosition(pos);
}

void TextGo::SetRotation(float angle)
{
	rotation = angle;
	text.setRotation(angle);
}

void TextGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	text.setScale(scale);
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::OnLocalize(Languages lang)
{
	text.setString(STRING_TABLE->Get(stringId, lang));
	SetOrigin(originPreset);
}

void TextGo::Set(int size, const sf::Color& color)
{
	SetFillColor(color);
	SetCharacterSize(size);
}

void TextGo::SetString(const std::string& id, bool useTable)
{
	if (!useTable)
	{
		text.setString(id);
		SetOrigin(originPreset);
		return;
	}
	stringId = id;
	std::wstring wstr = STRING_TABLE->Get(stringId);
	std::wstring newline = L"\\n";
	auto index = wstr.find(newline);
	if (index < 0 || index >= wstr.length())
	{
		text.setString(wstr);
		SetOrigin(originPreset);
		return;
	}
	wstr.replace(wstr.find(newline), newline.length(), L"\n");
	text.setString(wstr);
	SetOrigin(originPreset);
}

void TextGo::SetString(const std::wstring& id)
{
	text.setString(id);
}

void TextGo::SetString(const std::string& id, const std::wstring& str)
{
	stringId = id;
	text.setString(STRING_TABLE->Get(stringId) + str);
	SetOrigin(originPreset);
}

void TextGo::SetCharacterSize(int size)
{
	text.setCharacterSize(size);
	SetOrigin(originPreset);
}

void TextGo::SetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

void TextGo::SetOutlineColor(const sf::Color& color)
{
	text.setOutlineColor(color);
}

void TextGo::SetOutlineThickness(float thickness)
{
	text.setOutlineThickness(thickness);
}

void TextGo::Init()
{
}

void TextGo::Release()
{
}

void TextGo::Update(float dt)
{
}
