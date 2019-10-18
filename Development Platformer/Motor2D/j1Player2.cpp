#include "j1App.h"
#include "j1Player2.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "p2Log.h"
//
//j1Player2::j1Player2() //Constructor. Called at the first frame.
//{
//	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
//	name.create("player"); //The string has to be the same as the name of the node in the xml file.
//};
//
//j1Player2::~j1Player2()  //Destructor. Called at the last frame.
//{
//
//};
//
//bool j1Player2::Init()
//{
//	return true;
//};
//
//bool j1Player2::Awake(pugi::xml_node& config)
//{
//	p2.position.x = config.child("player_2").child("position").attribute("x").as_float();
//	p2.position.y = config.child("player_2").child("position").attribute("y").as_float();
//
//	p2.speed.x = config.child("player_2").child("speed").attribute("x").as_float();
//	p2.speed.y = config.child("player_2").child("speed").attribute("y").as_float();
//	p2.max_speed.x = config.child("player_2").child("max_speed").attribute("x").as_float();
//	p2.max_speed.y = config.child("player_2").child("max_speed").attribute("y").as_float();
//
//	p2.acceleration.x = config.child("player_2").child("acceleration").attribute("x").as_float();
//	p2.acceleration.y = config.child("player_2").child("acceleration").attribute("y").as_float();
//	p2.gravity = config.child("player_2").child("gravity").attribute("value").as_float();
//
//	return true;
//};
//
//bool j1Player2::Start()
//{
//	p2.position = { p2.position.x, p2.position.y };
//	p2.HitBox = { (int)p2.position.x,(int)p2.position.y, p2.sprite_width, p2.sprite_height }; //Casked to int "(int)" for optimization.
//
//	p2.p2_isGrounded(true);
//
//	return true;
//};
//
//bool j1Player2::PreUpdate()
//{
//	//p2.p1_SetGroundState(false); //set  to false when colliders are implemented.
//
//	p2.state = idle_P2;
//
//	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
//	{
//		p2.state = goingRight_P2;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
//	{
//		p2.state = goingLeft_P2;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
//	{
//		p2.state = jumping_P2;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
//	{
//		p2.state = crouch_P2;
//	}
//
//	return true;
//};
//
//bool j1Player2::Update(float dt)
//{
//	p2_frames++;
//
//	//p2.p1_lastGrounded = p2.p1_position;
//
//	switch (p2.state)
//	{
//
//	case idle_P2:
//
//		p2.speed.x = 0;
//
//		break;
//
//	case goingRight_P2:
//
//		LOG("P2 GOING RIGHT %d %d", p2.speed.x, p2.max_speed.x);
//
//		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
//		while (p2.speed.x != p2.max_speed.x)
//		{
//			p2.speed.x += p2.acceleration.x;
//		}
//
//		p2.position.x += p2.speed.x; //p2.speed_x is positive here.
//
//		LOG("P2 Position %d %d", p2.position.x, p2.position.y);
//
//		break;
//
//	case goingLeft_P2:
//
//		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
//		while (p2.speed.x != -p2.max_speed.x)
//		{
//			p2.speed.x -= p2.acceleration.x;
//		}
//
//		p2.position.x += p2.speed.x;  //p2.speed_x  is negative here.
//
//		break;
//
//	case jumping_P2:
//
//		if (p2.grounded == true /*|| p2.jumpCount != 2*/)
//		{
//			p2.speed.y = -p2.gravity;
//
//			/*jumpCount++;*/
//			p2.p2_isGrounded(false);
//		}
//
//		break;
//	}
//
//	//If the p2 is in the air then this function brings him/her back down to the floor.
//	if (p2.grounded == false)
//	{
//		p2.speed.y += p2.acceleration.y;
//
//		if (p2.speed.y > p2.max_speed.x)
//		{
//			p2.speed.y = p2.max_speed.x;
//		}
//
//		p2.position.y += p2.speed.y;
//	}
//
//	//In case the HitBox clips through the ground.
//	if (p2.position.y > p2.floor)
//	{
//		p2.position.y = p2.floor - 1;
//		p2.p2_isGrounded(true);
//	}
//
//	//We move the character according the position value after the state has been run.
//	p2.HitBox.x = p2.position.x;
//	p2.HitBox.y = p2.position.y;
//
//	//Draws the HitBox on-screen.
//	App->render->DrawQuad(p2.HitBox, 255, 255, 0);
//
//	return true;
//};
//
//bool j1Player2::PostUpdate()
//{
//	return true;
//};
//
//bool j1Player2::cleanUp()
//{
//	return true;
//};
//
//
//bool j1Player2::Load(pugi::xml_node& data)
//{
//	p2.position.x = data.child("position").attribute("x").as_int();
//	p2.position.y = data.child("position").attribute("y").as_int();
//
//
//	return true;
//}
//
//// Save Game State
//bool j1Player2::Save(pugi::xml_node& data) const
//{
//	pugi::xml_node pos = data.append_child("position");
//
//	pos.append_attribute("x") = p2.position.x;
//	pos.append_attribute("y") = p2.position.y;
//
//	return true;
//}

