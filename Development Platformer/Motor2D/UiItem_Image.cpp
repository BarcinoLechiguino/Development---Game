//#include "UiItem_Image.h"
//#include "j1App.h"
//#include "j1Textures.h"
//#include "j1Render.h"
//#include "j1Gui.h"
//#include "j1Scene.h"
//#include "Brofiler\Brofiler.h"
//
//UiItem_Image::UiItem_Image(SDL_Rect hitBox, const SDL_Rect * section, UiItem* const parent, p2Point<int> pivot) : UiItem(hitBox, parent, pivot)
//{
//	assert(parent != nullptr);
//	animationIdle.PushBack(*section);
//}
//
//void UiItem_Image::Draw(const float& dt)
//{
//	if (scaled)
//	{
//		App->render->Blit((SDL_Texture*)App->Gui->getTexture(), { hitBox.x - pos.x, hitBox.y - pos.y }, &animationIdle.GetCurrentFrame(dt), 0.0F, SDL_FLIP_NONE, scale);
//	}
//			
//	else
//	{
//		App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pos.x, hitBox.y - pos.y, &animationIdle.GetCurrentFrame(dt), SDL_FLIP_NONE, 0.0F);
//	}
//			
//}
