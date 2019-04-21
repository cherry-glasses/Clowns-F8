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
		position = App->map->MapToWorld(1, 10);
		break;
	case 5:
		position = App->map->MapToWorld(1, 9);
		break;
	case 6:
		position = App->map->MapToWorld(9, 14);
		break;
	case 7:
		position = App->map->MapToWorld(10, 14);
		break;
	case 8:
		position = App->map->MapToWorld(9, 13);
		break;
	case 9:
		position = App->map->MapToWorld(10, 13);
		break; 
	case 10:
		position = App->map->MapToWorld(9, 12);
		break;
	case 11:
		position = App->map->MapToWorld(10, 12);
		break;
	case 12:
		position = App->map->MapToWorld(14, 8);
		break;
	case 13:
		position = App->map->MapToWorld(15, 8);
		break;
	case 14:
		position = App->map->MapToWorld(16, 8);
		break;
	case 15:
		position = App->map->MapToWorld(14, 7);
		break;
	case 16:
		position = App->map->MapToWorld(15, 7);
		break;
	case 17:
		position = App->map->MapToWorld(16, 7);
		break;
	case 18:
		position = App->map->MapToWorld(14, 6);
		break;
	case 19:
		position = App->map->MapToWorld(15, 6);
		break;
	case 20:
		position = App->map->MapToWorld(16, 6);
		break;

	default:
		break;
	}
	
}
Tree1::~Tree1()
{
}




