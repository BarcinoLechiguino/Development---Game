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

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	//MapLayer* layer = data.layers[0]; //The problem lays here, the index remains at 0 so only the first layeris loaded
	//New
	p2List_item<TileSet*>* tileset = data.tilesets.start; //Tileset iteration list
	
	
	p2List_item<MapLayer*>* layer_iterator = data.layers.start;

	for (layer_iterator; layer_iterator != NULL; layer_iterator = layer_iterator->next) 
	{
		uint* gid = layer_iterator->data->gid;

		uint i = 0;
		for (uint y = 0; y < data.height; ++y)
		{
			for (uint x = 0; x < data.width; ++x)
			{
				//New
				App->render->Blit(data.tilesets[0]->texture, data.tilesets[0]->MapToWorld(x, y).x, data.tilesets[0]->MapToWorld(x, y).y, data.tilesets[0]->GetRect(gid[i]));
				i++;
			}
		}
		layer_iterator = layer_iterator->next; //Go to next layer.
	}

	//New
	/*while(layer_iterator != NULL  && tileset != NULL)
	{
		while (tileset != NULL)
		{
			uint* gid = layer_iterator->data->gid;

			uint i = 0;
			for (uint y = 0; y < data.height; ++y)
			{
				for (uint x = 0; x < data.width; ++x)
				{
					//New
					//App->render->Blit(data.tilesets[0]->texture, data.tilesets[0]->MapToWorld(x, y).x, data.tilesets[0]->MapToWorld(x, y).y, data.tilesets[0]->GetRect(gid[i]));
					App->render->Blit(tileset->data->texture, tileset->data->MapToWorld(x, y).x, tileset->data->MapToWorld(x, y).y, tileset->data->GetRect(gid[i]));
					i++;
				}
			}
			tileset = tileset->next;
		}
		layer_iterator = layer_iterator->next; //Go to next layer.
	}*/
}

//New Comment
//SDL_Rect* TileSet::GetRect(int tile_id)
//{
//	int relative_id = tile_id - firstgid;
//	SDL_Rect* rect = ReturnedRect;
//	rect->w = tile_width;
//	rect->h = tile_height;
//	rect->x = margin + ((rect->w + spacing) * (relative_id % num_tiles_width));
//	rect->y = margin + ((rect->h + spacing) * (relative_id / num_tiles_width));
//	return rect;
//}

