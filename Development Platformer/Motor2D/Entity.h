#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"


class Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		PLAYER1,
		PLAYER2,
		FLYING_ENEMY,
		LAND_ENEMY,
	};

public:
	Entity();
	Entity(entityType type); // here we will set initial position
	~Entity();
	virtual bool Awake(pugi::xml_node & config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool Move() { return true; }; //function to make the enemies move

	virtual void CleanUp() {};
	virtual void Save(pugi::xml_node& file) const {};
	virtual void Load(pugi::xml_node& file) {};
	virtual void ChangeAnimation() {};
	virtual void BecomeGrounded() {};
	virtual void Restart() {};
	virtual void LoadPushbacks() {};
	virtual void GodModeInput();

	//Colliders funnctions (poner)

	void FlipImage();
	void Draw();
	bool AddAnimationPushbacks(); //Adds animation pushbacks
	bool LoadProperties(pugi::xml_node&); //Adds properties from xml
private:

public:
	p2Point<float>	position;
	p2Point<float>	spawn_position;
	p2Point<float>	speed;
	p2Point<float>	max_speed;
	p2Point<float>	acceleration;
	p2Point<int>	body_margin;
	p2Point<int>	sprite_measures;
	float			gravity;
	int				collision_tolerance;
	bool			isAlive;
	bool			isDying;
	bool			grounded;
	bool			flip;
	bool			chasing_player;
	Animation* Current_Animation = nullptr;
	entityType type;
	Collider*	collider;
private:

};

#endif