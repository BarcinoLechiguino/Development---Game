#include "j1App.h"
#include "j1Player2.h"
#include "j1Player1.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1FadeScene.h"
#include "p2Log.h"
#include "j1Audio.h"

j1Player2::j1Player2() //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	name.create("player"); //The string has to be the same as the name of the node in the xml file.

	//P2 Idle animation.
	p2.idle.PushBack({ 28, 14, 37, 57 });
	p2.idle.PushBack({ 132, 12, 33, 59 });
	p2.idle.PushBack({ 230, 12, 37, 59 });
	p2.idle.PushBack({ 326, 14, 39, 57 });
	p2.idle.speed = 0.1f;

	//P2 Running animation.
	p2.running.PushBack({ 134, 90, 39, 55 });
	p2.running.PushBack({ 232, 92, 39, 53 });
	p2.running.PushBack({ 332, 96, 39, 49 });
	p2.running.PushBack({ 434, 90, 45, 55 });
	p2.running.PushBack({ 532, 92, 39, 53 });
	p2.running.PushBack({ 632, 96, 39, 49 });
	p2.running.speed = 0.2f;

	//P2 Crouching animation.
	p2.crouching.PushBack({ 432, 10, 37, 61 }); //20 Pixels of margin vertically up.
	p2.crouching.PushBack({ 530, 8, 39, 63 });
	p2.crouching.PushBack({ 630, 8, 37, 63 });
	p2.crouching.PushBack({ 34, 84, 33, 61 });
	p2.crouching.speed = 0.1f;

	//P2 Jumping Animation
	p2.jumping.PushBack({ 30, 172, 39, 47 });
	p2.jumping.PushBack({ 130, 176, 39, 43 });
	p2.jumping.PushBack({ 234, 162, 37, 53 });
	p2.jumping.PushBack({ 328, 158, 41, 45 });
	//p2.jumping.PushBack({ 436, 162, 29, 41 });
	p2.jumping.speed = 0.25f;

	//P2 Frontflip Animation
	p2.frontflip.PushBack({ 528, 168, 47, 33 });
	p2.frontflip.PushBack({ 528, 168, 47, 33 });
	p2.frontflip.PushBack({ 640, 168, 35, 41 });
	p2.frontflip.PushBack({ 640, 168, 35, 41 });
	p2.frontflip.PushBack({ 22, 248, 51, 33 });
	p2.frontflip.PushBack({ 22, 248, 51, 33 });
	p2.frontflip.speed = 0.2f;
	
	//P2 Boosted Jump Animation
	p2.boosted_jump.PushBack({ 30, 172, 39, 47 });
	p2.boosted_jump.PushBack({ 130, 176, 39, 43 });
	p2.boosted_jump.PushBack({ 234, 162, 37, 53 });
	p2.boosted_jump.PushBack({ 328, 158, 41, 45 });
	p2.boosted_jump.PushBack({ 436, 162, 29, 41 });
	p2.boosted_jump.PushBack({ 528, 168, 47, 33 });
	p2.boosted_jump.PushBack({ 640, 168, 35, 41 });
	p2.boosted_jump.PushBack({ 22, 248, 51, 33 });
	p2.boosted_jump.speed = 0.2f;

	//P2 Falling Animation
	p2.falling.PushBack({ 136, 224, 33, 61 });
	p2.falling.PushBack({ 236, 226, 33, 59 });
	p2.falling.speed = 0.2f;

	//P2 Death Animation
	p2.death.PushBack({ 10,1094,58,66 });
	p2.death.PushBack({ 10,1094,58,66 });
	p2.death.PushBack({ 10,1094,58,66 });
	p2.death.PushBack({ 10,1094,58,66 });
	p2.death.PushBack({ 10,1074,58,66 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 68,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 136,1074,58,86 });
	p2.death.PushBack({ 193,1074,58,86 });
	p2.death.PushBack({ 193,1074,58,86 });
	p2.death.PushBack({ 193,1074,58,86 });
	p2.death.PushBack({ 193,1074,58,66 });
	p2.death.speed = 0.2f;
};

j1Player2::~j1Player2()  //Destructor. Called at the last frame.
{

};

bool j1Player2::Init()
{
	return true;
};

