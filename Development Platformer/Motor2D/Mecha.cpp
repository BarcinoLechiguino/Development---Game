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
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"
#include "Mecha.h"

//Mecha::Mecha(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, ENTITY_TYPE::MECHA)
//{
//	name.create("entities");
//	
//	AddAnimationPushbacks();
//}
//
//Mecha::~Mecha()
//{
//}
//
//bool Mecha::Awake(pugi::xml_node& config)
//{
//	LoadMechaProperties(config);
//
//	//Sets the first cycle of animations to the idle set.
//	animation = &idle;
//
//	return true;
//}
//
//bool Mecha::Start()
//{
//	Load_Entity();
//	airborne = true;
//
//	state = IDLE;
//
//	return true;
//}
//
//bool Mecha::Update(float dt)
//{
// BROFILER_CATEGORY("Mecha Update", Profiler::Color::AliceBlue);
//	//CalculatePath
//
//	switch (state)
//	{
//	case IDLE:
//
//		animation = &idle;
//
//		break;
//	}
//	HitBox = animation->GetCurrentFrame(dt);
//	BlitEntity(position.x, position.y, HitBox, flip);
//
//	return true;
//}
//
//bool Mecha::PostUpdate()
//{
//	return true;
//}
//
//bool Mecha::CleanUp()
//{
//	return true;
//}
//
//void Mecha::OnCollision(Collider*c1, Collider* c2)
//{
//	//Entity_On collision function in entity
//}
//
//bool Mecha::Load(pugi::xml_node&)
//{
//	return true;
//}
//
//bool Mecha::Save(pugi::xml_node&) const
//{
//	return true;
//}
//
//bool Mecha::Load_Entity()
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
//
//void Mecha::Normal_Path()
//{
//
//}
//
//void Mecha::Chasing_Path()
//{
//
//}
//
//bool Mecha::AddAnimationPushbacks()
//{
//	idle.LoadAnimation("idle");
//	running.LoadAnimation("running");
//
//	return true;
//}
//
//bool Mecha::LoadMechaProperties(pugi::xml_node& config)
//{
//	//Gets all the required player variables from the config xml file
//	position.x = config.child("mecha").child("position").attribute("x").as_float();				//Position x from entity.h
//	position.y = config.child("mecha").child("position").attribute("y").as_float();				//Position y from entity.h
//	spawn_position.x = config.child("mecha").child("position").attribute("x").as_float();
//	spawn_position.y = config.child("mecha").child("position").attribute("y").as_float();
//
//	velocity.x = config.child("mecha").child("speed").attribute("x").as_float();
//	velocity.y = config.child("mecha").child("speed").attribute("y").as_float();
//	max_speed.x = config.child("mecha").child("max_speed").attribute("x").as_float();
//	max_speed.y = config.child("mecha").child("max_speed").attribute("y").as_float();
//
//	acceleration.x = config.child("mecha").child("acceleration").attribute("x").as_float();
//	acceleration.y = config.child("mecha").child("acceleration").attribute("y").as_float();
//	gravity = config.child("mecha").child("gravity").attribute("value").as_float();
//
//	sprite_measures.x = config.child("mecha").child("sprite_measures").attribute("w").as_int();
//	sprite_measures.y = config.child("mecha").child("sprite_measures").attribute("h").as_int();
//
//	lives = config.child("mecha").child("lives").attribute("lives").as_int();
//
//	return true;
//}