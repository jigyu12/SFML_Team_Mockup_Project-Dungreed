#include "stdafx.h"
#include "Framework.h"
#include "SpriteGo.h"

void Framework::Init(int width, int height, const std::string& name)
{
    window.create(sf::VideoMode(width, height), name);
    window.setFramerateLimit(100);
    DATATABLE_MGR.Init();
    Utils::Init();
    SOUND_MGR.Init();
    SCENE_MGR.Init();
    InputMgr::Init();
    ROOM_MGR.Init();
}

void Framework::Do()
{
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        realDeltaTime = deltaTime = dt.asSeconds();
        deltaTime *= timeScale;
        realTime += realDeltaTime;
        time += deltaTime;

        InputMgr::Clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            InputMgr::UpdateEvent(event);
        }
        InputMgr::Update(deltaTime);
        
        if (InputMgr::GetKeyDown(sf::Keyboard::F10))
        {
            Variables::isDrawHitBox = !Variables::isDrawHitBox;
        }
        SCENE_MGR.Update(deltaTime);
        SCENE_MGR.LateUpdate(deltaTime);

        SCENE_MGR.FixedUpdate(deltaTime);

        SOUND_MGR.Update(deltaTime);

        window.clear();
        SCENE_MGR.Draw(window);
        window.display();
    }
}

void Framework::Release()
{
    SCENE_MGR.Release();
    FONT_MGR.Unload("fonts/DS-DIGI.ttf");
    DATATABLE_MGR.Release();
    SOUND_MGR.Release();
}