bool j1Player2::Awake(pugi::xml_node& config)
{
	p2.position.x = config.child("player_2").child("position").attribute("x").as_float();
	p2.position.y = config.child("player_2").child("position").attribute("y").as_float();
	p2.spawn_position.x = config.child("player_2").child("position").attribute("x").as_float();
	p2.spawn_position.y = config.child("player_2").child("position").attribute("y").as_float();

	p2.speed.x = config.child("player_2").child("speed").attribute("x").as_float();
	p2.speed.y = config.child("player_2").child("speed").attribute("y").as_float();
	p2.max_speed.x = config.child("player_2").child("max_speed").attribute("x").as_float();
	p2.max_speed.y = config.child("player_2").child("max_speed").attribute("y").as_float();

	p2.acceleration.x = config.child("player_2").child("acceleration").attribute("x").as_float();
	p2.acceleration.y = config.child("player_2").child("acceleration").attribute("y").as_float();
	p2.gravity = config.child("player_2").child("gravity").attribute("value").as_float();

	p2.boost_jump.x = config.child("player_2").child("boost_jump").attribute("x").as_float();
	p2.boost_jump.y = config.child("player_2").child("boost_jump").attribute("y").as_float();

	p2.sprite_width = config.child("player_2").child("sprite_measures").attribute("w").as_int();
	p2.sprite_height = config.child("player_2").child("sprite_measures").attribute("h").as_int();

	p2.lives = config.child("player_2").child("lives").attribute("lives").as_int();
	p2.max_lives = config.child("player_2").child("lives").attribute("lives").as_int();

	/*p1.jumpFX = config.child("jumpFX").attribute("name").as_string();
	p1.deathFX = config.child("deathFX").attribute("name").as_string();
	p1.duoFX = config.child("landFX").attribute("name").as_string();*/

	p2.current_animation = &p2.idle;

	return true;
};

bool j1Player2::Start()
{
	LoadPlayer2();					//Loads P2 in game.
	//LoadPlayer2Textures();		//Loads P2's textures in game.

	p2.airborne = true;
	p2.item_activated = false;

	p2.state = idle_P2;

	return true;
};

