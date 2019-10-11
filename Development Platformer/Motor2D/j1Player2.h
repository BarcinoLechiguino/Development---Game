#ifndef __jPLAYER_2_H__
#define __jPLAYER_2_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"

enum P2_State {
	idle_P2,
	goingRight_P2,
	goingLeft_P2,
	jumpState_P2,
};

struct Input_P2
{
	bool H_active; //Go Left
	bool K_active; //Go Right
	bool U_active; //Jump
	bool J_active; //Crouch
	bool M_active; //Boost
};

class j1Player2 : public j1Module {
public://methods

	j1Player2();

	// Destructor
	virtual ~j1Player2();

	bool Init();

	bool Awake(pugi::xml_node config);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool cleanUp();

private:


public://variables





private:

	float floor = 500.0f;

	int boxW = 50;
	int boxH = 100;

	float playerSpeed = 5;

	int accelerationFrames = 0;


	SDL_Rect P2_Sprite;
	p2Point<float> position_P2;

	P2_State P2_State;
};

#endif // __j1Player_2_H__
