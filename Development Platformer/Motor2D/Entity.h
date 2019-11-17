#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"

enum entityType
{
	UNKNOWN_TYPE,
	PLAYER1,
	PLAYER2,
	FLYING_ENEMY,
	LAND_ENEMY,
};

struct SDL_Texture;

class Entity:public j1Module
{
public:
	
	entityType type;
	iPoint position; 
	fPoint velocity; 
	SDL_Texture* graphics = nullptr;
	Collider* collider = nullptr;
	p2DynArray<iPoint> entityPath;

public:
	Entity();
	Entity(entityType type); // here we will set initial position
//	~Entity();
	virtual bool LoadEntity();
	virtual bool LoadEntityProperties(pugi::xml_node& conﬁg);
	virtual bool AddAnimationPushbacks();
	virtual bool Save(pugi::xml_node& ﬁle) const;
	virtual bool Load(pugi::xml_node& ﬁle);
	virtual void Restart();
	void Entity_OnCollision(Collider* c1, Collider* c2);

};

#endif