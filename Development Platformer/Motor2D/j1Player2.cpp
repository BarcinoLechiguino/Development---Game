#include "j1App.h"
#include "j1Player2.h"
#include "j1Player1.h"
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

j1Player2::j1Player2(int x, int y, ENTITY_TYPE type) : j1Player(x, y, type) //Constructor. Called at the first frame.
{
	//Adds P2's animation pushback.
	LoadAnimationPushbacks();
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
	return true;
};

bool j1Player2::Start()
{
	entity_sprite = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2_Buena.png");

	SetPlayer2Position();									//Sets P1's origin position in the map.
	LoadEntityProperties();									//Loads the player's properties from the xml file. //THIS HERE
	InitPlayer();											//Loads P2 in game.
	LoadEntityAudio();
	
	player.airborne = true;
	player.item_activated = false;

	player.state = Player_State::Idle;

	return true;
};

bool j1Player2::PreUpdate()
{
	if (player.GodMode == false)
	{
		player.state = Player_State::Idle;

		if (speed.y > 2)													//For some reason the player's speed when colliding against the ground is 1.4f aprox instead of 0.
		{
			player.state = Player_State::Falling;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)			//Move Right
		{
			player.state = Player_State::Going_Right;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)			//Move Left
		{
			player.state = Player_State::Going_Left;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)			//Crouch
		{
			player.state = Player_State::Crouching;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN)				//Jump
		{
			if (player.grounded == true)
			{
				player.state = Player_State::Jumping;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)				//Drop from platform
		{
			player.platformDrop = true;
		}
		else
		{
			player.platformDrop = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN)				//Teleport
		{
			player.state = Player_State::Teleporting;
		}

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)				//Suicide Button
		{
			player.state = Player_State::Dying;
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

bool j1Player2::Update(float dt, bool doLogic)
{
	switch (player.state)
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

		animation = &crouching;
		player.isCrouching = true;

		break;

	case Player_State::Jumping:

		App->audio->PlayFx(5, 0);
		speed.y = -player.acceleration.y * dt;
		player.isJumping = true;						//Boolean for animations
		player.airborne = true;
		player.grounded = false;

		break;

	case Player_State::Falling:							//When dropping from platforms

		player.airborne = true;
		//p1.grounded = false;							//With this commented, jumping after falling off platforms is allowed.
		animation = &falling;

		break;

	case Player_State::Teleporting:

		TeleportP1ToP2();

		break;

	case Player_State::Dying:

		App->audio->PlayFx(2, 0);
		animation = &death;
		player.isDying = true;

		break;
	}

	//if (position.x > App->entityManager->player->position.x && App->entityManager->player->player.state == Player_State::Crouching) //LAST THING TO USE
	//{
	//	if (player.grounded == true)
	//	{
	//		speed.y -= player.boost_jump.y * App->GetDt();
	//		LOG("boost jump speed is");
	//		player.isBoostJumping = true;
	//		player.airborne = true;
	//		player.grounded = false;
	//		player.platformDrop = false;
	//		App->audio->PlayFx(3, 0);
	//	}
	//}
	
	//If P2 is in the air then this function brings them back down to the floor.
	if (player.airborne == true)
	{
		ApplyGravity();
	}

	//--------------------------------------- Miscelaneous Checks ---------------------------------------
	LivesCheck(player.lives);																// If P1 empties the lives pool then both players die and reset their positions to the spawn point.


	//--------------------------------------- Tp skill Cooldown ---------------------------------------
	if (player.tpInCd == true)
	{
		SkillCooldown(player.tpInCd, player.tpCdCount, player.tpCdTime);
	}
	
	//THIS HERE
	player.HitBox = animation->GetCurrentFrame(dt);											//THIS HERE //Sets the animation cycle that P1 will have. 
	collider->Set_Position(position.x, position.y);											//Makes P1's collider follow P1.
	//p1.atkCollider->Set_Position(position.x + sprite_size.x, sprite_size.y);				//Makes P1's attack collider follow P1.

	BlitEntity(position.x, position.y, player.HitBox, player.flip);							//Blits the player on screen with the data we have given the Blit() function.

	return true;
};

bool j1Player2::PostUpdate()
{
	//Resetting the collider related bools after the collision happened in Update() so it can be checked the next update/frame.
	player.againstCeiling = false;
	player.againstRightWall = false;
	player.againstLeftWall = false;

	return true;
};

bool j1Player2::Load(pugi::xml_node& data) {

	position.x = data.child("player2").child("position").attribute("x").as_int();
	position.y = data.child("player2").child("position").attribute("y").as_int();

	return true;
}

// Save game state
bool j1Player2::Save(pugi::xml_node& data) const {

	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

bool j1Player2::CleanUp()
{
	App->tex->UnLoad(entity_sprite);

	if (collider != nullptr)
	{
		collider->to_delete = true;						//THIS HERE
	}

	if (animation != nullptr)
	{
		animation = nullptr;					//THIS HERE
	}

	return true;
};

//Collision Handling ---------------------------------------
void j1Player2::OnCollision(Collider* C1, Collider* C2)
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
			if (C2->type == Object_Type::PLAYER /*|| C1->type == Object_Type::PLAYER*/)
			{	
				if (C1->collider.x + C1->collider.w > C2->collider.x || C1->collider.x < C2->collider.x + C2->collider.w) //As the boost can be done even if P1 is static, this allows for more precise jumps... hopefully.
				{
					if (App->entityManager->player->player.state == Player_State::Crouching)
					{	
						if (player.grounded == true)
						{
							speed.y -= player.boost_jump.y * App->GetDt();
							LOG("boost jump speed is");
							player.isBoostJumping = true;
							player.airborne = true;
							player.grounded = false;
							player.platformDrop = false;
							App->audio->PlayFx(3, 0);

							LOG("P2 IS BOOST JUMPING");
						}
					}
					LOG("P2 IS COLLIDING WITH P1 FROM THE LEFT");
				}
			}

			//Player colliding against solids
			if (C2->type == Object_Type::SOLID)
			{
				//Player Colliding from TOP or BOTTOM. 
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
						LOG("P2 IS COLLIDING WITH A SOLID FROM ABOVE");
					}

					//Player Colliding from BOTTOM.
					else if (C1->collider.y < C2->collider.y + C2->collider.h && C1->collider.y + 20 > C2->collider.y + C2->collider.h && C1->collider.y > C2->collider.y)	//This second part checks if C1 is actually colliding vertically down.
					{
						speed.y = 0;
						position.y = C2->collider.y + C2->collider.h + 1;		//THIS HERE

						player.againstCeiling = true;
						LOG("P2 IS COLLIDING WITH A SOLID FROM BELOW");
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
						LOG("P2 IS COLLIDING WITH A SOLID FROM THE LEFT");
					}

					//Player is colliding from RIGHT.
					if (C1->collider.x <= C2->collider.x + C2->collider.w && C1->collider.x >= C2->collider.x)						// This second part checks if C1 is actually colliding from the right side of the collider.
					{
						player.againstRightWall = true;
						player.againstLeftWall = false;
						LOG("P2 IS COLLIDING WITH A SOLID FROM THE RIGHT");
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
							LOG("P2 IS COLLIDING WITH A SOLID FROM ABOVE");
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
				RespawnP2ToP1();
			}

			if (C2->type == Object_Type::ENEMY)
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
						RespawnP2ToP1();
					}

					//Player is colliding from RIGHT.
					if (C1->collider.x <= C2->collider.x + C2->collider.w && C1->collider.x >= C2->collider.x)						// This second part checks if C1 is actually colliding from the right side of the collider.
					{
						//Death logic
						player.lives--;
						player.state = Player_State::Dying;

						//Antes la animacion de muerte tiene que haber finalizado - Gerard
						RespawnP2ToP1();
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
				player.item_activated = true;				//Records that P1 (or P2) has activated the item.
				App->entityManager->player->player.item_activated = true;		//Activates P2's boolean as well. THIS HERE Change all player2 by App->entitymanager->player2->player.pos...

				App->audio->PlayFx(4, 0);					//Item Activation sfx.
			}

			//Player colliding against Deactivable surfaces. 
			if (C2->type == Object_Type::DEACTIVABLE)
			{
				if (player.item_activated == false || App->entityManager->player->player.item_activated == false)
				{
					player.lives--;
					player.state = Player_State::Dying;
					
					RespawnP2ToP1();
				}
			}

			if (C2->type == Object_Type::CHECKPOINT)
			{
				LOG("P2 HAS REACHED A CHECKPOINT");															//Call Safe() method here.
				//App->SaveGame();
			}

			//Player colliding against the Goal
			if (C2->type == Object_Type::GOAL)
			{
				LoadNextMap();													//Loads the next map

				App->audio->PlayFx(6, 0);										//Sound effect of the Goal / Protal.
			}
		}
	}
}

