#include "j1App.h"
#include "j1Player2.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player2::j1Player2() {

	name.create("player");

};

j1Player2::~j1Player2() {


};

bool j1Player2::Init() {

	return true;
};

bool j1Player2::Awake(pugi::xml_node config) {


	return true;
};

bool j1Player2::Start() {

	position_P2 = { 200.0f,500.0f };
	P2_Sprite = { (int)position_P2.x,(int)position_P2.y,boxW,boxH };


	return true;
};

bool j1Player2::PreUpdate() {

	P2_State = idle_P2;

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		P2_State = goingRight_P2;
	}
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT) {
		P2_State = goingLeft_P2;
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		P2_State = jumpState_P2;
	}

	return true;
};

bool j1Player2::Update(float dt) {
	float a = 0.3f;

	switch (P2_State) {

	case idle_P2:
		accelerationFrames = 0;

		break;

	case goingRight_P2:




		position_P2.x += playerSpeed; // Move Right at Speed

		accelerationFrames++;

		break;

	case goingLeft_P2:
		position_P2.x -= playerSpeed; // Mode Left at Speed
		break;

	case jumpState_P2:

		break;
	}

	P2_Sprite.x = position_P2.x;

	App->render->DrawQuad(P2_Sprite, 255, 255, 0);


	return true;

};

bool j1Player2::PostUpdate() {

	return true;
};

bool j1Player2::cleanUp() {

	return true;
};