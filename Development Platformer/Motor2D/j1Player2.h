#ifndef __j1PLAYER_2_H__
#define __j1PLAYER_2_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Animation.h"
#include "j1Player.h"

struct Collider;
struct SDL_Texture;

class j1Player2 : public j1Player
{
public://methods

	j1Player2(int x, int y, ENTITY_TYPE type);
	// Destructor
	virtual ~j1Player2();

	bool Init();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt, bool doLogic);

	bool PostUpdate();

	bool CleanUp();

public: //P2 Variables
	void TeleportP1ToP2();							//Moves P1 directly in front of P2. It takes into account where P1 is looking at.
	void RespawnP2ToP1();							//Moves P1 directly behind P2 on death.
	void LivesCheck(int lives);						//Checks if the player has any lives left.
	void OnCollision(Collider* C1, Collider* C2);	//Collision Logic Handling.

	bool Load(pugi::xml_node &node);				//Loading from xml file.
	bool Save(pugi::xml_node &node) const;			//Saving to xml file.
	//bool LoadPlayer2Textures();					//Loads P2's textures on screen.

	void Restart();									//Resets P2's position to where P2 started the level. 
	void GodModeInput();							//Enables / Disables the God Mode.
};

#endif __j1Player_2_H__