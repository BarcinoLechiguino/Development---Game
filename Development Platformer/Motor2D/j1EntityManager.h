#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__

#include "j1Module.h"
#include "j1Entity.h"		
#include "p2List.h"

struct SDL_Texture;
class j1Entity;
class j1Player;

class j1EntityManager : public j1Module
{
public:

	p2List<j1Entity*> entities;

public:
	j1EntityManager();

	~j1EntityManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	j1Entity* CreateEntity(int x, int y, ENTITY_TYPE type);
	void CreatePlayer();
	void SpawnEnemy();
	void DestroyEntity(j1Entity* entity);
	
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* GetPlayer() const;

public:
	pugi::xml_node	config;
	
	j1Player* player = NULL;
	
	float			accumulated_time;	//Accumulates dt as time goes on.
	float			cycle_length;		//How much time needs to pass / be accumulated before running a cycle. 
	bool			doLogic;			//Keeps track whether or not the entity needs to do it's logic (pathfinding...)
};

#endif // !__j1ENTITY_MANAGER_H__