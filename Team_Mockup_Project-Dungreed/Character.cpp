#include "stdafx.h"

Character::Character(const std::string& name)
	: hp(0), speed(0.f), velocity({0.f, 0.f}), direction({0.f, 0.f}), textureId("Invaild textureId")
{
}