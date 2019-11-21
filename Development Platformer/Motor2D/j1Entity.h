#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"

class Animation;


struct Collider;
struct SDL_Texture;

enum entity_state	//Does not go here. Is it necessary?
{
	IDLE = 0,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DEAD,
	HURT
};

enum class ENTITY_TYPE
{
	UNKNOWN_TYPE,
	PLAYER,
	PLAYER2,
	MECHA,
	ALIEN,
};

struct EntityData
{
	ENTITY_TYPE type;
	iPoint position;
};

class j1Entity : public j1Module
{
public:
	j1Entity(int x, int y, ENTITY_TYPE type); // here we will set initial position

	//Must be virtual so when an entity is executed the compiler goes to the entity's methods instead of these
	virtual bool Awake(pugi::xml_node&);

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update(float dt, bool doLogic);

	virtual bool PostUpdate();

	virtual bool CleanUp();

public:
	//Entity Methods
	virtual bool LoadAnimationPushbacks();
	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);
	virtual void Restart();								//Maybe not needed THIS HERE

	virtual void BlitEntity(int x, int y, SDL_Rect entity_rect, bool flip);
	virtual void OnCollision(Collider* c1, Collider* c2); /*{};*/				//If {} are used then the OnCollision on the entity.cpp needs to be erased.
	//bool Calculate_Path();			//Only for enemiess

	//Entity Variables
	ENTITY_TYPE		type;					//Type of the entity (ENTITY_TYPE::PLAYER...)
	fPoint			position;				//Initial position of the entity.
	iPoint			sprite_size;			//Size of the entity sprite --> w and h of the entity collider.
	float			speed;					//Movement speed of the entity.

	SDL_Texture*	entity_sprite;			//Sprite / Spritesheet of the entity.
	Collider*		collider;				//Collider of the entity.
	Animation*		animation;				//Animation of the entity.
	
	//p2DynArray<iPoint> entityPath;		//Only for enemies
};

#endif