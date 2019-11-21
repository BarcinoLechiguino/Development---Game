#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "Alien.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"
#include <time.h>

Alien::Alien(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, ENTITY_TYPE::MECHA)
{
	name.create("entities");

	AddAnimationPushbacks();
}

Alien::~Alien()
{
}

bool Alien::Awake(pugi::xml_node& config)
{
	LoadAlienProperties(config);

	//Sets the first cycle of animations to the idle set.
	animation = &idle;

	return true;
}

bool Alien::Start()
{
	Load_Entity();
	airborne = true;

	//state = IDLE;

	return true;
}

bool Alien::Update(float dt)
{
	BROFILER_CATEGORY("Alien Update", Profiler::Color::Pink);
	//CalculatePath

	/*switch (state)
	{
	case IDLE:

		animation = &idle;

		break;
	}
	HitBox = animation->GetCurrentFrame(dt);
	BlitEntity(position.x, position.y, HitBox, flip);*/

	return true;
}

bool Alien::PostUpdate()
{
	return true;
}

bool Alien::CleanUp()
{
	return true;
}

void Alien::OnCollision(Collider*c1, Collider* c2)
{
	//Entity_On collision function in entity
}

bool Alien::Load(pugi::xml_node&)
{
	return true;
}

bool Alien::Save(pugi::xml_node&) const
{
	return true;
}

bool Alien::Load_Entity()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Enemies/alien-flying-enemy/alien-flying-enemy.png");

	//Hitbox & colliders

	airborne = false;
	flip = false;
	isGoingRight = false;
	isGoingLeft = false;
	fading = false;
	isAlive = true;
	//isDying = false;
	againstRightWall = false;
	againstLeftWall = false;

	return true;
}

void Alien::Normal_Path()
{

}

void Alien::Chasing_Path()
{

}

bool Alien::AddAnimationPushbacks()
{
	idle.LoadAnimation("alien", "idle");
	running.LoadAnimation("alien","running");

	return true;
}

bool Alien::LoadAlienProperties(pugi::xml_node& config)
{
	//Gets all the required player variables from the config xml file
	position.x = config.child("alien").child("position").attribute("x").as_float();				//Position x from entity.h
	position.y = config.child("alien").child("position").attribute("y").as_float();				//Position y from entity.h
	spawn_position.x = config.child("alien").child("position").attribute("x").as_float();
	spawn_position.y = config.child("alien").child("position").attribute("y").as_float();

	speed = config.child("alien").child("speed").attribute("x").as_float();
	//velocity.x = config.child("alien").child("speed").attribute("x").as_float();
	//velocity.y = config.child("alien").child("speed").attribute("y").as_float();
	//max_speed.x = config.child("alien").child("max_speed").attribute("x").as_float();
	//max_speed.y = config.child("alien").child("max_speed").attribute("y").as_float();

	//acceleration.x = config.child("alien").child("acceleration").attribute("x").as_float();
	//acceleration.y = config.child("alien").child("acceleration").attribute("y").as_float();
	//gravity = config.child("alien").child("gravity").attribute("value").as_float();

	//sprite_measures.x = config.child("alien").child("sprite_measures").attribute("w").as_int();
	//sprite_measures.y = config.child("alien").child("sprite_measures").attribute("h").as_int();

	//lives = config.child("alien").child("lives").attribute("lives").as_int();

	return true;
}
