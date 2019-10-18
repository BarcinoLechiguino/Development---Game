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
#include "j1FadeScene.h"
#include "j1Audio.h"

//j1Player1::j1Player1() //Constructor. Called at the first frame.
//{
//	//String that will be given to the different functions (Awake(), Load()...) to generate the handler node.
//	name.create("player"); //The string has to be the same as the name of the node in the xml file.
//};
//
//j1Player1::~j1Player1()  //Destructor. Called at the last frame.
//{
//
//};
//
//bool j1Player1::Init() 
//{
//	Graphicsp1 = App->tex->Load("textures/Spritesheets/Character 1/Character_Spritesheet.png");
//
//	return true;
//};
//
//bool j1Player1::Awake(pugi::xml_node& config) 
//{	
//	p1.position.x = config.child("player_1").child("position").attribute("x").as_float();
//	p1.position.y = config.child("player_1").child("position").attribute("y").as_float();
//
//	p1.speed.x = config.child("player_1").child("speed").attribute("x").as_float();
//	p1.speed.y = config.child("player_1").child("speed").attribute("y").as_float();
//	p1.max_speed.x = config.child("player_1").child("max_speed").attribute("x").as_float();
//	p1.max_speed.y = config.child("player_1").child("max_speed").attribute("y").as_float();
//
//	p1.acceleration.x = config.child("player_1").child("acceleration").attribute("x").as_float();
//	p1.acceleration.y = config.child("player_1").child("acceleration").attribute("y").as_float();
//	p1.gravity = config.child("player_1").child("gravity").attribute("value").as_float();
//	
//	return true;
//};
//
//bool j1Player1::Start() 
//{
//	p1.position = { p1.position.x, p1.position.y };
//	p1.HitBox = { (int)p1.position.x,(int)p1.position.y, p1.sprite_width, p1.sprite_height }; //Casked to int "(int)" for optimization.
//
//	p1.p1_isGrounded(true);
//	
//	return true;
//};
//
//bool j1Player1::PreUpdate() 
//{
//	//p1.p1_SetGroundState(false); //set  to false when colliders are implemented.
//	
//	p1.state = idle_P1;
//	
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
//	{
//		p1.state = goingRight_P1;
//	}
//	
//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
//	{
//		p1.state = goingLeft_P1;
//	}
//	
//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
//	{
//		p1.state = jumping_P1;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
//	{
//		p1.state = crouch_P1;
//	}
//
//	return true;
//};
//
//bool j1Player1::Update(float dt) 
//{
//	frames++;
//
//	//p1.p1_lastGrounded = p1.p1_position;
//	
//	switch (p1.state)
//	{
//	
//	case idle_P1:
//	
//		p1.speed.x = 0;
//	
//		break;
//	
//	case goingRight_P1:
//	
//		LOG("P2 GOING RIGHT %d %d", p1.speed.x, p1.max_speed.x);
//	
//		//As long as D is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
//		while (p1.speed.x != p1.max_speed.x)
//		{
//			p1.speed.x += p1.acceleration.x;
//		}
//	
//		p1.position.x += p1.speed.x; //p1.speed_x is positive here.
//	
//		LOG("P2 Position %d %d", p1.position.x, p1.position.y);
//	
//		break;
//	
//	case goingLeft_P1:
//	
//		//As long as W is pressed, speed will increase each loop until it reaches cruiser speed, which then speed will be constant.
//		while (p1.speed.x != -p1.max_speed.x)
//		{
//			p1.speed.x -= p1.acceleration.x;
//		}
//	
//		p1.position.x += p1.speed.x;  //p1.speed_x  is negative here.
//	
//		break;
//	
//	case jumping_P1:
//	
//		if (p1.grounded == true /*|| p1.jumpCount != 2*/)
//		{
//			p1.speed.y = -p1.gravity;
//	
//			/*jumpCount++;*/
//			p1.p1_isGrounded(false);
//		}
//
//		break;
//	}
//	
//	//If the p1 is in the air then this function brings him/her back down to the floor.
//	if (p1.grounded == false)
//	{
//		p1.speed.y += p1.acceleration.y;
//		
//		if (p1.speed.y > p1.max_speed.y)
//		{
//			p1.speed.y = p1.max_speed.y;
//		}
//	
//		p1.position.y += p1.speed.y;
//	}
//	
//	//In case the HitBox clips through the ground.
//	if (p1.position.y > p1.floor)
//	{
//		p1.position.y = p1.floor - 1;
//		p1.p1_isGrounded(true);
//	}
//
//	//We move the character according the position value after the state has been run.
//	p1.HitBox.x = p1.position.x; 
//	p1.HitBox.y = p1.position.y;
//	
//	//Draws the HitBox on-screen.
//	App->render->DrawQuad(p1.HitBox, 255, 0, 0);
//	
//	//rect = current_animation->GetCurrentFrame();
//	
//	//App->render->Blit(Graphicsp1, p1.position.x, p1.position.y, &rect); //falta el sdl rect
//	return true;
//};
//
//bool j1Player1::PostUpdate() 
//{
//	return true;
//};
//
//bool j1Player1::cleanUp() 
//{
//	return true;
//};
//
//
//bool j1Player1::Load(pugi::xml_node& data)
//{
//	p1.position.x = data.child("position").attribute("x").as_int();
//	p1.position.y = data.child("position").attribute("y").as_int();
//
//
//	return true;
//}
//
//// Save Game State
//bool j1Player1::Save(pugi::xml_node& data) const
//{
//	pugi::xml_node pos = data.append_child("position");
//
//	pos.append_attribute("x") = p1.position.x;
//	pos.append_attribute("y") = p1.position.y;
//	
//	return true;
//}

j1Player1::j1Player1() : j1Module()
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

	crouch.PushBack({440, 0, 110, 74});
	crouch.PushBack({550, 0, 110, 74});
	crouch.PushBack({660, 0, 110, 74});
	crouch.PushBack({0, 74, 110, 74});
	crouch.speed = 0.2f;

	death.PushBack({550, 370, 110, 74});
	death.PushBack({660, 370, 110, 74});
	death.PushBack({0, 444, 110, 74});
	death.PushBack({110, 444, 110, 74});
	death.PushBack({220, 444, 110, 74});
	death.PushBack({330, 444, 110, 74});
	death.PushBack({440, 444, 110, 74});
	death.speed = 0.2f;
}

bool j1Player1::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	current_animation = &idle;
	return ret;
}
bool j1Player1::Start() {

	texture = App->tex->Load("textures/Spritesheets/Character 1/Character_Spritesheet.png");
	texture2 = App->tex->Load("textures/Spritesheets/Character 1/adventurer-hand-combat-Sheet.png");

	return true;
}

bool j1Player1::PreUpdate()
{
	return true;
}

bool j1Player1::Update(float dt) {

	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		x += velocity*2;
		current_animation = &run;
		moving_right = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		x -= velocity*2;
		current_animation = &run;
		moving_left = true;

	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		
		current_animation = &crouch;
	}
	y += gravity;

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(texture, x, y, &r);

	return true;
}

