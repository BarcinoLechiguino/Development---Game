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

	float acceleration_x; //Time it takes the player to reach Cruiser Speed horizontally.
	float acceleration_y; //Time it takes the player to reach Cruiser Speed vertically.
	float p1_gravity; //Acceleration variable for jumps. Gravitational Pull.
	
	bool p1_grounded; //Defines whether the player is standing or jumping.

	//Changes the state of the player depending on the given argument. Also if true it records the position from where the player jumped.
	void p1_isGrounded(bool yesnt)
	{
		if (p1_grounded == true)
		{
			p1_pre_Jump_Position = p1_position;
		}

		p1_grounded = yesnt;
	};
	
	SDL_Rect p1_HitBox; //Rectangle that represents the player.
	p2Point<float> p1_position; //Vector with the position of P1
	p2Point<float> p1_pre_Jump_Position; //
	P1_State p1_State; //Adds the state enum to the player's variables.

	//Temporal Variables
	int sprite_width = /*20*/ 38;
	int sprite_height = /*30*/64;
	float floor = 1055.0f;
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

