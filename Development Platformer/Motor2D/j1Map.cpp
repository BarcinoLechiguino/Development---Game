#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Collisions.h"
#include "j1FadeScene.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"
#include <math.h>

//#include "mmgr/mmgr.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	/*spawn_position_cam.x = config.child("renderer").child("cam").attribute("x").as_float();
	spawn_position_cam.x = config.child("renderer").child("cam").attribute("y").as_float();*/

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw Map", Profiler::Color::Khaki);
	if (map_loaded == false)																						//If no map was loaded, return.
	{
		return;
	}

	App->win->GetWindowSize(winWidth, winHeight);																	//Gets the window size so it can be added to the camera collider as parameters.

	camera_collider.collider = { -App->render->camera.x, -App->render->camera.y, (int)winWidth, (int)winHeight };	//Sets the dimensions and position of the camera collider.

	int cam_tileWidth = winWidth / data.tile_width;																	//Width of the camera in tiles.
	int cam_tileHeight = winHeight / data.tile_height;																//Height of the camera in tiles.
	
	p2List_item<MapLayer*>* layer = data.layers.start;																//List_item that will iterate all layers.

	for (layer; layer != NULL; layer = layer->next)																	//As long as the item is not null.
	{
		cam_tilePos.x = (-App->render->camera.x * layer->data->speed) / data.tile_width;							//Position in the X axis of the camera in tiles. Takes into account parallax speed.
		cam_tilePos.y = (-App->render->camera.y * layer->data->speed) / data.tile_height;							//Position in the Y axis of the camera in tiles. Takes into account parallax speed.

		for (uint y = cam_tilePos.y; y < (cam_tilePos.y + cam_tileHeight + 2); ++y)									//While y is less than the camera's height in tiles //Change it so it is not hardcoded.
		{
			for (uint x = cam_tilePos.x; x < (cam_tilePos.x + cam_tileWidth + 2); ++x)								//While x is less than the camera's width in tiles. //Change it so it is not hardcoded.
			{	
				tile_index = layer->data->Get(x, y)/*x + y * data.tile_width*/;
				
				int tile_id = layer->data->gid[tile_index];															//Gets the tile id from the tile index.
				if (tile_id > 0)																					//If tile_id is not 0
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);												//Gets the tileset corresponding with the tile_id. If tile id is 34 and the current tileset first gid is 35, that means that the current  tile belongs to the previous tileset. 
					if (tileset != NULL)																			//While the tileset pointer is not null.
					{
						SDL_Rect tile_rect = tileset->GetTileRect(tile_id);											//Gets the position on the world and the dimensions of the rect of the given tile_id 
						iPoint pos = MapToWorld(x, y);																//Gets the position on the world (in pixels) of a specific point (in tiles). In the case of orthogonal maps the x and y are multiplied by the tile's width  or height. If 32x32, Map pos: x = 1 --> World pos: x = 32...
						SDL_Rect tile_hitBox = {pos.x, pos.y, data.tile_width, data.tile_height};					//Sets a rectangle that will act as hitbox when the cameraCulling on collision checks if the tile is inside or outside the camera boundaries. 

					
						if (layer->data->name == "Background")														//If the name of the layer  is "Background" its elements will be blitted with the specified parameters.
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Parallax")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect, false, layer->data->speed);	//As we need to add parallax to this layer, we pass a value as speed argument.
						}
						else if (layer->data->name == "ParallaxDecor")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect, false, layer->data->speed);
						}
						else if (layer->data->name == "Decor")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Ground")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Platforms")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Hazards")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Desactivable")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}
						else if (layer->data->name == "Portal")
						{
							App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						}

						//---------------------- PATHFINDING META TILES ----------------------
						/*else if (layer->data->name == "PathfindingCollisions")
						{
							if (PathfindingDebug == true)
							{
								App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
							}
						}*/
						
						//---------------------------TUTORIAL MAP LAYERS----------------------
						//else if (layer->data->name == "Floor")
						//{
						//	App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						//}
						//else if (layer->data->name == "Letters")
						//{
						//	App->render->Blit(tileset->texture, pos.x, pos.y, &tile_rect);
						//}
						
						//if (camera_collider.Check_Collision(tile_hitBox))											//Checks if the tile is inside or outside the camera boundaries by checking if there  has been a collision between the camera rect and the tile rect. 
						//{

						//}
					}
				}
			}
		}
	}
}

