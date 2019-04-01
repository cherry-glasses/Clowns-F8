#include "Log.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"

#include "SDL/include/SDL.h"

ModuleMap::ModuleMap() : Module()
{
	name= "map";
}

// Destructor
ModuleMap::~ModuleMap()
{}

// Called before render is available
bool ModuleMap::Awake(pugi::xml_node& _config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder = _config.child("folder").child_value();

	return ret;
}

void ModuleMap::Draw()
{
	if (map_loaded == false) 
	{
		return;
	}
		
	for (std::list<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->properties.Get("Nodraw") != 0) 
		{
			continue;
		}
			
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					std::pair<int, int> pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.first, pos.second, &r);
				}
			}
		}
	}
}

int Properties::Get(const char* _name, int _default_value) const
{
	std::list<Property*>::const_iterator item = list.begin();

	while (item != list.end())
	{
		if ((*item)->name == _name)
			return (*item)->value;
		++item;
	}

	return _default_value;
}

TileSet* ModuleMap::GetTilesetFromTileId(int _id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = *item;

	while (item != data.tilesets.end())
	{
		if (_id < (*item)->firstgid)
		{
			set = *--item;
			break;
		}
		set = *item;
		++item;
	}

	return set;
}

std::pair<int, int> ModuleMap::MapToWorld(int _x, int _y) const
{
	std::pair<int, int> ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.first = _x * data.tile_width;
		ret.second = _y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.first = (_x - _y) * (data.tile_width * 0.5f);
		ret.second = (_x + _y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.first = _x; ret.second = _y;
	}

	return ret;
}

std::pair<int, int> ModuleMap::WorldToMap(int _x, int _y) const
{
	std::pair<int, int> ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.first = _x / data.tile_width;
		ret.second = _y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.first = int((_x / half_width + _y / half_height) / 2);
		ret.second = int((_y / half_height - (_x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.first = _x; ret.second = _y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int _id) const
{
	int relative_id = _id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool ModuleMap::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator item;
	item = data.tilesets.begin();

	while (item != data.tilesets.end())
	{
		RELEASE(*item);
		++item;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::iterator item2;
	item2 = data.layers.begin();

	while (item2 != data.layers.end())
	{
		RELEASE(*item2);
		++item2;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool ModuleMap::Load(const char* _file_name)
{
	bool ret = true;
	std::string tmp_string = folder + _file_name;

	pugi::xml_parse_result result = map_file.load_file(tmp_string.c_str());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", _file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.push_back(lay);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", _file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		std::list<TileSet*>::iterator item = data.tilesets.begin();
		while (item != data.tilesets.end())
		{
			TileSet* s = *item;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			++item;
		}

		std::list<MapLayer*>::iterator item_layer = data.layers.begin();
		while (item_layer != data.layers.end())
		{
			MapLayer* l = *item_layer;
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			++item_layer;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool ModuleMap::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
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
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.size() > 0)
		{
			std::string red, green, blue;
			int v = 0;

			sscanf_s(red.c_str(), "%x", &v);
			if (v >= 0 && v <= 255)
			{
				data.background_color.r = v;
			}

			sscanf_s(green.c_str(), "%x", &v);
			if (v >= 0 && v <= 255)
			{
				data.background_color.g = v;
			}

			sscanf_s(blue.c_str(), "%x", &v);
			if (v >= 0 && v <= 255)
			{
				data.background_color.b = v;
			}
		}

		std::string orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
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

bool ModuleMap::LoadTilesetDetails(pugi::xml_node& _tileset_node, TileSet* _set)
{
	bool ret = true;
	_set->name = _tileset_node.attribute("name").as_string();
	_set->firstgid = _tileset_node.attribute("firstgid").as_int();
	_set->tile_width = _tileset_node.attribute("tilewidth").as_int();
	_set->tile_height = _tileset_node.attribute("tileheight").as_int();
	_set->margin = _tileset_node.attribute("margin").as_int();
	_set->spacing = _tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = _tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		_set->offset_x = offset.attribute("x").as_int();
		_set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		_set->offset_x = 0;
		_set->offset_y = 0;
	}

	return ret;
}

bool ModuleMap::LoadTilesetImage(pugi::xml_node& _tileset_node, TileSet* _set)
{
	bool ret = true;
	pugi::xml_node image = _tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		_set->texture = App->textures->Load(PATH(folder.c_str(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(_set->texture, NULL, NULL, &w, &h);
		_set->tex_width = image.attribute("width").as_int();

		if (_set->tex_width <= 0)
		{
			_set->tex_width = w;
		}

		_set->tex_height = image.attribute("height").as_int();

		if (_set->tex_height <= 0)
		{
			_set->tex_height = h;
		}

		_set->num_tiles_width = _set->tex_width / _set->tile_width;
		_set->num_tiles_height = _set->tex_height / _set->tile_height;
	}

	return ret;
}

bool ModuleMap::LoadLayer(pugi::xml_node& _node, MapLayer* _layer)
{
	bool ret = true;

	_layer->name = _node.attribute("name").as_string();
	_layer->width = _node.attribute("width").as_int();
	_layer->height = _node.attribute("height").as_int();
	LoadProperties(_node, _layer->properties);
	pugi::xml_node layer_data = _node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(_layer);
	}
	else
	{
		_layer->data = new uint[_layer->width*_layer->height];
		memset(_layer->data, 0, _layer->width*_layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			_layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool ModuleMap::LoadProperties(pugi::xml_node& _node, Properties& _properties)
{
	bool ret = false;

	pugi::xml_node data = _node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			_properties.list.push_back(p);
		}
	}

	return ret;
}

bool ModuleMap::CreateWalkabilityMap(int& _width, int& _height, uchar** _buffer) const
{
	bool ret = false;

	for (std::list<MapLayer*>::const_iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*_buffer = map;
		_width = data.width;
		_height = data.height;
		ret = true;

		break;
	}

	return ret;
}