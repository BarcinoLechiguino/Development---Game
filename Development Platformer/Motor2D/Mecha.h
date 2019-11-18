#ifndef __ModuleMecha__
#define __ModuleMecha__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Mecha : public j1Entity
{
public:
	Mecha(int x, int y, ENTITY_TYPE type);
	~Mecha();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Normal_Path();
	void Chasing_Path();
	bool Load_Entity();
	bool AddAnimationPushbacks();
	bool LoadMechaProperties(pugi::xml_node& config);

public:
	p2Point<float>	spawn_position;
	bool grounded = false;
	bool airborne = false;
	bool flip = false;
	bool isGoingRight = false;
	bool isGoingLeft = false;
	bool fading = false;
	bool isAlive = true;
	bool isDying = false;
	bool againstRightWall = false;
	bool againstLeftWall = false;

public:
	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.


};

#endif // !__ModuleMecha__
