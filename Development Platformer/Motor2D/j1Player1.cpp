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
	
	AddAnimationPushbacks();
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
	LoadPlayer1Properties(config);

	//Sets the first cycle of animations to the idle set.
	p1.current_animation = &p1.idle;
	

	return true;
};

bool j1Player1::Start() 
{
	LoadPlayer1();					//Loads P1 in game.
	//LoadPlayer1Textures();		//Loads P1's textures in game.

	p1.jumpFX		= App->audio->LoadFx("audio/fx/Jump.wav");
	p1.deathFX		= App->audio->LoadFx("audio/fx/Death.wav");
	p1.duoFX		= App->audio->LoadFx("audio/fx/Jump_Duo.wav");
	p1.activateFX	= App->audio->LoadFx("audio/fx/Activate.wav");
	p1.tpFX			= App->audio->LoadFx("audio/fx/TP.wav");
	p1.goalFX		= App->audio->LoadFx("audio/fx/Pass.wav");

	p1.airborne	= true;
	p1.item_activated = false;

	p1.state = idle_P1;

	return true;
};

bool j1Player1::PreUpdate() 
{
	if (p1.GodMode == false)
	{
		p1.state = idle_P1;
		
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)				//Move Right
		{
			if (p1.againstLeftWall == false)
			{
				p1.state = goingRight_P1;
			}	

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)				//Move Left
		{
			if (p1.againstRightWall == false)
			{
				p1.state = goingLeft_P1;
			}
			else
			{
				p1.state = idle_P1;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)				//Crouch
		{
			p1.state = crouching_P1;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)				//Jump
		{
			p1.state = jumping_P1;
			App->audio->PlayFx(5, 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)				//Drop from platform
		{
			p1.platformDrop = true;
		}
		else
		{
			p1.platformDrop = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)					//Teleport
		{
			p1.state = teleporting_P1;
			App->audio->PlayFx(1, 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)				//Suicide Button
		{
			p1.state = dying_P1;
			App->audio->PlayFx(2, 0);
		}
	}
	else
	{
		GodModeInput();														//GodMode 
	}

	//Switch Sprites Method Call
	/*if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->tex->UnLoad(p1.texture);
		LoadPlayer1Textures();
	}*/

	return true;
};

bool j1Player1::Update(float dt) 
{
	switch (p1.state)
	{
	
	case idle_P1:
	
		p1.speed.x = 0;
		p1.current_animation = &p1.idle;
		p1.isCrouching = false;

		break;
	
	case goingRight_P1:

		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != p1.max_speed.x)
		{
			p1.speed.x += p1.acceleration.x;
		}

		p1.flip = false;
		p1.current_animation = &p1.running;
		p1.isGoingRight = true;
	
		break;
	
	case goingLeft_P1:
	
		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != -p1.max_speed.x)
		{
			p1.speed.x -= p1.acceleration.x;
		}

		p1.flip = true;
		p1.current_animation = &p1.running;
		p1.isGoingLeft = true;
	
		break;
	
	case crouching_P1:

		p1.current_animation = &p1.crouching;
		p1.speed.x = 0;
		p1.isCrouching = true;

		break;

	case jumping_P1:
	
		if (p1.grounded == true)
		{
			p1.speed.y = -p1.acceleration.y;
			p1.isJumping = true;				//Boolean for animations
			p1.airborne = true;	
			p1.grounded = false;
		}

		break;

	case falling_P1:		//When dropping from platforms
		
		p1.airborne = true;
		p1.grounded = true; //No jumping

		break;

	case teleporting_P1:

		TeleportP2ToP1();
		
		break;

	case dying_P1:
			
		p1.current_animation = &p1.death;
		p1.isDying = true;

		break;
	}

	p1.position.x += p1.speed.x;		//Refreshes the vector speed of P1 in the X axis
								 
	//If P1 is in the air then this function brings him/her back down to the floor.
	if (p1.airborne == true)
	{	
		p1.speed.y += p1.gravity;
		
		if (p1.speed.y > p1.max_speed.y)
		{
			p1.speed.y = p1.max_speed.y;
		}
	
		p1.position.y += p1.speed.y;				//Refreshes the vector speed of P1 in the Y axis.

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
		else if (p1.isJumping == true)				//If P1 is jumping then this set of animations is played.
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
	}

	//Makes P1's collider follow P1. If there is a flip in spritesheet it is taken into account.
	if (p1.flip == false)
	{
		p1.collider->Set_Position(p1.position.x, p1.position.y);
	}
	else
	{
		p1.collider->Set_Position(p1.position.x + FLIP_MARGIN, p1.position.y);
	}

	// If P1 empties the lives pool then both players die and reset their positions to the spawn point.
	if (p1.lives == 0)
	{
		p1.isAlive = false;
		App->player2->p2.isAlive = false;

		if (p1.isAlive == false && App->player2->p2.isAlive == false)
		{
			App->audio->PlayFx(2, 0);
			Restart();
			App->player2->Restart();
			p1.lives = p1.max_lives;
		}
	}
	
	//We move the character according the position value after the state has been run.
	p1.HitBox.x = p1.position.x;															//Sets the position X of the Rect that represents P1 in game.
	p1.HitBox.y = p1.position.y;															//Sets the position Y of the Rect that represents P2 in game.

	p1.HitBox = p1.current_animation->GetCurrentFrame();									//Sets the animation cycle that P1 will have. 
	
	App->render->Blit(p1.texture, p1.position.x, p1.position.y, &p1.HitBox, p1.flip);		//Blits the player on screen with the data we have given the Blit() function.
	
	return true;
};

