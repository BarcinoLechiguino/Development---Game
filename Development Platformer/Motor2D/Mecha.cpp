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
#include "Mecha.h"

Mecha::Mecha(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, ENTITY_TYPE::MECHA)
{
	name.create("entities");
	//Collisions???
	//AddAnimations
}

Mecha::~Mecha()
{
}

bool Mecha::Awake(pugi::xml_node&)
{
	return true;
}

bool Mecha::Start()
{
	return true;
}

bool Mecha::Update(float dt)
{
	//CalculatePath
}

bool Mecha::PostUpdate()
{
	return true;
}

bool Mecha::CleanUp()
{
	return true;
}

void Mecha::OnCollision(Collider*c1, Collider* c2)
{
	//Entity_On collision function in entity
}

bool Mecha::Load(pugi::xml_node&)
{
	return true;
}

bool Mecha::Save(pugi::xml_node&) const
{
	return true;
}

void Mecha::Normal_Path()
{

}

void Mecha::Chasing_Path()
{

}