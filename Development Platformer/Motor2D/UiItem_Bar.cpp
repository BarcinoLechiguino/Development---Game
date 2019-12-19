#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Scene.h"
//#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "UiItem_Thumb.h"
#include "UiItem_Bar.h"
#include "UI_Image.h"
#include "UiItem_Button.h"

UiItem_Bar::UiItem_Bar(SDL_Rect slider_box, UI_Item* parent /*p2Point<int> pivo*/)
{
	s_box = slider_box;

}

bool UiItem_Bar::Start()
{
	thumb = App->gui->CreateThumb({ 619,182,24,22 }, this);
	return true;
}

bool UiItem_Bar::PostUpdate()
{
	bool ret = true;

	ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &s_box);



	return ret;
}

float UiItem_Bar::GetThumbValue()
{
	float ipos_bar = thumb->thumb_box.x + (thumb->thumb_box.w / 2);
	float fixed_pos = box.x + (thumb->thumb_box.w / 2);
	float fpos_bar = box.x + box.w - (thumb->thumb_box.w / 2);
	float final_pos = (ipos_bar - fixed_pos) / (fpos_bar - fixed_pos);

	return final_pos;
}
