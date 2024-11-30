#include "stdafx.h"
#include "HitBox.h"

HitBox::HitBox()
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);
}

void HitBox::UpdateTr(const sf::Transformable& tr, const sf::FloatRect& localBounds)
{
	rect.setOutlineColor(sf::Color::Green);
	rect.setSize({ localBounds.width, localBounds.height });
	rect.setOrigin(tr.getOrigin() - localBounds.getPosition());
	rect.setPosition(tr.getPosition());
	rect.setScale(tr.getScale());
	rect.setRotation(tr.getRotation());
}

void HitBox::Draw(sf::RenderWindow& window)
{
	if (Variables::isDrawHitBox)
		window.draw(rect);
}

sf::Vector2f HitBox::GetCenter()
{
	sf::Vector2f center = rect.getLocalBounds().getSize() * 0.5f + rect.getLocalBounds().getPosition();
	return rect.getTransform().transformPoint(center);
}
