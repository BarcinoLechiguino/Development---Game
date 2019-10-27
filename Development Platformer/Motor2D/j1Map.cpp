#include "p2Defs.h"
#include "p2Log.h"
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
#include "p2Point.h"
#include <math.h>

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
	spawn_position_cam.x = config.child("renderer").child("cam").attribute("x").as_float();
	spawn_position_cam.x = config.child("renderer").child("cam").attribute("y").as_float();

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
	{
		return;
	}
	
	MapLayer* layer = data.layers[0];

	p2List_item<MapLayer*>* item = data.layers.start;

	for (item; item != nullptr; item = item->next) {

		uint* gid = item->data->gid;
		int i = 0;
		for (uint y = 0; y < data.height; ++y)
		{
			for (uint x = 0; x < data.width; ++x)
			{
				App->render->Blit(data.tilesets[0]->texture,
					MapToWorld(x, y).x, MapToWorld(x, y).y,
					data.tilesets[0]->GetTileRect(gid[i]));
				i++;
			}
		}

	}
}

iPoint j1Map::MapToWorld(int x, int y) const 
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}

	return ret;
}

//From Handout 5
TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* tilesetIter = data.tilesets.start;

	TileSet* ret = NULL;

	while (tilesetIter != NULL)
	{
		if (id >= tilesetIter->data->firstgid)
		{

			ret = tilesetIter->data;
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
	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		SDL_DestroyTexture(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* map_layer_item;
	map_layer_item = data.layers.start;

	while (map_layer_item != NULL)
	{
		RELEASE(map_layer_item->data);
		map_layer_item = map_layer_item->next;
	}
	data.layers.clear();

	//Removing all Objects
	p2List_item<ObjectGroup*>* object_iterator = data.objectGroups.start;
	while (object_iterator != NULL)
	{
		RELEASE(object_iterator->data);				//RELEASE deletes all elements in a list (deletes a buffer).
		object_iterator = object_iterator->next;
	}
	data.objectGroups.clear();
	
	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
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

	//Load Collider Info ------------------------------------------
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

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// Adapt this code with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			LOG("parallax speed: %f", l->speed_x);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* obj_layer = data.objectGroups.start;
		while (obj_layer != NULL)
		{
			ObjectGroup* o = obj_layer->data;
			LOG("Group ----");
			LOG("Gname: %s", o->name.GetString());

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
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		data.music_File = map.child("properties").child("property").attribute("value").as_string();

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
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
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
		/*p2SString debug = folder.GetString();
		debug += image.attribute("source").as_string();
		set->texture = App->tex->Load(debug.GetString());*/
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

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->speed_x = node.child("properties").child("property").attribute("value").as_float();
	
	layer->gid = new uint[layer->height * layer->width];
	memset(layer->gid, 0, layer->width * layer->height * sizeof(uint));
	
	int i = 0;
	for (pugi::xml_node iterator_node = node.child("data").child("tile"); iterator_node; iterator_node = iterator_node.next_sibling("tile"))
	{
		layer->gid[i] = iterator_node.attribute("gid").as_uint();
		LOG("Layer %u of the tileset.", layer->gid[i]);
		i++;
	}
	
	pugi::xml_node layer_data = node.child("data");
	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->gid = new uint[layer->width*layer->height]; // New
		memset(layer->gid, 0, layer->width*layer->height); // New
		
		int i = 0;
		for (pugi::xml_node tileset = node.child("data").child("tile"); tileset; tileset = tileset.next_sibling("tile"))
		{
			layer->gid[i] = tileset.attribute("gid").as_uint();

			LOG("%u", layer->gid[i]);
			++i;
		}
		return true;
	}

	return true;
}

//Loads the object layers (colliders) from the xml map. It iterates through  a specific object layer (in the load() it is iterated through to get all the object info).
bool j1Map::LoadObjectLayers(pugi::xml_node& node, ObjectGroup * objectgroup)
{
	bool ret = true;

	objectgroup->id = node.attribute("id").as_int();			//Sets the id of a given objectgroup to the id loaded from the tmx map.
	objectgroup->name = node.attribute("name").as_string();		//Sets the name of a given objectgroup to the name loaded from the tmx map.

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
		objectgroup->object[index].id = object_iterator.attribute("id").as_int();			//Gets the id of the object being loaded from tmx and sets it to the corresponding object in the world.
		objectgroup->object[index].name = object_iterator.attribute("name").as_string();	//Gets the name of the object being loaded from tmx and sets it to the corresponding object in the world.

		SDL_Rect* collider = new SDL_Rect;					//Allocates memory for the buffer rect(x,y,w,z) that will receive the data members of an object from the objectgroup being iterated.

		collider->x = object_iterator.attribute("x").as_int();			//Sets the buffer rect's x position to the x position of the object given by the tmx map this iteration.
		collider->y = object_iterator.attribute("y").as_int();			//Sets the buffer rect's y position to the y position of the object given by the tmx map this iteration.
		collider->w = object_iterator.attribute("width").as_int();		//Sets the buffer rect's width to the width of the object given by the tmx map this iteration.
		collider->h = object_iterator.attribute("height").as_int();		//Sets the buffer rect's height to the height of the object given by the tmx map this iteration.

		objectgroup->object[index].collider = collider;		//Passes the buffer rect's data members to the object in this index position. Need to use a buffer due to objectgroup only accepting a class expression.


		p2SString object_type(object_iterator.attribute("type").as_string());		//Buffer string that improves readability of the code.

		//Checking the object type string being loaded from the tmx file. It's a string that's abitrarily set on Tiled, so it should be known exactly which type strings will be passed. 
		if (object_type == "solid")
		{
			objectgroup->object[index].type = SOLID;			//As the object type string matches "solid" the object's type will be set to SOLID.
		}
		else if (object_type == "platform")
		{
			objectgroup->object[index].type = PLATFORM;			//As the object type string matches "platform" the object's type will be set to PLATFORM.
		}
		else if (object_type == "hazard")
		{
			objectgroup->object[index].type = HAZARD;			//As the object type string matches "hazard" the object's type will be set to HAZARD.
		}
		else if (object_type == "item")
		{
			objectgroup->object[index].type = ITEM;				//As the object type string matches "item" the object's type will be set to ITEM.
		}
		else if (object_type == "desactivable")
		{
			objectgroup->object[index].type = DEACTIVABLE;		//As the object type string matches "desactivable" the object's type will be set to DESACTIVABLE.
		}
		else if (object_type == "respawn")
		{
			objectgroup->object[index].type = RESPAWN;			//As the object type string matches "respawn" the object's type will be set to RESPAWN.
		}
		else if (object_type == "goal")
		{
			objectgroup->object[index].type = GOAL;				//As the object type string matches "goal" the object's type will be set to GOAL.
		}
		else
		{
			objectgroup->object[index].type = UNKNOWN;			//If the object type string does not match any type, the object will be assigned the UKNOWN type.
		}

		index++;	//index is increased in one so the next object is iterated.
	}

	return ret;
}

bool j1Map::SwitchMaps(p2SString* new_map)
{
	CleanUp();
	App->scene->to_end = false; // we put this in false so there are no repetitions
	Load(new_map->GetString());
	App->audio->PlayMusic(App->map->data.music_File.GetString());

	return true;
}

void j1Map::Restart_Cam()
{
	App->render->camera.x = spawn_position_cam.x;
	App->render->camera.y = spawn_position_cam.y;
}

MapLayer::~MapLayer()
{
	/*delete[] gid;*/ //New comment
	RELEASE(gid); //New
}
