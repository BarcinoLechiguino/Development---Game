#include "j1App.h"
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

j1Player1::j1Player1() //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	name.create("player"); //The string has to be the same as the name of the node in the xml file.
	
	//P1 Idle animation.
	p1.idle.PushBack({ 28, 14, 37, 57 });
	p1.idle.PushBack({ 132, 12, 33, 59 });
	p1.idle.PushBack({ 230, 12, 37, 59 });
	p1.idle.PushBack({ 326, 14, 39, 57 });
	p1.idle.speed = 0.1f;
	
	//P1 Running animation.
	p1.running.PushBack({ 134, 90, 39, 55 });
	p1.running.PushBack({ 232, 92, 39, 53 });
	p1.running.PushBack({ 332, 96, 39, 49 });
	p1.running.PushBack({ 434, 90, 45, 55 });
	p1.running.PushBack({ 532, 92, 39, 53 });
	p1.running.PushBack({ 632, 96, 39, 49 });
	p1.running.speed = 0.2f;

	//P1 Crouching animation.
	/*p1.crouching.PushBack({ 30, 172, 39, 47 });
	p1.crouching.PushBack({ 130, 176, 39, 43 });*/
	p1.crouching.PushBack({ 432, 10, 37, 61 }); //20 Pixels of margin vertically up.
	p1.crouching.PushBack({ 530, 8, 39, 63 });
	p1.crouching.PushBack({ 630, 8, 37, 63 });
	p1.crouching.PushBack({ 34, 84, 33, 61 });
	p1.crouching.speed = 0.1f;

	//P1 Jumping Animation
	p1.jumping.PushBack({ 30, 172, 39, 47 });
	p1.jumping.PushBack({ 130, 176, 39, 43 });
	p1.jumping.PushBack({ 234, 162, 37, 53 });
	p1.jumping.PushBack({ 328, 158, 41, 45 });
	/*p1.jumping.PushBack({ 436, 162, 29, 41 });*/ //Cape coiling up a bit before falling. A bit glitchy.
	p1.jumping.speed = 0.25f;

	//P1 Frontflip Animation
	p1.frontflip.PushBack({ 528, 168, 47, 33 });
	p1.frontflip.PushBack({ 528, 168, 47, 33 });
	p1.frontflip.PushBack({ 640, 168, 35, 41 });
	p1.frontflip.PushBack({ 640, 168, 35, 41 });
	p1.frontflip.PushBack({ 22, 248, 51, 33 });
	p1.frontflip.PushBack({ 22, 248, 51, 33 });
	p1.frontflip.speed = 0.2f;

	//P1 Boosted Jump Animation
	p1.boosted_jump.PushBack({ 30, 172, 39, 47 });
	p1.boosted_jump.PushBack({ 130, 176, 39, 43 });
	p1.boosted_jump.PushBack({ 234, 162, 37, 53 });
	p1.boosted_jump.PushBack({ 328, 158, 41, 45 });
	p1.boosted_jump.PushBack({ 436, 162, 29, 41 });
	p1.boosted_jump.PushBack({ 528, 168, 47, 33 });
	p1.boosted_jump.PushBack({ 640, 168, 35, 41 });
	p1.boosted_jump.PushBack({ 22, 248, 51, 33 });
	p1.boosted_jump.speed = 0.2f;

	//P1 Falling Animation
	p1.falling.PushBack({ 136, 224, 33, 61 });
	p1.falling.PushBack({ 236, 226, 33, 59 });
	p1.falling.speed = 0.2f;

	//P1 Death Animation
	p1.death.PushBack({ 0,0,70,64 });
	p1.death.PushBack({ 70,0,70,64 });
	p1.death.PushBack({ 140,0,70,64 });
	p1.death.PushBack({ 210,0,70,64 });
	p1.death.PushBack({ 280,0,70,64 });
	p1.death.PushBack({ 350,0,70,64 });
	p1.death.PushBack({ 420,0,70,64 });
	p1.death.speed = 0.2f;
};

j1Player1::~j1Player1()  //Destructor. Called at the last frame.
{

};

