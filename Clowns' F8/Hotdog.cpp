#include "Log.h"
#include "Application.h"
#include "Hotdog.h"
#include "ModulePathfinding.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Hotdog::Hotdog(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{
	//string_texture = _config.child("texture").attribute("path").as_string();

	/*Default_States.Hp = _config.child("Hp").value.as_int(2);
	Default_States.Mp = _config.child("Mp").value.as_int(2);
	Default_States.Mana = _config.child("Mana").value.as_int(2);
	Default_States.Cp = _config.child("Cp").value.as_int();
	Default_States.AtkF = _config.child("AtkF").value.as_int();
	Default_States.AtkS = _config.child("AtkS").value.as_int();
	Default_States.DefF = _config.child("DefF").value.as_int();
	Default_States.DefS = _config.child("DefS").value.as_int();
	Default_States.Crit_hit = _config.child("Crit_hit").value.as_int();*/

	/*LoadAnimation(config.child("animations").child("idle").child("frame"), idle);
	idle.speed = config.child("animations").child("idle").attribute("speed").as_float();
	idle.loop = config.child("animations").child("idle").attribute("loop").as_bool(true);*/

	/*LoadAnimation(config.child("animations").child("walk").child("frame"), walk);
	walk.speed = config.child("animations").child("walk").attribute("speed").as_float();
	walk.loop = config.child("animations").child("walk").attribute("loop").as_bool(true);

	LoadAnimation(config.child("animations").child("die").child("frame"), dead);
	dead.speed = config.child("animations").child("die").attribute("speed").as_float();
	dead.speed = config.child("animations").child("die").attribute("loop").as_bool(false);*/

	entity_texture = App->textures->Load(string_texture.c_str());
	debug_texture = App->textures->Load("Assets/Maps/meta.png");
	position.first = 300;
	position.second = 200;
	SDL_Rect rect;
	rect.h = 65;
	rect.w = 40;
	rect.x = 0;
	rect.y = 0;
	idle.PushBack(rect);
	idle.speed = 0;
	current_animation = &idle;
	current = current_animation->GetCurrentFrame();

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
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second);
		}
	}
}
