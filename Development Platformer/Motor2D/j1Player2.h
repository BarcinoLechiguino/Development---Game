#ifndef __j1PLAYER_2_H__
#define __j1PLAYER_2_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;

enum P2_State
{
	idle_P2,
	goingRight_P2,
	goingLeft_P2,
	crouching_P2,
	jumping_P2,
	boost_jump_P2,
	teleporting_P2
};

struct Player2
{
	p2Point<float>	position;			//Point with the position of P2 on the world.
	p2Point<float>	last_grounded;		//Keeps record of the last position(x, y) that P2 was grounded.
	p2Point<float>	initial_position;	//Keeps record of the first position(x, y) of P2 when spawns in the map.
	p2Point<float>	speed;				//P2's Speed Vector Variable. (Not actually declared as a vector but that is irrelevant to us right now)
	p2Point<float>	max_speed;			//P2's Cruiser Speed for both axis.
	p2Point<float>	acceleration;		//Sets how much time it takes P2 to reach Cruiser Speed horizontally and/or vertically.
	float			gravity;			//Acceleration variable for jumps. Gravitational Pull.

	bool			grounded;			//Keeps track of P2 and returns true when P2 is not jumping or falling.
	bool			flip;				//Keeps track of which direction P2's is looking at. Changes the sprite orientation when returns true. 
	bool	isCrouching;

	//Changes the state of the player depending on the given argument. Also if true it records the position from where the player jumped.
	void isGrounded(bool status)
	{
		if (grounded == true)
		{
			last_grounded = position;	//Tracks P1's position as long as P1's is not jumping or falling.
		}

		grounded = status;
	};

	//Changes the state of the player depending on the given argument. Also if true it records the position from where the player jumped.
	void p2_isGrounded(bool yesnt)
	{
		if (grounded == true)
		{
			last_grounded = position;
		}

		grounded = yesnt;
	};

	//Animation Variables
	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;

	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.
	Animation	crouching;			//Crouching animation.
	Animation	jumping;			//Jumping animation.
	Animation	falling;			//Falling animation.
	Animation	boosted_jump;		//Boosted Jump Animation.
	Animation	death;				//Death animation.

	Animation*	current_animation;	//P2's current animation.

	bool moving_right = false;
	bool moving_left = false;

	SDL_Rect	HitBox;					//Rectangle that represents P2.
	P2_State	state;					//Adds the state enum to P2's variables.
	Collider*	collider;				//Collider that will be assigned to P2 

	uint duojump_fx, jump_fx, death_fx;


	//Temporal Variables
	int sprite_width = /*20*/ 38;
	int sprite_height = /*30*/58;
	float floor = 0.0f;
};

class j1Player2 : public j1Module
{
public://methods

	j1Player2();
	// Destructor
	virtual ~j1Player2();

	bool Init();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public: //P1 Variables

	Player2 p2;

	void TeleportP1ToP2();

	bool Load(pugi::xml_node &node);				//Loading from xml file.
	bool Save(pugi::xml_node &node) const;			//Saving to xml file.
	void OnCollision(Collider* C1, Collider* C2);	//Collision Logic Handling.
	void Restart();									//Resets P2's position to where P2 started the level. 
	void GodModeInput();							//Enables / Disables the God Mode.

	bool player2_alive = false;
	bool GodMode = false;
	bool fading = false; // fade character when changing scenes

private:
	float p2_frames = 0;
	//bool runFrames = false;
	float p2_startFrame = 0;
};

#endif __j1Player_1_H__