bool j1Player1::PostUpdate() 
{
	//Resetting the collider related bools after the collision has happened.
	p1.againstCeiling = false;
	p1.againstRightWall = false;
	p1.againstLeftWall = false;
	
	return true;
};

bool j1Player1::CleanUp() 
{
	App->tex->UnLoad(p1.texture);
	App->player1->Disable();

	return true;
};

void j1Player1::TeleportP2ToP1()	//Method that teleports P2 directly in front of P1. Takes into account which direction P1 is facing. Can trigger some fun interactions between players :)
{
	if (p1.flip == false)			//The players will be always teleported directly in front of one another. 
	{
		App->player2->p2.position.x = p1.position.x + p1.collider->collider.w;
		App->player2->p2.position.y = p1.position.y - 60;
	}
	else
	{
		App->player2->p2.position.x = p1.position.x - p1.collider->collider.w / 2;
		App->player2->p2.position.y = p1.position.y - 60;
	}
}

void j1Player1::RespawnP1ToP2()		//Method that, on death, will respawn P1 behind P2.
{
	if (p1.flip == true)			//The players will be always respawned directly behind of one another. 
	{
		p1.position.x = App->player2->p2.position.x + App->player2->p2.collider->collider.w;
		p1.position.y = App->player2->p2.position.y - 40;
	}
	else
	{
		p1.position.x = App->player2->p2.position.x + App->player2->p2.collider->collider.w / 2;
		p1.position.y = App->player2->p2.position.y - 40;
	}
}

