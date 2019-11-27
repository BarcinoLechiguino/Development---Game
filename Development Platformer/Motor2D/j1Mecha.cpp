#include "j1App.h"
#include "j1Player.h"
#include "j1Player2.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1FadeScene.h"
#include "j1Audio.h"
#include "j1Mecha.h"
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"

j1Mecha::j1Mecha(int x, int y, ENTITY_TYPE type) : j1Enemy(x, y, ENTITY_TYPE::MECHA)
{
	//name.create("entities");		//NOT NECESSARY
	
	position.x = x;					//REVISE THIS. SPAWNING THE ENEMIES AT THEIR RESPECTIVE POSITIONS
	position.y = y;

	LoadAnimationPushbacks();
}

j1Mecha::~j1Mecha()
{
}

bool j1Mecha::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Mecha::Start()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Enemies/mech-unit/mech-unit2.png");
	
	//Load_Entity();
	LoadEntityProperties();
	InitEnemy();

	airborne = true;

	state = Entity_State::IDLE;

	return true;
}

bool j1Mecha::Update(float dt, bool doLogic)
{
	BROFILER_CATEGORY("Mecha Update", Profiler::Color::AliceBlue);
	//CalculatePath

	if (doLogic)
	{
		//MECHA DEBUG INPUTS
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = Entity_State::PATHING_RIGHT;
			App->audio->PlayFx(18, 0);
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = Entity_State::IDLE;
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = Entity_State::PATHING_LEFT;
			App->audio->PlayFx(18, 0);
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = Entity_State::IDLE;
		}

		if (doLogic == true)
		{
			Normal_Path();
			Chasing_Path();
		}

		switch (state)
		{
		case Entity_State::IDLE:

			animation = &idle;

			break;

		case Entity_State::PATHING_RIGHT:

			position.x += speed.x * dt;
			flip = false;
			animation = &running;

			break;

		case Entity_State::PATHING_LEFT:

			position.x -= speed.x * dt;
			flip = true;
			animation = &running;

			break;
		}

		if (airborne = true)
		{
			ApplyMechaGravity();
		}

		enemy_HitBox = animation->GetCurrentFrame(dt);						//REVISE THIS HERE.
		collider->Set_Position(position.x, position.y);
		BlitEntity(position.x, position.y, enemy_HitBox, flip);
	}
	

	return true;
}

bool j1Mecha::PostUpdate()
{
	return true;
}

bool j1Mecha::CleanUp()
{
	App->tex->UnLoad(entity_sprite);

	if (collider != nullptr)
	{
		/*delete collider;
		collider = nullptr;*/

		collider->to_delete = true;
		//collider = nullptr;
	}

	if (animation != nullptr)
	{
		animation = nullptr;
	}

	//RELEASE(App->entityManager->mecha);					//Breaks everything
	
	return true;
}

void j1Mecha::OnCollision(Collider* C1, Collider* C2)
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

bool j1Mecha::Load(pugi::xml_node&)
{
	return true;
}

bool j1Mecha::Save(pugi::xml_node&) const
{
	return true;
}


// ------------------------ MECHA ENTITY METHODS ------------------------
void j1Mecha::Normal_Path()
{

}

void j1Mecha::Chasing_Path()
{

}

void j1Mecha::LoadAnimationPushbacks()
{
	idle.LoadAnimation("mecha", "idle");
	//idle.LoadAnimation("mecha", "running");
	running.LoadAnimation("mecha", "running");

	return;
}

void j1Mecha::LoadEntityProperties()
{
	config_file.load_file("config.xml");

	enemy_entity = config_file.child("config").child("entities").child("mecha");

	//Gets all the required enemy variables from the config xml file
	sprite_width			= enemy_entity.child("sprite_size").attribute("w").as_int();
	sprite_height			= enemy_entity.child("sprite_size").attribute("h").as_int();

	speed.x					= enemy_entity.child("speed").attribute("x").as_float();
	speed.y					= enemy_entity.child("speed").attribute("y").as_float();
	mecha_max_speed.x		= enemy_entity.child("max_speed").attribute("x").as_float();
	mecha_max_speed.y		= enemy_entity.child("max_speed").attribute("y").as_float();

	mecha_acceleration.x	= enemy_entity.child("acceleration").attribute("x").as_float();
	mecha_acceleration.y	= enemy_entity.child("acceleration").attribute("y").as_float();
	mecha_gravity			= enemy_entity.child("gravity").attribute("value").as_float();

	return;
}

void j1Mecha::LoadEntityAudio()
{

}

void j1Mecha::ApplyMechaGravity()
{
	speed.y += mecha_gravity * App->GetDt();

	if (speed.y > mecha_max_speed.y * App->GetDt())
	{
		speed.y = mecha_max_speed.y * App->GetDt();
	}

	position.y += speed.y;				//Refreshes the vector speed of P1 in the Y axis.
}