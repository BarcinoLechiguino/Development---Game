//#ifndef __ModuleAlien__
//#define __ModuleAlien__
//
//#include "j1Module.h"
//#include "j1Entity.h"
//#include "Animation.h"
//#include "p2Point.h"
//
//class Alien : public j1Entity
//{
//public:
//	Alien(int x, int y, ENTITY_TYPE type);
//	~Alien();
//
//	bool Awake(pugi::xml_node&);
//	bool Start();
//	bool Update(float dt);
//	bool PostUpdate();
//	bool CleanUp();
//	void OnCollision(Collider* c1, Collider* c2);
//
//	bool Load(pugi::xml_node&);
//	bool Save(pugi::xml_node&) const;
//
//	bool Load_Entity();
//	bool AddAnimationPushbacks();
//	bool LoadAlienProperties(pugi::xml_node& config);
//
//	void Chasing_Path();
//	void Normal_Path();
//
//public:
//	
//	p2Point<float>	spawn_position;
//	bool airborne = false;
//	bool flip = false;
//	bool isGoingRight = false;
//	bool isGoingLeft = false;
//	bool fading = false;
//	bool isAlive = true;
//	bool isDying = false;
//	bool againstRightWall = false;
//	bool againstLeftWall = false;
//
//public:
//	Animation	idle;				//Idle animation.
//	Animation	running;			//Running animation.
//};
//
//#endif // !__ModuleAlien__




