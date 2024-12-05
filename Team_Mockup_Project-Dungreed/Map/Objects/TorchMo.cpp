#include "stdafx.h"
#include "TorchMo.h"
#include "LightGo.h"

TorchMo::TorchMo(const std::string& name)
	: MapObject(name)
{
}

void TorchMo::SetActive(bool active)
{
	GameObject::SetActive(active);
	if (light != nullptr)
	{
		light->SetActive(active);
	}
}

void TorchMo::Release()
{
	MapObject::Release();
}

void TorchMo::SetPosition(const sf::Vector2f& pos)
{
	MapObject::SetPosition(pos);
	if (light != nullptr)
	{
		light->SetPosition(position);
	}
}

void TorchMo::SetRotation(float angle)
{
	MapObject::SetRotation(angle);
	if (light != nullptr)
	{
		light->SetRotation(angle);
	}
}

void TorchMo::SetScale(const sf::Vector2f& scale)
{
	MapObject::SetScale(scale);
	if (light != nullptr)
	{
		light->SetScale(scale);
	}
}

void TorchMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);
	animator.ClearEvent();
	animator.Play(RESOURCEID_TABLE->Get("Animation", "Torch"));
	animator.AddEvent("torchidle", 0, [this]() {SetOrigin(originPreset);});
	animator.AddEvent("torchidle", 2, [this]() {SetOrigin(originPreset);});
	animator.AddEvent("torchidle", 4, [this]() {SetOrigin(originPreset);});
	animator.AddEvent("torchidle", 5, [this]() {SetOrigin(originPreset);});
	animator.AddEvent("torchidle", 6, [this]() {SetOrigin(originPreset);});
	animator.AddEvent("torchidle", 7, [this]() {SetOrigin(originPreset);});
	SetOrigin(Origins::BC);
	light = SCENE_MGR.GetCurrentScene()->AddGo(new LightGo());
	light->Init();
	light->Reset();
	light->SetOrigin(Origins::MC);
	light->SetOrigin(light->GetOrigin() + sf::Vector2f(0.f, 13.f));
}
