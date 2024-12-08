#include "stdafx.h"
#include "SkellBossUi.h"
#include "SkellBoss.h"
#include "Room.h"

SkellBossUi::SkellBossUi(const std::string& name)
	:GameObject(name)
{
}

void SkellBossUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	SetPosition(position);
}

void SkellBossUi::SetRotation(float angle)
{
	rotation = angle;
	SetRotation(rotation);
}

void SkellBossUi::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	SetScale(this->scale);
}

void SkellBossUi::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void SkellBossUi::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void SkellBossUi::Init()
{
	sortingLayer = SortingLayers::UI;






}

void SkellBossUi::Release()
{
}

void SkellBossUi::Reset()
{
	skellBoss = dynamic_cast<SkellBoss*>(SCENE_MGR.GetCurrentScene()->FindGo("SkellBoss"));
	skellBossHp.setTexture(TEXTURE_MGR.Get("graphics/ui/skellbossHpbar.png"));
	bossHpBarBack.setTexture(TEXTURE_MGR.Get("graphics/ui/BossHpBarBack.png"));
	redBar.setTexture(TEXTURE_MGR.Get("graphics/ui/LifeBar.png"));

	sf::Vector2f pos = FRAMEWORK.GetWindowSizeF();
	float BossHpXpos = pos.x / 2;
	float BossHpYpos = pos.y / 2 + 480;

	Utils::SetOrigin(skellBossHp, Origins::MC);
	skellBossHp.setPosition({ BossHpXpos,BossHpYpos });

	Utils::SetOrigin(bossHpBarBack, Origins::MC);
	bossHpBarBack.setScale({ 1.06f,0.85 });
	bossHpBarBack.setPosition({ BossHpXpos,BossHpYpos });

	Utils::SetOrigin(redBar, Origins::ML);
	redBar.setScale({ 800.f,8.f });
float sssss=	BossHpXpos + 77 - redBar.getGlobalBounds().width * 0.5f;
	redBar.setPosition({ BossHpXpos+77-redBar.getGlobalBounds().width*0.5f,BossHpYpos});

	isBgmPlaying = false;
}

void SkellBossUi::Update(float dt)
{
	skellBoss = nullptr;
	const auto& currentRoomMonsters = ROOM_MGR.GetCurrentRoom()->GetMonsters();
	for (const auto& monster : currentRoomMonsters)
	{
		if (monster->GetName() == "SkellBoss")
		{
			skellBoss = dynamic_cast<SkellBoss*>(monster);
			break;
		}
	}
	if (skellBoss != nullptr)
	{
		if (!isBgmPlaying)
		{
			isBgmPlaying = true;

			SOUND_MGR.PlaySfx("sound/Sfx/boss/Resources_Audio_beliallaugh_rev.mp3");
			SOUND_MGR.PlayBgm("sound/Bgm/1.JailBoss.wav");
		}

		redBar.setScale(800 * skellBoss->GetCurrentHpRatio(), redBar.getScale().y);
	}
	else
	{
		if (isBgmPlaying)
		{
			isBgmPlaying = false;

			SOUND_MGR.PlayBgm(""); 
		}
	}
}

void SkellBossUi::Draw(sf::RenderWindow& window)
{
	if (skellBoss != nullptr)
	{
		window.draw(bossHpBarBack);
		window.draw(redBar);
		window.draw(skellBossHp);

	}
}