bool j1Player2::PreUpdate()
{
	if (p2.GodMode == false)
	{
		p2.state = idle_P2;

		if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
		{
			p2.state = goingRight_P2;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
		{
			p2.state = goingLeft_P2;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)
		{
			p2.state = crouching_P2;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN)
		{
			p2.state = jumping_P2;
			App->audio->PlayFx(7, 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		{
			p2.platformDrop = true;
		}
		else
		{
			p2.platformDrop = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN)
		{
			p2.state = teleporting_P2;
			App->audio->PlayFx(1, 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
		{

			p2.state = dying_P2;
			App->audio->PlayFx(2, 0);
		}
	}
	else
	{
		GodModeInput();
	}

	//Switch Sprites Method Call
	/*if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->tex->UnLoad(p2.texture);
		LoadPlayer2Textures();
	}*/

	return true;

};

bool j1Player2::Update(float dt)
{
	p2.previous_position = p2.position;

	switch (p2.state)
	{

	case idle_P2:

		p2.speed.x = 0;
		p2.current_animation = &p2.idle;
		p2.isCrouching = false;

		break;

	case goingRight_P2:

		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed.x != p2.max_speed.x)
		{
			p2.speed.x += p2.acceleration.x;
		}

		p2.flip = false;
		p2.current_animation = &p2.running;
		p2.isGoingRight = true; //rev

		break;

	case goingLeft_P2:

		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed.x != -p2.max_speed.x)
		{
			p2.speed.x -= p2.acceleration.x;
		}

		p2.flip = true;
		p2.current_animation = &p2.running;
		p2.isGoingLeft = true; //rev

		break;

	case crouching_P2:

		p2.current_animation = &p2.crouching;
		p2.isCrouching = true;

		break;
	
	case jumping_P2:

		if (p2.grounded == true)
		{
			p2.speed.y = -p2.acceleration.y;
			p2.isJumping = true;
			p2.airborne = true;
			p2.grounded = false;
		}

		break;

	case falling_P2:
		
		p2.airborne = true;
		p2.grounded = true; //No jumping

		break;

	case teleporting_P2:

		TeleportP1ToP2();

		break;

	case dying_P1:

		p2.current_animation = &p2.death;
		p2.isDying = true;

		break;
	}

	p2.position.x += p2.speed.x; //Refreshes the vector speed of P2 in the X axis

	//If the p2 is in the air then this function brings him/her back down to the floor.
	if (p2.airborne == true)
	{
		p2.speed.y += p2.gravity;

		if (p2.speed.y > p2.max_speed.y)
		{
			p2.speed.y = p2.max_speed.y;
		}

		p2.position.y += p2.speed.y;				//Refreshes the vector speed of P2 in the Y axis.

		//Jump animation modifications.
		if (p2.isBoostJumping == true)				//If P2 is boost jumping then this set of animations is played.
		{
			if (p2.speed.y < FRONTFLIP_START)
			{
				p2.current_animation = &p2.jumping;
			}
			else if (p2.speed.y < FRONTFLIP_END)
			{
				p2.current_animation = &p2.frontflip;
			}
			else
			{
				p2.current_animation = &p2.falling;
			}
		}
		else if (p2.isJumping == true)				//If P1 is jumping then this set of animations is played.
		{
			if (p2.speed.y < APEX)
			{
				p2.current_animation = &p2.jumping;
			}
			else
			{
				p2.current_animation = &p2.falling;
			}
		}
	}

	//Makes P2's collider follow P2. If there is a flip in spritesheet it is taken into account.
	if (p2.flip == false)
	{
		p2.collider->Set_Position(p2.position.x, p2.position.y); //Makes the collider follow the player.
	}
	else
	{
		p2.collider->Set_Position(p2.position.x + FLIP_MARGIN, p2.position.y);
	}

	//If player dies:
	if (p2.lives == 0)
	{
		p2.isAlive = false;
		App->player1->p1.isAlive = false;

		if (p2.isAlive == false && App->player1->p1.isAlive == false)
		{
			App->audio->PlayFx(2, 0);
			Restart();
			App->player1->Restart();
			p2.lives = p2.max_lives;
		}
	}
	
	//We move the character according the position value after the state has been run.
	p2.HitBox.x = p2.position.x;
	p2.HitBox.y = p2.position.y;

	p2.HitBox = p2.current_animation->GetCurrentFrame();

	App->render->Blit(p2.texture, p2.position.x, p2.position.y, &p2.HitBox, p2.flip);

	return true;
};

bool j1Player2::PostUpdate()
{
	return true;
};

bool j1Player2::CleanUp()
{
	App->tex->UnLoad(p2.texture);
	App->player2->Disable();
	return true;
};

void j1Player2::TeleportP1ToP2()
{
	if (p2.flip == false) //The players will be always teleported directly in front of one another. 
	{
		App->player1->p1.position.x = p2.position.x + p2.collider->collider.w;
		App->player1->p1.position.y = p2.position.y;
	}
	else
	{
		App->player1->p1.position.x = p2.position.x - p2.collider->collider.w / 2;
		App->player1->p1.position.y = p2.position.y;
	}
}

void j1Player2::RespawnP2ToP1()		//Method that, on death, will respawn P2 behind P1.
{
	if (p2.flip == true)			//The players will be always respawned directly behind of one another. 
	{
		p2.position.x = App->player1->p1.position.x + App->player1->p1.collider->collider.w;
		p2.position.y = App->player1->p1.position.y;
	}
	else
	{
		p2.position.x = App->player1->p1.position.x + App->player1->p1.collider->collider.w/2;
		p2.position.y = App->player1->p1.position.y;
	}
}

//Collision Handling
void j1Player2::OnCollision(Collider* C1, Collider* C2)
{
	if (C2->type == PLAYER)
	{
		Collider* temp = C1;
		C1 = C2;
		C2 = temp;
	}
	if (C1->type != PLAYER)
	{
		return;
	}

	if (p2.GodMode == false)
	{
		//Player Colliding Against Another Player
		if (C1->type == PLAYER && C2->type == PLAYER)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w  /*&& p1.speed.x == p1.max_speed.x*/) //As the boost can be done even if P1 is static, this allows for more precise jumps... hopefully.
			{
				if (App->player1->p1.state == crouching_P1 /*App->player2->p2.isCrouching == true*/)
				{
					if (p2.grounded == true)
					{
						p2.speed.y -= p2.boost_jump.y;
						p2.isBoostJumping = true;
						p2.airborne = true;
						p2.grounded = false;
						App->audio->PlayFx(3, 0);
					}
				}
				LOG("P2 IS COLLIDING WITH P1");
			}
		}

		//Player colliding against solids
		if (C1->type == PLAYER && C2->type == SOLID)
		{
			//Player Colliding from Above the Solid. The first part checks the y axis and the second and third satements check that P1 is inside the bounds.
			if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x)
			{
				p2.speed.y = 0;
				p2.position.y = C2->collider.y - C1->collider.h;
				p2.isJumping = false;
				p2.isBoostJumping = false;
				p2.grounded = true;

				LOG("P2 IS COLLIDING WITH SOLID FROM AVOBE");
			}

			//Player Colliding from below the Solid
			if (p2.previous_position.y > C2->collider.y + C2->collider.h)
			{
				p2.speed.y = 0;
				p2.position.y = C2->collider.y + C2->collider.h;
				LOG("P2 IS COLLIDING WITH SOLID FROM BELOW");
			}

			////Player is colliding from left (going right)
			//if (C1->collider.x + C1->collider.w > C2->collider.x && C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h > C2->collider.y)
			//{
			//	p2.speed.x = 0;
			//	p2.position.x = C2->collider.x + C2->collider.w;
			//	LOG("P1 IS COLLIDING WITH SOLID FROM THE RIGHT");
			//}

			////Player is colliding from right (going left)
			//if (C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h > C2->collider.y)
			//{
			//	p2.speed.x = 0;
			//	p2.position.x = C2->collider.x - C1->collider.w;
			//	LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
		}

		if (C1->type == PLAYER && C2->type == PLATFORM)
		{

		}

		if (C1->type == PLAYER && C2->type == HAZARD)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w || C1->collider.y < C2->collider.y + C2->collider.h || C1->collider.y + C1->collider.h > C2->collider.y)
			{
				//Death logic
				p2.lives--;
				//Antes la animacion de muerte tiene que haber finalizado
				RespawnP2ToP1();
			}
		}

		//Player Colliding against an Activable Item
		if (C1->type == PLAYER && C2->type == ITEM)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w)
			{
				p2.item_activated = true;
				App->player1->p1.item_activated = true;

				//Assign fx --> An activating Beep and the  sound  of  a lock being opened?
				App->audio->PlayFx(4, 1);
			}
		}

		//Player colliding against Deactivable surfaces. 
		if (C1->type == PLAYER && C2->type == DEACTIVABLE)
		{
			if (p2.item_activated == false || App->player1->p1.item_activated == false)
			{
				if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x)
				{
					p2.speed.y = 0;
					p2.position.y = C2->collider.y - C1->collider.h;
					p2.isJumping = false;
					p2.isBoostJumping = false;
					p2.grounded = true;
					LOG("P2 IS COLLIDING WITH SOLID FROM AVOBE");
				}
			}
		}

		//Player colliding against the Goal
		if (C1->type == PLAYER && C2->type == GOAL)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w)
			{
				if (C1->collider.y > GOAL_Y && C1->collider.y < GOAL_HEIGHT)
				{
					App->fadescene->FadeToBlack("1st_Level.tmx");
					App->map->Restart_Cam();
				}
				else
				{
					App->fadescene->FadeToBlack("Tutorial_Level.tmx");
					App->map->Restart_Cam();
				}

				//Assign Fx --> A teleporting or warping sound.
				App->audio->PlayFx(6, 0);
			}
		}
	}
}

