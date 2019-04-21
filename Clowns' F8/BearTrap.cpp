#include "Log.h"
#include "Application.h"
#include "BearTrap.h"
#include "ModuleTextures.h"


BearTrap::BearTrap(ENTITY_TYPE _type, pugi::xml_node _config) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/bear_trap.png");
	current = { 0 , 0, 32, 32 };
}
BearTrap::~BearTrap()
{
}




