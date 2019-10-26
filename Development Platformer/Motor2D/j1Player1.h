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
	falling_P1,
	boost_jump_P1,
	teleporting_P1
};

struct Player1
{
	p2Point<float>	position;			//Point with the position of P1 on the world.
	p2Point<float>	last_grounded;		//Keeps record of the last position(x, y) that P1 was grounded.
	p2Point<float>	initial_position;	//Keeps record of the first position(x, y) of P1 when spawns in the map.
	p2Point<float>	speed;				//P1's Speed Vector Variable. (Not actually declared as a vector but that is irrelevant to us right now)
	p2Point<float>	max_speed;			//P1's Cruiser Speed for both axis.
	p2Point<float>	acceleration;		//Sets how much time it takes P1 to reach Cruiser Speed horizontally and/or vertically.
	p2Point<float>	boost_jump;			//Sets how much vertical or horizontal impulse will P1 get 
	float			gravity;			//Acceleration variable for jumps. Gravitational Pull.

	bool			grounded;			//Keeps track of P1 and returns true when P1 is not jumping or falling.
	bool			flip;				//Keeps track of which direction P1's is looking at. Changes the sprite orientation when returns true.
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
	
	//Animation Variables
	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;
	
	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.
	Animation	crouching;			//Crouching animation.
	Animation	jumping;			//Jumping animation.
	Animation	frontflip;			//Frontflip  animation.
	Animation	falling;			//Falling animation.
	Animation	boosted_jump;		//Boosted Jump Animation.
	Animation	death;				//Death animation.

	Animation*	current_animation;	//P1's current animation.

	bool moving_right = false;
	bool moving_left = false;
	
	SDL_Rect	HitBox;					//Rectangle that represents P1.
	P1_State	state;					//Adds the state enum to P1's variables.
	Collider*	collider;				//Collider that will be assigned to P1 

	//Temporal Variables
	int sprite_width = 38;  //Make center pos: mid_pos = p1.position.x +/- (p1.hitbox.width / 2). +/- depending on which side P1 is facing.
	int sprite_height = 58;
	float floor = 0.0f;

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

	bool CleanUp();

public: //P1 Variables
	
	Player1 p1;

	void TeleportP2ToP1();
	
	bool Load(pugi::xml_node &node);				//Loading from xml file.
	bool Save(pugi::xml_node &node) const;			//Saving to xml file.
	void OnCollision(Collider* C1, Collider* C2);	//Collision Logic Handling.
	void Restart();									//Resets P1's position to where P1 started the level. 
	void GodModeInput();							//Enables / Disables the God Mode.

	bool fading = false; // fade character when changing scenes
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