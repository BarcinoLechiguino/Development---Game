#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"

struct Collider;
class Animation;
struct SDL_Texture;

enum entity_state	//Does not go here. Is it necessary?
{
	IDLE = 0,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DEAD,
	HURT
};

enum class ENTITY_TYPE
{
	UNKNOWN_TYPE,
	PLAYER,
	PLAYER2,
	MECHA,
	ALIEN,
};

class j1Entity : public j1Module
{
public:
	j1Entity(int x, int y, ENTITY_TYPE type); // here we will set initial position

	//Must be virtual so when an entity is executed the compiler goes to the entity's methods instead of these
	virtual bool Awake(pugi::xml_node&);

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update(float dt, bool doLogic);

	virtual bool PostUpdate();

	virtual bool CleanUp();

public:
	//Entity Methods
	/*virtual bool LoadEntity();
	virtual bool LoadEntityProperties(pugi::xml_node& conﬁg);
	virtual bool AddAnimationPushbacks();*/
	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);
	virtual void Restart();								//Maybe not needed

	virtual void BlitEntity(int x, int y, SDL_Rect entity_rect, bool flip);
	virtual void OnCollision(Collider* c1, Collider* c2); /*{};*/				//If {} are used then the OnCollision on the entity.cpp needs to be erased.
	//bool Calculate_Path();			//Only for enemiess

	
	//p2Point<float>	velocity;		//Only for players and maybe land enemy.
	//p2Point<float>	spawn_position;
	//p2Point<float>	max_speed;		//Only for players and maybe land enemy.
	//p2Point<float>	acceleration;	//Only for players and maybe land enemy.
	//p2Point<int>		sprite_measures;
	//float				gravity;
	//int				lives;

	//Entity Variables
	ENTITY_TYPE		type;					//Type of the entity (ENTITY_TYPE::PLAYER...)
	fPoint			position;				//Initial position of the entity.
	iPoint			sprite_size;			//Size of the entity sprite --> w and h of the entity collider.

	SDL_Texture*	entity_sprite;			//Sprite / Spritesheet of the entity.
	Collider*		collider;				//Collider of the entity.
	Animation*		animation;				//Animation of the entity.
	//entity_state	state;
	//SDL_Rect		HitBox;
	
	//p2DynArray<iPoint> entityPath;		//Only for enemies

};

#endif