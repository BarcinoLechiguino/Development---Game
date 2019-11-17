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
	falling_P2,
	boost_jump_P2,
	teleporting_P2,
	dying_P2
};

struct Player2
{
	p2Point<float>	position;				//Point with the position of P2 on the world.
	p2Point<float>	spawn_position;			//Keeps record of the first position(x, y) of P2 when spawns in the map.
	p2Point<float>	speed;					//P2's Speed Vector Variable. (Not actually declared as a vector but that is irrelevant to us right now)
	p2Point<float>	max_speed;				//P2's Cruiser Speed for both axis.
	p2Point<float>	acceleration;			//Sets how much time it takes P2 to reach Cruiser Speed horizontally and/or vertically.
	p2Point<float>	boost_jump;				//Sets how much vertical or horizontal impulse will P2 get.
	p2Point<int>	body_margin;			//Collider adjustment to the body of P2
	p2Point<int>	sprite_measures;		//Sprite measurements (w, h), sets the width and height of the collider. / Calculate center pos: mid_pos = p1.position.x +/- (p1.hitbox.width / 2). +/- depending on which side P1 is facing.
	float			gravity;				//Acceleration variable for jumps. Gravitational Pull.
	int				lives;					//Number of lives P2 has. It just defines how many times P2 can get impaled with spikes before having to restart the whole level over.
	int				max_lives;				//Maximum number of lives a player can have.
	int				collision_tolerance;	//Maximum tolerance that OnCollision will have with collisions.

	bool			grounded;				//Keeps track of P1 and returns true when P2 is not jumping or falling.
	bool			airborne;				//Keeps track of whether or not P2 is on the air.
	bool			flip;					//Keeps track of which direction P2's is looking at. Changes the sprite orientation when returns true.
	bool			isCrouching;			//Keeps track of the action P2 is performing. In this case crouching.
	bool			isJumping;				//Keeps track of the action P2 is performing. In this case jumping.
	bool			isBoostJumping;			//Keeps track of the action P2 is performing. In this case boost jumping.
	bool			item_activated;			//Keeps track of the items P2 interacts with.
	bool			isGoingRight;			//Keeps track of whether or not P2 is going to the right.
	bool			isGoingLeft;			//Keeps track of whether or not P2 is going to the left.
	bool			platformDrop;			//Keeps track of whether or  not P2 wants to drop from a plaform.
	bool			isAlive;				//If P2 runs out of lives, then this bool returns false.
	bool			isDying;				//If P2 runs out of lives, then this bool returns true.
	bool			againstRightWall;		//If P2 is against a right wall then this is true.
	bool			againstLeftWall;		//If P2 is against a left wall then this is true.
	bool			againstCeiling;			//If P2 is against a ceiling then this is true.
	bool			fading;					//Fade P2 when changing scenes.
	bool			GodMode;				//Defines whether GodMode is active or not for P2.
	bool			switch_sprites;			//Defines which sprite will P2 have.

	//Animation Variables
	SDL_Texture* texture = nullptr;

	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.
	Animation	crouching;			//Crouching animation.
	Animation	jumping;			//Jumping animation.
	Animation	frontflip;			//Frontflip  animation.
	Animation	falling;			//Falling animation.
	Animation	boosted_jump;		//Boosted Jump Animation.
	Animation	death;				//Death animation.
	Animation*	current_animation;	//P1's current animation.

	//Sound Variables
	uint		jumpFX;				//Jump sfx.
	uint		deathFX;			//Death sfx.
	uint		duoFX;				//Boost Jump sfx.
	uint		goalFX;				//Goal Reached sfx.
	uint		tpFX;				//Teleport sfx.
	uint		activateFX;			//Item Activation sfx.


	SDL_Rect	HitBox;				//Rectangle that represents P2.
	P2_State	state;				//Adds the state enum to P2's variables.
	Collider*	collider;			//Collider that will be assigned to P2. 

	/*p2SString		jumpFX;
	p2SString		deathFX;
	p2SString		duoFX;*/
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

	void TeleportP1ToP2();							//Moves P1 directly in front of P2. It takes into account where P1 is looking at.
	void RespawnP2ToP1();							//Moves P1 directly behind P2 on death.
	void OnCollision(Collider* C1, Collider* C2);	//Collision Logic Handling.

	bool Load(pugi::xml_node &node);				//Loading from xml file.
	bool Save(pugi::xml_node &node) const;			//Saving to xml file.
	bool LoadPlayer2();								//Loads P2 on screen.
	bool LoadPlayer2Properties(pugi::xml_node&);	//Loads P2's data from the config xml file.
	bool AddAnimationPushbacks();					//Adds P2's animation pushbacks.
	//bool LoadPlayer2Textures();					//Loads P2's textures on screen.

	void Restart();									//Resets P2's position to where P2 started the level. 
	void GodModeInput();							//Enables / Disables the God Mode.

private:
	float half = 0.5f;
	float quarter = 0.25f;
	float threeFourths = 0.75f;
};

#endif __j1Player_1_H__