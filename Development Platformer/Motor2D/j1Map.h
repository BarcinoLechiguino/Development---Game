#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

//Kinds of colliders we are working with
enum Object_Type //Cuidado con el class. If enum class-> Object_Type::HAZARD and not just HAZARD
{
	PLAYER,
	SOLID,
	PLATFORM,
	HAZARD,
	ITEM,
	DEACTIVABLE,
	RESPAWN,
	GOAL,
	UNKNOWN = -1
};

//Information of a specific object in the map.
struct ObjectData
{
	uint				id;			//Object's id.
	p2SString			name;		//Object's name.
	Object_Type			type;		//Type of collider associated with the object.
	SDL_Rect*			collider;	//Rectangle that represents the object. As x, y, w and h are object properties, they can be grouped in a SDL_Rect.
	float				rotation;	//Rotation of the object in degrees clockwise.
	SDL_Texture*		texture;	//Visible image of the object.
};

//Object layer that has all the objects in the same "plane".
struct ObjectGroup
{
	uint				id;				//ObjectGroup layer id.
	p2SString			name;			//ObjectGroup's name.
	ObjectData*			object;			//Individual info of each object in the ObjectGroup layer.
	p2SString			type;			//ObjectGroup's type. It's an arbitrary string added in Tiled to the Object/ObjectGroup.
	uint				num_objects;	//Quantity of objects. Treure per obj def.
};

struct MapLayer
{
	p2SString			name;			//Map name
	uint*				gid;			//Tile Id
	uint				width;			//Tile Width
	uint				height;			//Tile Height
	uint				size;			//Tile / Map size?
	float				speed_x = 0.0f;	//Parallax speed

	MapLayer() : gid(NULL) {} //New Comment

	~MapLayer(); //Defined at j1Map.cpp

	inline uint Get(uint x, uint y) const
	{
		return x + y * width;
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect* TileRect = new SDL_Rect; //Allocates memory for TileRect.
	
	//This method calculates the position of each tile when given a tile id. 
	SDL_Rect* GetTileRect(uint tile_id) 
	{
		SDL_Rect* tile_rect = TileRect; //The memory allocated for TileRect is now allocated to tile_rect. (?)

		int relative_id = tile_id - firstgid;  //Calculates the relative position of the tile_id respect the  first initial global id.
		
		tile_rect->w = tile_width;			//Sets the width of the Rect holding the tile to the width of the tile in pixels.
		tile_rect->h = tile_height;			//Sets the width of the Rect holding the tile to the height of the tile in pixels.
		tile_rect->x =  margin + ((tile_rect->w + spacing) * (relative_id % num_tiles_width));
		tile_rect->y =  margin + ((tile_rect->h + spacing) * (relative_id / num_tiles_width));

		return tile_rect;
	}

	//This method translates the position of the tile on the map to its equivalent position on screen.
	p2Point<uint> MapToWorld(uint x, uint y)
	{
		p2Point<uint> position;				//Position of the tile on the world.

		position.x = x * tile_width;		//Position x of the tile on the map in pixels. For tile_width = 32 --> Tile 1: x = 0, Tile 2: x = 32.
		position.y = y * tile_height;		//Position y of the tile on the map in pixels. For tile_height = 32 --> Tile 1: y = 0, Tile 2: y = 32.

		return position;
	}

	p2SString			name;				//Tileset name.
	int					firstgid;			//First global tile id. Maps to the first id in the tileset.
	int					tile_width;			//Maximum width of tiles in a given tileset.
	int					tile_height;		//Maximum height of tiles in a given tilesset.
	int					spacing;			//Space in pixels between the tiles in a given tileset.
	int					margin;				//Margin around the tiles in a given tileset.
	SDL_Texture*		texture = nullptr;	//Image that will be embedded on the tileset.
	int					tex_width;			//Image width in pixels.
	int					tex_height;			//Image height in pixels.
	int					num_tiles_width;	//Number of tiles at the X axis that will have a given texture. Ex: num_tiles_width = tile_width / tex_width; 
	int					num_tiles_height;	//Number of tiles at the Y axis that will have a given texture. Ex: num_tiles_height = tile_height / tex_height;
	int					offset_x;			//Horizontal offset in pixels.
	int					offset_y;			//Vertical offset in pixels.

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED,
	MAPTYPE_HEXAGONAL
};



// ----------------------------------------------------
struct MapData
{
	int						width;				//Map width in tiles.
	int						height;				//Map height in tiles.
	int						tile_width;			//Tile width in pixels.
	int						tile_height;		//Tile height in pixels.
	MapTypes				type;				//Type of map (Orthogonal, Isometric, Staggered or Hexagonal)
	p2List<TileSet*>		tilesets;			//List that accesses all tilesets and their data members/properties.
	p2List<MapLayer*>		layers;				//List that accesses all layers and their data members/properties.
	p2List<ObjectGroup*>	objectGroups;		//List that accesses all object groups and their data members/properties.

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
	iPoint MapToWorld(int x, int y) const; //Ya hay un map to world en TileSet

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectLayers(pugi::xml_node& node, ObjectGroup* group);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	bool activated = false; //Bool for the deactivable colliders.
	MapData data;

private:
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__