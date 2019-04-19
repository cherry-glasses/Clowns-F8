#include "Log.h"
#include "Application.h"
#include "MapLevel1.h"
#include "ModuleEntityManager.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


MapLevel1::MapLevel1(ENTITY_TYPE _type, pugi::xml_node _config) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Maps/map_level_1_trees.png");
}
MapLevel1::~MapLevel1()
{
}

bool MapLevel1::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool MapLevel1::Update(float dt)
{
	return true;
}

bool MapLevel1::PostUpdate()
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, 0 - (App->window->GetScreenWidth() / 2), 0 - (App->window->GetScreenHeight() / 8));
	}

	return true;
}

// Load and Save
bool MapLevel1::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool MapLevel1::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}