bool j1Player2::Load(pugi::xml_node& data)
{
	p2.position.x = data.child("position2").attribute("x").as_int();
	p2.position.y = data.child("position2").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Player2::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position2");

	pos.append_attribute("x") = p2.position.x;
	pos.append_attribute("y") = p2.position.y;

	return true;
}

bool j1Player2::LoadPlayer2()		//Loads P2 on screen.
{
	//Loads the textures of P2. Switches them according to switch_sprites
	p2.texture = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2_Buena.png");
	
	//--------------------------------------------Loading the data and colliders of P2--------------------------------------------
	//Loads the position of P2 from the xml.
	p2.position.x = p2.position.x;
	p2.position.y = p2.position.y;

	//p2.spawn_position = p2.position;	//Sets the respawn position to the first position the player was in the map. 

	//Loads the data of the rectangle that contains P2.
	p2.HitBox.x = p2.position.x;
	p2.HitBox.y = p2.position.y;
	p2.HitBox.w = p2.sprite_width;
	p2.HitBox.h = p2.sprite_height;

	//Adds a collider for the player.
	p2.collider = App->collisions->AddCollider(p2.HitBox, PLAYER, this);

	//Boolean resetting
	p2.grounded = false;
	p2.flip = false;
	p2.isCrouching = false;
	p2.isJumping = false;
	p2.isBoostJumping = false;
	p2.item_activated = false;
	p2.isGoingRight = false;
	p2.isGoingLeft = false;
	p2.platformDrop = false;
	p2.fading = false;
	p2.isAlive = true;
	p2.isDying = false;
	p2.GodMode = false;
	p2.switch_sprites = false;

	return true;
}

/*bool j1Player2::LoadPlayer2Textures()
{
	//Loads the textures of P2. Switches them according to switch_sprites
	if (p2.switch_sprites == true)
	{
		p2.texture = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2_Buena.png");
	}
	else
	{
		p2.texture = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");
	}

	return true;
}*/

void j1Player2::Restart()
{
	p2.position = p2.spawn_position;
	p2.isAlive = true;
}

void j1Player2::GodModeInput()
{
	p2.airborne = false;
	
	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
	{
		p2.position.x += GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
	{
		p2.position.x -= GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
	{
		p2.position.y -= GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)
	{
		p2.position.y += GOD_MODE_SPEED;
	}
}