bool j1Player1::Init() 
{
	return true;
};

bool j1Player1::Awake(pugi::xml_node& config) 
{	

	//Gets all the required player variables from the config xml file
	p1.position.x = config.child("player_1").child("position").attribute("x").as_float();
	p1.position.y = config.child("player_1").child("position").attribute("y").as_float();

	p1.speed.x = config.child("player_1").child("speed").attribute("x").as_float();
	p1.speed.y = config.child("player_1").child("speed").attribute("y").as_float();
	p1.max_speed.x = config.child("player_1").child("max_speed").attribute("x").as_float();
	p1.max_speed.y = config.child("player_1").child("max_speed").attribute("y").as_float();

	p1.acceleration.x = config.child("player_1").child("acceleration").attribute("x").as_float();
	p1.acceleration.y = config.child("player_1").child("acceleration").attribute("y").as_float();
	p1.gravity = config.child("player_1").child("gravity").attribute("value").as_float();

	p1.boost_jump.x = config.child("player_1").child("boost_jump").attribute("x").as_float();
	p1.boost_jump.y = config.child("player_1").child("boost_jump").attribute("y").as_float();

	/*p1.jumpFX = config.child("jumpFX").attribute("name").as_string();
	p1.deathFX = config.child("deathFX").attribute("name").as_string();
	p1.duoFX = config.child("landFX").attribute("name").as_string();*/

	//Sets the first cycle of animations to the idle set.
	p1.current_animation = &p1.idle;
	

	return true;
};

bool j1Player1::Start() 
{
	LoadPlayer1();

	jumpFX = App->audio->LoadFx("audio/fx/Jump.wav");
	deathFX = App->audio->LoadFx("audio/fx/Death.wav");
	duoFX = App->audio->LoadFx("audio/fx/Jump_Duo.wav");
	activateFX = App->audio->LoadFx("audio/fx/Activate.wav");
	tpFX = App->audio->LoadFx("audio/fx/TP.wav");
	passFX = App->audio->LoadFx("audio/fx/Pass.wav");

	p1.isGrounded(true);
	p1.item_activated = false;

	p1.state = idle_P1;

	player1_alive = true;

	return true;
};

bool j1Player1::PreUpdate() 
{
	/*p1.isGrounded(false); //set  to false when colliders are implemented.
	
	if (p1.state != jumping_P1 && p1.state != falling_P1)
	{
		p1.state = idle_P1;
		p1.grounded = true;
	}*/
	
	p1.state = idle_P1;

	if (!GodMode)
	{

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			p1.state = goingRight_P1;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			p1.state = goingLeft_P1;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			p1.state = crouching_P1;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			p1.state = jumping_P1;
			App->audio->PlayFx(1, 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			p1.state = teleporting_P1;
			App->audio->PlayFx(5, 0);
		}

		//if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN ) //first player dies
		//{
		//	//Death logic
		//	lives--;
		//	//Antes la animacion de muerte tiene que haber finalizado
		//	RespawnP1ToP2();
		//	
		//}
		//if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) //second player dies
		//{
		//	//Death logic
		//	lives--;
		//	TeleportP2ToP1();
		//}

		/*if (lives == 0) 
		{
			player1_alive = false;
			App->player2->player2_alive = false;

			if (player1_alive == false && App->player2->player2_alive == false) 
			{
				Restart();
				App->player2->Restart();
				lives = 3;
			}
		}*/
	}
	else
	{
		GodModeInput();
	}
	return true;
};

