#include "stdafx.h"
#include "InputMgr.h"

std::vector<std::bitset<3>> InputMgr::vecInput(sf::Keyboard::KeyCount + sf::Mouse::ButtonCount, 0);
sf::Vector2i InputMgr::mousePosition;
std::unordered_map<Axis, AxisInfo> InputMgr::axisInfoMap;

void InputMgr::Init()
{
	 // Horizontal
    AxisInfo infoH;
    infoH.axis = Axis::Horizontal;
    infoH.AddKey(true, sf::Keyboard::D);
	infoH.AddKey(true, sf::Keyboard::Right);
	infoH.AddKey(false, sf::Keyboard::A);
	infoH.AddKey(false, sf::Keyboard::Left);
	axisInfoMap.insert({ infoH.axis, infoH });

    // Vertical
    AxisInfo infoV;
    infoV.axis = Axis::Vertical;
    infoV.AddKey(true, sf::Keyboard::S);
	infoV.AddKey(true, sf::Keyboard::Down);
	infoV.AddKey(false, sf::Keyboard::W);
	infoV.AddKey(false, sf::Keyboard::Up);
	axisInfoMap.insert({ infoV.axis, infoV });
}

void InputMgr::Clear()
{
	std::bitset<3> remover = 0x01 << (int)State::Pressing;

	for (auto& it : vecInput)
	{
		it &= remover;
	}
}

void InputMgr::UpdateEvent(const sf::Event& ev)
{
	int idx = 0;
	std::bitset<3> setter = 0;

	switch (ev.type)
	{
	case sf::Event::KeyPressed:
		idx = ev.key.code;
		if (idx < 0 || idx >= sf::Keyboard::KeyCount)
		{
			return;
		}
		setter[(int)State::Down] = !vecInput[idx][(int)State::Pressing];
		setter[(int)State::Pressing] = true;
		break;
	case sf::Event::KeyReleased:
		idx = ev.key.code;
		if (idx < 0 || idx >= sf::Keyboard::KeyCount)
		{
			return;
		}
		setter[(int)State::Release] = true;
		break;
	case sf::Event::MouseButtonPressed:
		idx = sf::Keyboard::KeyCount + ev.mouseButton.button;
		setter[(int)State::Down] = !vecInput[idx][(int)State::Pressing];
		setter[(int)State::Pressing] = true;
		break;
	case sf::Event::MouseButtonReleased:
		idx = sf::Keyboard::KeyCount + ev.mouseButton.button;
		setter[(int)State::Release] = true;
		break;
	case sf::Event::MouseMoved:
		mousePosition.x = ev.mouseMove.x;
		mousePosition.y = ev.mouseMove.y;
		return;
	default:
		return;
	}
	vecInput[idx] = setter;
}

void InputMgr::Update(float dt)
{
	 for (auto& pair : axisInfoMap)
    {
        auto& axisInfo = pair.second;
        float raw = GetAxisRaw(axisInfo.axis);
        float dir = raw;
        if (dir == 0.f && axisInfo.value != 0.f)
        {
            dir = axisInfo.value > 0.f ? -1.f : 1.f;
        }

        axisInfo.value += dir * axisInfo.sensi * dt;
        axisInfo.value = Utils::Clamp(axisInfo.value, -1.f, 1.f);
        
        float stopThreshold = std::abs(dir * axisInfo.sensi * dt);
        if (raw == 0.f && std::abs(axisInfo.value) < stopThreshold)
        {
            axisInfo.value = 0.f;
        }
    }
}

bool InputMgr::GetKeyDown(const sf::Keyboard::Key& key)
{
	return vecInput[key][(int)State::Down];
}

bool InputMgr::GetKey(const sf::Keyboard::Key& key)
{
	return vecInput[key][(int)State::Pressing];
}

bool InputMgr::GetKeyUp(const sf::Keyboard::Key& key)
{
	return vecInput[key][(int)State::Release];
}

bool InputMgr::GetMouseButtonDown(const sf::Mouse::Button& button)
{
	return vecInput[sf::Keyboard::KeyCount + button][(int)State::Down];
}

bool InputMgr::GetMouseButton(const sf::Mouse::Button& button)
{
	return vecInput[sf::Keyboard::KeyCount + button][(int)State::Pressing];
}

bool InputMgr::GetMouseButtonUp(const sf::Mouse::Button& button)
{
	return vecInput[sf::Keyboard::KeyCount + button][(int)State::Release];
}

float InputMgr::GetAxisRaw(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
    if (findIt == axisInfoMap.end())
        return 0.f;

	auto& axisInfo = findIt->second;

	float axisvalue = 0.f;

	auto itInt = axisInfo.positives.begin();
	while (itInt != axisInfo.positives.end())
	{
		if (GetKey((sf::Keyboard::Key)*itInt))
		{
			axisvalue += 1.f;
			break;
		}
		++itInt;
	}
	itInt = axisInfo.negatives.begin();
	while (itInt != axisInfo.negatives.end())
	{
		if (GetKey((sf::Keyboard::Key)*itInt))
		{
			axisvalue -= 1.f;
			break;
		}

		++itInt;
	}

	return axisvalue;
}

float InputMgr::GetAxis(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
    if (findIt == axisInfoMap.end())
        return 0.f;
    return findIt->second.value;
}
