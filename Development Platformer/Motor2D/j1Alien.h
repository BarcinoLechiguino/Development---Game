#ifndef __j1ALIEN_H__
#define __j1ALIEN_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Enemy.h"
#include "Animation.h"
#include "p2Point.h"

class j1Alien : public j1Enemy
{
public:
	j1Alien(int x, int y, ENTITY_TYPE type);
	~j1Alien();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt, bool doLogic);

	bool PostUpdate();

	bool CleanUp();

public:
	void OnCollision(Collider* C1, Collider* C2);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadAnimationPushbacks();
	void LoadEntityProperties();
	void LoadEntityAudio();

	void Chasing_Path();
	void Normal_Path();

	void PathfindingLogic();
	void SetEnemyState(iPoint enemyPos, iPoint playerPos);

	//bool Load_Entity();

public:
	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.
};

#endif // __j1ALIEN_H__




