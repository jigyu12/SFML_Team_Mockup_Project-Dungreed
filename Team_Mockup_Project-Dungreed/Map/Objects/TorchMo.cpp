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
	SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (scene == nullptr)
	{
		return;
	}
	if (active && light == nullptr)
	{
		light = scene->TakeObjectLight();
		light->Init();
		light->Reset();
		light->SetOrigin(Origins::MC);
		light->SetOrigin(light->GetOrigin() + sf::Vector2f(0.f, 13.f));
		SetPosition(position);
	}
	else if (!active && light != nullptr)
	{
		scene->ReturnObjectLight(light);
		light = nullptr;
	}
}

void TorchMo::Draw(sf::RenderWindow& window)
{
	switch (status)
	{
	case Status::Idle:
		window.draw(body);
		break;
	case Status::Broken:
		window.draw(body, rs);
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

void TorchMo::SetStatus(const Status& status)
{
	MapObject::SetStatus(status);
	switch (status)
	{
	case Status::Broken:
		shader.loadFromFile(RESOURCEID_TABLE->Get("Shader", "ColorThreshold"), sf::Shader::Fragment);
		shader.setUniform("th_g", 0.1f);
		shader.setUniform("th_b", 0.8f);
		rs.shader = &shader;
		if (light != nullptr)
		{
			light->SetCenterColor({ 50,10,50,0 });
		}
		break;
	}
}
