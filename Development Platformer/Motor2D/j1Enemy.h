#ifndef __j1Enemy_H__
#define __j1Enemy_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

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

	void OnCollision(Collider* c1, Collider* c2);

public:
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Normal_Path();							//Land Path?
	void Chasing_Path();						//Aerial Path?
	bool Calculate_Path();			
	//bool Load_Entity();
	void LoadAnimationPushbacks();				//Loads a set of animation pushbacks.
	void LoadEntityProperties();				//Loads an entity's specific properties.
	void LoadEntityAudio();						//Loads an entity's specific audios/sfx.
	void InitEnemy();							//Initializes an enemy's specific data members.

public:
	fPoint			spawn_position;
	Entity_State	state;
	SDL_Rect		enemy_HitBox;

	bool grounded = false;
	bool airborne = false;
	bool flip = false;
	bool isGoingRight = false;
	bool isGoingLeft = false;
	bool fading = false;
	bool isAlive = true;
	bool isDying = false;
	bool againstRightWall = false;
	bool againstLeftWall = false;

	p2DynArray<iPoint> entityPath;		//Only for enemies

public:
	pugi::xml_document	config_file;	//
	pugi::xml_node		enemy_entity;	//
	
	Animation	idle;					//Idle animation.
	Animation	running;				//Running animation.
};

#endif // __j1ENEMY_H__