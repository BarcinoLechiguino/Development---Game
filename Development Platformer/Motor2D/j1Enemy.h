#ifndef __j1Enemy_H__
#define __j1Enemy_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class j1Player;

enum class Entity_State	//Does not go here. Is it necessary?
{
	IDLE = 0,
	PATHING_RIGHT,
	PATHING_LEFT,
	JUMPING,
	FALLING,
	DEAD,
	HURT
};

class j1Enemy : public j1Entity
{
public:
	j1Enemy(int x, int y, ENTITY_TYPE type);

	~j1Enemy();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Init();

	bool PreUpdate();

	bool Update(float dt,  bool doLogic);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* C1, Collider* C2);

public:
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadAnimationPushbacks();							//Loads a set of animation pushbacks.
	void LoadEntityProperties();							//Loads an entity's specific properties.
	void LoadEntityAudio();									//Loads an entity's specific audios/sfx.

	void InitEnemy();										//Initializes an enemy's specific data members.
	void Normal_Path();										//Land Path?
	void Chasing_Path();									//Aerial Path?
	bool Calculate_Path();
	//bool Load_Entity();

	float DistanceFromP1() const;							//Calculates the distance between P1 and an enemy entity.
	float DistanceFromP2() const;							//Calculates the distance between P1 and an enemy entity.
	float DistanceFromPlayer(j1Player* player) const;		//Calculates the distance between a player and an enemy entity.

public:
	Entity_State	state;
	SDL_Rect		enemy_HitBox;
	iPoint			detectionRadius;

	bool grounded;
	bool airborne;
	bool flip;
	bool isGoingRight;
	bool isGoingLeft;
	bool fading;
	bool isAlive;
	bool isDying;
	bool againstRightWall;
	bool againstLeftWall;

	p2DynArray<iPoint> entityPath;		//Only for enemies

public:
	pugi::xml_document	config_file;	//
	pugi::xml_node		enemy_entity;	//
	
	Animation	idle;					//Idle animation.
	Animation	running;				//Running animation.
};

#endif // __j1ENEMY_H__