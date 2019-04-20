#include "Log.h"
#include "Application.h"
#include "BearTrap.h"
#include "ModuleEntityManager.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


BearTrap::BearTrap(ENTITY_TYPE _type, pugi::xml_node _config) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/UI/stunned_icon.png");
}
BearTrap::~BearTrap()
{
}

bool BearTrap::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool BearTrap::Update(float dt)
{
	return true;
}

bool BearTrap::PostUpdate()
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first, position.second - current.h + position_margin.second);
	}

	return true;
}

// Load and Save
bool BearTrap::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool BearTrap::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}



