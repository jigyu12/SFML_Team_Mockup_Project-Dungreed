#include "stdafx.h"
#include "BreakableMo.h"
#include "ParticleGo.h"
#include "SceneGame.h"

BreakableMo::BreakableMo(const std::string& name)
	: MapObject(name)
{
}

void BreakableMo::Update(float dt)
{
	hitbox.UpdateTr(body, body.getLocalBounds());
	hitbox.rect.setOutlineColor(sf::Color::Red);
}

void BreakableMo::Draw(sf::RenderWindow& window)
{
	MapObject::Draw(window);
	hitbox.Draw(window);
}

void BreakableMo::Set(const ObjectData::Type& type)
{
	MapObject::Set(type);

	switch (this->type)
	{
	case ObjectData::Type::Box:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BoxIcon")));
		break;
	case ObjectData::Type::BigBox:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BigBoxIcon")));
		break;
	case ObjectData::Type::OakDrum:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "OakDrumIcon")));
		break;
	case ObjectData::Type::Table:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "TableIcon")));
		break;
	case ObjectData::Type::SkullTable:
		body.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "SkullTableIcon")));
		break;
	}
	SetOrigin(Origins::BC);
}

void BreakableMo::SetStatus(const Status& status)
{
	MapObject::SetStatus(status);
	SetOrigin(Origins::BC);
}

void BreakableMo::OnDamaged(int damage)
{
	SetActive(false);
	SetStatus(Status::Broken);

	SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (scene == nullptr)
		return;
	sf::Vector2f pos = Utils::GetCenter(body, body.getLocalBounds());
	int particlecount = Utils::RandomRange(2, 4);
	switch (type)
	{
	case ObjectData::Type::Box:
		for (int i = 0; i < particlecount;++i)
		{
			int rand = Utils::RandomRange(0, 6);
			ParticleGo* particle = scene->TakeObjectParticle();

			particle->Start("BoxParticle" + std::to_string(rand), pos);
		}
		break;
	case ObjectData::Type::BigBox:
		for (int i = 0; i < particlecount + 1;++i)
		{
			int rand = Utils::RandomRange(0, 6);
			ParticleGo* particle = scene->TakeObjectParticle();

			particle->Start("BigBoxParticle" + std::to_string(rand), pos);
		}
		break;
	case ObjectData::Type::OakDrum:
		for (int i = 0; i < particlecount;++i)
		{
			int rand = Utils::RandomRange(0, 4);
			ParticleGo* particle = scene->TakeObjectParticle();

			particle->Start("OakDrumParticle" + std::to_string(rand), pos);
		}
		break;
	case ObjectData::Type::SkullTable:
		for (int i = 0; i < particlecount;++i)
		{
			int rand = Utils::RandomRange(0, 7);
			ParticleGo* particle = scene->TakeObjectParticle();

			particle->Start("TableParticle" + std::to_string(rand), pos);
		}
		break;
	case ObjectData::Type::Table:
		for (int i = 0; i < particlecount;++i)
		{
			int rand = Utils::RandomRange(2, 7);
			ParticleGo* particle = scene->TakeObjectParticle();

			particle->Start("TableParticle" + std::to_string(rand), pos);
		}
		break;
	}
}
