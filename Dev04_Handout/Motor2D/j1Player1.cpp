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
//#include "j1FadeScene.h"
#include "j1Audio.h"

j1Player1::j1Player1() : j1Module()
{
	name.create("player");

	float speed = 0.1f;
	idle.PushBack({ 0,0,110,74 });
	idle.PushBack({ 132,0,110,74 });
	idle.PushBack({ 220,0,110,74 });
	idle.PushBack({ 330,0,110,74 });
	
	float speed = 0.1f;
	run.PushBack({ 110,74,110,74 });
	run.PushBack({ 220,74,110,74 });
	run.PushBack({ 330,74,110,74 });
	run.PushBack({ 440,74,110,74 });
	run.PushBack({ 550,74,110,74 });
	run.PushBack({ 660,74,110,74 });
	
	float speed = 0.1f;
	jump.PushBack({ 0,148,110,74 }, speed);
	jump.PushBack({ 110,148,110,74 },speed);
	jump.PushBack({ 220,148,110,74 },speed);
	jump.PushBack({ 330,148,110,74 },speed);
	jump.PushBack({ 440,148,110,74 },speed);
	jump.PushBack({ 550,148,110,74 },speed);
	jump.PushBack({ 660,148,110,74 },speed);
	jump.PushBack({ 0,222,110,74 }, speed);
	jump.PushBack({ 110,222,110,74 }, speed);
	jump.PushBack({ 220,222,110,74 }, speed);

	float speed = 0.1f;
	crouch.PushBack({440, 0, 110, 74}, speed);
	crouch.PushBack({550, 0, 110, 74}, speed);
	crouch.PushBack({660, 0, 110, 74}, speed);
	crouch.PushBack({0, 74, 110, 74}, speed);
	

	float speed = 0.1f;
	death.PushBack({550, 370, 110, 74}, speed);
	death.PushBack({660, 370, 110, 74}, speed);
	death.PushBack({0, 444, 110, 74}, speed);
	death.PushBack({110, 444, 110, 74}, speed);
	death.PushBack({220, 444, 110, 74}, speed);
	death.PushBack({330, 444, 110, 74}, speed);
	death.PushBack({440, 444, 110, 74}, speed);
	
}

bool j1Player1::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	current_animation = &idle;
	return ret;
}
bool j1Player1::Start() {

	texture = App->tex->Load(""textures / Spritesheets / Character 1 / character_spritesheet_I_Buena.png"");
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
	//y += gravity;

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(texture, x, y, &r);

	return true;
}

bool j1Player1::CleanUp() {

	App->tex->UnLoad(texture);
	App->tex->UnLoad(texture2);
}


