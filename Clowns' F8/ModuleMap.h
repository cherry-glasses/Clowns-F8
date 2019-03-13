#ifndef __ModuleMap_H_
#define __ModuleMap_H_

#include "Module.h"


// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		std::list<Property*>::iterator item;
		item = list.begin();

		while (item != list.end())
		{
			RELEASE(*item);
			++item;
		}

		list.clear();
	}

	int Get(const char* _name, int _default_value = 0) const;

	std::list<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int _x, int _y) const
	{
		return data[(_y*width) + _x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int _id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
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
	SDL_Color			background_color;
	MapTypes			type;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*> layers;
};

// ----------------------------------------------------
class ModuleMap : public Module
{
public:

	ModuleMap();

	// Destructor
	virtual ~ModuleMap();

	// Called before render is available
	bool Awake(pugi::xml_node& _config);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* _path);

	std::pair<int, int> MapToWorld(int _x, int _y) const;
	std::pair<int, int> WorldToMap(int _x, int _y) const;
	bool CreateWalkabilityMap(int& _width, int& _height, uchar** _buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& _tileset_node, TileSet* _set);
	bool LoadTilesetImage(pugi::xml_node& _tileset_node, TileSet* _set);
	bool LoadLayer(pugi::xml_node& _node, MapLayer* _layer);
	bool LoadProperties(pugi::xml_node& _node, Properties& _properties);

	TileSet* GetTilesetFromTileId(int _id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded = false;
};

#endif // __ModuleMap_H_




