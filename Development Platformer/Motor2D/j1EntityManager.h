#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__

#include "j1Module.h"
#include "j1Entity.h"		
#include "p2List.h"

struct SDL_Texture;
class j1Entity;
class j1Player;
class j1Player1;			//Temporal(?)
class j1Player2;

class j1EntityManager : public j1Module
{
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
	j1Entity* CreateEntity(ENTITY_TYPE type, int x = 0, int y = 0);
	void CreatePlayers();
	void SpawnEnemy();
	void DestroyEntity(j1Entity* entity);
	
	void OnCollision(Collider* C1, Collider* C2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* GetPlayer() const;

public:
	pugi::xml_node	config;
	
	p2List<j1Entity*>	entities;			//List of entities. Each created entity will be added to this list and then iterated in the class methods (Update()...)
	j1Player1*			player;				//Pointer to the j1Player1 class. Will represent P1. Change name to player1 later.
	j1Player2*			player2;			//Pointer to the j1Player1 class. Will represent P2.
	//j1Mecha*			mecha;
	//j1Alien*			alien;

	float				accumulated_time;	//Accumulates dt as time goes on.
	float				cycle_length;		//How much time needs to pass / be accumulated before running a cycle. 
	bool				doLogic;			//Keeps track whether or not the entity needs to do it's logic (pathfinding...)
};

#endif // !__j1ENTITY_MANAGER_H__