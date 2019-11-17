#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "Entity.h"

struct SDL_Texture;

class j1EntityManager : public j1Module
{
public:

	p2List<Entity*> entities;

public:

	Entity* CreateEntity(entityType type, int x, int y);
	Entity* EntityFactory();
	void DestroyEntity(Entity* entity);
	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Entity* getPlayer() const;

public:
	pugi::xml_node	config;
	float			accumulated_time;	//Accumulates dt as time goes on.
	float			cycle_length;		//How much time needs to pass / be accumulated before running a cycle. 
	bool			doLogic;			//Keeps track  whether or not the entity needs to do it's logic (pathfinding...)
};

#endif // !__j1ENTITY_MANAGER_H__