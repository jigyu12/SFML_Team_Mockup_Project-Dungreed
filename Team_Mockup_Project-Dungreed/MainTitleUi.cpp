#include "stdafx.h"
#include "MainTitleUi.h"

MainTitleUi::MainTitleUi(const std::string& name)
	:GameObject(name)
{
}

void MainTitleUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	SetPosition(position);
}

void MainTitleUi::SetRotation(float angle)
{
	rotation = angle;
	SetRotation(rotation);
}

void MainTitleUi::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
	SetScale(this->scale);
}

void MainTitleUi::SetOrigin(Origins preset)
{
	if (originPreset != Origins::Custom)
	{
		originPreset = preset;
	}
}

void MainTitleUi::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void MainTitleUi::Init()
{
	sortingLayer = SortingLayers::UI;
	SetStatus(MainTitleUi::Staus::Aniplaytime);
}

void MainTitleUi::Release()
{

}

void MainTitleUi::Reset()
{
	animator.SetTarget(&mainbody);
	//
	mainbody.setPosition({ 1920.f / 2,1080.f / 2 });
	mainbody.setScale({ 3.f,3.f });
	animator.Play("animations/MainTitle.csv");
	Utils::SetOrigin(mainbody, Origins::MC);
	//
	backCloud.setTexture(TEXTURE_MGR.Get("graphics/maintitle/BackCloud.png"));
	frontCloud.setTexture(TEXTURE_MGR.Get("graphics/maintitle/FrontCloud.png"));
	//
	exitButton.setTexture(TEXTURE_MGR.Get("graphics/maintitle/ExitOff.png"));
	playButton.setTexture(TEXTURE_MGR.Get("graphics/maintitle/PlayOff.png"));
	//
	mainLogo.setTexture(TEXTURE_MGR.Get("graphics/maintitle/MainLogo.png"));


	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	float sizeX = size.x / 2;
	float sizeY = size.y / 3;
	mainLogo.setPosition({ sizeX ,sizeY });
	mainLogo.setScale({ 5.f,5.f });
	Utils::SetOrigin(mainLogo, Origins::MC);

	backCloud.setPosition(0.f, 0.f);
	backCloud.setScale(6.f, 6.f);
	Utils::SetOrigin(backCloud, Origins::TL);

	frontCloud.setPosition(0.f, 0.f);
	frontCloud.setScale(6.f, 6.f);
	Utils::SetOrigin(frontCloud, Origins::TL);
	SetPosition(position);

}

void MainTitleUi::AniplayertimeUpdate(float dt)
{
}

void MainTitleUi::AniStopTimeUpdate(float dt)
{
	position.x += backCloudSpeed * dt;
	backCloud.setPosition(position);
}

void MainTitleUi::Update(float dt)
{
	animator.Update(dt);

	aniTimer += dt;

	switch (status)
	{
	case MainTitleUi::Staus::Aniplaytime:
		AniplayertimeUpdate(dt);
		break;
	case MainTitleUi::Staus::AniStoptime:
		AniStopTimeUpdate(dt);
		break;
	default:
		break;
	}

	if (aniTimer > 3.f)
	{
		SetStatus(MainTitleUi::Staus::AniStoptime);
	}

}

void MainTitleUi::Draw(sf::RenderWindow& window)
{

	switch (status)
	{
	case MainTitleUi::Staus::Aniplaytime:
		window.draw(mainbody);
		break;
	case MainTitleUi::Staus::AniStoptime:
		window.draw(frontCloud);
		window.draw(backCloud);
		window.draw(mainLogo);
		
		break;
	default:
		break;
	}
}


void MainTitleUi::SetStatus(Staus status)
{
	this->status = (status);
}


