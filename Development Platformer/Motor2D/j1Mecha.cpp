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
	name.create("entities");
	
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
	//Load_Entity();
	LoadEntityProperties();		//Revise this
	InitMecha();

	//Sets the first cycle of animations to the idle set.
	animation = &idle;
	airborne = true;

	state = Entity_State::IDLE;

	return true;
}

bool j1Mecha::Update(float dt)
{
 BROFILER_CATEGORY("Mecha Update", Profiler::Color::AliceBlue);
	//CalculatePath

	switch (state)
	{
	case Entity_State::IDLE:

		animation = &idle;

		break;
	}

	enemy_HitBox = animation->GetCurrentFrame(dt);						//REVISE THIS HERE.
	BlitEntity(position.x, position.y, enemy_HitBox, flip);

	return true;
}

bool j1Mecha::PostUpdate()
{
	return true;
}

bool j1Mecha::CleanUp()
{
	return true;
}

void j1Mecha::OnCollision(Collider*c1, Collider* c2)
{
	//Entity_On collision function in entity
}

bool j1Mecha::Load(pugi::xml_node&)
{
	return true;
}

bool j1Mecha::Save(pugi::xml_node&) const
{
	return true;
}

//bool j1Mecha::Load_Entity()
//{
//	entity_sprite = App->tex->Load("textures/Spritesheets/Enemies/mech-unit/mech-unit.png");
//
//	//Hitbox & colliders
//
//	grounded = false;
//	airborne = false;
//	flip = false;
//	isGoingRight = false;
//	isGoingLeft = false;
//	fading = false;
//	isAlive = true;
//	isDying = false;
//	againstRightWall = false;
//	againstLeftWall = false;
//
//	return true;
//}

void j1Mecha::Normal_Path()
{

}

void j1Mecha::Chasing_Path()
{

}

void j1Mecha::LoadAnimationPushbacks()
{
	idle.LoadAnimation("mecha", "idle");
	running.LoadAnimation("mecha", "running");

	return;
}

void j1Mecha::LoadEntityProperties()
{
	//Gets all the required player variables from the config xml file
	//position.x = config.child("mecha").child("position").attribute("x").as_float();				//Position x from entity.h
	//position.y = config.child("mecha").child("position").attribute("y").as_float();				//Position y from entity.h
	//spawn_position.x = config.child("mecha").child("position").attribute("x").as_float();
	//spawn_position.y = config.child("mecha").child("position").attribute("y").as_float();

	/*velocity.x = config.child("mecha").child("speed").attribute("x").as_float();
	velocity.y = config.child("mecha").child("speed").attribute("y").as_float();
	max_speed.x = config.child("mecha").child("max_speed").attribute("x").as_float();
	max_speed.y = config.child("mecha").child("max_speed").attribute("y").as_float();

	acceleration.x = config.child("mecha").child("acceleration").attribute("x").as_float();
	acceleration.y = config.child("mecha").child("acceleration").attribute("y").as_float();
	gravity = config.child("mecha").child("gravity").attribute("value").as_float();

	sprite_measures.x = config.child("mecha").child("sprite_measures").attribute("w").as_int();
	sprite_measures.y = config.child("mecha").child("sprite_measures").attribute("h").as_int();

	lives = config.child("mecha").child("lives").attribute("lives").as_int();*/

	return;
}

void j1Mecha::InitMecha()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Enemies/mech-unit/mech-unit.png");

	//Hitbox & colliders
	enemy_HitBox.x = position.x;
	enemy_HitBox.y = position.y;
	enemy_HitBox.w = sprite_width;
	enemy_HitBox.h = sprite_height;

	collider = App->collisions->AddCollider(enemy_HitBox, Object_Type::ENEMY, App->entityManager);				//THIS HERE

	grounded = true;
	isAlive = true;
	airborne = false;
	flip = false;
	isGoingRight = false;
	isGoingLeft = false;
	fading = false;
	isDying = false;
	againstRightWall = false;
	againstLeftWall = false;

	return;
}

void j1Mecha::LoadEntityAudio()
{

}