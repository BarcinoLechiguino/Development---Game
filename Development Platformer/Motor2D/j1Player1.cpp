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
	
	
	p1.idle.PushBack({ 28, 14, 37, 57 });
	p1.idle.PushBack({ 132, 12, 33, 59 });
	p1.idle.PushBack({ 230, 12, 37, 59 });
	p1.idle.PushBack({ 326, 14, 39, 57 });
	p1.idle.speed = 0.1f;

	p1.running_right.PushBack({ 134, 90, 39, 55 });
	p1.running_right.PushBack({ 232, 92, 39, 53 });
	p1.running_right.PushBack({ 332, 96, 39, 49 });
	p1.running_right.PushBack({ 434, 90, 45, 55 });
	p1.running_right.PushBack({ 532, 92, 39, 53 });
	p1.running_right.PushBack({ 632, 96, 39, 49 });
	p1.running_right.speed = 0.15f;

	p1.running_left.PushBack({ 134, 90, 39, 55 });
	p1.running_left.PushBack({ 232, 92, 39, 53 });
	p1.running_left.PushBack({ 332, 96, 39, 49 });
	p1.running_left.PushBack({ 434, 90, 45, 55 });
	p1.running_left.PushBack({ 532, 92, 39, 53 });
	p1.running_left.PushBack({ 632, 96, 39, 49 });
	p1.running_left.speed = 0.2f;

	p1.jumping.PushBack({ 30, 172, 39, 47 });
	p1.jumping.PushBack({ 130, 176, 39, 43 });
	p1.jumping.PushBack({ 234, 162, 37, 53 });
	p1.jumping.PushBack({ 328, 158, 41, 45 });
	p1.jumping.PushBack({ 436, 162, 29, 41 });
	p1.jumping.speed = 0.3f;

	p1.mid_jump.PushBack({ 528, 168, 47, 33 });
	p1.mid_jump.PushBack({ 640, 168, 35, 41 });
	p1.mid_jump.PushBack({ 22, 248, 51, 33 });
	p1.mid_jump.speed = 0.2f;

	p1.falling.PushBack({ 136, 224, 33, 61 });
	p1.falling.PushBack({ 236, 226, 33, 59 });
	p1.falling.speed = 0.2f;

	/*p1.crouching.PushBack({ 30, 172, 39, 47 });
	p1.crouching.PushBack({ 130, 176, 39, 43 });*/
	p1.crouching.PushBack({ 432, 10, 37, 61 }); //20 Pixels of margin vertically up.
	p1.crouching.PushBack({ 530, 8, 39, 63 });
	p1.crouching.PushBack({ 630, 8, 37, 63 });
	p1.crouching.PushBack({ 34, 84, 33, 61 });
	p1.crouching.speed = 0.1f;

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

	/*p1.jumpFX = config.child("jumpFX").attribute("name").as_string();
	p1.deathFX = config.child("deathFX").attribute("name").as_string();
	p1.duoFX = config.child("landFX").attribute("name").as_string();*/

	//Sets the first cycle of animations to the idle set.
	p1.current_animation = &p1.idle;
	
	jumpFX = App->audio->LoadFx("audio/fx/Jump.wav");
	deathFX = App->audio->LoadFx("audio/fx/Death.wav");

	return true;
};

bool j1Player1::Start() 
{
	p1.texture = App->tex->Load("textures/Spritesheets/Character 1/character_spritesheet_I_Buena.png");
	p1.texture2 = App->tex->Load("textures/Spritesheets/Character 1/adventurer-hand-combat-Sheet.png");

	p1.position = { p1.position.x, p1.position.y };
	p1.HitBox = { (int)p1.position.x,(int)p1.position.y, p1.sprite_width, p1.sprite_height }; //Casked to int "(int)" for optimization.
	p1.collider = App->collisions->AddCollider(p1.HitBox, PLAYER, this); //Adds a collider for the player.

	jumpFX = App->audio->LoadFx("audio/fx/Jump.wav");
	deathFX = App->audio->LoadFx("audio/fx/Death.wav");

	if (p1.grounded)
	{
		p1.state = idle_P1;
	}
	/*else
	{
		p1.state = falling_P1;
	}*/

	player1_alive = true;

	return true;
};

