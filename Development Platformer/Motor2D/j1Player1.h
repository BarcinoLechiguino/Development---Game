#ifndef __j1PLAYER_1_H__
#define __j1PLAYER_1_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Animation.h"

enum P1_State 
{
	idle_P1,
	goingRight_P1,
	goingLeft_P1,
	jumping_P1,
};

struct Input_P1
{
	bool A_active; //Go Left
	bool D_active; //Go Right
	bool W_active; //Jump
	bool S_active; //Crouch
	bool C_active; //Boost
};

struct Player1
{
	float speed_x; //Speed on X. Basic Movement
	float speed_y; //Speed on Y. Used for jumps.
	float max_speed; //Cruiser speed for the player.
	float acceleration; //Time it takes the player to reach Cruiser Speed.

	float p1_gravity; //Acceleration variable for jumps. Gravitational Pull

	int p1_time; //Time P1 has been running.
	int p1_time_at_jump; //Time when the jump button is pressed.
	int p1_jump_time; //jumptimeP1 = time_P1 - timeAtJump_P1
	
	int sprite_width = 20;
	int sprite_height = 30;

	float floor = 500.0f;

	SDL_Rect p1_HitBox;
	p2Point<float> p1_position;
	P1_State p1_State;
};

class j1Player1 : public j1Module {
public://methods

	j1Player1();

	// Destructor
	virtual ~j1Player1();

	bool Init();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool cleanUp();

private:


public://variables
	Animation idle;

	Animation * current_P1_animation;


private:
	float p1_frames;
	//bool runFrames = false;
	float p1_startFrame;

	Player1 p1;
};

#endif __j1Player_1_H__