bool j1Player1::Update(float dt) 
{
	p1_frames++;

	p1.previous_position = p1.position;
	
	switch (p1.state)
	{
	
	case idle_P1:
	
		p1.speed.x = 0;
		p1.current_animation = &p1.idle;
		p1.isCrouching = false;

		break;
	
	case goingRight_P1:
	
		LOG("P2 GOING RIGHT %d %d", p1.speed.x, p1.max_speed.x);
		
		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != p1.max_speed.x)
		{
			p1.speed.x += p1.acceleration.x;
		}
	
		p1.position.x += p1.speed.x; //p1.speed_x is positive here.

		p1.flip = false;
		p1.current_animation = &p1.running;
		p1.moving_right = true;
	
		LOG("P1 Position %d %d", p1.position.x, p1.position.y);
	
		break;
	
	case goingLeft_P1:
	
		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != -p1.max_speed.x)
		{
			p1.speed.x -= p1.acceleration.x;
		}
	
		p1.position.x += p1.speed.x;  //p1.speed_x  is negative here.

		p1.flip = true;
		p1.current_animation = &p1.running;
		p1.moving_left = true;
	
		break;
	
	case crouching_P1:

		p1.current_animation = &p1.crouching;
		p1.isCrouching = true;


		break;

	case jumping_P1:
	
		if (p1.grounded == true )
		{
			p1.speed.y = -p1.acceleration.y;
			App->audio->PlayFx(jumpFX);
			p1.isJumping = true;
			p1.isGrounded(false);
		}

		break;

	case falling_P1:
		
		p1.grounded = false;

		break;

	case teleporting_P1:

		TeleportP2ToP1();
		

		break;
	}
	
	//If the p1 is in the air then this function brings him/her back down to the floor.
	if (p1.grounded == false)
	{	
		p1.speed.y += p1.gravity;
		
		if (p1.speed.y > p1.max_speed.y)
		{
			p1.speed.y = p1.max_speed.y;
		}
	
		p1.position.y += p1.speed.y;

		//Jump animation modifications.
		if (p1.isBoostJumping == true)				//If P1 is boost jumping then this set of animations is played.
		{
			if (p1.speed.y < FRONTFLIP_START)
			{
				p1.current_animation = &p1.jumping;
			}
			else if (p1.speed.y < FRONTFLIP_END)
			{
				p1.current_animation = &p1.frontflip;
			}
			else
			{
				p1.current_animation = &p1.falling;
			}
		}
		else if(p1.isJumping == true)				//If P1 is jumping then this set of animations is played.
		{
			if (p1.speed.y < APEX)
			{
				p1.current_animation = &p1.jumping;
			}
			else
			{
				p1.current_animation = &p1.falling;
			}
		}
		else
		{
			p1.current_animation = &p1.falling;		//Set the last case to falling in case something goes wrong.
		}
		
	}

	//We move the character according the position value after the state has been run.
	p1.HitBox.x = p1.position.x; 
	p1.HitBox.y = p1.position.y;

	p1.HitBox = p1.current_animation->GetCurrentFrame();
	
	App->render->Blit(p1.texture, p1.position.x, p1.position.y, &p1.HitBox, p1.flip);
	
	//Makes P1's collider follow P1. If there is a flip in spritesheet it is taken into account.
	if (p1.flip == false)
	{
		p1.collider->Set_Position(p1.position.x, p1.position.y);
	}
	else
	{
		p1.collider->Set_Position(p1.position.x + 22, p1.position.y);
	}

	//If player dies:
	if (lives == 0)
	{
		player1_alive = false;
		App->player2->player2_alive = false;

		if (player1_alive == false && App->player2->player2_alive == false)
		{
			Restart();
			App->player2->Restart();
			lives = 3;
		}
	}

	//Draws the HitBox on-screen.
	//App->render->DrawQuad(p1.HitBox_Alpha, 255, 0, 0, 200);

	return true;
};

bool j1Player1::PostUpdate() 
{
	return true;
};

bool j1Player1::CleanUp() 
{
	App->tex->UnLoad(p1.texture);
	return true;
};

void j1Player1::TeleportP2ToP1()	//Method that teleports P2 directly in front of P1. Takes into account which direction P1 is facing. Can trigger some fun interactions between players :)
{
	if (p1.flip == false)			//The players will be always teleported directly in front of one another. 
	{
		App->player2->p2.position.x = p1.position.x + p1.collider->collider.w;
		App->player2->p2.position.y = p1.position.y;
	}
	else
	{
		App->player2->p2.position.x = p1.position.x - p1.collider->collider.w / 2;
		App->player2->p2.position.y = p1.position.y;
	}
}

