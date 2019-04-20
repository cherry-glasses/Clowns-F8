#include "Log.h"
#include "Application.h"
#include "Object.h"
#include "ModuleEntityManager.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


bool Object::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Object::Update(float dt)
{
	return true;
}

bool Object::PostUpdate()
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first, position.second);
	}

	return true;
}

// Load and Save
bool Object::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Object::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}




