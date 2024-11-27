#pragma once

class HitBox
{
public:
	HitBox();

	sf::RectangleShape rect;

	void UpdateTr(const sf::Transformable& tr, const sf::FloatRect& localBound);
	void SetColor(const sf::Color& color) { rect.setOutlineColor(color); }
	void Draw(sf::RenderWindow& window);
};