iPoint j1Map::MapToWorld(int x, int y) const 
{
	iPoint ret(0, 0);																//Position of the tile on the world.

	if (data.type == MAPTYPE_ORTHOGONAL)											//Position calculus for orhogonal maps
	{
		ret.x = x * data.tile_width;												//Position in the X axis of the tile on the world in pixels. For tile_width = 32 --> Tile 1: x = 0, Tile 2: x = 32.
		ret.y = y * data.tile_height;												//Position in the Y axis of the tile on the world in pixels. For tile_height = 32 --> Tile 1: y = 0, Tile 2: y = 32.
	}

	if (data.type == MAPTYPE_ISOMETRIC)												//Position calculus for isometric maps
	{
		ret.x = (x / (data.tile_width / 2) + y / (data.tile_height / 2)) / 2;
		ret.y = (y / (data.tile_height / 2) - x / (data.tile_width / 2)) / 2;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(uint tile_id) const
{
	SDL_Rect tile_rect;						//Declares a SDL_Rect where the tile's rect data members will be stored.

	int relative_id = tile_id - firstgid;	//Calculates the relative position of the tile_id respect the  first initial global id.

	tile_rect.w = tile_width;				//Sets the width of the Rect holding the tile to the width of the tile in pixels.
	tile_rect.h = tile_height;				//Sets the width of the Rect holding the tile to the height of the tile in pixels.
	tile_rect.x = margin + ((tile_rect.w + spacing) * (relative_id % num_tiles_width));
	tile_rect.y = margin + ((tile_rect.h + spacing) * (relative_id / num_tiles_width));

	return tile_rect;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const				//Revise. Its possible that it should do id < tilesetIter->data->firstgid.
{
	p2List_item<TileSet*>* tilesetIter = data.tilesets.start;
	TileSet* ret = NULL;

	while (tilesetIter != NULL)
	{
		if (id >= tilesetIter->data->firstgid)
		{
			ret = tilesetIter->data;
			break;
		}
		tilesetIter = tilesetIter->next;
	}
	
	return ret;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	//App->player1->Disable(); NEED A DISABLE FUNCTION
	// Remove all tilesets from memory
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{	
		SDL_DestroyTexture(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers from memory
	p2List_item<MapLayer*>* map_layer_item;
	map_layer_item = data.layers.start;

	while (map_layer_item != NULL)
	{
		RELEASE(map_layer_item->data);
		map_layer_item = map_layer_item->next;
	}
	data.layers.clear();

	//Removing all Objects from memory
	p2List_item<ObjectGroup*>* object_iterator = data.objectGroups.start;
	while (object_iterator != NULL)
	{	
		delete[] object_iterator->data->object;		//Frees the memory allocated to the object array. LoadObjectLayers() line 544.
		
		//RELEASE(object_iterator->data->object->collider);

		RELEASE(object_iterator->data);				//RELEASE frees all memory allocated for a list item. All declared news that were added to the list will be deleted here.
		object_iterator = object_iterator->next;
	}
	data.objectGroups.clear();
	
	////Removing all data of all layers from memory.
	//data.layers.clear();		//Deletes from memory all the items inside the layers list.
	//data = { 0 };				//Frees all memory allocated to the data struct.

	//// Clean up the pugui tree
	//map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	BROFILER_CATEGORY("Load Map", Profiler::Color::Khaki);
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;

	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		MapLayer* set_layer = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set_layer);
		}

		data.layers.add(set_layer);
	}

	//Load Object / ObjectGroup / Collider Info ------------------------------------------
	pugi::xml_node objectgroup;
	for (objectgroup = map_file.child("map").child("objectgroup"); objectgroup && ret; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* new_objectgroup = new ObjectGroup();			//Allocates memory for the objectgroup being iterated.

		if (ret == true)
		{
			ret = LoadObjectLayers(objectgroup, new_objectgroup);	//Loads the data members of the objectgroup that is being iterated.
		}
		data.objectGroups.add(new_objectgroup);						//Adds the object group being iterated to the list.
	}

	//--------------------------- LOGS ----------------------------
	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* tileset = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", tileset->name.GetString(), tileset->firstgid);
			LOG("tile width: %d tile height: %d", tileset->tile_width, tileset->tile_height);
			LOG("spacing: %d margin: %d", tileset->spacing, tileset->margin);
			item = item->next;
		}

		// Adapt this code with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* layer = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", layer->name.GetString());
			LOG("layer width: %d layer height: %d", layer->width, layer->height);
			LOG("parallax speed: %f", layer->speed);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* obj_layer = data.objectGroups.start;
		while (obj_layer != NULL)
		{
			ObjectGroup* object = obj_layer->data;
			LOG("Group ----");
			LOG("Gname: %s", object->name.GetString());

			obj_layer = obj_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width			= map.attribute("width").as_int();
		data.height			= map.attribute("height").as_int();
		data.tile_width		= map.attribute("tilewidth").as_int();
		data.tile_height	= map.attribute("tileheight").as_int();
		data.music_File		= map.child("properties").child("property").attribute("value").as_string();

		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid		= tileset_node.attribute("firstgid").as_int();
	set->tile_width		= tileset_node.attribute("tilewidth").as_int();
	set->tile_height	= tileset_node.attribute("tileheight").as_int();
	set->margin			= tileset_node.attribute("margin").as_int();
	set->spacing		= tileset_node.attribute("spacing").as_int();

	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name			= node.attribute("name").as_string();
	layer->width		= node.attribute("width").as_int();
	layer->height		= node.attribute("height").as_int();
	layer->speed		= node.child("properties").child("property").attribute("value").as_float();		//Gets the speed property of a layer.
	//LoadProperties(node, layer->properties);															//Loads the layer's properties and stores them in a list (property_list) //REVISE THIS HERE
	
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->gid = new uint[layer->width*layer->height];
		memset(layer->gid, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->gid[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

//Loads the object layers (colliders) from the xml map. It iterates through  a specific object layer (in the load() it is iterated through to get all the object info).
bool j1Map::LoadObjectLayers(pugi::xml_node& node, ObjectGroup * objectgroup)
{
	bool ret = true;

	objectgroup->id		= node.attribute("id").as_int();					//Sets the id of a given objectgroup to the id loaded from the tmx map.
	objectgroup->name	= node.attribute("name").as_string();				//Sets the name of a given objectgroup to the name loaded from the tmx map.

	int object_count = 0;
	//This loop iterates all the childs with the object tag, with each iteration of the loop one object is added to the count. Used to reduce memory space waste.
	for (pugi::xml_node object_iterator = node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling("object"))	//Iterates the data members inside the object tag and stops when the pointer is NULL(there are no objects left to iterate) or if ret == false.
	{
		object_count++;
	}

	objectgroup->num_objects = object_count;							//Sets the value of num_objects to the value gotten from iteration all the objectgroups.
	objectgroup->object = new ObjectData[object_count];					//Individually allocates memory for each object. Here object_count is the exact number of objects all objectgroups contain, so there is no memory waste.
	memset(objectgroup->object, 0, object_count * sizeof(ObjectData));	//Sets all allocated memory to 0;

	int index = 0;
	for (pugi::xml_node object_iterator = node.child("object"); object_iterator; object_iterator = object_iterator.next_sibling("object")) //Iterates again all objects passed in the tmx map.
	{
		objectgroup->object[index].id		= object_iterator.attribute("id").as_int();			//Gets the id of the object being loaded from tmx and sets it to the corresponding object in the world.
		objectgroup->object[index].name		= object_iterator.attribute("name").as_string();	//Gets the name of the object being loaded from tmx and sets it to the corresponding object in the world.

		SDL_Rect* collider = new SDL_Rect();

		collider->x = object_iterator.attribute("x").as_int();			//Sets the buffer rect's x position to the x position of the object given by the tmx map this iteration.
		collider->y = object_iterator.attribute("y").as_int();			//Sets the buffer rect's y position to the y position of the object given by the tmx map this iteration.
		collider->w = object_iterator.attribute("width").as_int();		//Sets the buffer rect's width to the width of the object given by the tmx map this iteration.
		collider->h = object_iterator.attribute("height").as_int();		//Sets the buffer rect's height to the height of the object given by the tmx map this iteration.

		objectgroup->object[index].collider = collider;					//Passes the buffer rect's data members to the object in this index position. Need to use a buffer due to objectgroup only accepting a class expression.

		p2SString object_type(object_iterator.attribute("type").as_string());		//Buffer string that improves readability of the code.

		//Checking the object type string being loaded from the tmx file. It's a string that's abitrarily set on Tiled, so it should be known exactly which type strings will be passed. 
		if (object_type == "solid")
		{
			objectgroup->object[index].type = SOLID;					//As the object type string matches "solid" the object's type will be set to SOLID.
		}
		else if (object_type == "platform")
		{
			objectgroup->object[index].type = PLATFORM;					//As the object type string matches "platform" the object's type will be set to PLATFORM.
		}
		else if (object_type == "hazard")
		{
			objectgroup->object[index].type = HAZARD;					//As the object type string matches "hazard" the object's type will be set to HAZARD.
		}
		else if (object_type == "item")
		{
			objectgroup->object[index].type = ITEM;						//As the object type string matches "item" the object's type will be set to ITEM.
		}
		else if (object_type == "desactivable")
		{
			objectgroup->object[index].type = DEACTIVABLE;				//As the object type string matches "desactivable" the object's type will be set to DESACTIVABLE.
		}
		else if (object_type == "respawn")
		{
			objectgroup->object[index].type = RESPAWN;					//As the object type string matches "respawn" the object's type will be set to RESPAWN.
		}
		else if (object_type == "checkpoint")
		{
			objectgroup->object[index].type = CHECKPOINT;				//As the object type string matches "checkpoint", the object's type will be set to CHECKPOINT.
		}
		else if (object_type == "goal")
		{
			objectgroup->object[index].type = GOAL;						//As the object type string matches "goal" the object's type will be set to GOAL.
		}
		
		// ------------------------------------------- LOADING ENEMIES FROM THE MAP -------------------------------------------
		else if (object_type == "mecha")								//If the object loaded from the map matches the "mecha" type
		{
			App->entityManager->AddEnemy(ENTITY_TYPE::MECHA, object_iterator.attribute("x").as_int(), object_iterator.attribute("y").as_int());
			LOG("Added Mecha Enemy at Position %d, %d", object_iterator.attribute("x").as_int(), object_iterator.attribute("y").as_int());

			objectgroup->object[index].type = NONE;
		}
		else if (object_type == "alien")								//If the object loaded from the map matches the "alien" type
		{
			App->entityManager->AddEnemy(ENTITY_TYPE::ALIEN , object_iterator.attribute("x").as_int(), object_iterator.attribute("y").as_int());
			LOG("Added Alien Enemy at Position %d, %d", object_iterator.attribute("x").as_int(), object_iterator.attribute("y").as_int());

			objectgroup->object[index].type = NONE;
		}
		// --------------------------------------------------------------------------------------------------------------------

		else
		{
			objectgroup->object[index].type = SOLID;					//If the object type string does not match any type, the object will be assigned the UKNOWN type.
		}

		index++;	//index is increased in one so the next object is iterated.
	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)							//REVISE THIS HERE. Check why it crashes the game at exit time.
{
	bool ret = true;

	pugi::xml_node data = node.child("properties");													//Sets an xml_node with the properties child of the layer node in the map tmx file.

	if (data != NULL)
	{
		pugi::xml_node prop;
		
		for (prop = data.child("property"); prop != NULL; prop = prop.next_sibling("property"))		//Iterates all property childs from the layer's properties child.
		{
			Properties::Property* p = new Properties::Property();									//Allocates memory for a new property.

			p->name				= prop.attribute("name").as_string();								//Sets the Property's name to the name of the property being iterated.
			p->value.un_float	= prop.attribute("value").as_float();								//Sets the Property's value to the value of the property being iterated.

			properties.property_list.add(p);														//Adds the property beint iterated to property_list.
		}
	}

	return ret;
}

int Properties::Get(p2SString name, int default_value) const					//Revise how to be able to not have a property without default value being nullptr.
{
	p2List_item<Property*>* prop_iterator = property_list.start;

	while (prop_iterator != NULL)
	{
		if (prop_iterator->data->name == name)										//If the string passed as argument matches the name of a property in the property_list.						
		{
			return prop_iterator->data->intValue;										//Returns the value of the property.
		}

		prop_iterator = prop_iterator->next;
	}

	return default_value;															//If no property is found then the value returned is 0.
}

//values Properties::Get(p2SString name, values* default_value) const					//Revise how to be able to not have a property without default value being nullptr.
//{
//	p2List_item<Property*>* prop_iterator = property_list.start;
//
//	while (prop_iterator != NULL)
//	{
//		if (prop_iterator->data->name == name)										//If the string passed as argument matches the name of a property in the property_list.						
//		{
//			return prop_iterator->data->value;										//Returns the value of the property.
//		}
//
//		prop_iterator = prop_iterator->next;
//	}
//
//	return *default_value;															//If no property is found then the value returned is 0.
//}

bool j1Map::SwitchMaps(p2SString* new_map) // switch map function that passes the number of map defined in config.xml
{
	CleanUp();
	App->scene->to_end = false; // we put this in false so there are no repetitions
	Load(new_map->GetString());
	App->audio->PlayMusic(App->map->data.music_File.GetString());

	return true;
}

bool j1Map::ChangeMap(const char* newMap)
{
	bool ret = true;

	//Put this on scene CleanUp()
	App->scene->CleanUp();
	//App->audio->CleanUp();

	App->map->Load(newMap);						//Loads a specified map
	App->collisions->LoadColliderFromMap();		//Load Collisions

	if (newMap == "Test_map.tmx")
	{
		App->scene->firstMap	= true;
		App->scene->secondMap	= false;
	}
	if (newMap == "Test_map_2.tmx")
	{
		App->scene->secondMap	= true;
		App->scene->firstMap	= false;
	}

	App->entityManager->player->Start();		//Load / Reset P1	//REVISE THIS HERE. Should players be loaded like this?
	App->entityManager->player2->Start();		//Load / Reset P2

	//App->scene->Start();						//This breaks the game

	return ret;
}

void j1Map::Restart_Cam() // function that resets the camera
{
	App->render->camera.x = spawn_position_cam.x;
	App->render->camera.y = spawn_position_cam.y;
}

MapLayer::~MapLayer()
{
	RELEASE(gid); //New		//Breaks with mmgr.
}
