#pragma once
#include <list>

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	std::list<int> positives;		// 1.0f
	std::list<int> negatives;		// -1.0f
	float sensi = 10.f;
	float value = 0.f;

	void AddKey(bool positive, int code) 
	{ 
		if (positive) 
			positives.push_back(code);
		else 
			negatives.push_back(code); 
	}
};


class InputMgr
{
private:
	enum class State
	{
		Release,
		Down,
		Pressing,
	};

	static std::vector<std::bitset<3>> vecInput;

	static sf::Vector2i mousePosition;

	static std::unordered_map<Axis, AxisInfo> axisInfoMap;

public:
	static void Init();
	static void Clear();

	static void UpdateEvent(const sf::Event& ev);
	static void Update(float dt);

	static bool GetKeyDown(const sf::Keyboard::Key& key);
	static bool GetKey(const sf::Keyboard::Key& key);
	static bool GetKeyUp(const sf::Keyboard::Key& key);

	static bool GetMouseButtonDown(const sf::Mouse::Button& button);
	static bool GetMouseButton(const sf::Mouse::Button& button);
	static bool GetMouseButtonUp(const sf::Mouse::Button& button);

	static sf::Vector2i GetMousePosition() { return mousePosition; }

	static float GetAxisRaw(Axis axis);	// -1.0 0 1.0
	static float GetAxis(Axis axis);	// -1.0 ~ 1.0
};

