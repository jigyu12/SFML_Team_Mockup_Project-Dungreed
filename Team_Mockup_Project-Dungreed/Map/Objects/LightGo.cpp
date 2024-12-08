#include "stdafx.h"
#include "LightGo.h"

LightGo::LightGo(const std::string& name)
	: GameObject(name)
{
}

void LightGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	UpdateTransform();
}

void LightGo::SetRotation(float angle)
{
	rotation = angle;
	UpdateTransform();
}

void LightGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	UpdateTransform();
}

void LightGo::SetOrigin(Origins preset)
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

void LightGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	UpdateTransform();
}

sf::FloatRect LightGo::GetLocalBounds() const
{
	return sf::FloatRect(0.f,0.f,radius*2.f,radius*2.f);
}

sf::FloatRect LightGo::GetGlobalBounds() const
{
	return transform.transformRect(GetLocalBounds());
}

void LightGo::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 200;

	vaLight.resize(16);

	rs.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::Add);
}

void LightGo::Release()
{
}

void LightGo::Reset()
{
	vaLight.setPrimitiveType(sf::PrimitiveType::TriangleFan);
	SetCenterColor({ 40,40,20 });
	SetRadius(30.f);
}

void LightGo::Update(float dt)
{
}

void LightGo::Draw(sf::RenderWindow& window)
{
	rs.transform = transform;
	window.draw(vaLight, rs);
}

void LightGo::SetRadius(float radius)
{
	this->radius = radius;
	int count = vaLight.getVertexCount();
	int angler = count - 2;
	vaLight[0].position = { this->radius,this->radius };
	for (int i = 1;i < count;++i)
	{
		vaLight[i].position.x = this->radius * (1 + cosf((i - 1) * Utils::PI * 2.f / angler));
		vaLight[i].position.y = this->radius * (1 + sinf((i - 1) * Utils::PI * 2.f / angler));
		vaLight[i].color = { 0,0,0,0 };
	}
}

void LightGo::SetCenterColor(const sf::Color& color)
{
	vaLight[0].color = color;
}

void LightGo::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}
