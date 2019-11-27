#include "j1App.h"
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
#include "j1Enemy.h"
#include "j1EntityManager.h"

j1Enemy::j1Enemy(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, type)  //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	/*name.create("entities");*/ //The string has to be the same as the name of the node in the xml file.
};

j1Enemy::~j1Enemy()  //Destructor. Called at the last frame.
{

};

bool j1Enemy::Init()
{
	return true;
};

bool j1Enemy::Awake(pugi::xml_node& config)
{
	return true;
};

bool j1Enemy::Start()
{
	return true;
};

bool j1Enemy::PreUpdate()
{
	return true;
};

bool j1Enemy::Update(float dt, bool doLogic)
{
	return true;
};

bool j1Enemy::PostUpdate()
{
	return true;
};

bool j1Enemy::CleanUp()
{
	return true;
};

// Collision Handling ---------------------------------------
void j1Enemy::OnCollision(Collider* C1, Collider* C2)
{
	return;
}

bool j1Enemy::Load(pugi::xml_node& data)
{
	return true;
}

bool j1Enemy::Save(pugi::xml_node&  data) const
{
	return true;
}

//---------------------------- Enemy methods ----------------------------
bool j1Enemy::Calculate_Path()
{
	return true;
}

void j1Enemy::Normal_Path()
{

}

void j1Enemy::Chasing_Path()
{

}

//bool j1Enemy::Load_Entity()
//{
//	return true;
//}

void j1Enemy::LoadAnimationPushbacks()
{
	return;
}

void j1Enemy::LoadEntityProperties()
{
	return;
}

void j1Enemy::InitEnemy()
{
	enemy_HitBox.x = position.x;
	enemy_HitBox.y = position.y;
	enemy_HitBox.w = sprite_width;
	enemy_HitBox.h = sprite_height;

	collider = App->collisions->AddCollider(enemy_HitBox, Object_Type::ENEMY, App->entityManager);				//THIS HERE This one loads the enemy collider (orange)

	// ------------------- ENEMY STATUS BOOLS -------------------
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
}

void j1Enemy::LoadEntityAudio()
{
	return;
}