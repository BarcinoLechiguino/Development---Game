#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct ObjectsData
{
	p2SString name;
	uint width;
	uint height;
	int	x;
	int	y;
};

struct ObjectsGroup
{
	p2SString name;
	p2List<ObjectsData*> objects;
	~ObjectsGroup();
};

struct MapLayer
{
	p2SString name = nullptr;
	uint* gid = nullptr;
	uint width = 0;
	uint height = 0;
	uint size = 0;
	float speed_x = 0.0f;

	~MapLayer();

	inline uint Get(uint x, uint y) const
	{
		return x + y * width;
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
	SDL_Texture* texture = nullptr;
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



// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	float gravity = 0.2f;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ObjectsGroup*> objLayers;
	p2SString music_File;
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

	//Unloads the map and changes by another one. 
	bool SwitchMaps(p2SString* new_map);

	// Load new map
	iPoint MapToWorld(int x, int y);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectLayers(pugi::xml_node& node, ObjectsGroup* group);

public:

	MapData data;


private:
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

};

#endif // __j1MAP_H__