//Collision Handling ---------------------------------------
void j1Player1::OnCollision(Collider* C1, Collider* C2)
{
	if (p1.GodMode == false)
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
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w ) //As the boost can be done even if P1 is static, this allows for more precise jumps... hopefully.
			{
				if (App->player2->p2.state == crouching_P2 /*App->player2->p2.isCrouching == true*/)
				{
					if (p1.grounded == true)
					{
						p1.speed.y -= p1.boost_jump.y;
						p1.isBoostJumping = true;
						p1.airborne = true;
						p1.grounded = false;
						App->audio->PlayFx(3, 0);
					}
				}
				LOG("P1 IS COLLIDING WITH P2 FROM THE LEFT");
			}
		}
		
		//Player colliding against solids
		if (C1->type == PLAYER && C2->type == SOLID)
		{
			//Player Colliding vertically against the Solid. The first part checks if C1 is contained in the X axis of C2. 
			if (C1->collider.x + C1->collider.w > C2->collider.x + p1.collision_tolerance && C1->collider.x + p1.collision_tolerance < C2->collider.x + C2->collider.w)
			{
				//Player Colliding from Above the Solid, ergo colliding with the ground. This second part checks if C1 is actually colliding vertically down.
				if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.y < C2->collider.y)
				{
					p1.speed.y = 0;
					p1.position.y = C2->collider.y - C1->collider.h;
					p1.isJumping = false;
					p1.isBoostJumping = false;
					p1.grounded = true;
					LOG("P1 IS COLLIDING WITH SOLID FROM ABOVE");
				}

				//Player Colliding from Below the Solid. ergo colliding with the ceiling. This second part checks if C1 is actually colliding vertically down.
				else
				{
					if (C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + (C1->collider.h * half) > C2->collider.y + C2->collider.h && p1.speed.y < 0)
					{
						p1.speed.y = 0;
						p1.position.y = C2->collider.y + C2->collider.h;
						p1.againstCeiling = true;
						LOG("P1 IS COLLIDING WITH SOLID FROM BELOW");
					}
				}
			}

			//Player is colliding from the sides. The first part checks if C1 is contained in  the Y axis of C2.
			if (C1->collider.y + C1->collider.h > C2->collider.y + p1.collision_tolerance && C1->collider.y  < C2->collider.y + C2->collider.h)
			{
				LOG("P1 IS COLLIDING WITH SOLID FROM THE SIDES");

				//Player is colliding from left (going right). This second part checks if C1 is actually colliding from the left side of the collider. Additionally it checks whether or not P1 has collided against the ceiling.
				if (C1->collider.x + C1->collider.w > C2->collider.x && C1->collider.x < C2->collider.x && p1.againstCeiling == false)
				{
					//This third part checks whether P1 is at a certain distance in the collider. It checks if three fourths of P1's width is not actually colliding.    
					if (C1->collider.x + (C1->collider.w * half) < C2->collider.x)
					{
						p1.againstLeftWall = true;
						//p1.position.x = C2->collider.x - C2->collider.w - 1;
						p1.position.x = C2->collider.x - p1.HitBox.w;
						LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
					}
				}

				//Player is colliding from right (going left). This second part checks if  C1 is actually colliding from the right side of the collider.
				if (C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x + C2->collider.w && p1.againstCeiling == false)
				{
					//This third part checks whether P1 is at a certain distance in the collider. It checks if a quarter of P1's width is still colliding. 
					if (C1->collider.x + (C1->collider.w * quarter) > C2->collider.x + C2->collider.w)
					{
						p1.againstRightWall = true;
						p1.position.x = C2->collider.x + C2->collider.w - p1.collision_tolerance;
						//p1.position.x = C2->collider.x + C2->collider.w - p1.HitBox.w + 20;
						LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
					}
				}
			}
		}

		if (C1->type == PLAYER && C2->type == PLATFORM)
		{
			if (p1.platformDrop == false)
			{
				if (C1->collider.x + C1->collider.w > C2->collider.x + p1.collision_tolerance && C1->collider.x + p1.collision_tolerance < C2->collider.x + C2->collider.w)
				{
					//Player Colliding from Above the Platform, ergo colliding with the ground. This second part checks if C1 is actually colliding vertically down.
					if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.y < C2->collider.y)
					{
						p1.speed.y = 0;
						p1.position.y = C2->collider.y - C1->collider.h;
						p1.isJumping = false;
						p1.isBoostJumping = false;
						p1.grounded = true;
						LOG("P1 IS COLLIDING WITH PLATFORM FROM ABOVE");
					}
				}
			}
		}

		if (C1->type == PLAYER && C2->type == HAZARD)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w || C1->collider.y < C2->collider.y + C2->collider.h || C1->collider.y + C1->collider.h > C2->collider.y)
			{
				//Death logic
				p1.lives--;

				p1.state = dying_P1;
				
				//Antes la animacion de muerte tiene que haber finalizado
				RespawnP1ToP2();
			}
		}

		//Player Colliding against an Activable Item
		if (C1->type == PLAYER && C2->type == ITEM)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w) //Just a general detection of collisions, as what only matters is that it activates.
			{
				p1.item_activated = true;					//Records that P1 (or P2) has activated the item.
				App->player2->p2.item_activated = true;		//Activates P2's boolean as well.

				App->audio->PlayFx(4, 1);					//Item Activation sfx.
			}
		}

		//Player colliding against Deactivable surfaces. 
		if (C1->type == PLAYER && C2->type == DEACTIVABLE)
		{
			if (p1.item_activated == false || App->player2->p2.item_activated == false)
			{
				//Player Colliding vertically against the Solid. The first part checks if C1 is contained in the X axis of C2. 
				if (C1->collider.x + C1->collider.w > C2->collider.x + p1.collision_tolerance && C1->collider.x + p1.collision_tolerance < C2->collider.x + C2->collider.w)
				{
					//Player Colliding from Above the Solid, ergo colliding with the ground. This second part checks if C1 is actually colliding vertically down.
					if (C1->collider.y + C1->collider.h > C2->collider.y && C1->collider.y < C2->collider.y)
					{
						p1.speed.y = 0;
						p1.position.y = C2->collider.y - C1->collider.h;
						p1.isJumping = false;
						p1.isBoostJumping = false;
						p1.grounded = true;
						LOG("P1 IS COLLIDING WITH SOLID FROM ABOVE");
					}

					//Player Colliding from Below the Solid. ergo colliding with the ceiling. This second part checks if C1 is actually colliding vertically down.
					else
					{
						if (C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + (C1->collider.h * half) > C2->collider.y + C2->collider.h && p1.speed.y < 0)
						{
							p1.speed.y = 0;
							p1.position.y = C2->collider.y + C2->collider.h;
							p1.againstCeiling = true;
							LOG("P1 IS COLLIDING WITH SOLID FROM BELOW");
						}
					}
				}

				//Player is colliding from the sides. The first part checks if C1 is contained in  the Y axis of C2.
				if (C1->collider.y + C1->collider.h > C2->collider.y + p1.collision_tolerance && C1->collider.y < C2->collider.y + C2->collider.h)
				{
					LOG("P1 IS COLLIDING WITH SOLID FROM THE SIDES");

					//Player is colliding from left (going right). This second part checks if C1 is actually colliding from the left side of the collider. Additionally it checks whether or not P1 has collided against the ceiling.
					if (C1->collider.x + C1->collider.w > C2->collider.x && C1->collider.x < C2->collider.x && p1.againstCeiling == false)
					{
						//This third part checks whether P1 is at a certain distance in the collider. It checks if three fourths of P1's width is not actually colliding.    
						if (C1->collider.x + (C1->collider.w * threeFourths) < C2->collider.x)
						{
							p1.againstLeftWall = true;
							p1.position.x = C2->collider.x - C2->collider.w - 1;
							LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
						}
					}

					//Player is colliding from right (going left). This second part checks if  C1 is actually colliding from the right side of the collider.
					if (C1->collider.x < C2->collider.x + C2->collider.w && C1->collider.x + C1->collider.w > C2->collider.x + C2->collider.w && p1.againstCeiling == false)
					{
						//This third part checks whether P1 is at a certain distance in the collider. It checks if a quarter of P1's width is still colliding. 
						if (C1->collider.x + (C1->collider.w * quarter) > C2->collider.x + C2->collider.w)
						{
							p1.againstRightWall = true;
							p1.position.x = C2->collider.x + C2->collider.w - p1.collision_tolerance;
							LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
						}
					}
				}
			}
		}

		//Player colliding against the Goal
		if (C1->type == PLAYER && C2->type == GOAL)
		{
			if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w)	//Just a general detection of collisionss, as what only matters is that it activates.
			{
				if (C1->collider.y > GOAL_Y && C1->collider.y < GOAL_HEIGHT)	//Dirty way to know which portal goal has been reached.
				{
					App->fadescene->FadeToBlack("1st_Level.tmx");				//Loads the 1st level.
					App->map->Restart_Cam();
				}
				else
				{
					App->fadescene->FadeToBlack("Tutorial_Level.tmx");			//Loads the 2nd level.
					App->map->Restart_Cam();
				}

				App->audio->PlayFx(6, 0);										//Sound effect of the Goal / Protal.							
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
	//Loads the textures of P1. Switches them according to switch_sprites
	p1.texture = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");
	
	//--------------------------------------------Loading the data and colliders of P1--------------------------------------------
	//Loads the position of P1 from the xml.
	p1.position.x = p1.position.x;
	p1.position.y = p1.position.y;

	//p1.spawn_position = p1.position;	//Sets the respawn position to the first position the player was in the map. 

	//Loads the data of the rectangle that contains P1.
	p1.HitBox.x = p1.position.x;			//Represents the position in the X axis of P1.
	p1.HitBox.y = p1.position.y;			//Represents the position in the Y axis of P1.
	p1.HitBox.w = p1.sprite_measures.x;		//Represents the width of P1.
	p1.HitBox.h = p1.sprite_measures.y;		//Represents the height of P1.

	//Adds a collider for the player.
	p1.collider = App->collisions->AddCollider(p1.HitBox, PLAYER, this);

	//Boolean resetting
	p1.grounded = false;
	p1.airborne = false;
	p1.flip = false;
	p1.isCrouching = false;
	p1.isJumping = false;
	p1.isBoostJumping = false;
	p1.item_activated = false;
	p1.isGoingRight = false;
	p1.isGoingLeft = false;
	p1.platformDrop = false;
	p1.fading = false;
	p1.isAlive = true;
	p1.isDying = false;
	p1.againstRightWall = false;
	p1.againstLeftWall = false;
	p1.againstCeiling = false;
	p1.GodMode = false;
	p1.switch_sprites = false;

	return true;
}