j1Player2::j1Player2() : j1Module()
{
	name.create("player");

	idle.PushBack({ 0,0,110,74 });
	idle.PushBack({ 132,0,110,74 });
	idle.PushBack({ 220,0,110,74 });
	idle.PushBack({ 330,0,110,74 });
	idle.speed = 0.2f;

	run.PushBack({ 110,74,110,74 });
	run.PushBack({ 220,74,110,74 });
	run.PushBack({ 330,74,110,74 });
	run.PushBack({ 440,74,110,74 });
	run.PushBack({ 550,74,110,74 });
	run.PushBack({ 660,74,110,74 });
	run.speed = 0.2f;

	jump.PushBack({ 0,148,110,74 });
	jump.PushBack({ 110,148,110,74 });
	jump.PushBack({ 220,148,110,74 });
	jump.PushBack({ 330,148,110,74 });
	jump.PushBack({ 440,148,110,74 });
	jump.PushBack({ 550,148,110,74 });
	jump.PushBack({ 660,148,110,74 });
	jump.PushBack({ 0,222,110,74 });
	jump.PushBack({ 110,222,110,74 });
	jump.PushBack({ 220,222,110,74 });
	jump.speed = 0.2f;

	crouch.PushBack({ 440, 0, 110, 74 });
	crouch.PushBack({ 550, 0, 110, 74 });
	crouch.PushBack({ 660, 0, 110, 74 });
	crouch.PushBack({ 0, 74, 110, 74 });
	crouch.speed = 0.2f;

	death.PushBack({ 550, 370, 110, 74 });
	death.PushBack({ 660, 370, 110, 74 });
	death.PushBack({ 0, 444, 110, 74 });
	death.PushBack({ 110, 444, 110, 74 });
	death.PushBack({ 220, 444, 110, 74 });
	death.PushBack({ 330, 444, 110, 74 });
	death.PushBack({ 440, 444, 110, 74 });
	death.speed = 0.2f;
}

bool j1Player2::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	current_animation = &idle;
	return ret;
}
bool j1Player2::Start() {

	texture = App->tex->Load("textures/Spritesheets/Character 2/Character_Spritesheet2.png");
	texture2 = App->tex->Load("textures/Spritesheets/Character 2/adventurer-hand-combat-Sheet2.png");

	return true;
}

bool j1Player2::PreUpdate()
{

	return true;
}

bool j1Player2::Update(float dt) {

	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		x += velocity * 2;
		current_animation = &run;
		moving_right = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		x -= velocity * 2;
		current_animation = &run;
		moving_left = true;

	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

		current_animation = &crouch;
	}
	y += gravity;

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(texture, x, y, &r);

	return true;
}
