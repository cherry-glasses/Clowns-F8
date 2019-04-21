#include "Log.h"
#include "Application.h"
#include "Tree1.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"


Tree1::Tree1(ENTITY_TYPE _type, pugi::xml_node _config, int _copy) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/tree_1.png");
	switch (_copy) {
	case 0:
		position = App->map->MapToWorld(4, 24);
		break;
	case 1:
		position = App->map->MapToWorld(1, 15);
		break;
	case 2:
		position = App->map->MapToWorld(1, 13);
		break;
	case 3:
		position = App->map->MapToWorld(1, 11);
		break;
	case 4:
		position = App->map->MapToWorld(1, 9);
		break;
	case 5:
		position = App->map->MapToWorld(1, 7);
		break;

	default:
		break;
	}
	current = { 0 , 0, 96, 128 };
}
Tree1::~Tree1()
{
}