bool j1Player1::AddAnimationPushbacks()
{
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
	p1.death.PushBack({ 10,1094,58,66 });
	p1.death.PushBack({ 10,1094,58,66 });
	p1.death.PushBack({ 10,1094,58,66 });
	p1.death.PushBack({ 10,1094,58,66 });
	p1.death.PushBack({ 10,1074,58,66 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 68,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 136,1074,58,86 });
	p1.death.PushBack({ 193,1074,58,86 });
	p1.death.PushBack({ 193,1074,58,86 });
	p1.death.PushBack({ 193,1074,58,86 });
	p1.death.PushBack({ 193,1074,58,66 });
	p1.death.speed = 0.2f;

	return true;
}

bool j1Player1::LoadPlayer1Properties(pugi::xml_node& config)
{
	//Gets all the required player variables from the config xml file
	p1.position.x = config.child("player_1").child("position").attribute("x").as_float();
	p1.position.y = config.child("player_1").child("position").attribute("y").as_float();
	p1.spawn_position.x = config.child("player_1").child("position").attribute("x").as_float();
	p1.spawn_position.y = config.child("player_1").child("position").attribute("y").as_float();

	p1.speed.x = config.child("player_1").child("speed").attribute("x").as_float();
	p1.speed.y = config.child("player_1").child("speed").attribute("y").as_float();
	p1.max_speed.x = config.child("player_1").child("max_speed").attribute("x").as_float();
	p1.max_speed.y = config.child("player_1").child("max_speed").attribute("y").as_float();

	p1.acceleration.x = config.child("player_1").child("acceleration").attribute("x").as_float();
	p1.acceleration.y = config.child("player_1").child("acceleration").attribute("y").as_float();
	p1.gravity = config.child("player_1").child("gravity").attribute("value").as_float();

	p1.boost_jump.x = config.child("player_1").child("boost_jump").attribute("x").as_float();
	p1.boost_jump.y = config.child("player_1").child("boost_jump").attribute("y").as_float();

	p1.sprite_measures.x = config.child("player_1").child("sprite_measures").attribute("w").as_int();
	p1.sprite_measures.y = config.child("player_1").child("sprite_measures").attribute("h").as_int();

	p1.lives = config.child("player_1").child("lives").attribute("lives").as_int();
	p1.max_lives = config.child("player_1").child("lives").attribute("lives").as_int();

	p1.body_margin.x = config.child("player_1").child("body_margin").attribute("x").as_int();
	p1.body_margin.y = config.child("player_1").child("body_margin").attribute("y").as_int();

	p1.collision_tolerance = config.child("player_1").child("collider_tolerance").attribute("tolerance").as_int();

	return true;
}

/*bool j1Player1::LoadPlayer1Textures()
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

void j1Player1::Restart()
{
	p1.position = p1.spawn_position;
	p1.isAlive = true;
}

void j1Player1::GodModeInput()
{
	p1.airborne = false;
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		p1.position.x += GOD_MODE_SPEED * App->dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		p1.position.x -= GOD_MODE_SPEED * App->dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		p1.position.y -= GOD_MODE_SPEED * App->dt;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		p1.position.y += GOD_MODE_SPEED * App->dt;
	}
}