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

class SceneGame : public Scene
{
protected:
	Player* player;
	std::vector<Weapon*> weaponList;

	SkellBoss* skellBoss;
	SkellBossLeftHand* leftHand;
	SkellBossLeftHand* rightHand;
	//std::vector<Bat*> batList;
	//std::vector<SkeletonDog*> skeletonDogList;

	std::list<ParticleGo*> particles;
	ObjectPool<ParticleGo> particlePool;

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
	void ClearTookObject();
};

