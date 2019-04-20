#include "Log.h"
#include "Application.h"
#include "Tree.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"


Tree::Tree(ENTITY_TYPE _type, pugi::xml_node _config) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Maps/map_level_1_trees.png");
	position.first = - App->window->GetScreenWidth() / 2;
	position.second = - App->window->GetScreenHeight() / 8;
}
Tree::~Tree()
{
}




