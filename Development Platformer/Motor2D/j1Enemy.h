#ifndef __ModuleMecha__
#define __ModuleMecha__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class j1Enemy : public j1Entity
{
public:
	j1Enemy(int x, int y, ENTITY_TYPE type);

	~j1Enemy();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Init();

	bool PreUpdate();

	bool Update(float dt,  bool doLogic);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

public:
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Normal_Path();
	void Chasing_Path();
	bool Load_Entity();
	bool AddAnimationPushbacks();
	bool LoadEnemyProperties(pugi::xml_node& config);

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