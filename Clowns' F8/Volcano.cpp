#include "Application.h"
#include "Volcano.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"


Volcano::Volcano(ENTITY_TYPE _type, pugi::xml_node _config) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/volcano.png");
	position = App->map->MapToWorld(3, 24);
	
	current = { 0 , 0, 96, 128 };
}
Volcano::~Volcano()
{
}