//Old Map to World, passed to the tileset struct in map.h
/*iPoint j1Map::MapToWorld(int x, int y)
{
	iPoint vec;

	vec.x = x * data.tile_width;
	vec.y = y * data.tile_height;

	return vec;
}*/

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

	//Clean Colliders
	data.layers.clear();

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
	pugi::xml_node object_group;
	for (object_group = map_file.child("map").child("objectgroup"); object_group && ret; object_group = object_group.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup(); //New Object group pointer that will iterate through the memebers of the ObjectGroup struct to fill the object group data (id, name, object...) layer as well as the info of each object.

		if (ret == true)
		{
			ret = LoadObjectLayers(object_group, set); //
		}
		data.objectGroups.add(set);
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
		/*data.background_colour = map.attribute("backgroundcolor").as_string();

		//Setting the values of the SDL_Color struct to 0 (black).
		data.colour.r = 0;
		data.colour.g = 0;
		data.colour.b = 0;
		data.colour.a = 0;

		//Sets the values of the colours of the background to the values recieved from the map file.
		if (data.background_colour.Length() > 0) //If any of the values recieved from the map file are different from 0 then this is run.
		{
			p2SString red;
			p2SString green;
			p2SString blue;

			//Pastes the strings into a buffer. Pastes the value of background_colour on red/green/blue.
			data.background_colour.SubString(1, 2, red);
			data.background_colour.SubString(3, 4, green);
			data.background_colour.SubString(5, 6, blue);

			int rgb_value = 0;

			//Sets the received values from the tmx file as the background colour.
			sscanf_s(red.GetString(), "%x", &rgb_value);//Sscanf_s gets a buffer and a format (int). %x is the translation to rgb from hexadecimal.
			if (rgb_value >= 0 && rgb_value <= 255)
			{
				data.colour.r = rgb_value;
			}
			
			sscanf_s(green.GetString(), "%x", &rgb_value);
			if (rgb_value >= 0 && rgb_value <= 255) 
			{
				data.colour.g = rgb_value;
			} 

			sscanf_s(blue.GetString(), "%x", &rgb_value);
			if (rgb_value >= 0 && rgb_value <= 255)
			{
				data.colour.b = rgb_value;
			}	
		}*/

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
	//New comment
	/*layer->gid = new uint[layer->width*layer->height];				
	memset(layer->gid, 0u, sizeof(uint)*layer->height*layer->width);*/

	//New
	/*Goes through all the objects and records how many of them there are.
	int layer_quantity = 0;
	for (pugi::xml_node layerIterator = node.child("layer"); layerIterator; layerIterator = layerIterator.next_sibling("layer"))
	{
		layer_quantity++;
	}

	//Sets the amount of layers to be drawn (Allocates memory for all layers)
	layer->size = layer_quantity;
	layer->tileset = new TileSet[layer_quantity];
	memset(layer->tileset, 0, layer_quantity * sizeof(TileSet));*/
	
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

	objectgroup->id = node.attribute("id").as_uint();
	objectgroup->name = node.attribute("name").as_string();

	//Goes through all the objects and records how many of them there are.
	int object_quantity = 0;
	for (pugi::xml_node objIterator = node.child("object"); objIterator; objIterator = objIterator.next_sibling("object"))
	{
		object_quantity++;
	}

	//Sets the amount of objects to be drawn (Allocates memory for all the objects)
	objectgroup->object_size = object_quantity;
	objectgroup->object = new ObjectData[object_quantity];
	memset(objectgroup->object, 0, object_quantity * sizeof(ObjectData));

	//A SDL_rect recieves the matching variable values and then identifies which type of object it is.
	int index = 0;
	for (pugi::xml_node objIterator = node.child("object"); objIterator; objIterator = objIterator.next_sibling("object"), index++)
	{
		SDL_Rect* hitbox = new SDL_Rect;

		hitbox->x = objIterator.attribute("x").as_uint();
		hitbox->y = objIterator.attribute("y").as_uint();
		hitbox->w = objIterator.attribute("width").as_uint();
		hitbox->h = objIterator.attribute("height").as_uint();

		objectgroup->object[index].hitbox = hitbox;
		objectgroup->object[index].id = objIterator.attribute("id").as_uint();
		objectgroup->object[index].name = objIterator.attribute("name").as_uint();

		//The type string that the object passes is passed to a string.
		//p2SString type(objIterator.attribute("type").as_string());
		objectgroup->type = objIterator.attribute("type").as_string();

		//If enum class-> Object_Type::HAZARD and not just HAZARD  and etc.
		if (objectgroup->type == "solid")
		{
			objectgroup->object[index].type = SOLID;
		}
		else if (objectgroup->type == "platform")
		{
			objectgroup->object[index].type = PLATFORM;
		}
		else if (objectgroup->type == "hazard")
		{
			objectgroup->object[index].type = HAZARD;
		}
		else if (objectgroup->type == "item")
		{
			objectgroup->object[index].type = ITEM;
		}
		else if (objectgroup->type == "desactivable")
		{
			objectgroup->object[index].type = DESACTIVABLE;
		}

		//These last two can be put as position variables in the config.xml file.
		else if (objectgroup->type == "respawn")
		{
			objectgroup->object[index].type = RESPAWN;
		}
		else if (objectgroup->type == "goal")
		{
			objectgroup->object[index].type = GOAL;
		}

		//Revise Positioning on this
		else
		{
			objectgroup->object[index].type = UNKNOWN;
		}

	}
	
	/*for (pugi::xml_node& obj = node.child("object"); obj && ret; obj = obj.next_sibling("object"))
	{
		ObjectData* data = new ObjectData;

		data->height = obj.attribute("height").as_uint();
		data->width = obj.attribute("width").as_uint();
		data->x = obj.attribute("x").as_uint();
		data->y = obj.attribute("y").as_uint();
		data->name = obj.attribute("name").as_string();

		objectgroup->objects.add(data);
	}*/

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

MapLayer::~MapLayer()
{
	/*delete[] gid;*/ //New comment
	RELEASE(gid); //New
}

//ObjectsGroup::~ObjectsGroup()
//{
//	objects.clear();
//}
