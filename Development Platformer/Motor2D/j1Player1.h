//#ifndef __j1PLAYER_1_H__
//#define __j1PLAYER_1_H__
//
//#include "j1Module.h"
//#include "p2Point.h"
//#include "j1Render.h"
//#include "j1Input.h"
//#include "Animation.h"
//
//enum P1_State 
//{
//	idle_P1,
//	goingRight_P1,
//	goingLeft_P1,
//	jumping_P1,
//	crouch_P1
//};
//
////struct Input_P1
////{
////	bool A_active; //Go Left
////	bool D_active; //Go Right
////	bool W_active; //Jump
////	bool S_active; //Crouch
////	bool C_active; //Boost
////};
//
//struct Player1
//{
//	p2Point<float> position; //Vector with the position of P1
//	p2Point<float> pre_Jump_Position; //
//	
//	p2Point<float> speed; //Speed on X. Basic Movement
//	p2Point<float> max_speed; //Cruiser speed for both axis.
//
//	p2Point<float> acceleration; //Time it takes the player to reach Cruiser Speed horizontally and/or vertically.
//	float gravity; //Acceleration variable for jumps. Gravitational Pull.
//	
//	bool grounded; //Defines whether the player is standing or jumping.
//
//	//Changes the state of the player depending on the given argument. Also if true it records the position from where the player jumped.
//	void p1_isGrounded(bool yesnt)
//	{
//		if (grounded == true)
//		{
//			pre_Jump_Position = position;
//		}
//
//		grounded = yesnt;
//	};
//	
//	SDL_Rect HitBox; //Rectangle that represents the player.
//	P1_State state; //Adds the state enum to the player's variables.
//
//	//Temporal Variables
//	int sprite_width = /*20*/ 38;
//	int sprite_height = /*30*/64;
//	float floor = 1055.0f;
//};
//
//struct SDL_Texture;
//
//class j1Player1 : public j1Module 
//{
//public://methods
//
//	j1Player1();
//	// Destructor
//	virtual ~j1Player1();
//
//	bool Init();
//
//	bool Awake(pugi::xml_node&);
//
//	bool Start();
//
//	bool PreUpdate();
//
//	bool Update(float dt);
//
//	bool PostUpdate();
//
//	bool cleanUp();
//
//public: //P1 Variables
//	
//	Player1 p1;
//
//	void OnCollision(Collider* collider);
//	bool Load(pugi::xml_node &node);
//	bool Save(pugi::xml_node &node) const;
//
//	SDL_Texture* Graphicsp1 = nullptr;
//	SDL_Rect rect;
//	Animation* current_animation = nullptr;
//
//	bool fading = false; // fade character when changing scenes
//
//private:
//
//	//Animation idle;
//	//Animation jump_anim;
//	//Animation run;
//	//Animation crouch;
//	//iPoint player_size;
//
//	//pugi::xml_document file;
//
//	float frames = 0;
//	//bool runFrames = false;
//	float startFrame = 0;
//	
//	
//};
//
//#endif __j1Player_1_H__
//

#ifndef __j1PLAYER_1_H__
#define __j1PLAYER_1_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class j1Player1 : public j1Module
{
public:
	j1Player1();
	~j1Player1() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);

public:

	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;
	Animation idle;
	Animation run;
	Animation death;
	Animation jump;
	Animation crouch;
	Animation* current_animation;

public:

	float x = 0;  
	float y = 0;
	int lives = 1;
	float decrease_vel = 0.1f;
	float velocity = 1.0f;
	float gravity = 3.0f;
	bool moving_right = false;
	bool moving_left = false;
	uint duojump_fx, jump_fx, death_fx;

};



#endif __j1Player_1_H__