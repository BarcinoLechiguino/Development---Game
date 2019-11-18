#include "j1EntityManager.h"
//
//
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Window.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{
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

	return true;
}

bool j1EntityManager::Update(float dt)
{
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

j1Entity* j1EntityManager::CreateEntity(int x, int y, ENTITY_TYPE type)
{
	//static_assert?
	
	j1Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPE::PLAYER:							//If the ENTITT_TYPE passed as argument is PLAYER.
		ret = new j1Player(x, y, type);					//Allocates memory for an entity from the j1Player module.
		break;
	
	case ENTITY_TYPE::MECHA:							//If the ENTITT_TYPE passed as argument is a MECHA.
		//
		break;
	case ENTITY_TYPE::ALIEN:							//If the ENTITT_TYPE passed as argument is an ALIEN.
		//
		break;
	}
	//ret->type = type;

	if (ret != nullptr)									//If the j1Entity* pointer is not NULL.
	{
		entities.add(ret);								//Adds the generated entity to the entities list.
	}

	return ret;
}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(0, 0, ENTITY_TYPE::PLAYER);	//Revise 0, 0. Maybe default x and y of the CreateEntity method to 0.
}

void j1EntityManager::SpawnEnemy()
{

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
		CreateEntity(mecha.attribute("position_x").as_int(), mecha.attribute("position_y").as_int(), ENTITY_TYPE::MECHA);
	}  
	 
	for (pugi::xml_node alien = data.child("alien"); alien; alien = alien.next_sibling("alien"))
	{
		CreateEntity(alien.attribute("position_x").as_int(), alien.attribute("position_y").as_int(), ENTITY_TYPE::ALIEN);
	}

	return true;
}

