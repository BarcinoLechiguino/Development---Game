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
#include "j1Module.h"
#include "j1EntityManager.h"

j1Player1::j1Player1(int x, int y, ENTITY_TYPE type) : j1Player(x, y, type) //THIS HERE //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	/*name.create("entities");*/ //The string has to be the same as the name of the node in the xml file.

	LoadAnimationPushbacks();		//REVISE THIS HERE. This needs to be separated from the enemies, there is a need to create a separate LoadEnemyAnimations().

	//tag = "player_1";
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
	return true;
};

bool j1Player1::Start()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");

	SetPlayer1Position();									//Sets P1's origin position in the map.
	LoadEntityProperties();									//Loads the player's properties from the xml file. //THIS HERE
	InitPlayer();											//Loads P1 in game.
	LoadEntityAudio();										//Loads the sfx for player 1.
								
	//LoadPlayer1Textures();		//Loads P1's textures in game.

	player.airborne = true;
	player.item_activated = false;

	player.state = Player_State::Idle;

	return true;
};

bool j1Player1::PreUpdate()
{
	if (player.GodMode == false)
	{
		player.state = Player_State::Idle;

		if (speed.y > 2)													//For some reason the player's speed when colliding against the ground is 1.4f aprox instead of 0.
		{
			player.state = Player_State::Falling;
		}

		LOG("Speed y is %f", speed.y);

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)				//Move Right
		{
			/*if (p1.againstLeftWall == false)
			{
				p1.state = goingRight_P1;
			}*/

			player.state = Player_State::Going_Right;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)				//Move Left
		{
			/*if (p1.againstRightWall == false)				//THIS HERE, for some reason the bool here is always set to false now that the entity system has been implemented
			{
				p1.state = goingLeft_P1;
			}*/

			player.state = Player_State::Going_Left;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)				//Crouch
		{
			/*if (p1.state != falling_P1)						//THIS HERE
			{

			}*/

			player.state = Player_State::Crouching;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)				//Jump
		{
			if (player.grounded == true)
			{
				player.state = Player_State::Jumping;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)				//Drop from platform
		{
			player.platformDrop = true;
		}
		else
		{
			player.platformDrop = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)					//Teleport
		{
			player.state = Player_State::Teleporting;
		}

		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)				//Suicide Button
		{
			player.state = Player_State::Dying;
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

bool j1Player1::Update(float dt, bool doLogic)
{
	switch (player.state)							//REVISE THIS HERE Can this be put in the  j1Player? Maybe it is simmilar to OnCollision and needs to be here.
	{
	case Player_State::Idle:

		animation = &idle;	
		player.isCrouching = false;

		break;

	case Player_State::Going_Right:

		if (player.againstLeftWall == false)
		{
			position.x += speed.x * dt;

			player.flip = false;
			player.isGoingRight = true;

			if (speed.y > 2)
			{
				animation = &falling;
			}
			else
			{
				animation = &running;
			}
		}

		break;

	case Player_State::Going_Left:

		if (player.againstRightWall == false)
		{
			position.x -= speed.x * dt;

			player.flip = true;
			player.isGoingLeft = true;

			if (speed.y > 2)
			{
				animation = &falling;
			}
			else
			{
				animation = &running;
			}
		}

		break;

	case Player_State::Crouching:

		/*if (player.speed.y < 2)
		{
			animation = &crouching;
		}*/

		animation = &crouching;
		player.isCrouching = true;

		break;

	case Player_State::Jumping:

		App->audio->PlayFx(5, 0);
		speed.y = -player.acceleration.y * dt;
		player.isJumping = true;					//Boolean for animations
		player.airborne = true;
		player.grounded = false;

		break;

	case Player_State::Falling:		//When dropping from platforms

		player.airborne = true;
		//p1.grounded = false;					//With this commented, jumping after falling off platforms is allowed.
		animation = &falling;

		break;

	case Player_State::Teleporting:

		TeleportP2ToP1();

		break;

	case Player_State::Dying:

		//App->audio->PlayFx(2, 0);
		animation = &death;
		player.isDying = true;

		break;
	}

	//If P1 is in the air then this function brings them back down to the floor.
	if (player.airborne == true)
	{
		ApplyGravity();
	}

	//--------------------------------------- Miscelaneous Checks ---------------------------------------
	LivesCheck(player.lives);																	// If P1 empties the lives pool then both players die and reset their positions to the spawn point.

	//--------------------------------------- Tp skill Cooldown ---------------------------------------
	if (player.tpInCd == true)
	{
		SkillCooldown(player.tpInCd, player.tpCdCount, player.tpCdTime);
	}

	//THIS HERE
	player.HitBox = animation->GetCurrentFrame(dt);											//Sets the animation cycle that P1 will have. 
	collider->Set_Position(position.x, position.y);											//Makes P1's collider follow P1.
	//p1.atkCollider->Set_Position(position.x + sprite_size.x, sprite_size.y);				//Makes P1's attack collider follow P1.

	BlitEntity(position.x, position.y, player.HitBox, player.flip);							//Blits the player on screen with the data we have given the Blit() function.
	//
	return true;
};

bool j1Player1::PostUpdate()
{
	//Resetting the collider related bools after the collision happened in Update() so it can be checked the next update/frame.
	player.againstCeiling = false;
	player.againstRightWall = false;
	player.againstLeftWall = false;

	return true;
};

bool j1Player1::CleanUp()
{
	LOG("Unloading Player 1");
	App->tex->UnLoad(entity_sprite);

	if (collider != nullptr)
	{
		collider->to_delete = true;
	}
	
	if (animation != nullptr)
	{
		animation = nullptr;
	}

	//RELEASE(App->entityManager->player);		//Player cannot be started again when a new map is loaded.

	return true;
};

//Collision Handling ---------------------------------------
void j1Player1::OnCollision(Collider* C1, Collider* C2)
{
	if (player.GodMode == false)
	{	
		if (C2->type == Object_Type::PLAYER)
		{
			Collider* temp = C1;
			C1 = C2;
			C2 = temp;
		}
		if (C1->type != Object_Type::PLAYER)
		{
			return;
		}

		if (C1->type == Object_Type::PLAYER)
		{
			//Player Colliding Against Another Player
			if (C2->type == Object_Type::PLAYER)
			{
				if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w) //As the boost can be done even if P1 is static, this allows for more precise jumps... hopefully.
				{
					/*if (player.grounded == true)
					{
						speed.y -= player.boost_jump.y * App->GetDt();
						player.isBoostJumping = true;
						player.airborne = true;
						player.grounded = false;
						player.platformDrop = false;
						App->audio->PlayFx(3, 0);
					}*/

					if (App->entityManager->player2->player.state == Player_State::Crouching)
					{
						if (player.grounded == true)
						{
							speed.y -= player.boost_jump.y * App->GetDt();
							player.isBoostJumping = true;
							player.airborne = true;
							player.grounded = false;
							player.platformDrop = false;
							App->audio->PlayFx(3, 0);
						}
					}
					LOG("P1 IS COLLIDING WITH P2 FROM THE LEFT");
				}
			}

			//Player colliding against solids
			if (C2->type == Object_Type::SOLID)
			{
				//Player Colliding from TOP or BOTTOM. sssssa
				if (C1->collider.x + C1->collider.w >= C2->collider.x && C1->collider.x <= C2->collider.x + C2->collider.y)		//The first part checks if C1 is contained in the X axis of C2. 
				{
					//Player Colliding from TOP.
					if (C1->collider.y + C1->collider.h >= C2->collider.y && C1->collider.y < C2->collider.y && speed.y != 0)			//This second part checks if C1 is actually colliding vertically down.
					{
						speed.y = 0;
						position.y = C2->collider.y - C1->collider.h + 1;		//THIS HERE
						player.isJumping = false;
						player.isBoostJumping = false;
						player.grounded = true;
						LOG("P1 IS COLLIDING WITH A SOLID FROM ABOVE");
					}

					//Player Colliding from BOTTOM.
					else if (C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + 20 > C2->collider.y + C2->collider.h && C1->collider.y > C2->collider.y)	//This second part checks if C1 is actually colliding vertically down.
					{
						speed.y = 0;
						position.y = C2->collider.y + C2->collider.h + 1;		//THIS HERE

						player.againstCeiling = true;
						LOG("P1 IS COLLIDING WITH A SOLID FROM BELOW");
					}
				}

				//Player is colliding from LEFT or RIGHT.
				if (C1->collider.y <= C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h - 4 >= C2->collider.y)		//The first part checks if C1 is contained in the Y axis of C2.
				{
					//Player is colliding from LEFT.
					if (C1->collider.x + C1->collider.w >= C2->collider.x && C1->collider.x <= C2->collider.x)						//This second part checks if C1 is actually colliding from the left side of the collider.
					{
						player.againstLeftWall = true;
						player.againstRightWall = false;
						LOG("P1 IS COLLIDING WITH A SOLID FROM THE LEFT");
					}

					//Player is colliding from RIGHT.
					if (C1->collider.x <= C2->collider.x + C2->collider.w && C1->collider.x >= C2->collider.x)						// This second part checks if C1 is actually colliding from the right side of the collider.
					{
						player.againstRightWall = true;
						player.againstLeftWall = false;
						LOG("P1 IS COLLIDING WITH A SOLID FROM THE RIGHT");
					}
				}
			}

			if (C2->type == Object_Type::PLATFORM)
			{
				if (player.platformDrop == false)
				{
					if (C1->collider.x >= C2->collider.x && C1->collider.x + C1->collider.w <= C2->collider.x + C2->collider.w)
					{
						//Player Colliding from Above the Solid, ergo colliding with the ground. This second part checks if C1 is actually colliding vertically down.
						if (C1->collider.y + C1->collider.h >= C2->collider.y && C1->collider.y < C2->collider.y)
						{
							speed.y = 0;
							position.y = C2->collider.y - C1->collider.h + 1;
							player.isJumping = false;
							player.isBoostJumping = false;
							player.grounded = true;
							LOG("P1 IS COLLIDING WITH A SOLID FROM ABOVE");
						}
					}
				}
			}

			if (C2->type == Object_Type::HAZARD)
			{
				//Death logic
				player.lives--;
				player.state = Player_State::Dying;

				//Antes la animacion de muerte tiene que haber finalizado - Gerard
				RespawnP1ToP2();
			}

			if (C2->type == Object_Type::ENEMY)		//Change this when using the attack collider? Change it in the enemies.cpp.
			{
				//Player is colliding from LEFT or RIGHT.
				if (C1->collider.y <= C2->collider.y + C2->collider.h && C1->collider.y + C1->collider.h - 4 >= C2->collider.y)		//The first part checks if C1 is contained in the Y axis of C2.
				{
					//Player is colliding from LEFT.
					if (C1->collider.x + C1->collider.w >= C2->collider.x && C1->collider.x <= C2->collider.x)						//This second part checks if C1 is actually colliding from the left side of the collider.
					{
						//Death logic
						player.lives--;
						player.state = Player_State::Dying;

						//Antes la animacion de muerte tiene que haber finalizado - Gerard
						RespawnP1ToP2();
					}

					//Player is colliding from RIGHT.
					if (C1->collider.x <= C2->collider.x + C2->collider.w && C1->collider.x >= C2->collider.x)						// This second part checks if C1 is actually colliding from the right side of the collider.
					{
						//Death logic
						player.lives--;
						player.state = Player_State::Dying;

						//Antes la animacion de muerte tiene que haber finalizado - Gerard
						RespawnP1ToP2();
					}
				}
				else
				{
					//Destroy Entity;
				}
			}

			//Player Colliding against an Activable Item
			if (C2->type == Object_Type::ITEM)
			{
				//if (player.item_activated == false)
				//{
				//	App->audio->PlayFx(4, 0);					//Item Activation sfx.
				//}

				player.item_activated = true;				//Records that P1 (or P2) has activated the item.
				App->entityManager->player2->player.item_activated = true;		//Activates P2's boolean as well. THIS HERE Change all player2 by App->entitymanager->player2->player.pos...
				App->audio->PlayFx(4, 0);
			}

			//Player colliding against Deactivable surfaces. 
			if (C2->type == Object_Type::DEACTIVABLE)
			{
				if (C2->type == Object_Type::DEACTIVABLE)
				{
					if (player.item_activated == false || App->entityManager->player->player.item_activated == false)
					{
						player.lives--;
						player.state = Player_State::Dying;

						RespawnP1ToP2();
					}
				}
			}

			if (C2->type == Object_Type::CHECKPOINT)
			{
				LOG("P1 HAS REACHED A CHECKPOINT");															//Call Safe() method here.
			}

			//Player colliding against the Goal
			if (C2->type == Object_Type::GOAL)		//CHANGE THIS, USE SwitchMaps() or ChangeMaps()
			{
				if (C1->collider.y > GOAL_Y && C1->collider.y < GOAL_HEIGHT)	//Dirty way to know which portal goal has been reached.
				{
					App->fadescene->FadeToBlack("Test_Map.tmx");				//Loads the 1st level.
					//App->map->Restart_Cam();
				}
				else
				{
					App->fadescene->FadeToBlack("Test_Map_2.tmx");				//Loads the 2nd level.
					//App->map->Restart_Cam();
				}

				App->audio->PlayFx(6, 0);										//Sound effect of the Goal / Protal.
			}
		}
	}
}

bool j1Player1::Load(pugi::xml_node& data)
{
	position.x = data.child("position1").attribute("x").as_int();	//Loads the position of the X axis saved in the save_file.xml.
	position.y = data.child("position1").attribute("y").as_int();	//Loads the position of the X axis saved in the save_file.xml.
	return true;
}

bool j1Player1::Save(pugi::xml_node&  data) const
{
	pugi::xml_node pos = data.append_child("position1");	//Declares the node and with "append_" it is set to overwrite the data in the given xml. 

	pos.append_attribute("x") = position.x;			//Saves the position of P1 on  the X axis the moment Save() is called. "append_" used again to overwrite previous position data.
	pos.append_attribute("y") = position.y;			//Saves the position of P1 on  the Y axis the moment Save() is called. "append_" used again to overwrite previous position data.
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

// ------------------------------------ PLAYER 1 METHODS ------------------------------------
void j1Player1::TeleportP2ToP1()		//Method that teleports P2 directly in front of P1. Takes into account which direction P1 is facing. Can trigger some fun interactions between players :)
{
	if (!player.tpInCd)
	{
		if (player.flip == false)			//The players will be always teleported directly in front of one another. 
		{
			if (player.againstLeftWall == false)
			{
				App->entityManager->player2->position.x = position.x + collider->collider.w;		//THIS HERE
				App->entityManager->player2->position.y = position.y - 60;
				App->audio->PlayFx(1, 0);
				player.tpInCd = true;
			}
			else
			{
				//Sfx indicating that teleport cannot be used.
			}
		}
		else
		{
			if (player.againstRightWall == false)
			{
				App->entityManager->player2->position.x = position.x - collider->collider.w / 2;
				App->entityManager->player2->position.y = position.y - 60;
				App->audio->PlayFx(1, 0);
				player.tpInCd = true;
			}
			else
			{
				//Sfx indicating that teleport cannot be used.
			}
		}
	}
}

void j1Player1::RespawnP1ToP2()		//Method that, on death, will respawn P1 behind P2.
{
	if (player.flip == true)			//The players will be always respawned directly behind of one another. 
	{
		position.x = App->entityManager->player2->position.x + App->entityManager->player2->collider->collider.w;
		position.y = App->entityManager->player2->position.y - 40;
	}
	else
	{
		position.x = App->entityManager->player2->position.x + App->entityManager->player2->collider->collider.w / 2;
		position.y = App->entityManager->player2->position.y - 40;
	}
}

void j1Player1::SetPlayer1Position()
{
	config_file.load_file("config.xml");				//REVISE THIS HERE  Can a pugi object be reused as a copy in another class?

	player_entity = config_file.child("config").child("entities").child("player").child("player_1");

	nameTag = player_entity.attribute("name").as_string();

	if (App->scene->firstMap == true)
	{
		//LoadPlayerPosition("player_1", "factoryMap");		//Change this so strings arent hardcoded //Loads Player 1's position on the map //REVISE THIS HERE For now it is set to the position in the first map, maybe in scene it can be switched? 
		player.mapTag = player_entity.child("factoryMap").attribute("mapName").as_string();
		LoadPlayerPosition(nameTag.GetString(), player.mapTag.GetString());
	}
	if (App->scene->secondMap == true)
	{
		//LoadPlayerPosition("player_1", "forestMap");		//Change this so strings arent hardcoded //Loads Player 1's position on the map //REVISE THIS HERE For now it is set to the position in the first map, maybe in scene it can be switched? 
		player.mapTag = player_entity.child("forestMap").attribute("mapName").as_string();
		LoadPlayerPosition(nameTag.GetString(), player.mapTag.GetString());
	}
}

//---------------------------------------------- General Checks ----------------------------------------------
void j1Player1::LivesCheck(int lives)	//REVISE THIS HERE. Can it be put in the j1Player?
{
	if (lives == 0)
	{
		player.isAlive = false;
		App->entityManager->player2->player.isAlive = false;

		if (player.isAlive == false && App->entityManager->player2->player.isAlive == false)
		{
			App->audio->PlayFx(2, 0);					//THIS HERE Death sound
			Restart();
			App->entityManager->player2->Restart();	
			player.lives = player.max_lives;
		}
	}
	
	return;
}

void j1Player1::Restart()
{
	position = player.spawn_position;		//THIS HERE
	player.isAlive = true;
}

void j1Player1::GodModeInput()
{
	player.airborne = false;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)		//THIS HERE
	{
		position.x += player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += player.godModeSpeed * App->GetDt();
	}
}