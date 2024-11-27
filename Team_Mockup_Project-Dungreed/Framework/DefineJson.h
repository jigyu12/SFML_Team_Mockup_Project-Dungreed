#pragma once

namespace sf
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2f, x, y)

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2i, x, y)

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::FloatRect, left, top, width, height)
}
