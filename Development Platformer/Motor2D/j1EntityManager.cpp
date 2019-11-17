#include "j1EntityManager.h"
//
//
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Player1.h"
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

	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Awake(config.child(entity->data->name.GetString()));
	}

	return true;
}

bool j1EntityManager::Start()
{
	//
	return true;
}

bool j1EntityManager::Update(float dt)
{
	accumulated_time += dt;

	if (accumulated_time >= cycle_length)
	{
		doLogic = true;
	}




	return true;
}

bool j1EntityManager::PostUpdate()
{
	//
	return true;
}

bool j1EntityManager::CleanUp()
{
	Entity* player = getPlayer();
	if (player)
		getPlayer()->CleanUp();

	p2List_item<Entity*>* item;
	item = entities.start->next; //Skips first entity

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	entities.clear();
	entities.add(player);

	return true;
}

void j1EntityManager::DestroyEntity(Entity* entity)
{
	p2List_item<Entity*>* finder = entities.start;
	while (finder != NULL)
	{
		if (finder->data == entity)
		{
			if (finder->data == getPlayer())
				getPlayer()->CleanUp();
			entities.del(finder);
			RELEASE(finder->data);
			break;
		}
		finder = finder->next;
	}
}

Entity* j1EntityManager::createEntity(entityType type, int x, int y)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case LAND_ENEMY:
		//
		break;
	case FLYING_ENEMY:
		//
		break;
	}
	ret->type = type;

	entities.add(ret);

	return ret;
}

Entity* j1EntityManager::getPlayer() const
{
	Entity* ret = nullptr;

	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->type == PLAYER)
		{
			ret = entity->data;
			break;
		}
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	getPlayer()->Save(data.append_child("player"));
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
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
	getPlayer()->Load(data.child("player"));
	for (pugi::xml_node land_enemy = data.child("land_enemy"); land_enemy; land_enemy = land_enemy.next_sibling("land_enemy"))
	{
		createEntity(LAND_ENEMY, land_enemy.attribute("position_x").as_int(), land_enemy.attribute("position_y").as_int());
	}

	for (pugi::xml_node flying_enemy = data.child("bat"); flying_enemy; flying_enemy = flying_enemy.next_sibling("flying_enemy"))
	{
		createEntity(FLYING_ENEMY, flying_enemy.attribute("position_x").as_int(), flying_enemy.attribute("position_y").as_int());
	}

	return true;
}

