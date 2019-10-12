#include "j1App.h"
#include "j1Player1.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"

j1Player1::j1Player1() //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	name.create("player"); //The string has to be the same as the name of the node in the xml file.
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
	p1.speed_x = config.child("speed_x").attribute("value").as_float();
	p1.speed_y = config.child("speed_y").attribute("value").as_float();
	p1.max_speed = config.child("max_speed").attribute("value").as_float();
	
	p1.acceleration = config.child("acceleration").attribute("value").as_float();
	p1.p1_gravity = config.child("gravity").attribute("value").as_float();

	return true;
};

bool j1Player1::Start() 
{
	p1.p1_position = { 300.0f, p1.floor };
	p1.p1_HitBox = { (int)p1.p1_position.x,(int)p1.p1_position.y, p1.sprite_width, p1.sprite_height }; //Casked to int "(int)" for optimization.

	return true;
};

bool j1Player1::PreUpdate() 
{
	//p1.p1_time = SDL_GetTicks() / 20;
	
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

		LOG("GOING RIGHT %d %d", p1.speed_x, p1.max_speed);

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

void j1Player1::Draw(SDL_Texture* sprites, float dt) 
{

}

/*
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
*/

/*
Awake(int x, int y) code Gerard:
pugi::xml_parse_result result = file.load_file("Config.xml");

	if (result != NULL)
	{
		pugi::xml_node node = file.child("config");

		speed.x = node.child("player_1").child("speed").attribute("x").as_float();
		speed.y = node.child("player_1").child("speed").attribute("y").as_float();

		player_size.x = node.child("player_1").child("idle_anim").attribute("w1").as_int();
		player_size.y = node.child("player_1").child("idle_anim").attribute("h1").as_int();

		gravity = node.child("player_1").child("speed").attribute("gravity").as_float();

		jump = node.child("player").child("jump").attribute("normal").as_float();

		pugi::xml_node anim = node.child("player");

		idle.PushBack({ anim.child("idle_anim").attribute("x1").as_int(),anim.child("idle_anim").attribute("y1").as_int(),anim.child("idle_anim").attribute("w1").as_int(),anim.child("idle_anim").attribute("h1").as_int() });
		idle.PushBack({ anim.child("idle_anim").attribute("x2").as_int(),anim.child("idle_anim").attribute("y2").as_int(),anim.child("idle_anim").attribute("w2").as_int(),anim.child("idle_anim").attribute("h2").as_int() });
		idle.PushBack({ anim.child("idle_anim").attribute("x3").as_int(),anim.child("idle_anim").attribute("y3").as_int(),anim.child("idle_anim").attribute("w3").as_int(),anim.child("idle_anim").attribute("h3").as_int() });
		idle.PushBack({ anim.child("idle_anim").attribute("x4").as_int(),anim.child("idle_anim").attribute("y4").as_int(),anim.child("idle_anim").attribute("w4").as_int(),anim.child("idle_anim").attribute("h4").as_int() });
		idle.loop = true;
		idle.speed = anim.child("idle_anim").attribute("speed").as_float();

		crouch.PushBack({ anim.child("crouch_anim").attribute("x1").as_int(),anim.child("crouch_anim").attribute("y1").as_int(),anim.child("crouch_anim").attribute("w1").as_int(),anim.child("crouch_anim").attribute("h1").as_int() });
		crouch.PushBack({ anim.child("crouch_anim").attribute("x2").as_int(),anim.child("crouch_anim").attribute("y2").as_int(),anim.child("crouch_anim").attribute("w2").as_int(),anim.child("crouch_anim").attribute("h2").as_int() });
		crouch.PushBack({ anim.child("crouch_anim").attribute("x3").as_int(),anim.child("crouch_anim").attribute("y3").as_int(),anim.child("crouch_anim").attribute("w3").as_int(),anim.child("crouch_anim").attribute("h3").as_int() });
		crouch.PushBack({ anim.child("crouch_anim").attribute("x4").as_int(),anim.child("crouch_anim").attribute("y4").as_int(),anim.child("crouch_anim").attribute("w4").as_int(),anim.child("crouch_anim").attribute("h4").as_int() });
		crouch.loop = true;
		crouch.speed = anim.child("crouch_anim").attribute("speed").as_float();

		run.PushBack({ anim.child("run_anim").attribute("x1").as_int(),anim.child("run_anim").attribute("y1").as_int(),anim.child("run_anim").attribute("w1").as_int(),anim.child("run_anim").attribute("h1").as_int() });
		run.PushBack({ anim.child("run_anim").attribute("x2").as_int(),anim.child("run_anim").attribute("y2").as_int(),anim.child("run_anim").attribute("w2").as_int(),anim.child("run_anim").attribute("h2").as_int() });
		run.PushBack({ anim.child("run_anim").attribute("x3").as_int(),anim.child("run_anim").attribute("y3").as_int(),anim.child("run_anim").attribute("w3").as_int(),anim.child("run_anim").attribute("h3").as_int() });
		run.PushBack({ anim.child("run_anim").attribute("x4").as_int(),anim.child("run_anim").attribute("y4").as_int(),anim.child("run_anim").attribute("w4").as_int(),anim.child("run_anim").attribute("h4").as_int() });
		run.PushBack({ anim.child("run_anim").attribute("x5").as_int(),anim.child("run_anim").attribute("y5").as_int(),anim.child("run_anim").attribute("w5").as_int(),anim.child("run_anim").attribute("h5").as_int() });
		run.PushBack({ anim.child("run_anim").attribute("x6").as_int(),anim.child("run_anim").attribute("y6").as_int(),anim.child("run_anim").attribute("w6").as_int(),anim.child("run_anim").attribute("h6").as_int() });
		run.loop = true;
		run.speed = anim.child("run_anim").attribute("speed").as_float();

		jump_anim.PushBack({ anim.child("jump_anim").attribute("x1").as_int(),anim.child("jump_anim").attribute("y1").as_int(),anim.child("jump_anim").attribute("w1").as_int(),anim.child("jump_anim").attribute("h1").as_int() });
		jump_anim.PushBack({ anim.child("jump_anim").attribute("x2").as_int(),anim.child("jump_anim").attribute("y2").as_int(),anim.child("jump_anim").attribute("w2").as_int(),anim.child("jump_anim").attribute("h2").as_int() });
		jump_anim.loop = false;
		jump_anim.speed = anim.child("jump_anim").attribute("speed").as_float();

		top_jump = true;
	}
*/

/*
animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && dt != 0)
	{
		animation = &run;
		position.x += speed.x * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && dt != 0)
	{
		animation = &run;
		position.x -= speed.x * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && dt != 0 || App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN && dt != 0)
	{
		animation = &crouch;
		position.x = speed.x;
	}

	if (stay_in_platform)
	{
		position.y -= gravity * dt;
		jump_anim.Reset();
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && dt != 0)
		{
			start_jump = false;
			if (start_jump == false)
			{
				jump_anim.Reset();
				distance = position.y - jump;
				position.y -= (speed.y + gravity)*dt;
				start_jump = true;
				stay_in_platform = false;
				top_jump = false;
			}
		}
	}
	else
	{
		if (position.y > distance && top_jump == false)
		{
			animation = &jump_anim;
			position.y -= (speed.y + gravity)*dt;
		}

	}

	position.y += gravity * dt;
	stay_in_platform = false;


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

*/