void j1Player1::RespawnP1ToP2()		//Method that, on death, will respawn P1 behind P2.
{
	if (p1.flip == true)			//The players will be always respawned directly behind of one another. 
	{
		p1.position.x = App->player2->p2.position.x + App->player2->p2.collider->collider.w;
		p1.position.y = App->player2->p2.position.y;
	}
	else
	{
		p1.position.x = App->player2->p2.position.x + App->player2->p2.collider->collider.w / 2;
		p1.position.y = App->player2->p2.position.y;
	}
}

//Collision Handling ---------------------------------------
void j1Player1::OnCollision(Collider* C1, Collider* C2)
{
	if (GodMode == false)
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

		//Player Colliding Against Another Player
		if (C1->type == PLAYER && C2->type == PLAYER)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w  /*&& p1.speed.x == p1.max_speed.x*/) //As the boost can be done even if P1 is static, this allows for more precise jumps... hopefully.
			{
				if (App->player2->p2.state == crouching_P2 /*App->player2->p2.isCrouching == true*/)
				{
					if (p1.grounded == true)
					{
						p1.speed.y -= p1.boost_jump.y;
						p1.isBoostJumping = true;
						p1.grounded = false;
					}
				}
				LOG("P1 IS COLLIDING WITH P2 FROM THE LEFT");
			}
		}
		
		//Player colliding against solids
		if (C1->type == PLAYER && C2->type == SOLID)
		{
			//Player Colliding from Above the Solid. The first part checks the y axis and the second and third satements check that P1 is inside the bounds.
			if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x)
			{
				p1.speed.y = 0;
				p1.position.y = C2->collider.y - C1->collider.h;
				p1.isJumping = false;
				p1.isBoostJumping = false;
				p1.grounded = true;
				LOG("P1 IS COLLIDING WITH SOLID FROM AVOBE");

				/*if (p1.grounded == false)
				{
					p1.speed.y = 0;
					p1.position.y = C2->collider.y - C1->collider.h;
					p1.grounded = true;
					LOG("P1 IS COLLIDING WITH SOLID FROM AVOBE");
				}*/

				/*if (p1.speed.y > 0)
				{
					p1.speed.y = 0;
				}

				p1.position.y = C2->collider.y - p1.collider->collider.h;

				p1.grounded = true*/;
			}

			//Player Colliding from below the Solid
			if (C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x)
			{
				/*p1.speed.y = 0;
				p1.position.y = C2->collider.y + C2->collider.h;
				LOG("P1 IS COLLIDING WITH SOLID FROM BELOW");*/
			}

			//Player is colliding from left (going right)
			if (C1->collider.x + C1->collider.w > C2->collider.x && C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h > C2->collider.y)
			{
				p1.speed.x = 0;
				p1.position.x = C2->collider.x + C2->collider.w;
				LOG("P1 IS COLLIDING WITH SOLID FROM THE RIGHT");
			}

			//Player is colliding from right (going left)
			if (C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h > C2->collider.y)
			{
				p1.speed.x = 0;
				p1.position.x = C2->collider.x - C1->collider.w;
				LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
			}
		}

		if (C1->type == PLAYER && C2->type == PLATFORM)
		{

		}

		if (C1->type == PLAYER && C2->type == HAZARD)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w || C1->collider.y < C2->collider.y + C2->collider.h || C1->collider.y + C1->collider.h > C2->collider.y)
			{
				//Death logic
				lives--;
				//Antes la animacion de muerte tiene que haber finalizado
				RespawnP1ToP2();
			}
		}

		//Player Colliding against an Activable Item
		if (C1->type == PLAYER && C2->type == ITEM)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w)
			{
				p1.item_activated = true;
				App->player2->p2.item_activated = true;

				//Assign fx --> An activating Beep and the  sound  of  a lock being opened?
				App->audio->PlayFx(4, 1);
			}
		}

		//Player colliding against Deactivable surfaces. 
		if (C1->type == PLAYER && C2->type == DEACTIVABLE)
		{
			if (p1.item_activated == false || App->player2->p2.item_activated == false)
			{
				if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x)
				{
					p1.speed.y = 0;
					p1.position.y = C2->collider.y - C1->collider.h;
					p1.isJumping = false;
					p1.isBoostJumping = false;
					p1.grounded = true;
					LOG("P1 IS COLLIDING WITH SOLID FROM AVOBE");

					/*if (p1.grounded == false)
					{
						p1.speed.y = 0;
						p1.position.y = C2->collider.y - C1->collider.h;
						p1.grounded = true;
						LOG("P1 IS COLLIDING WITH SOLID FROM AVOBE");
					}*/

					/*if (p1.speed.y > 0)
					{
						p1.speed.y = 0;
					}

					p1.position.y = C2->collider.y - p1.collider->collider.h;

					p1.grounded = true*/;
				}

				//Player Colliding from below the Solid
				/*if (p1.position.y < C2->collider.y + C2->collider.h)
				{
					p1.speed.y = 0;
					p1.position.y = C2->collider.y + C2->collider.h;
					LOG("P1 IS COLLIDING WITH SOLID FROM BELOW");
				}*/

				//Player is colliding from right (going left)
				//if (p1.position.x < C2->collider.x + C2->collider.w)
				//{
				//	p1.speed.x = 0;
				//	p1.position.x = C2->collider.x + C2->collider.w;
				//	LOG("P1 IS COLLIDING WITH SOLID FROM THE RIGHT");
				//}

				////Player is colliding from left (going right)
				//if (p1.position.x + C1->collider.w < C2->collider.x)
				//{
				//	p1.speed.x = 0;
				//	p1.position.x = C2->collider.x - C1->collider.w;
				//	LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
				//}
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
				}
				else
				{
					App->fadescene->FadeToBlack("Tutorial_Level.tmx");
				}

				//Assign Fx --> A teleporting or warping sound.
				App->audio->PlayFx(6, 0);
			}
		}
	}
	
}

