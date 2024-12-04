#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "Utils.h"
#include <cmath>

std::mt19937 Utils::generator;
const float Utils::PI = acosf(-1.f);

void Utils::Init()
{
	std::random_device rd;
	generator.seed(rd());
}

int Utils::RandomRange(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(generator);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}

float Utils::RandomValue()
{
	return RandomRange(0.f, 1.f);
}

sf::Vector2f Utils::OnUnitCircle(float angle)
{
	return sf::Vector2f(std::cosf(angle), std::sinf(angle));
}

sf::Vector2f Utils::RandomOnUnitCircle()
{
	float angle = RandomRange(0.f, 2.f * PI);
	return sf::Vector2f(std::cosf(angle), std::sinf(angle));
}

sf::Vector2f Utils::RandomInUnitCircle()
{
	return RandomOnUnitCircle() * RandomValue();
}

sf::Color Utils::RandomColor(bool opaque)
{
	sf::Uint8 r = RandomRange(0, 255);
	sf::Uint8 g = RandomRange(0, 255);
	sf::Uint8 b = RandomRange(0, 255);
	sf::Uint8 a = opaque ? 255 : RandomRange(0, 255);
	return sf::Color(r, g, b, a);
}

sf::Vector2f Utils::RandomPointInRect(const sf::FloatRect& rect)
{
	return sf::Vector2f(
		RandomRange(rect.left, rect.left + rect.width),
		RandomRange(rect.top, rect.top + rect.height));
}


sf::Vector2f Utils::SetOrigin(sf::Transformable& obj, Origins preset, const sf::FloatRect rect)
{
	sf::Vector2f newOrigin(rect.width, rect.height);
	newOrigin.x *= ((int)preset % 3) * 0.5f;
	newOrigin.y *= ((int)preset / 3) * 0.5f;
	obj.setOrigin(newOrigin);
	return newOrigin;
}

sf::Vector2f Utils::SetOrigin(sf::Shape& obj, Origins preset)
{
	return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Text& obj, Origins preset)
{
	return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Sprite& obj, Origins preset)
{
	return SetOrigin(obj, preset, obj.getLocalBounds());
}

float Utils::Clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

float Utils::Clamp01(float value)
{
	return Clamp(value, 0.f, 1.f);
}

float Utils::Lerp(float min, float max, float t, bool clamp)
{
	if (clamp)
		t = Clamp01(t);
	return min + (max - min) * t;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamp)
{
	if (clamp)
		t = Clamp01(t);
	return min + (max - min) * t;
}

sf::Color Utils::Lerp(const sf::Color& min, const sf::Color& max, float t, bool clamp)
{
	if (clamp)
		t = Clamp01(t);

	return sf::Color(
		(sf::Uint8)Lerp(min.r, max.r, t, clamp),
		(sf::Uint8)Lerp(min.g, max.g, t, clamp),
		(sf::Uint8)Lerp(min.b, max.b, t, clamp),
		(sf::Uint8)Lerp(min.a, max.a, t, clamp)
	);
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return std::sqrtf(SqrMagnitude(vec));
}

void Utils::Normailize(sf::Vector2f& vec)
{
	float mag = Magnitude(vec);

	if (ABS(mag - 1.f) > EPSILON)
		vec /= mag;
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& vec)
{
	float mag = Magnitude(vec);

	if (ABS(mag) < EPSILON)
	{
		return { 0.f, 0.f };
	}

	if (ABS(mag - 1.f) < EPSILON)
		return vec;

	return vec / mag;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}

float Utils::RadianToDegree(float radian)
{
	return radian * (180.f / PI);
}

float Utils::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

float Utils::AngleRadian(const sf::Vector2f& vec)
{
	return std::atan2(vec.y, vec.x);
}

float Utils::Angle(const sf::Vector2f& vec)
{
	return RadianToDegree(AngleRadian(vec));
}

float Utils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

