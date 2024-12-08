#include "stdafx.h"
#include "MainTitleUi.h"
#include "Button.h"
#include "BirdGo.h"

MainTitleUi::MainTitleUi(const std::string& name)
	:GameObject(name)
{
}

void MainTitleUi::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void MainTitleUi::SetRotation(float angle)
{
	rotation = angle;
}

void MainTitleUi::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
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
	startButton.Init();
	continueButton.Init();
	editorButton.Init();
	endButton.Init();


	sortingLayer = SortingLayers::UI;
	SetStatus(MainTitleUi::Status::Aniplaytime);


	birds.resize(3);
	for (int i = 0; i < birds.size(); ++i)
	{
		birds[i].Init();
	}
}

void MainTitleUi::Release()
{

}

void MainTitleUi::Reset()
{
	startButton.Reset();
	continueButton.Reset();
	editorButton.Reset();
	endButton.Reset();
	//
	sf::Vector2f buttonpos = FRAMEWORK.GetWindowSizeF();
	float mousePosX = buttonpos.x / 2;
	float startMousePosY = buttonpos.y / 2;

	startButton.SetPosition({ mousePosX ,startMousePosY+100.f });
	startButton.Set({ 400.f,100.f }, 50);
	startButton.SetString(L"게임시작");
	startButton.SetOrigin(Origins::MC);
	startButton.SetClickedEvent([this]()
		{
			SAVELOAD_MGR.Save(SaveDataVC());
			SCENE_MGR.ChangeScene(SceneIds::Game);
		});

	continueButton.SetPosition({ mousePosX ,startMousePosY + 200.f });
	continueButton.Set({ 400.f,100.f }, 50);
	continueButton.SetString(L"이어하기");
	continueButton.SetOrigin(Origins::MC);
	continueButton.SetClickedEvent([this]()
		{
			SCENE_MGR.ChangeScene(SceneIds::Game);
		});

	editorButton.SetPosition({ mousePosX ,startMousePosY + 300.f });
	editorButton.Set({ 400.f,100.f }, 50);
	editorButton.SetString(L"맵 편집");
	editorButton.SetOrigin(Origins::MC);
	editorButton.SetClickedEvent([this]()
		{
			SCENE_MGR.ChangeScene(SceneIds::MapEdit);
		});

	endButton.SetPosition({ mousePosX ,startMousePosY + 400.f });
	endButton.Set({ 400.f,100.f }, 48);
	endButton.SetString(L"종료");
	endButton.SetOrigin(Origins::MC);
	endButton.SetClickedEvent([this]()
		{
			FRAMEWORK.GetWindow().close();
		});



	animator.SetTarget(&mainbody);
	//
	mainbody.setPosition({ 1920.f / 2,1080.f / 2 });
	mainbody.setScale({ 3.f,3.f });
	animator.Play("animations/MainTitle.csv");
	Utils::SetOrigin(mainbody, Origins::MC);
	//
	backCloud.setTexture(TEXTURE_MGR.Get("graphics/maintitle/BackCloud1.png"));
	frontCloud.setTexture(TEXTURE_MGR.Get("graphics/maintitle/FrontCloud1.png"));
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
	for (int i = 0; i < birds.size(); ++i)
	{
		birds[i].Reset();
	}

	isBgmPlaying = false;
}

void MainTitleUi::AniplayertimeUpdate(float dt)
{
}

void MainTitleUi::AniStopTimeUpdate(float dt)
{
	for (int i = 0; i < birds.size(); ++i)
	{
		birds[i].Update(dt);
	}

	bCloudPos.x -= backCloudSpeed * dt;

	if (bCloudPos.x < -640.f * 6.f)
	{
		bCloudPos.x += 640.f * 6.f;

	}
	backCloud.setPosition(bCloudPos);
	fCloudPos.x -= frontCloudSpeed * dt;

	if (fCloudPos.x < -640.f * 6.f)
	{
		fCloudPos.x += 640.f * 6.f;
	}
	frontCloud.setPosition(fCloudPos);
}

void MainTitleUi::ButtonUpdate(float dt)
{
	startButton.Update(dt);
	continueButton.Update(dt);
	editorButton.Update(dt);
	endButton.Update(dt);

	startButton.SetTextOutlineColor(sf::Color::Black);
	startButton.SetTextOutlineThickness(3.f);

	continueButton.SetTextOutlineColor(sf::Color::Black);
	continueButton.SetTextOutlineThickness(3.f);

	editorButton.SetTextOutlineColor(sf::Color::Black);
	editorButton.SetTextOutlineThickness(3.f);

	endButton.SetTextOutlineColor(sf::Color::Black);
	endButton.SetTextOutlineThickness(3.f);

	if (startButton.isMouseOn())
	{
		startButton.SetTextFillColor(sf::Color::White);
	}
	else
	{
		startButton.SetTextFillColor({145,155,185});
	}
	startButton.SetButtonFillColor(sf::Color::Transparent);
	startButton.SetButtonOutlineColor(sf::Color::Transparent);

	if (continueButton.isMouseOn())
	{
		continueButton.SetTextFillColor(sf::Color::White);
	}
	else
	{
		continueButton.SetTextFillColor({145,155,185});
	}
	continueButton.SetButtonFillColor(sf::Color::Transparent);
	continueButton.SetButtonOutlineColor(sf::Color::Transparent);

	if (editorButton.isMouseOn())
	{
		editorButton.SetTextFillColor(sf::Color::White);
	}
	else
	{
		editorButton.SetTextFillColor({ 145,155,185 });
	}
	editorButton.SetButtonFillColor(sf::Color::Transparent);
	editorButton.SetButtonOutlineColor(sf::Color::Transparent);

	if (endButton.isMouseOn())
	{
		endButton.SetTextFillColor(sf::Color::White);
	}
	else
	{
		endButton.SetTextFillColor({ 145,155,185 });
	}
	endButton.SetButtonFillColor(sf::Color::Transparent);
	endButton.SetButtonOutlineColor(sf::Color::Transparent);
}

void MainTitleUi::Update(float dt)
{
	animator.Update(dt);

	ButtonUpdate(dt);


	aniTimer += dt;

	switch (status)
	{
	case MainTitleUi::Status::Aniplaytime:
		AniplayertimeUpdate(dt);
		break;
	case MainTitleUi::Status::AniStoptime:
		AniStopTimeUpdate(dt);
		break;
	default:
		break;
	}

	if (aniTimer > 4.5f)
	{
		SetStatus(MainTitleUi::Status::AniStoptime);
	}

}

void MainTitleUi::Draw(sf::RenderWindow& window)
{

	switch (status)
	{
	case MainTitleUi::Status::Aniplaytime:
		window.draw(mainbody);
		break;
	case MainTitleUi::Status::AniStoptime:
		window.draw(backCloud);
		window.draw(frontCloud);
		for (int i = 0; i < birds.size(); ++i)
		{
			birds[i].Draw(window);
		}
		window.draw(mainLogo);
		startButton.Draw(window);
		continueButton.Draw(window);
		endButton.Draw(window);
		editorButton.Draw(window);
		
		break;
	default:
		break;
	}
}


void MainTitleUi::SetStatus(Status status)
{
	this->status = (status);

	if (status == MainTitleUi::Status::AniStoptime)
	{
		if (!isBgmPlaying)
		{
			isBgmPlaying = true;
			SOUND_MGR.PlayBgm("sound/Bgm/title.wav");
		}
		
	}
}