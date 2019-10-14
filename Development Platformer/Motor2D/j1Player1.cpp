#include "j1App.h"
#include "j1Player1.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Textures.h"

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
	Graphicsp1 = App->tex->Load("textures/Spritesheets/Character 1/Character_Spritesheet.png");

	return true;
};

bool j1Player1::Awake(pugi::xml_node& config) 
{	
	p1.position.x = config.child("player_1").child("position").attribute("x").as_float();
	p1.position.y = config.child("player_1").child("position").attribute("y").as_float();

	p1.speed.x = config.child("player_1").child("speed").attribute("x").as_float();
	p1.speed.y = config.child("player_1").child("speed").attribute("y").as_float();
	p1.max_speed.x = config.child("player_1").child("max_speed").attribute("x").as_float();
	p1.max_speed.y = config.child("player_1").child("max_speed").attribute("y").as_float();

	p1.acceleration.x = config.child("player_1").child("acceleration").attribute("x").as_float();
	p1.acceleration.y = config.child("player_1").child("acceleration").attribute("y").as_float();
	p1.gravity = config.child("player_1").child("gravity").attribute("value").as_float();
	
	return true;
};

bool j1Player1::Start() 
{
	p1.position = { p1.position.x, p1.position.y };
	p1.HitBox = { (int)p1.position.x,(int)p1.position.y, p1.sprite_width, p1.sprite_height }; //Casked to int "(int)" for optimization.

	p1.p1_isGrounded(true);
	
	return true;
};

bool j1Player1::PreUpdate() 
{
	//p1.p1_SetGroundState(false); //set  to false when colliders are implemented.
	
	p1.state = idle_P1;
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		p1.state = goingRight_P1;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		p1.state = goingLeft_P1;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
	{
		p1.state = jumping_P1;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		p1.state = crouch_P1;
	}

	return true;
};

bool j1Player1::Update(float dt) 
{
	frames++;

	//p1.p1_lastGrounded = p1.p1_position;
	
	switch (p1.state)
	{
	
	case idle_P1:
	
		p1.speed.x = 0;
	
		break;
	
	case goingRight_P1:
	
		LOG("P2 GOING RIGHT %d %d", p1.speed.x, p1.max_speed.x);
	
		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != p1.max_speed.x)
		{
			p1.speed.x += p1.acceleration.x;
		}
	
		p1.position.x += p1.speed.x; //p1.speed_x is positive here.
	
		LOG("P2 Position %d %d", p1.position.x, p1.position.y);
	
		break;
	
	case goingLeft_P1:
	
		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p1.speed.x != -p1.max_speed.x)
		{
			p1.speed.x -= p1.acceleration.x;
		}
	
		p1.position.x += p1.speed.x;  //p1.speed_x  is negative here.
	
		break;
	
	case jumping_P1:
	
		if (p1.grounded == true /*|| p1.jumpCount != 2*/)
		{
			p1.speed.y = -p1.gravity;
	
			/*jumpCount++;*/
			p1.p1_isGrounded(false);
		}

		break;
	}
	
	//If the p1 is in the air then this function brings him/her back down to the floor.
	if (p1.grounded == false)
	{
		p1.speed.y += p1.acceleration.y;
		
		if (p1.speed.y > p1.max_speed.y)
		{
			p1.speed.y = p1.max_speed.y;
		}
	
		p1.position.y += p1.speed.y;
	}
	
	//In case the HitBox clips through the ground.
	if (p1.position.y > p1.floor)
	{
		p1.position.y = p1.floor - 1;
		p1.p1_isGrounded(true);
	}

	//We move the character according the position value after the state has been run.
	p1.HitBox.x = p1.position.x; 
	p1.HitBox.y = p1.position.y;
	
	//Draws the HitBox on-screen.
	App->render->DrawQuad(p1.HitBox, 255, 0, 0);
	
	
	/*App->render->Blit(Graphicsp1, p1.position.x, p1.position.y, &)*/ //falta el sdl rect
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


bool j1Player1::Load(pugi::xml_node& data)
{
	p1.position.x = data.child("position").attribute("x").as_int();
	p1.position.y = data.child("position").attribute("y").as_int();


	return true;
}

// Save Game State
bool j1Player1::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = p1.position.x;
	pos.append_attribute("y") = p1.position.y;
	
	return true;
}

//void j1Player1::Draw(SDL_Texture* sprites, float dt) 
//{
//
//}

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



