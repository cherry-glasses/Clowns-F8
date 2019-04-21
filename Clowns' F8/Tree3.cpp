#include "Log.h"
#include "Application.h"
#include "Tree3.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"


Tree3::Tree3(ENTITY_TYPE _type, pugi::xml_node _config, int _copy) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/tree_3.png");
	switch (_copy) {
	
	case 0:
		position = App->map->MapToWorld(8, 14);
		break;
	case 1:
		position = App->map->MapToWorld(9, 14);
		break;
	case 2:
		position = App->map->MapToWorld(8, 13);
		break;
	case 3:
		position = App->map->MapToWorld(9, 13);
		break;
	case 4:
		position = App->map->MapToWorld(8, 12);
		break;
	case 5:
		position = App->map->MapToWorld(9, 12);
		break;
	case 6:
		position = App->map->MapToWorld(13, 8);
		break;
	case 7:
		position = App->map->MapToWorld(14, 8);
		break;
	case 8:
		position = App->map->MapToWorld(15, 8);
		break;
	case 9:
		position = App->map->MapToWorld(13, 7);
		break;
	case 10:
		position = App->map->MapToWorld(14, 7);
		break;
	case 11:
		position = App->map->MapToWorld(15, 7);
		break;
	case 12:
		position = App->map->MapToWorld(13, 6);
		break;
	case 13:
		position = App->map->MapToWorld(14, 6);
		break;
	case 14:
		position = App->map->MapToWorld(15, 6);
		break;

	default:
		break;
	}
	current = { 0 , 0, 128, 96 };

}
Tree3::~Tree3()
{
}




