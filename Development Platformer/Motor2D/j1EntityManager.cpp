#include "j1EntityManager.h"
#include "Alien.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Player1.h"
#include "j1Player2.h"
#include "Mecha.h"
#include "j1Window.h"
#include "Brofiler\Brofiler.h"

j1EntityManager::j1EntityManager() : player(nullptr), player2(nullptr)		//Sets the j1Player1* pointers declared in the header to nullptr
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{
	//As the pointers have been set in the constructor, they must be destroyed / deleted in the destructor.
	delete player;
	delete player2;
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;

	cycle_length = config.child("enemies").child("update_cycles_sec").attribute("seconds").as_float();

	//Iterates all entities and calls their Awake() methods.
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->Awake(config.child(entity_iterator->data->name.GetString()));
	}

	return true;
}

bool j1EntityManager::Start()
{
	//Iterates all entities in the entities list and calls their Start() method.
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	//Enemies PreUpdate()

	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->PreUpdate();
	}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::FireBrick);
	accumulated_time += dt;

	if (accumulated_time >= cycle_length) //Timer that will set doLogic to true 10 times per second (cycle_length = 0.1 sec).
	{
		doLogic = true;
	}

	//Calls the Update method of all entities. Passes dt and doLogic as arguments (mainly for pathfinding enemies).
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->Update(dt, doLogic);
	}

	if (doLogic == true)				//Resets the doLogic timer.
	{
		doLogic = false;
		accumulated_time = 0;
	}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	//Iterates all entities and calls their PostUpdate() methods.
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	//Iterates all entities in the entities list and calls their CleanUp() method.
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		entity_iterator->data->CleanUp();
	}

	entities.clear();									//Deletes all items in the entities list and frees all allocated memory.

	player = NULL;										//Sets the j1Player* player pointer to NULL.

	return true;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::FireBrick);
	//Iterates all entities in the entities list and searches for the entity passed as argument, if it is inside the list and is found, it is then destroyed.
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		if (entity_iterator->data == entity)
		{
			if (entity_iterator->data == GetPlayer())		//Revise. Should this be here?
			{
				GetPlayer()->CleanUp();
			}
				
			entities.del(entity_iterator);
			RELEASE(entity_iterator->data);
			break;
		}
	}
}

void j1EntityManager::OnCollision(Collider* C1, Collider* C2)		//This OnCollision will manage the collisions of all entities and derive them to their respective OnCollision methods()
{
	for (p2List_item<j1Entity*>* entity_iterator = entities.start; entity_iterator != NULL; entity_iterator = entity_iterator->next)
	{
		if (C1 == entity_iterator->data->collider)					//Will be run if there is a collision and any of the colliders are of the type PLAYER.
		{
			entity_iterator->data->OnCollision(C1, C2);
			break;
		}
		else if (C2 == entity_iterator->data->collider)
		{
			entity_iterator->data->OnCollision(C2, C1);
			break;
		}
	}
}

j1Entity* j1EntityManager::CreateEntity(ENTITY_TYPE type, int x, int y)
{
	//static_assert?

	j1Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPE::PLAYER:							//If the ENTITT_TYPE passed as argument is PLAYER.
		//ret = new j1Player(x, y, type);				//Allocates memory for an entity from the j1Player module.
		ret = new j1Player1(x, y, type);
		break;

	case ENTITY_TYPE::PLAYER2:
		ret = new j1Player2(x, y, type);				//REVISE THIS HERE. Check if we can pass only j1Player and thats it or if both can be ENTITY_TYPE player
		break;

	case ENTITY_TYPE::MECHA:							//If the ENTITT_TYPE passed as argument is a MECHA.
		//ret = new j1Mecha(x, y, type);
		break;
	case ENTITY_TYPE::ALIEN:							//If the ENTITT_TYPE passed as argument is an ALIEN.
		//ret = new j1Alien(x, y, type);
		break;
	}
	//ret->type = type;

	if (ret != nullptr)									//If the j1Entity* pointer is not NULL.
	{
		entities.add(ret);								//Adds the generated entity to the entities list.
	}

	return ret;
}

void j1EntityManager::CreatePlayers()
{
	player = (j1Player1*)CreateEntity(ENTITY_TYPE::PLAYER);	//Revise 0, 0. Maybe default x and y of the CreateEntity method to 0.
	//playere = (j1Player1*)CreateEntity(ENTITY_TYPE::PLAYER);
	player2 = (j1Player2*)CreateEntity(ENTITY_TYPE::PLAYER2);//REVISE THIS HERE. Check if we can pass only j1Player and thats it or if both can be ENTITY_TYPE PLAYER
	//player2 = (j1Player2*)CreateEntity(ENTITY_TYPE::PLAYER2);

	//Maybe create methods for them?
	//mecha = (j1Mecha*)CreateEntity(ENTITY_TYPE::MECHA);
	//alien = (j1Alien*)CreateEntity(ENTITY_TYPE::ALIEN);
}

void j1EntityManager::AddEnemy(ENTITY_TYPE type, int x, int y)
{
	p2List_item<EntityData*>* enemy_iterator = entityData_list.start;

	for (enemy_iterator; enemy_iterator != NULL; enemy_iterator = enemy_iterator->next)
	{
		enemy_iterator->data->position.x = x;
		enemy_iterator->data->position.y = y;
		enemy_iterator->data->type = type;
	}
}

void j1EntityManager::SpawnEnemy()
{
	p2List_item<EntityData*>* enemy_iterator = entityData_list.start;
	
	for (enemy_iterator; enemy_iterator != NULL; enemy_iterator = enemy_iterator->next)												//Iterates the entityData_list.
	{
		j1Entity * enemy;																											//Pointer that will be assigned to each enemy entity.

		switch (enemy_iterator->data->type)			//REVISE TYPE, maybe it will not work.
		{
		case ENTITY_TYPE::MECHA:
			//enemy = new j1Mecha(enemy_iterator->data->position.x, enemy_iterator->data->position.y, enemy_iterator->data->type);	//Spawns a MECHA type enemy.
			//
			break;

		case ENTITY_TYPE::ALIEN:
			//enemy = new j1Alien(enemy_iterator->data->position.x, enemy_iterator->data->position.y, enemy_iterator->data->type);	//Spawns an ALIEN type enemy.
			break;
		}

		//if (enemy != NULL)
		{
			entities.add(enemy);																									//The entity is added to the entities list
			enemy->Start();																											//The entity's start method is called.
			break;
		}
	}
}

j1Entity* j1EntityManager::GetPlayer() const
{
	j1Entity* ret = nullptr;

	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->type == ENTITY_TYPE::PLAYER)
		{
			ret = entity->data;
			break;
		}
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	GetPlayer()->Save(data.append_child("player"));
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		pugi::xml_node child = data.append_child(entity->data->name.GetString());
		child.append_attribute("position_x") = entity->data->position.x;
		child.append_attribute("position_y") = entity->data->position.y;
	}

	return true;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	CleanUp();
	GetPlayer()->Load(data.child("player"));
	for (pugi::xml_node mecha = data.child("mecha"); mecha; mecha = mecha.next_sibling("mecha"))
	{
		CreateEntity(ENTITY_TYPE::MECHA, mecha.attribute("position_x").as_int(), mecha.attribute("position_y").as_int());
	}  
	 
	for (pugi::xml_node alien = data.child("alien"); alien; alien = alien.next_sibling("alien"))
	{
		CreateEntity(ENTITY_TYPE::ALIEN, alien.attribute("position_x").as_int(), alien.attribute("position_y").as_int());
	}

	return true;
}