bool Utils::CheckCollision(const sf::Sprite& shapeA, const sf::Sprite& shapeB)
{
	auto pointsA = GetShapePoints(shapeA);
	auto pointsB = GetShapePoints(shapeB);
	return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::LineIntersect(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2, sf::Vector2f& result)
{
	// t = ((x1-x3)(y3-y4)-(y1-y3)(x3-x4))/((x1-x2)(y3-y4)-(y1-y2)(x3-x4))
	// y = -((x1-x2)(y1-y3)-(y1-y2)(x1-x3))/((x1-x2)(y3-y4)-(y1-y2)(x3-x4))
	float det = ((a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x));
	if (det == 0.f)
	{
		return false;
	}
	float t = ((a1.x - b1.x) * (b1.y - b2.y) - (a1.y - b1.y) * (b1.x - b2.x))
		/ det;
	float u = -((a1.x - a2.x) * (a1.y - b1.y) - (a1.y - a2.y) * (a1.x - b1.x))
		/ det;
	if (t < 0.f || t > 1.f || u < 0.f || u > 1.f)
	{
		result.x = 0.f;
		result.y = 0.f;
		return false;
	}
	result.x = a1.x + t * (a2.x - a1.x);
	result.y = a1.y + t * (a2.y - a1.y);
	return true;
}

sf::Vector2f Utils::GetCenter(const sf::FloatRect& rect)
{
	return rect.getPosition()+ rect.getSize()*0.5f;
}

bool Utils::CheckCollision(const HitBox& boxA, const HitBox& boxB)
{
	return CheckCollision(boxA.rect, boxB.rect);
}

/// <summary>
/// collisionstate를 미리 할당한 후 넣을 것
/// </summary>
/// <param name="boxA">충돌 주체(플레이어 등)</param>
/// <param name="boxB">충돌 대상</param>
/// <param name="collisionState">충돌 주체 기준 충돌 상태</param>
/// <returns></returns>
bool Utils::CheckCollision(const HitBox& boxA, const HitBox& boxB, CollisionState& collisionState)
{
	return CheckCollision(boxA.rect, boxB.rect, collisionState);
}

bool Utils::CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB)
{
	auto pointsA = GetShapePoints(shapeA);
	auto pointsB = GetShapePoints(shapeB);
	return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB, CollisionState& collisionState)
{
	auto pointsA = GetShapePoints(shapeA);
	auto pointsB = GetShapePoints(shapeB);
	sf::Transform aTrans = shapeA.getTransform();
	sf::Transform bTrans = shapeB.getTransform();

	bool collided = PolygonsIntersect(pointsA, aTrans, pointsB, bTrans);

	if (collided)
	{
		if ((shapeA.getRotation() == 0.f || shapeA.getRotation() == 360.f)
			&& (shapeB.getRotation() == 0.f || shapeB.getRotation() == 360.f))
		{
			sf::FloatRect player = shapeA.getGlobalBounds();
			sf::FloatRect target = shapeB.getGlobalBounds();
		
			float playerright = player.left + player.width;
			float playerbottom = player.top + player.height;
			float targetright = target.left + target.width;
			float targetbottom = target.top + target.height;
		
			collisionState.area.left = std::max(player.left, target.left);
			collisionState.area.top = std::max(player.top, target.top);
			collisionState.area.width = std::min(playerright, targetright) - collisionState.area.left;
			collisionState.area.height = std::min(playerbottom, targetbottom) - collisionState.area.top;
		
			if (targetbottom > player.top
				&& playerbottom > targetbottom)
				collisionState.Up = true;
			if (targetright > player.left
				&& playerright > targetright)
				collisionState.Left = true;
			if (playerright > target.left
				&& target.left > player.left)
				collisionState.Right = true;
			if (playerbottom > target.top
				&& target.top > player.top)
				collisionState.Down = true;
		}
		else
		{
			//01
			//32
			sf::FloatRect player = shapeA.getLocalBounds();
			sf::FloatRect target = shapeB.getLocalBounds();
			std::vector<sf::Vector2f> pointsBatA;
			sf::Transform aInverse = aTrans.getInverse();
			sf::Transform bInverse = bTrans.getInverse();
			std::vector<bool> containsa;
			sf::VertexArray va;
			sf::Vector2f intersectPoint;
			for (auto& pointA : pointsA)
			{
				sf::Vector2f point = bInverse.transformPoint(aTrans.transformPoint(pointA));
				bool contain = target.contains(point);
				containsa.push_back(contain);
				if (contain)
				{
					va.append(pointA);
				}
			}
			for (auto& pointB : pointsB)
			{
				sf::Vector2f point = aInverse.transformPoint(bTrans.transformPoint(pointB));
				pointsBatA.push_back(point);
				if (player.contains(point))
				{
					va.append(point);
				}
			}
			collisionState.Up = (containsa[0] || containsa[1]) && !(containsa[2] || containsa[3]);
			collisionState.Down = !(containsa[0] || containsa[1]) && (containsa[2] || containsa[3]);
			collisionState.Left = (containsa[0] || containsa[3]) && !(containsa[1] || containsa[2]);
			collisionState.Right = !(containsa[0] || containsa[3]) && (containsa[1] || containsa[2]);

			int aSize = pointsA.size();
			int bSize = pointsBatA.size();
			for (int i = 0; i < aSize; ++i)
			{
				for (int j = 0; j < bSize; ++j)
				{
					if (LineIntersect(pointsA[i], pointsA[(i + 1) % aSize], pointsBatA[j], pointsBatA[(j + 1) % bSize], intersectPoint))
					{
						va.append(intersectPoint);
					}
				}
			}

			//sf::FloatRect ar = va.getBounds();
			//collisionState.area = aTrans.transformRect(ar);

			for (int i = 0; i < va.getVertexCount(); ++i)
			{
				va[i].position = aTrans.transformPoint(va[i].position);
			}
			sf::FloatRect ar = va.getBounds();
			collisionState.area = ar;
		}
	}

	return collided;
}

