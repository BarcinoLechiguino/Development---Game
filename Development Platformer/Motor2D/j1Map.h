#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

//Kinds of colliders we are working with
enum Object_Type //Cuidado con el class. If enum class-> Object_Type::HAZARD and not just HAZARD
{
	UNKNOWN = -1,
	PLAYER,
	SOLID,
	PLATFORM,
	HAZARD,
	ITEM,
	DESACTIVABLE,
	RESPAWN,
	GOAL
};

//Information of a specific object in the map.
struct ObjectData
{
	uint id;				//Id number of the object.
	p2SString name;			//Name of the object.
	Object_Type type;		//Type of collider associated with the object.
	SDL_Rect* hitbox;		//Rectangle that represents the object.
	SDL_Texture* texture;	//Visible image of the object.
};

//Object layer that has all the objects in the same "plane".
struct ObjectGroup
{
	uint id;			//Id number of the object group.
	p2SString name;		//Name of the object group.
	ObjectData* object;	//Individual info of each object.
	p2SString type;		//Object type
	uint object_size;	//Quantity of objects.
};




struct MapLayer
{
	p2SString name = nullptr;	//Map name
	uint* gid;					//Tile Id
	uint width = 0;				//Tile Width
	uint height = 0;			//Tile Height
	uint size = 0;				//Tile / Map size?
	float speed_x = 0.0f;		//Parallax speed

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
	p2List<ObjectGroup*> objectGroups;
	
	p2SString background_colour; //Sets the string that will recieve the value of the string in the map file.
	SDL_Color colour; //Info of which colour the background has. SDL_Color is a struct that represents colours.

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
	bool LoadObjectLayers(pugi::xml_node& node, ObjectGroup* group);

public:

	MapData data;


private:
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

};

#endif // __j1MAP_H__