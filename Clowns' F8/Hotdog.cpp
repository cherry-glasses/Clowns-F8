#include "Log.h"
#include "Application.h"
#include "Hotdog.h"
#include "ModulePathfinding.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Hotdog::Hotdog(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{

	

	moving = true;

}
Hotdog::~Hotdog()
{
}

bool Hotdog::PreUpdate()
{
	bool ret = true;
	if (current_movement == IDLE) {
		if (moving)
		{
			App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(position.first + 100, position.second));
			Walk(App->pathfinding->GetLastPath());
			App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(position.first - 10, position.second));
			Walk(App->pathfinding->GetLastPath());
			App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(position.first, position.second + 100));
			Walk(App->pathfinding->GetLastPath());
			App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(position.first, position.second - 10));
			Walk(App->pathfinding->GetLastPath());
		}
		else {
			current_movement = IDLE;
			current_animation = &idle;
		}
	}

	return ret;
}

bool Hotdog::Update(float dt)
{
	if (current_movement == IDLE)
	{
		current_animation = &idle;
	}
	else if (current_movement == LEFTUP)
	{
		current_animation = &walk;
	}
	else if (current_movement == LEFTDOWN)
	{
		current_animation = &walk;
	}
	else if (current_movement == RIGHTUP)
	{
		current_animation = &walk;
	}
	else if (current_movement == RIGHTDOWN)
	{
		current_animation = &walk;
	}

	return true;
}

bool Hotdog::PostUpdate()
{
	if (entity_texture != nullptr) 
	{
		App->render->Blit(entity_texture, position.first, position.second, &current, 1.0f);
	}
	

	return true;
}


bool Hotdog::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Hotdog::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;


	return ret;
}


void Hotdog::Walk(const std::vector<std::pair<int, int>> *_path)
{
	if (_path->size() > 0)
	{
		for (uint i = 0; i < _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_1);
		}
	}
}
