#include "Log.h"
#include "Application.h"
#include "Hotdog.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Hotdog::Hotdog(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{
}
Hotdog::~Hotdog()
{
}

bool Hotdog::PreUpdate()
{
	bool ret = true;
	if (current_movement == IDLE) {
		if (current_turn == MOVE)
		{
 			
		}
		
	}

	return ret;
}

bool Hotdog::Update(float dt)
{
	
	if (current_movement == IDLE)
	{
		if (current_turn == MOVE)
		{
			current_movement = IDLE;
			current_animation = &idle;
			std::pair<int,int> nearposition = App->entity_manager->NearestCharacter(position);
			App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
			Walk(App->pathfinding->GetLastPath());
		}
		
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
	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 0;
	tmp = App->map->MapToWorld((int)tmp.first, (int)tmp.second);
	//App->render->Blit(debug_texture, tmp.first, tmp.second, &debug_green);
	for (uint i = 0; i < _path->size(); ++i)
	{
		std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
		App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		
		/*tmp = App->map->WorldToMap((int)tmp.first, (int)tmp.second);*/
		position = App->map->MapToWorld(_path->at(1).first, _path->at(1).second);;
		current_turn = END_TURN;
	}

}
