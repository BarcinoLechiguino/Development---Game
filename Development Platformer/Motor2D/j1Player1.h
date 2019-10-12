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
	jumping_P1
};

//struct Input_P1
//{
//	bool A_active; //Go Left
//	bool D_active; //Go Right
//	bool W_active; //Jump
//	bool S_active; //Crouch
//	bool C_active; //Boost
//};

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

	SDL_Rect p1_HitBox; //Rectangle that represents the player.
	p2Point<float> p1_position; //Vector with the position of P1
	P1_State p1_State; //Adds the state enum to the player's variables.
};

struct SDL_Texture;

class j1Player1 : public j1Module 
{
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

public: //P1 Variables
	
	Player1 p1;

	/*Animation * animation = nullptr;

	int jump;
	int distance;
	float gravity;
	bool start_jump = false;
	bool top_jump = true;
	bool stay_in_platform = false;
	iPoint speed;*/
	
	iPoint position;

	void OnCollision(Collider* collider);
	bool Load(pugi::xml_node &node);
	bool Save(pugi::xml_node &node) const;

private:

	//Animation idle;
	//Animation jump_anim;
	//Animation run;
	//Animation crouch;
	//iPoint player_size;

	//pugi::xml_document file;

	float p1_frames = 0;
	//bool runFrames = false;
	float p1_startFrame = 0;
};

#endif __j1Player_1_H__

