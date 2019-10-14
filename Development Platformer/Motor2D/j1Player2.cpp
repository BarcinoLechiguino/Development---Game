#include "j1App.h"
#include "j1Player2.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

j1Player2::j1Player2() //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	name.create("player"); //The string has to be the same as the name of the node in the xml file.
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

	p2.speed_x = config.child("player_2").child("speed").attribute("x").as_float();
	p2.speed_y = config.child("player_2").child("speed").attribute("y").as_float();
	p2.max_speed_x = config.child("player_2").child("max_speed").attribute("x").as_float();
	p2.max_speed_y = config.child("player_2").child("max_speed").attribute("y").as_float();

	p2.acceleration_x = config.child("player_2").child("acceleration").attribute("x").as_float();
	p2.acceleration_y = config.child("player_2").child("acceleration").attribute("y").as_float();
	p2.gravity = config.child("player_2").child("gravity").attribute("value").as_float();

	return true;
};

bool j1Player2::Start()
{
	p2.position = { p2.position.x, p2.position.y };
	p2.HitBox = { (int)p2.position.x,(int)p2.position.y, p2.sprite_width, p2.sprite_height }; //Casked to int "(int)" for optimization.

	p2.p2_isGrounded(true);

	return true;
};

bool j1Player2::PreUpdate()
{
	//p2.p1_SetGroundState(false); //set  to false when colliders are implemented.

	p2.state = idle_P2;

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		p2.state = goingRight_P2;
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
	{
		p2.state = goingLeft_P2;
	}

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		p2.state = jumping_P2;
	}

	return true;
};

bool j1Player2::Update(float dt)
{
	p2_frames++;

	//p2.p1_lastGrounded = p2.p1_position;

	switch (p2.state)
	{

	case idle_P2:

		p2.speed_x = 0;

		break;

	case goingRight_P2:

		LOG("P2 GOING RIGHT %d %d", p2.speed_x, p2.max_speed_x);

		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed_x != p2.max_speed_x)
		{
			p2.speed_x += p2.acceleration_x;
		}

		p2.position.x += p2.speed_x; //p2.speed_x is positive here.

		LOG("P2 Position %d %d", p2.position.x, p2.position.y);

		break;

	case goingLeft_P2:

		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed_x != -p2.max_speed_x)
		{
			p2.speed_x -= p2.acceleration_x;
		}

		p2.position.x += p2.speed_x;  //p2.speed_x  is negative here.

		break;

	case jumping_P2:

		if (p2.grounded == true /*|| p2.jumpCount != 2*/)
		{
			p2.speed_y = -p2.gravity;

			/*jumpCount++;*/
			p2.p2_isGrounded(false);
		}

		break;
	}

	//If the p2 is in the air then this function brings him/her back down to the floor.
	if (p2.grounded == false)
	{
		p2.speed_y += p2.acceleration_y;

		if (p2.speed_y > p2.max_speed_x)
		{
			p2.speed_y = p2.max_speed_x;
		}

		p2.position.y += p2.speed_y;
	}

	//In case the HitBox clips through the ground.
	if (p2.position.y > p2.floor)
	{
		p2.position.y = p2.floor - 1;
		p2.p2_isGrounded(true);
	}

	//We move the character according the position value after the state has been run.
	p2.HitBox.x = p2.position.x;
	p2.HitBox.y = p2.position.y;

	//Draws the HitBox on-screen.
	App->render->DrawQuad(p2.HitBox, 255, 255, 0);

	return true;
};

bool j1Player2::PostUpdate()
{
	return true;
};

bool j1Player2::cleanUp()
{
	return true;
};


bool j1Player2::Load(pugi::xml_node& data)
{
	p2.position.x = data.child("position").attribute("x").as_int();
	p2.position.y = data.child("position").attribute("y").as_int();


	return true;
}

// Save Game State
bool j1Player2::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = p2.position.x;
	pos.append_attribute("y") = p2.position.y;

	return true;
}