void j1Player2::TeleportP1ToP2()
{
	//App->entityManager->player->p1.grounded = false;
	
	if (!player.tpInCd)
	{
		if (player.flip == false)			//The players will be always teleported directly in front of one another. 
		{
			if (player.againstLeftWall == false)
			{
				App->entityManager->player->position.x = position.x + collider->collider.w;
				App->entityManager->player->position.y = position.y - 60;
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
				App->entityManager->player->position.x = position.x - collider->collider.w / 2;	//THIS HERE
				App->entityManager->player->position.y = position.y - 60;
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

void j1Player2::RespawnP2ToP1()		//Method that, on death, will respawn P2 behind P1.
{
	if (player.flip == true)			//The players will be always respawned directly behind of one another. 
	{
		position.x = App->entityManager->player->position.x + App->entityManager->player->collider->collider.w;
		position.y = App->entityManager->player->position.y - 40;
	}
	else
	{
		position.x = App->entityManager->player->position.x + App->entityManager->player->collider->collider.w / 2;
		position.y = App->entityManager->player->position.y - 40;
	}
}

void j1Player2::SetPlayer2Position()
{
	config_file.load_file("config.xml");				//REVISE THIS HERE  Can a pugi object be reused as a copy in another class?

	player_entity = config_file.child("config").child("entities").child("player").child("player_2");

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

//---------------------------------------------- General Checks ----------------------------------------------
void j1Player2::LivesCheck(int lives)
{
	if (lives == 0)
	{
		player.isAlive = false;
		App->entityManager->player->player.isAlive = false;

		if (player.isAlive == false && App->entityManager->player->player.isAlive == false)
		{
			App->audio->PlayFx(2, 0);					//THIS HERE Death sound
			Restart();
			App->entityManager->player->Restart();
			player.lives = player.max_lives;
		}
	}

	return;
}

void j1Player2::Restart()
{
	position = player.spawn_position;		//THIS HERE
	player.isAlive = true;
}

void j1Player2::GodModeInput()
{
	player.airborne = false;

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)		//THIS HERE
	{
		position.x += player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
	{
		position.x -= player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
	{
		position.y -= player.godModeSpeed * App->GetDt();
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)
	{
		position.y += player.godModeSpeed * App->GetDt();
	}
}