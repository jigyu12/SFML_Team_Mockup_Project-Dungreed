#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	: name(name), originPreset(Origins::TL), scale(1.f, 1.f)
{

}

void GameObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = { 0.f, 0.f };
}
