#ifndef _UI_Item__H
#define _UI_Item__H

#include "j1Module.h"
#include "p2Point.h"

enum UI_State //Types of states
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATE
};


class UI_Item
{
public:
	UI_Item() {};
	virtual ~UI_Item() {};

	virtual bool Start() { return true; };
	bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate() { return true; };

	virtual bool OnHover() { return true; }; //To do an action on hover
	bool OnClick(); //Same but with click state
	bool OnClick2(); //Click on thumb to drag 

public:
	UI_State	state = IDLE; //Initial state
	iPoint		pos;
	iPoint		position;
	iPoint		mouse_position;
	UI_Item*		parent = nullptr; //Global item

	bool		drag = false; //For draggable objects
	bool		static_object = false; //Static button
	bool		visible = true; //Show UI items
};

#endif  // !_UI_Item__H
