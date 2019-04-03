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
	current_movement = IDLE_LEFT_FRONT;
	current_animation = &idle_left_front;
	current = current_animation->GetCurrentFrame();
}
Hotdog::~Hotdog()
{
}

bool Hotdog::PreUpdate()
{
	bool ret = true;

	if (current_turn == SEARCH_MOVE)
	{
		std::pair<int, int> nearposition = App->entity_manager->NearestCharacter(position);
		App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
		SearchWalk(App->pathfinding->GetLastPath());
		current_turn = MOVE;
	}
	else if (current_turn == SEARCH_ATTACK)
	{
		std::pair<int, int> nearposition = App->entity_manager->NearestCharacter(position);
		App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
		current_turn = ATTACK;
	}

	return ret;
}

bool Hotdog::Update(float dt)
{
	if (current_turn == NONE)
	{
		if (current_movement == IDLE_LEFT_FRONT) 
		{
			current_animation = &idle_left_front;
		}
		else if(current_movement == IDLE_RIGHT_FRONT) 
		{
			current_animation = &idle_right_front;
		} 
		else if (current_movement == IDLE_LEFT_BACK) 
		{
			current_animation = &idle_left_back;
		}
		else {
			current_animation = &idle_right_back;
		}
	}
	else if (current_turn == MOVE)
	{
		Walk(App->pathfinding->GetLastPath());

		if (current_movement == WALK_LEFT_FRONT) 
		{
			current_animation = &walk_left_front;
			position.first -= 2;
			position.second++;
		}
		else if (current_movement == WALK_RIGHT_FRONT) 
		{
			current_animation = &walk_right_front;
			position.first += 2;
			position.second++;
		}
		else if (current_movement == WALK_LEFT_BACK) 
		{
			current_animation = &walk_left_back;
			position.first -= 2;
			position.second--;
		}
		else 
		{
			current_animation = &walk_right_back;
			position.first += 2;
			position.second--;
		}
	}
	else if (current_turn == ATTACK)
	{
		
		Attack(App->pathfinding->GetLastPath());

		if (current_movement == ATTACK_LEFT_FRONT)
		{
			current_animation = &attack_left_front;
		}
		else if (current_movement == ATTACK_RIGHT_FRONT)
		{
			current_animation = &attack_right_front;
		}
		else if (current_movement == ATTACK_LEFT_BACK)
		{
			current_animation = &attack_left_back;
		}
		else
		{
			current_animation = &attack_right_back;
		}
	}
	

	return true;
}

bool Hotdog::PostUpdate()
{
	if (entity_texture != nullptr) 
	{
		App->render->Blit(entity_texture, position.first, position.second - (current.h / 3), &current_animation->GetCurrentFrame(), 1.0f);
	}

	return true;
}

// Load and Save
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

// Actions (SearchWalk, Walk, Attack, Hability 1, Hability 2, Die)
void Hotdog::SearchWalk(const std::vector<std::pair<int, int>> *_path)
{
	if (_path->size() > 2 && (_path->at(0).first == _path->at(2).first || _path->at(0).second == _path->at(2).second))
	{
		new_position = App->map->MapToWorld(_path->at(2).first, _path->at(2).second);
	}
	else if (_path->size() > 1)
	{
		new_position = App->map->MapToWorld(_path->at(1).first, _path->at(1).second);
	}

	if (_path->size() > 1) {
		if (_path->at(0).first == _path->at(1).first && _path->at(0).second < _path->at(1).second) {
			current_movement = WALK_LEFT_FRONT;
		}
		else if (_path->at(0).first < _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			current_movement = WALK_RIGHT_FRONT;
		}
		else if (_path->at(0).first > _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			current_movement = WALK_LEFT_BACK;
		}
		else if (_path->at(0).first == _path->at(1).first && _path->at(0).second > _path->at(1).second) {
			current_movement = WALK_RIGHT_BACK;
		}
		current_turn = MOVE;
	}
	else {
		current_turn = SEARCH_ATTACK;
	}

	
}

void Hotdog::Walk(const std::vector<std::pair<int, int>> *_path)
{
	for (uint i = 0; i < _path->size(); ++i)
	{
		std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
		App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);
	}

	if (new_position.first == position.first || new_position.second == position.second) {
		current_turn = SEARCH_ATTACK;
	}
	
}

void Hotdog::Attack(const std::vector<std::pair<int, int>> *_path)
{
	/*for (uint i = 0; i < _path->size(); ++i)
	{
		std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
		App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		if (_path->size() > 2 && (_path->at(0).first == _path->at(2).first || _path->at(0).second == _path->at(2).second))
		{
			position = App->map->MapToWorld(_path->at(2).first, _path->at(2).second);
		}
		else if (_path->size() > 1)
		{
			position = App->map->MapToWorld(_path->at(1).first, _path->at(1).second);
		}
		if (_path->size() > 1) {
			if (_path->at(0).first == _path->at(1).first && _path->at(0).second < _path->at(1).second) {
				current_movement = WALK_LEFT_FRONT;
			}
			else if (_path->at(0).first < _path->at(1).first && _path->at(0).second == _path->at(1).second) {
				current_movement = WALK_RIGHT_FRONT;
			}
			else if (_path->at(0).first > _path->at(1).first && _path->at(0).second == _path->at(1).second) {
				current_movement = WALK_LEFT_BACK;
			}
			else if (_path->at(0).first == _path->at(1).first && _path->at(0).second > _path->at(1).second) {
				current_movement = WALK_RIGHT_BACK;
			}
		}
		current_turn = END_TURN;
	}*/

	current_turn = END_TURN;
	current_movement = IDLE_LEFT_FRONT;
	
}