bool Utils::PointInTransformBounds(const sf::Transformable& transformable, const sf::FloatRect& localBounds, const sf::Vector2f point)
{
	sf::Transform inverse = transformable.getInverseTransform();
	sf::Vector2f localPoint = inverse.transformPoint(point);
	return localBounds.contains(localPoint);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::RectangleShape& shape)
{
	sf::FloatRect localBounds = shape.getLocalBounds();
	return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::Sprite& shape)
{
	sf::FloatRect localBounds = shape.getLocalBounds();
	return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetRectanglePointsFromBounds(const sf::FloatRect& localBounds)
{
	std::vector<sf::Vector2f> points(4);
	points[0] = sf::Vector2f(localBounds.left, localBounds.top);
	points[1] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top);
	points[2] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top + localBounds.height);
	points[3] = sf::Vector2f(localBounds.left, localBounds.top + localBounds.height);
	return points;
}

bool Utils::PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA, const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB)
{
	std::vector<sf::Vector2f> axes;
	int countA = polygonA.size();
	for (int i = 0; i < countA; ++i)
	{
		sf::Vector2f p1 = transformA.transformPoint(polygonA[i]);
		sf::Vector2f p2 = transformA.transformPoint(polygonA[(i + 1) % countA]);
		sf::Vector2f edge = p2 - p1;
		sf::Vector2f normal(-edge.y, edge.x);
		axes.push_back(Utils::GetNormal(normal));
	}

	int countB = polygonB.size();
	for (int i = 0; i < countB; ++i)
	{
		sf::Vector2f p1 = transformB.transformPoint(polygonB[i]);
		sf::Vector2f p2 = transformB.transformPoint(polygonB[(i + 1) % countB]);
		sf::Vector2f edge = p2 - p1;
		sf::Vector2f normal(-edge.y, edge.x);
		axes.push_back(Utils::GetNormal(normal));
	}

	for (const auto& axis : axes)
	{
		float minA = std::numeric_limits<float>::max();
		float maxA = std::numeric_limits<float>::lowest();
		for (const auto& point : polygonA)
		{
			sf::Vector2f transformedPoint = transformA.transformPoint(point);
			float projection = Dot(axis, transformedPoint);
			minA = std::min(minA, projection);
			maxA = std::max(maxA, projection);
		}

		float minB = std::numeric_limits<float>::max();
		float maxB = std::numeric_limits<float>::lowest();
		for (const auto& point : polygonB)
		{
			sf::Vector2f transformedPoint = transformB.transformPoint(point);
			float projection = Dot(axis, transformedPoint);
			minB = std::min(minB, projection);
			maxB = std::max(maxB, projection);
		}

		if (maxA < minB || maxB < minA)
		{
			return false;
		}
	}
	return true;
}
