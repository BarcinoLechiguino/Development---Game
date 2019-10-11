#include "j1App.h"
#include "j1Player1.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player1::j1Player1() //Constructor. Called at the first frame.
{
	name.create("player_1");
};

j1Player1::~j1Player1()  //Destructor. Called at the first frame.
{

};

bool j1Player1::Init() 
{
	return true;
};

bool j1Player1::Awake(pugi::xml_node& config) 
{	
	p1.speed_x = config.child("player_1").child("speed_x").attribute("value").as_float();
	p1.speed_y = config.child("speed_y").attribute("value").as_float();
	p1.max_speed = config.child("player_1").child("max_speed").attribute("value").as_float();
	p1.acceleration = config.child("player_1").child("acceleration").attribute("value").as_float();
	p1.p1_gravity = config.child("gravity").attribute("value").as_float();

	return true;
};

bool j1Player1::Start() 
{
	p1.p1_position = { 250, p1.floor };
	p1.p1_HitBox = { (int)p1.p1_position.x,(int)p1.p1_position.y, p1.sprite_width, p1.sprite_height }; //Casked to int "(int)" for optimization.

	return true;
};

bool j1Player1::PreUpdate() 
{
	p1.p1_time = SDL_GetTicks() / 20;
	
	p1.p1_State = idle_P1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		p1.p1_State = goingRight_P1;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		p1.p1_State = goingLeft_P1;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
	{
		//p1.p1_time_at_jump = p1.p1_time;
		p1.p1_State = jumping_P1;
	}

	return true;
};

bool j1Player1::Update(float dt) 
{
	p1_frames++;

	switch (p1.p1_State) 
	{

	case idle_P1:
		
		p1.speed_x = 0;

		break;

	case goingRight_P1:

		//p1.p1_position.x += 10;
	

		while (p1.speed_x != p1.max_speed)
		{
			p1.speed_x += p1.acceleration;

			if (p1.speed_x > p1.max_speed)
			{
				p1.speed_x = p1.max_speed;
			}
		}
		
		p1.p1_position.x += p1.speed_x; //p1.speed_x is positive here.

		break;

	case goingLeft_P1:
		p1.p1_position.x -= 10;
		
		while (p1.speed_x != -p1.max_speed)
		{
			p1.speed_x -= p1.acceleration;

			if (p1.speed_x < -p1.max_speed)
			{
				p1.speed_x = -p1.max_speed;
			}
		}

		p1.p1_position.x += p1.speed_x;  //p1.speed_x  is negative here.

		break;

	case jumping_P1:
		//p1.p1_jump_time = p1.p1_time - p1.p1_time_at_jump;
		//p1.p1_position.y = p1.floor - (10*p1.p1_jump_time) + (0.5*(p1.p1_gravity) * (p1.p1_jump_time*p1.p1_jump_time));
		//p1.p1_position.x = p1.speed_x;
		//position_P1.y += p1.speed_y;

		break;
	}

	p1.p1_HitBox.x = p1.p1_position.x;
	//p1.p1_HitBox.y = p1.p1_position.y;

	if (p1.p1_position.y <= p1.floor)
	{
		p1.p1_position.y = p1.floor;
	}
	
	App->render->DrawQuad(p1.p1_HitBox, 255, 0, 0);

	return true;
};

bool j1Player1::PostUpdate() 
{
	return true;
};

bool j1Player1::cleanUp() 
{
	return true;
};