bool j1Player1::Load(pugi::xml_node& data)
{
	p1.position.x = data.child("position1").attribute("x").as_int();	//Loads the position of the X axis saved in the save_file.xml.
	p1.position.y = data.child("position1").attribute("y").as_int();	//Loads the position of the X axis saved in the save_file.xml.
	return true;
}

bool j1Player1::Save(pugi::xml_node&  data) const
{
	pugi::xml_node pos = data.append_child("position1");	//Declares the node and with "append_" it is set to overwrite the data in the given xml. 

	pos.append_attribute("x") = p1.position.x;				//Saves the position of P1 on  the X axis the moment Save() is called. "append_" used again to overwrite previous position data.
	pos.append_attribute("y") = p1.position.y;				//Saves the position of P1 on  the Y axis the moment Save() is called. "append_" used again to overwrite previous position data.
	return true;
}

bool j1Player1::LoadPlayer1()		//Loads P1 on screen.
{
	//Loads the textures of P1
	p1.texture = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");
	p1.texture2 = App->tex->Load("textures/Spritesheets/Character 1/adventurer-hand-combat-Sheet.png");
	
	//Loads the data and colliders of P1
	p1.position = { p1.position.x, p1.position.y };												//Loads the position of P1 from the xml.
	p1.spawn_position = p1.position;													//Sets the respawn position to the first position the player was in the map. 
	p1.HitBox = { (int)p1.position.x,(int)p1.position.y, p1.sprite_width, p1.sprite_height };	//Casked to int as the values passed by p2.position are floats.
	p1.collider = App->collisions->AddCollider(p1.HitBox, PLAYER, this);						//Adds a collider for the player.

	return true;
}

void j1Player1::Restart()
{
	p1.position = p1.spawn_position;
	player1_alive = true;
}

void j1Player1::GodModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		p1.position.x += GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		p1.position.x -= GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		p1.position.y -= GOD_MODE_SPEED;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		p1.position.y += GOD_MODE_SPEED;
	}
}