bool j1Player1::PreUpdate() 
{
	p1.p1_isGrounded(false); //set  to false when colliders are implemented.
	
	/*if (!p1.grounded)
	{
		p1.state = falling_P1;
	}*/
	
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
			
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			p1.position.x = App->player2->p2.position.x + 40;
			p1.position.y = App->player2->p2.position.y;
		}
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			//Death logic
		}
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

	//p1.p1_lastGrounded = p1.p1_position;
	
	switch (p1.state)
	{
	
	case idle_P1:
	
		p1.speed.x = 0;
		p1.current_animation = &p1.idle;
	
		break;
	
	case goingRight_P1:
		
		GoRight();

		break;
	
	case goingLeft_P1:
	
		GoLeft();
	
		break;
	
	case crouching_P1:

		Crouch();

		break;

	case jumping_P1:
	
		Jump();

		break;

	case falling_P1:

		Fall();

		break;
	}
	
	//If the p1 is in the air then this function brings him/her back down to the floor.
	/*if (p1.grounded == false)
	{	
		p1.speed.y += p1.acceleration.y;
		
		if (p1.speed.y > p1.max_speed.y)
		{
			p1.speed.y = p1.max_speed.y;
		}
	
		p1.position.y += p1.speed.y;

		if (p1.speed.y < 0)
		{
			p1.current_animation = &p1.jumping;
		}
		else
		{
			p1.current_animation = &p1.falling;
		}
	}*/

	//We move the character according the position value after the state has been run.
	p1.HitBox.x = p1.position.x; 
	p1.HitBox.y = p1.position.y;
	
	//Draws the HitBox on-screen.
	//App->render->DrawQuad(p1.HitBox_Alpha, 255, 0, 0, 200);

	p1.HitBox = p1.current_animation->GetCurrentFrame();
	
	App->render->Blit(p1.texture, p1.position.x, p1.position.y, &p1.HitBox, p1.flip);

	//Making the collider follow the player while also taking into account if P1 is flipped.
	if (!p1.flip)
	{
		p1.collider->Set_Position(p1.position.x, p1.position.y);
	}
	else
	{
		p1.collider->Set_Position(p1.position.x + 22, p1.position.y);
	}

	return true;
};

bool j1Player1::PostUpdate() 
{
	return true;
};

bool j1Player1::cleanUp() 
{
	App->tex->UnLoad(p1.texture);
	return true;
};


bool j1Player1::Load(pugi::xml_node& data)
{
	p1.position.x = data.child("position1").attribute("x").as_int();
	p1.position.y = data.child("position1").attribute("y").as_int();
	return true;
}

bool j1Player1::Save(pugi::xml_node&  data) const
{
	pugi::xml_node pos = data.append_child("position1");

	pos.append_attribute("x") = p1.position.x;
	pos.append_attribute("y") = p1.position.y;
	return true;
}

void j1Player1::Restart()
{
	p1.position.x = p1.initial_position_x;
	p1.position.y = p1.initial_position_y;
	player1_alive = true;
}

void j1Player1::GodModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		p1.position.x += 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		p1.position.x -= 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		p1.position.y -= 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		p1.position.y += 10;
	}
}

//Collision Handling ---------------------------------------
void j1Player1::OnCollision(Collider* C1, Collider* C2)
{
	if (!GodMode)					//Will only check collisions if GodMode is false.
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

		//Player colliding against solids
		if (C1->type == PLAYER && C2->type == SOLID)
		{
			//Player Colliding from above the Solid
			if (p1.position.y + C1->collider.h > C2->collider.y /*p1.position.y + C1->collider.h - p1.max_speed.y - 2 < C2->collider.y
				&& C1->collider.x < C2->collider.x + C2->collider.w
				&& C1->collider.x + C1->collider.w > C2->collider.x*/)
			{
				p1.speed.y = 0;
				p1.grounded = true;
				p1.position.y = C2->collider.y - C1->collider.h;
				LOG("P1 IS COLLIDING WITH SOLID FROM AVOBE");

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
		/*	if (p1.position.x < C2->collider.x + C2->collider.w)
			{
				p1.speed.x = 0;
				p1.position.x = C2->collider.x + C2->collider.w;
				LOG("P1 IS COLLIDING WITH SOLID FROM THE RIGHT");
			}*/

			//Player is colliding from left (going right)
			if (p1.position.x + C1->collider.w < C2->collider.x)
			{
				p1.speed.x = 0;
				p1.position.x = C2->collider.x - C1->collider.w;
				LOG("P1 IS COLLIDING WITH SOLID FROM THE LEFT");
			}
		}
	}
}

//Movement methods
void j1Player1::GoRight()
{
	LOG("P2 GOING RIGHT %d %d", p1.speed.x, p1.max_speed.x);

	//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
	while (p1.speed.x != p1.max_speed.x)
	{
		p1.speed.x += p1.acceleration.x;
	}

	p1.position.x += p1.speed.x; //p1.speed_x is positive here.

	p1.flip = false;
	p1.current_animation = &p1.running_right;
	p1.GoingRight = true;

	LOG("P1 Position %d %d", p1.position.x, p1.position.y);
}

void j1Player1::GoLeft()
{
	//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
	while (p1.speed.x != -p1.max_speed.x)
	{
		p1.speed.x -= p1.acceleration.x;
	}

	p1.position.x += p1.speed.x;  //p1.speed_x  is negative here.

	p1.flip = true;
	p1.current_animation = &p1.running_left;
	p1.GoingLeft = true;
}

void j1Player1::Crouch()
{
	p1.current_animation = &p1.crouching;
}

void j1Player1::Jump()
{
	if (p1.grounded == true)
	{
		p1.speed.y = -p1.gravity;
		App->audio->PlayFx(jumpFX);
		p1.p1_isGrounded(false);
	}
}

void j1Player1::Fall()
{
	p1.speed.y += p1.acceleration.y;

	if (p1.speed.y > p1.max_speed.y)
	{
		p1.speed.y = p1.max_speed.y;
	}

	p1.position.y += p1.speed.y;

	if (p1.speed.y < 0)
	{
		p1.current_animation = &p1.jumping;
	}
	else
	{
		p1.current_animation = &p1.falling;
	}
}

