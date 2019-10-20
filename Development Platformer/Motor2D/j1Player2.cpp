#include "j1App.h"
#include "j1Player2.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "p2Log.h"

j1Player2::j1Player2() //Constructor. Called at the first frame.
{
	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
	name.create("player"); //The string has to be the same as the name of the node in the xml file.

	p2.idle.PushBack({ 28, 14, 37, 57 });
	p2.idle.PushBack({ 132, 12, 33, 59 });
	p2.idle.PushBack({ 230, 12, 37, 59 });
	p2.idle.PushBack({ 326, 14, 39, 57 });
	p2.idle.speed = 0.1f;

	p2.running_right.PushBack({ 134, 90, 39, 55 });
	p2.running_right.PushBack({ 232, 92, 39, 53 });
	p2.running_right.PushBack({ 332, 96, 39, 49 });
	p2.running_right.PushBack({ 434, 90, 45, 55 });
	p2.running_right.PushBack({ 532, 92, 39, 53 });
	p2.running_right.PushBack({ 632, 96, 39, 49 });
	p2.running_right.speed = 0.2f;

	p2.running_left.PushBack({ 134, 90, 39, 55 });
	p2.running_left.PushBack({ 232, 92, 39, 53 });
	p2.running_left.PushBack({ 332, 96, 39, 49 });
	p2.running_left.PushBack({ 434, 90, 45, 55 });
	p2.running_left.PushBack({ 532, 92, 39, 53 });
	p2.running_left.PushBack({ 632, 96, 39, 49 });
	p2.running_left.speed = 0.2f;

	p2.jumping.PushBack({ 30, 172, 39, 47 });
	p2.jumping.PushBack({ 130, 176, 39, 43 });
	p2.jumping.PushBack({ 234, 162, 37, 53 });
	p2.jumping.PushBack({ 328, 158, 41, 45 });
	p2.jumping.PushBack({ 436, 162, 29, 41 });
	p2.jumping.speed = 0.2f;

	p2.mid_jump.PushBack({ 528, 168, 47, 33 });
	p2.mid_jump.PushBack({ 640, 168, 35, 41 });
	p2.mid_jump.PushBack({ 22, 248, 51, 33 });
	p2.mid_jump.speed = 0.2f;

	p2.falling.PushBack({ 136, 224, 33, 61 });
	p2.falling.PushBack({ 236, 226, 33, 59 });
	p2.falling.speed = 0.2f;

	p2.crouching.PushBack({ 432, 30, 37, 41 });
	p2.crouching.PushBack({ 530, 28, 39, 43 });
	p2.crouching.PushBack({ 630, 28, 37, 43 });
	p2.crouching.PushBack({ 34, 104, 33, 41 });
	p2.crouching.speed = 0.1f;

	p2.death.PushBack({ 550, 370, 110, 74 });
	p2.death.PushBack({ 660, 370, 110, 74 });
	p2.death.PushBack({ 0, 444, 110, 74 });
	p2.death.PushBack({ 110, 444, 110, 74 });
	p2.death.PushBack({ 220, 444, 110, 74 });
	p2.death.PushBack({ 330, 444, 110, 74 });
	p2.death.PushBack({ 440, 444, 110, 74 });
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

	p2.speed.x = config.child("player_2").child("speed").attribute("x").as_float();
	p2.speed.y = config.child("player_2").child("speed").attribute("y").as_float();
	p2.max_speed.x = config.child("player_2").child("max_speed").attribute("x").as_float();
	p2.max_speed.y = config.child("player_2").child("max_speed").attribute("y").as_float();

	p2.acceleration.x = config.child("player_2").child("acceleration").attribute("x").as_float();
	p2.acceleration.y = config.child("player_2").child("acceleration").attribute("y").as_float();
	p2.gravity = config.child("player_2").child("gravity").attribute("value").as_float();

	p2.current_animation = &p2.idle;

	return true;
};

bool j1Player2::Start()
{
	p2.texture = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2_Buena.png");
	p2.texture2 = App->tex->Load("textures/Spritesheets/Character 2/adventurer-hand-combat-Sheet2.png");
	
	p2.position = { p2.position.x, p2.position.y };
	p2.HitBox = { (int)p2.position.x,(int)p2.position.y, p2.sprite_width, p2.sprite_height }; //Casked to int "(int)" for optimization.

	p2.collider = App->collisions->AddCollider(p2.HitBox, PLAYER, this);

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

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		p2.state = crouching_P2;
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

		p2.speed.x = 0;
		p2.current_animation = &p2.idle;

		break;

	case goingRight_P2:

		LOG("P2 GOING RIGHT %d %d", p2.speed.x, p2.max_speed.x);

		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed.x != p2.max_speed.x)
		{
			p2.speed.x += p2.acceleration.x;
		}

		p2.position.x += p2.speed.x; //p2.speed_x is positive here.

		p2.current_animation = &p2.running_right;
		p2.moving_right = true;

		LOG("P2 Position %d %d", p2.position.x, p2.position.y);

		break;

	case goingLeft_P2:

		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
		while (p2.speed.x != -p2.max_speed.x)
		{
			p2.speed.x -= p2.acceleration.x;
		}

		p2.position.x += p2.speed.x;  //p2.speed_x  is negative here.

		p2.current_animation = &p2.running_left;
		p2.moving_left = true;

		break;

	case crouching_P2:

		p2.current_animation = &p2.crouching;

	break;
	
	case jumping_P2:

		if (p2.grounded == true /*|| p2.jumpCount != 2*/)
		{
			p2.speed.y = -p2.gravity;

			/*jumpCount++;*/
			p2.p2_isGrounded(false);
		}

		break;
	}

	//If the p2 is in the air then this function brings him/her back down to the floor.
	if (p2.grounded == false)
	{
		p2.speed.y += p2.acceleration.y;

		if (p2.speed.y > p2.max_speed.x)
		{
			p2.speed.y = p2.max_speed.x;
		}

		p2.position.y += p2.speed.y;

		if (p2.speed.y < 0)
		{
			p2.current_animation = &p2.jumping;
		}
		else
		{
			p2.current_animation = &p2.falling;
		}
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
	//App->render->DrawQuad(p2.HitBox, 255, 255, 0);

	p2.HitBox = p2.current_animation->GetCurrentFrame();

	App->render->Blit(p2.texture, p2.position.x, p2.position.y, &p2.HitBox);

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

//Collision Handling
void j1Player2::OnCollision(Collider* C1, Collider* C2) //See if without * it works
{
	if (C2->type == PLAYER)
	{

	}
}

void j1Player2::GodModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{

	}
}