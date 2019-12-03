//#include "UiItem_Button.h"
//#include "j1App.h"
//#include "j1Render.h"
//#include "j1Gui.h"
//#include "p2Point.h"
//#include "j1Scene.h"
//#include "j1FadeScene.h"
//#include "j1Map.h"
//#include "Brofiler\Brofiler.h"
//
//UiItem_Button::UiItem_Button(SDL_Rect hitBox, const SDL_Rect * idle, const SDL_Rect * click, const SDL_Rect * hover, p2Point<int> pos, UiItem* const parent) : UiItem(hitBox, parent, pos)	//Prints on screen the button depending on whic state is the button
//{
//	assert (parent != nullptr);
//	
//		frames[IDLE] = *idle;
//		
//			if (click)
//				frames[CLICK] = *click;
//			else
//				frames[CLICK] = *idle;
//		
//			if (hover)
//				frames[HOVER] = *hover;
//			else
//				frames[HOVER] = *idle;
//	
//	
//}
//
//
//void UiItem_Button::AddFunction(p2SString & string, bool ClickDown)  //Adds the function that is suposed to execute that certain button, detects the string from xml and checks the state of the mouse
//{
//	BROFILER_CATEGORY("AddFunctionOnButton.cpp", Profiler::Color::Fuchsia)
//
//		if (&string != nullptr)
//		{
//			uint num = App->Gui->mapOfFunctions.Find(string);
//			if (ClickDown)
//				this->functionClickDown = App->Gui->mapOfFunctions.At(num);
//			else
//				this->functionClickUp = App->Gui->mapOfFunctions.At(num);
//		}
//}
//
//void UiItem_Button::Draw(const float&)  // Draws the button on screen
//{
//	
//		App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pos.x, hitBox.y - pos.y, &frames[state], SDL_FLIP_NONE, 0.0f);
//}
//
//void UiItem_Button::ClickDown() // Detects if the mouse is on click down state
//{
//	if (functionClickDown)
//	{
//		functionClickDown();
//	}
//}
//
//void UiItem_Button::ClickUp()  // Detects if the mouse is on click up state
//{
//	if (functionClickUp)
//	{
//		functionClickUp();
//	}
//}