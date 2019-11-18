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

enum entity_state
{
	IDLE = 0,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DEAD,
	HURTED
};

enum class ENTITY_TYPE
{
	UNKNOWN_TYPE,
	PLAYER = 0,
	PLAYER2,
	MECHA,
	ALIEN,
};

class j1Entity : public j1Module
{
public:  //Set to private later.
	j1Entity(int x, int y, ENTITY_TYPE type); // here we will set initial position

	virtual ~j1Entity();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt, bool doLogic);

	bool PostUpdate();

	bool CleanUp();

public:
	//Entity Methods
	/*virtual bool LoadEntity();
	virtual bool LoadEntityProperties(pugi::xml_node& conﬁg);
	virtual bool AddAnimationPushbacks();*/
	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);
	virtual void Restart();								//Maybe not needed

	virtual void BlitEntity(int x, int y, SDL_Rect entity_rect, bool flip);
	virtual void Entity_OnCollision(Collider* c1, Collider* c2); /*{};*/				//If {} are used then the OnCollision on the entity.cpp needs to be erased.
	bool Calculate_Path();

	//Entity Variables
	ENTITY_TYPE type;
	p2Point<float>	position;
	p2Point<float>	velocity;
	p2Point<float>	spawn_position;
	p2Point<float>	max_speed;
	p2Point<float>	acceleration;
	p2Point<int>	sprite_measures;
	float			gravity;
	int				lives;

	SDL_Texture* entity_sprite = nullptr;
	Collider* collider = nullptr;
	Animation* current_animation = nullptr;
	entity_state state;
	SDL_Rect	HitBox;
	
	//p2DynArray<iPoint> entityPath;		//Only for enemies

};

#endif