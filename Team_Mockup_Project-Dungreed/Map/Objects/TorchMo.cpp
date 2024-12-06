#include "stdafx.h"
#include "TorchMo.h"
#include "LightGo.h"
#include "SceneGame.h"

TorchMo::TorchMo(const std::string& name)
	: MapObject(name)
{
}

void TorchMo::SetActive(bool active)
{
	GameObject::SetActive(active);
	if (active && light == nullptr)
	{
		SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		if (scene == nullptr)
		{
			return;
		}
		light = scene->TakeObjectLight();
		light->Init();
		light->Reset();
		light->SetOrigin(Origins::MC);
		light->SetOrigin(light->GetOrigin() + sf::Vector2f(0.f, 13.f));
		SetPosition(position);
	}
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

}
