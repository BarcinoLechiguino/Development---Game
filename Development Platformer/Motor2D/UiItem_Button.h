//#ifndef UIITEM_BUTTON_H
//#define UIITEM_BUTTON_H
//
//#include "UiItem.h"
//#include "p2Point.h"
//#include "j1Textures.h"
//#include <string>
//#include <map>
//
//
//class UiItem_Button :public UiItem
//{
//protected: // Protected because OnClick functions are overriding from other UI classee from UiItem module
//
//	SDL_Rect frames[MAX_STATES];
//	void(*functionClickDown) () = nullptr;
//	void(*functionClickUp)() = nullptr;
//
//public:
//	UiItem_Button(SDL_Rect hitBox, const SDL_Rect * idle,  const SDL_Rect * click = nullptr, const SDL_Rect * hover = nullptr, p2Point<int> pos = { 0,0 }, UiItem* const parent); // All parameters are from config.xml
//
//	void AddFunction(p2SString & string, bool Down = true);
//
//	void Draw(const float&) override;
//	void ClickUp() override;
//	void ClickDown() override;
//
//};
//
//#endif