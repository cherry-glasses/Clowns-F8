#include "Log.h"
#include "Application.h"
#include "Tree2.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"


Tree2::Tree2(ENTITY_TYPE _type, pugi::xml_node _config, int _copy) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/tree_2.png");
	switch (_copy) {
	case 0:
		position = App->map->MapToWorld(0, 24);
		break;
	case 1:
		position = App->map->MapToWorld(1, 24);
		break;
	case 2:
		position = App->map->MapToWorld(2, 24);
		break;
	case 3:
		position = App->map->MapToWorld(0, 22);
		break;
	case 4:
		position = App->map->MapToWorld(0, 14);
		break;
	case 5:
		position = App->map->MapToWorld(0, 12);
		break;
	case 6:
		position = App->map->MapToWorld(0, 10);
		break;
	default:
		break;
	}
	current = { 0 , 0, 128, 128 };
}
Tree2::~Tree2()
{
}




