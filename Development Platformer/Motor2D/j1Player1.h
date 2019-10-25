#ifndef __j1PLAYER_1_H__
#define __j1PLAYER_1_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;

enum P1_State 
{
	idle_P1,
	goingRight_P1,
	goingLeft_P1,
	crouching_P1,
	jumping_P1,
	falling_P1
};

struct Player1
{
	p2Point<float>	position;			//Point with the position of P1 on the world.
	p2Point<float>	pre_Jump_Position;	//Keeps record of the last position(x, y) that P1 was grounded.
	
	p2Point<float>	speed;				//P1's Speed Vector Variable. (Not actually declared as a vector but that is irrelevant to us right now)
	p2Point<float>	max_speed;			//P1's Cruiser Speed for both axis.

	p2Point<float>	acceleration;		//Time it takes the player to reach Cruiser Speed horizontally and/or vertically.
	float			gravity;			//Acceleration variable for jumps. Gravitational Pull.
	

	bool			GoingRight = false;
	bool			GoingLeft = false;
	bool			Jumping = false;
	bool			Falling = false;
	bool			grounded;			//Tracks whether the player is standing on the ground or is jumping.
	bool			flip;				//Tracks the direction in which P1 is facing (animation sprite flip).

	//Tracks whether the player is grounded or not. If P1 is not on the ground then it also keeps record of the last position P1 was before jumping. 
	void p1_isGrounded(bool status)
	{
		if (grounded == true)
		{
			pre_Jump_Position = position;
		}

		grounded = status;
	};
	
	//Animation Variables
	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;
	
	Animation idle;					//Idle Animation of P1.
	Animation running_right;		//Running right Animation of P1.
	Animation running_left;			//Idle Animation of P1.
	Animation death;				//Idle Animation of P1.
	Animation crouching;			//Idle Animation of P1.
	Animation jumping;				//Idle Animation of P1.
	Animation mid_jump;				//Idle Animation of P1.
	Animation falling;				//Idle Animation of P1.

	Animation* current_animation;	//Tracks the current animation of P1.
	
	SDL_Rect	HitBox;				//Rectangle that represents the player.
	P1_State	state;				//Adds the state enum to the player's variables.
	Collider*	collider;			//Collider of P1.

	//Temporal Variables
	int		sprite_width = /*20*/ 38;
	int		sprite_height = /*30*/60;

	/*p2SString		jumpFX;
	p2SString		deathFX;
	p2SString		duoFX;*/
};

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

	void GoRight();									//Method that moves P1 to the right.
	void GoLeft();									//Method that moves P1 to the left.
	void Crouch();									//Method that makes P1 crouch.
	void Jump();									//Method that makes P1 jump.
	void Fall();									//Method that makes P1 fall.

	void OnCollision(Collider* C1, Collider* C2);	//Collision Handling
	bool Load(pugi::xml_node &node);				//Loading from xml file
	bool Save(pugi::xml_node &node) const;			//Saving to xml file
	void Restart();
	void GodModeInput();

	bool fading = false;							// fade character when changing scenes
	bool player1_alive = false;
	bool GodMode = false;

private:
	float p1_frames = 0;
	//bool runFrames = false;
	float p1_startFrame = 0;

	float x = 0; 
	float y = 0;
	
	uint		jumpFX;
	uint		deathFX;
	uint		duoFX;
	
	float velocity = 2.0f;
	float gravity = 3.0f;

	bool moving_right = false;
	bool moving_left = false;
};

#endif __j1Player_1_H__