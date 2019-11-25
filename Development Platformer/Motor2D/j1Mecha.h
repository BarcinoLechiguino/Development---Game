#ifndef __ModuleMecha__
#define __ModuleMecha__

#include "j1Module.h"
#include "j1Enemy.h"
#include "Animation.h"
#include "p2Point.h"

class j1Mecha : public j1Enemy
{
public:
	j1Mecha(int x, int y, ENTITY_TYPE type);

	~j1Mecha();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

public:
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Normal_Path();
	void Chasing_Path();
	//bool Load_Entity();
	void InitMecha();							
	void LoadAnimationPushbacks();
	void LoadEntityProperties();
	void LoadEntityAudio();

public:

public:
	Animation	idle;				//Idle animation.
	Animation	running;			//Running animation.


};

#endif // !__ModuleMecha__
