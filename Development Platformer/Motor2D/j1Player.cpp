#include "j1App.h"
#include "j1Player.h"
#include "j1Player1.h"
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

j1Player::j1Player(int x, int y, ENTITY_TYPE type) : j1Entity(x, y, ENTITY_TYPE::PLAYER)  //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	//name.create("entities"); //The string has to be the same as the name of the node in the xml file.
};

j1Player::~j1Player()  //Destructor. Called at the last frame.
{

};

bool j1Player::Init()
{
	return true;
};

bool j1Player::Awake(pugi::xml_node& config)
{
	return true;
};

bool j1Player::Start()
{
	return true;
};

bool j1Player::PreUpdate()
{
	return true;
};

bool j1Player::Update(float dt, bool doLogic)
{
	return true;
};

bool j1Player::PostUpdate()
{
	return true;
};

bool j1Player::CleanUp()
{
	return true;
};

// Collision Handling ---------------------------------------
void j1Player::OnCollision(Collider* C1, Collider* C2)
{
	return;
}

bool j1Player::Load(pugi::xml_node& data)
{
	return true;
}

bool j1Player::Save(pugi::xml_node&  data) const
{
	return true;
}

//-------------------------------- PLAYER METHODS--------------------------------
void j1Player::LoadAnimationPushbacks()
{
	idle.LoadAnimation("player", "idle");					//Player's Idle animation.
	running.LoadAnimation("player", "running");			//Player's Running animation.
	crouching.LoadAnimation("player", "crouching");		//Player's Crouching animation.	
	jumping.LoadAnimation("player", "jumping");			//Player's Jumping Animation.
	frontflip.LoadAnimation("player", "frontflip");		//Player's Frontflip Animation.
	boosted_jump.LoadAnimation("player", "boosted_jump");	//Player's Boosted Jump Animation.
	falling.LoadAnimation("player", "falling");			//Player's Falling Animation.
	death.LoadAnimation("player", "death");				//Player's Death Animation.

	return;
}

bool j1Player::LoadPlayerPosition(p2SString playerPosition, p2SString map)
{
	config_file.load_file("config.xml");				//REVISE THIS HERE  Can a pugi object be reused as a copy in another class?

	player_entity = config_file.child("config").child("entities").child("player").child(playerPosition.GetString());

	position.x = player_entity.child(map.GetString()).attribute("x").as_float();
	position.y = player_entity.child(map.GetString()).attribute("y").as_float();

	//Sets the spawn position to where the player appears at first.
	player.spawn_position.x = position.x;
	player.spawn_position.y = position.y;

	return true;
}

void j1Player::LoadEntityProperties()
{
	config_file.load_file("config.xml");

	player_entity = config_file.child("config").child("entities").child("player");

	//Gets all the required player variables from the config xml file
	sprite_width			= player_entity.child("sprite_size").attribute("w").as_int();
	sprite_height			= player_entity.child("sprite_size").attribute("h").as_int();

	speed.x					= player_entity.child("speed").attribute("x").as_float();
	speed.y					= player_entity.child("speed").attribute("y").as_float();
	player.max_speed.x		= player_entity.child("max_speed").attribute("x").as_float();
	player.max_speed.y		= player_entity.child("max_speed").attribute("y").as_float();

	player.acceleration.x	= player_entity.child("acceleration").attribute("x").as_float();
	player.acceleration.y	= player_entity.child("acceleration").attribute("y").as_float();
	player.gravity			= player_entity.child("gravity").attribute("value").as_float();

	player.boost_jump.x		= player_entity.child("boost_jump").attribute("x").as_float();
	player.boost_jump.y		= player_entity.child("boost_jump").attribute("y").as_float();	

	player.frontflipStart	= player_entity.child("frontflip").attribute("start").as_float();
	player.frontflipEnd		= player_entity.child("frontflip").attribute("end").as_float();

	player.lives			= player_entity.child("lives").attribute("lives").as_int();
	player.max_lives		= player_entity.child("lives").attribute("lives").as_int();

	player.tpCdCount		= player_entity.child("tpCooldown").attribute("timer").as_float();
	player.tpCdTime			= player_entity.child("tpCooldown").attribute("cd").as_float();

	player.godModeSpeed		= player_entity.child("godspeed").attribute("gspeed").as_float();

	return;
}

