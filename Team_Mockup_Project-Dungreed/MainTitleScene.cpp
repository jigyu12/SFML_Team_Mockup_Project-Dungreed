#include "stdafx.h"
#include "MainTitleScene.h"
#include "MainTitleUi.h"

MainTitleScene::MainTitleScene()
	:Scene(SceneIds::MainTitle)
{
}

void MainTitleScene::Init()
{
	mainTitleui = AddGo(new MainTitleUi());

	Scene::Init();

	SOUND_MGR.PlaySfx("sound/Bgm/IntroSound.wav");
}

void MainTitleScene::Release()
{
	Scene::Release();
}

void MainTitleScene::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	size.x /= 6.f;
	size.y /= 6.f;

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);

	mouseCursor.setTexture(TEXTURE_MGR.Get(RESOURCEID_TABLE->Get("Graphic", "BasicCursor")));
	mouseCursor.setScale(4.f, 4.f);
	Utils::SetOrigin(mouseCursor, Origins::MC);
}

void MainTitleScene::Exit()
{
	Scene::Exit();
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
}

void MainTitleScene::Update(float dt)
{
	Scene::Update(dt);

	mouseCursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));
}

void MainTitleScene::Draw(sf::RenderWindow& window)
{
	if (mainTitleui->GetStatus() == MainTitleUi::Status::AniStoptime)
	{
		window.clear({ 121,185,255 });
	}
	
	Scene::Draw(window);

	window.draw(mouseCursor);
}