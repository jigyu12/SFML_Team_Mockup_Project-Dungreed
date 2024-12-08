#pragma once

#include "Scene.h"

class Player;
class Room;
class Bat;
class SkeletonDog;
class Weapon;
class SkellBoss;
class SkellBossLeftHand;
class ParticleGo;
class LightGo;
class WorldMapUi;
class UiAbility;
class PortalEffect;

class SceneGame : public Scene
{
protected:
	Player* player;
	std::vector<Weapon*> weaponList;

	WorldMapUi* worldMapUi;
	UiAbility* uiAbility;

	std::list<ParticleGo*> particles;
	ObjectPool<ParticleGo> particlePool;

	std::list<LightGo*> lights;
	ObjectPool<LightGo> lightPool;

	std::list<PortalEffect*> portalEffects;
	ObjectPool<PortalEffect> portalEffectPool;

	sf::Sprite mouseCursor;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	ParticleGo* TakeObjectParticle();
	void ReturnObjectParticle(ParticleGo* particle);

	LightGo* TakeObjectLight();
	void ReturnObjectLight(LightGo* light);

	PortalEffect* TakeObjectPortalEffect();
	void ReturnObjectPortalEffect(PortalEffect* portalEffect);

	void ClearTookObject();
};