void j1Player::LoadEntityAudio()
{
	jumpFX = App->audio->LoadFx("audio/fx/Jump.wav");
	deathFX = App->audio->LoadFx("audio/fx/Death.wav");
	duoFX = App->audio->LoadFx("audio/fx/Jump_Duo.wav");
	activateFX = App->audio->LoadFx("audio/fx/Activate.wav");
	tpFX = App->audio->LoadFx("audio/fx/TP.wav");
	goalFX = App->audio->LoadFx("audio/fx/Pass.wav");
	mecha_die = App->audio->LoadFx("audio/fx/Mecha die.wav");
	alien_die = App->audio->LoadFx("audio/fx/Alien die.wav");
}

bool j1Player::InitPlayer()
{

	// --------------------------------------------Loading the data and colliders of P1--------------------------------------------
	//Loads the data of the rectangle that contains P1.
	player.HitBox.x = position.x;							//Represents the position in the X axis of P1.		//THIS HERE
	player.HitBox.y = position.y;							//Represents the position in the Y axis of P1.
	player.HitBox.w = sprite_width;						//Represents the width of P1.
	player.HitBox.h = sprite_height;						//Represents the height of P1.

	player.atkHitBox.x = position.x + sprite_width;		//Position in the X axis of P1's attack collider.
	player.atkHitBox.y = position.y;						//Position in the Y axis of P1's attack collider.
	player.atkHitBox.w = sprite_width;						//Width of P1's attack collider.
	player.atkHitBox.y = sprite_height;						//Height of P1's attack collider.

	//Adds a collider for the player.
	collider = App->collisions->AddCollider(player.HitBox, Object_Type::PLAYER, App->entityManager);				//THIS HERE

	player.atkCollider = App->collisions->AddCollider(player.atkHitBox, Object_Type::ATTACK, App->entityManager);

	//Boolean resetting
	player.grounded				= false;
	player.airborne				= false;
	player.flip					= false;
	player.isCrouching			= false;
	player.isJumping			= false;
	player.isBoostJumping		= false;
	player.item_activated		= false;
	player.isGoingRight			= false;
	player.isGoingLeft			= false;
	player.platformDrop			= false;
	player.tpInCd				= false;
	player.fading				= false;
	player.isAlive				= true;
	player.isDying				= false;
	player.againstRightWall		= false;
	player.againstLeftWall		= false;
	player.againstCeiling		= false;
	player.GodMode				= false;
	player.switch_sprites		= false;

	return true;
}

/*bool j1Player::LoadPlayer1Textures()
{
	//Loads the textures of P1. Switches them according to switch_sprites
	if (p1.switch_sprites == true)
	{
		p1.texture = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");
	}
	else
	{
		p1.texture = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2_Buena.png");
	}

	return true;
}*/

//---------------------------------------------- General Checks ----------------------------------------------
void j1Player::ApplyGravity()
{
	speed.y += player.gravity * App->GetDt();

	if (speed.y > player.max_speed.y * App->GetDt())
	{
		speed.y = player.max_speed.y * App->GetDt();
	}

	position.y += speed.y;				//Refreshes the vector speed of P1 in the Y axis.

	//Jump animation modifications.
	if (player.isBoostJumping == true)				//If P1 is boost jumping then this set of animations is played.
	{
		if (speed.y < player.frontflipStart)
		{
			animation = &jumping;
		}
		else if (speed.y < player.frontflipEnd)
		{
			animation = &frontflip;
		}
		else
		{
			animation = &falling;
		}
	}
	else if (player.isJumping == true)				//If P1 is jumping then this set of animations is played.
	{
		if (speed.y < 0)
		{
			animation = &jumping;
		}
		else
		{
			animation = &falling;
		}
	}
}

void j1Player::LivesCheck(int lives)
{
	return;
}

void j1Player::SkillCooldown(bool& inCd, float& cdCounter, float& cdTime)			//Arguments need to be passed as reference (&) so the method can modify the actual variable instead of a copy.
{
	cdCounter += App->GetDt();				//Adds the time elapsed in a frame to a count.

	if (cdCounter > cdTime)					//Checks if the cd Count has reached the required amount of time.
	{
		inCd = false;						//Resets the bool so tp can be used again.
		cdCounter = 0;						//Resets the count so it can be used the next time tp is in cd.
	}
}

//void j1Player::Restart()
//{
//	return;
//}

void j1Player::GodModeInput()
{
	return;
}