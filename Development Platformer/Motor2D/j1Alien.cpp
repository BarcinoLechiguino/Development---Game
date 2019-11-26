#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Alien.h"
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"
#include <time.h>

j1Alien::j1Alien(int x, int y, ENTITY_TYPE type) : j1Enemy(x, y, ENTITY_TYPE::ALIEN)
{
	position.x = x;
	position.y = y;
	
	LoadAnimationPushbacks();
}

j1Alien::~j1Alien()
{
}

bool j1Alien::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Alien::Start()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Enemies/alien-flying-enemy/alien-enemy-flying.png");
	
	//Load_Entity();
	LoadEntityProperties();
	InitEnemy();

	airborne = true;

	state = Entity_State::IDLE;

	return true;
}

bool j1Alien::Update(float dt, bool doLogic)
{
	BROFILER_CATEGORY("Alien Update", Profiler::Color::Pink);
	//CalculatePath

	//ALIEN DEBUG INPUTS
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		state = Entity_State::PATHING_RIGHT;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		state = Entity_State::IDLE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		state = Entity_State::PATHING_LEFT;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		state = Entity_State::IDLE;
	}

	switch (state)
	{
	case Entity_State::IDLE:

		animation = &idle;

		break;

	case Entity_State::PATHING_RIGHT:
		
		position.x += speed.x * dt;
		flip = true;						//Change the alien's orientation in the spritesheet so this can be false.
		animation = &running;

		break;

	case Entity_State::PATHING_LEFT:
		
		position.x -= speed.x * dt;
		flip = false;
		animation = &running;

		break;
	}

	enemy_HitBox = animation->GetCurrentFrame(dt);
	collider->Set_Position(position.x, position.y);
	BlitEntity(position.x, position.y, enemy_HitBox, flip);

	return true;
}

bool j1Alien::PostUpdate()
{
	return true;
}

bool j1Alien::CleanUp()
{
	App->tex->UnLoad(entity_sprite);

	if (collider != nullptr)
	{
		collider->to_delete = true;
	}

	if (animation != nullptr)
	{
		animation = nullptr;
	}
	
	return true;
}

void j1Alien::OnCollision(Collider* C1, Collider* C2)
{
	//Entity_On collision function in entity
	if (C1->type == Object_Type::ENEMY)
	{
		//Enemy Colliding Against a Player
		if (C2->type == Object_Type::ATTACK)
		{
			//DeathSound
			CleanUp();
		}

		//Enemy colliding against solids
		if (C2->type == Object_Type::SOLID)
		{
			//Enemy Colliding from TOP or BOTTOM. sssssa
			if (C1->collider.x + C1->collider.w >= C2->collider.x && C1->collider.x <= C2->collider.x + C2->collider.y)		//The first part checks if C1 is contained in the X axis of C2. 
			{
				//Enemy Colliding from TOP.
				if (C1->collider.y + C1->collider.h >= C2->collider.y && C1->collider.y < C2->collider.y && speed.y != 0)			//This second part checks if C1 is actually colliding vertically down.
				{
					speed.y = 0;
					position.y = C2->collider.y - C1->collider.h + 1;		//THIS HERE
					grounded = true;
					LOG("MECHA IS COLLIDING WITH A SOLID FROM ABOVE");
				}
			}

			//Enemy is colliding from LEFT or RIGHT.
			if (C1->collider.y <= C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h - 4 >= C2->collider.y)		//The first part checks if C1 is contained in the Y axis of C2.
			{
				//Enemy is colliding from LEFT.
				if (C1->collider.x + C1->collider.w >= C2->collider.x && C1->collider.x <= C2->collider.x)						//This second part checks if C1 is actually colliding from the left side of the collider.
				{
					againstLeftWall = true;
					againstRightWall = false;
					LOG("MECHA IS COLLIDING WITH A SOLID FROM THE LEFT");
				}

				//Enemy is colliding from RIGHT.
				if (C1->collider.x <= C2->collider.x + C2->collider.w && C1->collider.x >= C2->collider.x)						// This second part checks if C1 is actually colliding from the right side of the collider.
				{
					againstRightWall = true;
					againstLeftWall = false;
					LOG("MECHA IS COLLIDING WITH A SOLID FROM THE RIGHT");
				}
			}
		}
	}
}

bool j1Alien::Load(pugi::xml_node&)
{
	return true;
}

bool j1Alien::Save(pugi::xml_node&) const
{
	return true;
}


// ------------------------ ALIEN ENTITY METHODS ------------------------
void j1Alien::Normal_Path()
{

}

void j1Alien::Chasing_Path()
{

}

void j1Alien::LoadAnimationPushbacks()
{
	idle.LoadAnimation("alien", "idle");
	running.LoadAnimation("alien","running");

	return;
}

void j1Alien::LoadEntityProperties()
{
	config_file.load_file("config.xml");

	enemy_entity = config_file.child("config").child("entities").child("alien");
	
	//Gets all the required player variables from the config xml file
	sprite_width	= enemy_entity.child("sprite_size").attribute("w").as_int();
	sprite_height	= enemy_entity.child("sprite_size").attribute("h").as_int();

	speed.x			= enemy_entity.child("speed").attribute("x").as_float();
	speed.y			= enemy_entity.child("speed").attribute("y").as_float();

	return;
}

void j1Alien::LoadEntityAudio()
{

}
