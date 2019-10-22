#include "j1App.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Render.h"


j1Collisions::j1Collisions() : j1Module()
{
	name.create("collisions");
}

bool j1Collisions::Init()
{

	return true;
};

bool j1Collisions::Awake(pugi::xml_node& config)
{
	return true;
};

bool j1Collisions::Start() 
{
	LoadFromMap(); //Revise this (name)

	return true;
};

bool j1Collisions::PreUpdate() 
{
	/* Old code for reference

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}*/

	p2List_item<Collider*>* collider_item = collider_list.start; //Change item for collider.

	//Iterates all colliders to delete them if the related bool is true.
	while (collider_item != NULL)
	{
		if (collider_item->data->delete_collider == true)
		{
			collider_list.del(collider_item);
		}
		collider_item = collider_item->next;
	}

	//Revise this (Although this was how it was named in our code)
	//Checking all collisions.
	Collider * c1; //Pointer to the collider struct. "Creates" the first collider to compare.
	Collider * c2; //Pointer to the collider struct. "Creates" the first collider to compare.

	//This resets the list to the first item.
	collider_item = collider_list.start;
	
	//This loop checks the collisions between 2 player colliders.
	while (collider_item != NULL)
	{
		c1 = collider_item->data;

		p2List_item<Collider*>* collider_item2 = collider_list.start;

		while (collider_item2 != NULL)
		{
			c2 = collider_item2->data;

			if (c1 != c2)
			{
				if (c1->check_collision(c2->collider) == true && (c1->type == PLAYER || c2->type == PLAYER))
				{
					if (c1->callback)
					{
						c1->callback->OnCollision(c1, c2);
					}

					if (c2->callback)
					{
						c1->callback->OnCollision(c2, c1);
					}
				}
			}
			collider_item2 = collider_item2->next;
		}
		collider_item = collider_item->next;
	}
	
	return true;
};

bool j1Collisions::Update(float dt) 
{
	Collider_Debug();
	return true;
};

//Revise this
void j1Collisions::Collider_Debug()
{
	if (collider_debug == false)
	{
		return;
	}

	Uint8 alpha = 80; //Opacity of the colliders. Need to put it in the xml file.
	
	p2List_item<Collider*>* collider_item = collider_list.start;
	while (collider_item != NULL)
	{
		switch (collider_item->data->type)
		{
		case PLAYER: //Orange
			App->render->DrawQuad(collider_item->data->collider, 255, 100, 0, alpha);
			break;

		case SOLID: //Blue
			App->render->DrawQuad(collider_item->data->collider, 0, 0, 255, alpha);
			break;

		case PLATFORM: //Yellow
			App->render->DrawQuad(collider_item->data->collider, 255, 200, 0, alpha);
			break;

		case HAZARD: //Red
			App->render->DrawQuad(collider_item->data->collider, 255, 0, 0, alpha);
			break;

		case ITEM: //Green
			App->render->DrawQuad(collider_item->data->collider, 0, 255, 0, alpha);
			break;

		case DESACTIVABLE: //White
			App->render->DrawQuad(collider_item->data->collider, 255, 255, 255, alpha);
			break;

		case RESPAWN: //Black or maybe purple
			App->render->DrawQuad(collider_item->data->collider, 0, 0, 0, alpha);
			break;

		case GOAL: //Pink
			App->render->DrawQuad(collider_item->data->collider, 255, 0, 150, alpha);
		}
		collider_item = collider_item->next;
	}
}

bool j1Collisions::PostUpdate() 
{
	return true;
};

bool j1Collisions::CleanUp() 
{
	return true;
};

//Revise this
Collider* j1Collisions::AddCollider(SDL_Rect collider, Object_Type type, j1Module* callback)
{
	Collider* coll = new Collider;

	coll->callback = callback;
	coll->collider = collider;
	coll->type = type;

	collider_list.add(coll);

	return coll;
}

//Loads all the colliders that are in the map file and iterates through them assigning them a type.
void j1Collisions::LoadFromMap() // Remember to put in fade to black.
{
	p2List_item<ObjectGroup*>* object_list = App->map->data.objectGroups.start;
	while (object_list != NULL)
	{
		for (int i = 0; i < object_list->data->object_size; i++)
		{
			AddCollider(*object_list->data->object[i].hitbox, object_list->data->object[i].type, NULL);
		}
		object_list = object_list->next;
	}
}

bool Collider::check_collision(const SDL_Rect& r) const
{
	return ((r.x + r.w > collider.x) && (r.x < collider.x + collider.w) &&
		(r.y + r.h > collider.y) && (r.y < collider.y + collider.h));
}

Collider::Collider(ObjectData object)
{
	collider = *object.hitbox;
	type = object.type;
}


