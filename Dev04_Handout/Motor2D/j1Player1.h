#ifndef _PLAYER_
#define _PLAYER_

#include "j1Module.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Collider;

class j1Player1 : public j1Module
{
public:
	j1Player1();
	~j1Player1() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

public:

	Animation idle;
	Animation run;
	Animation death;
	Animation jump;
	Animation crouch;
	Animation* current_animation;

	SDL_Texture* texture = nullptr;
	SDL_Texture* texture2 = nullptr;

public:

	int lives = 1;
	float x = 32.0f;  // 2 blocks * 16 each one
	float y = 350.0f; // 27 blocks * 16 each one - height pj

	float velocity = 2.0f;
	float gravity = 3.0f;

	bool moving_right = false;
	bool moving_left = false;
};



#endif // !_PLAYER_
