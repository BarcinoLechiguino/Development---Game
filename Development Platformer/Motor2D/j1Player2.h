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
	jumping_P2
};

//struct Input_P2
//{
//	bool H_active; //Go Left
//	bool K_active; //Go Right
//	bool U_active; //Jump
//	bool J_active; //Crouch
//	bool N_active; //Boost
//};

struct Player2
{
	p2Point<float> position; //Vector with the position of P1
	p2Point<float> pre_Jump_Position; //

	p2Point<float> speed; //Speed on X. Basic Movement
	p2Point<float> max_speed; //Cruiser speed for both axis.

	p2Point<float> acceleration; //Time it takes the player to reach Cruiser Speed horizontally and/or vertically.
	float gravity; //Acceleration variable for jumps. Gravitational Pull.

	bool grounded; //Defines whether the player is standing or jumping.

	//Changes the state of the player depending on the given argument. Also if true it records the position from where the player jumped.
	void p2_isGrounded(bool yesnt)
	{
		if (grounded == true)
		{
			pre_Jump_Position = position;
		}

		grounded = yesnt;
	};

	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;

	Animation idle;
	Animation running_right;
	Animation running_left;
	Animation death;
	Animation crouching;
	Animation jumping;
	Animation mid_jump;
	Animation falling;
	Animation* current_animation;

	bool moving_right = false;
	bool moving_left = false;
	bool flip = false;
	uint duojump_fx, jump_fx, death_fx;

	SDL_Rect HitBox; //Rectangle that represents the player.
	P2_State state; //Adds the state enum to the player's variables.
	Collider* collider;

	//Temporal Variables
	int sprite_width = /*20*/ 38;
	int sprite_height = /*30*/64;
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

	bool cleanUp();

public: //P1 Variables

	Player2 p2;

	void OnCollision(Collider* C1, Collider* C2);
	bool Load(pugi::xml_node &node);
	bool Save(pugi::xml_node &node) const;
	void GodModeInput();

	bool fading = false; // fade character when changing scenes

private:
	float p2_frames = 0;
	//bool runFrames = false;
	float p2_startFrame = 0;
};

#endif __j1Player_1_H__