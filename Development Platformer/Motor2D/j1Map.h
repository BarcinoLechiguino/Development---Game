#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct MapLayer
{
	p2SString name = nullptr;
	uint width = 0;
	uint height = 0;
	uint* gid = nullptr;
	float speed_x = 0.0f;

	uint Get(int x, int y) const
	{
		return gid[width*y + x];;
	}
};

// ----------------------------------------------------
struct TileSet
{

	SDL_Rect GetRect(int id);

	p2SString name;
	int	firstgid;
	int	margin;
	int	spacing;
	int	tile_width;
	int	tile_height;
	SDL_Texture* texture;
	int	tex_width;
	int	tex_height;
	int	num_tiles_width;
	int	num_tiles_height;
	int	offset_x;
	int	offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

//Items that scene has
struct Scenes
{
	bool active = false;
	uint levelnum = 0;
	p2SString level_tmx;
	p2SString musicPath;

};

// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;

	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<Scenes*>		scenes_List;
};



// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	inline uint ArrayPos(int x, int y);
	iPoint MapToWorld(int x, int y);

	uint num_thismaplvl = 1; //number of the level that is loaded

	p2List_item<Scenes*>* activateScene(uint lvlnum)//function that identificates the active scene, turns it to false and loads the new one
	{
		p2List_item<Scenes*>* item_scene;

		for (item_scene = data.scenes_List.start; item_scene; item_scene = item_scene->next)
		{
			item_scene->data->active = false;
		}
		for (item_scene = data.scenes_List.start; item_scene->data->levelnum != lvlnum && item_scene != nullptr; item_scene = item_scene->next)
		{
		}
		item_scene->data->active = true;
		atualSceneItem = item_scene;
		return item_scene;
	}

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	TileSet* GetTileset(int id);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	p2List_item<Scenes*>* atualSceneItem = nullptr;
};

#endif // __j1MAP_H__