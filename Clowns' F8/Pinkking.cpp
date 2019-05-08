#include "Application.h"
#include "Pinkking.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleRender.h"
#include "ModuleMap.h"
#include "Log.h"



Pinkking::Pinkking(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{
	CurrentMovement(IDLE_LEFT);
	current = current_animation->GetCurrentFrame();
}
Pinkking::~Pinkking()
{
}




// Actions (SearchWalk, Walk, Attack, Hability 1, Hability 2, Die)
void Pinkking::SearchWalk()
{
	//objective_position.clear();
	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);
	App->pathfinding->CreatePathBishop(App->map->WorldToMap(position.first, position.second), nearposition, current_stats.PMove);

	//Skill_1
	if (timer_skill_1 == 3) {
		current_turn = SEARCH_ABILITY_1;
		timer_skill_1 = 0;
	}
	else
		current_turn = MOVE;
	timer_skill_1++;

}

void Pinkking::Walk(const std::vector<std::pair<int, int>> *_path)
{

	if (App->debug)
	{
		for (uint i = 0; i < _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);
		}
	}

	//Bishop Movment + Rush_attack

	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);

	std::pair<int, int> object;


	if (_path->size() > 1 )
	{
		if (!App->pathfinding->IsUsed(_path->at(1), this)) 
		{
			if (!inRange) {
				objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
				
			}
				

			inRange = false;
			
			if (_path->at(0).first >= _path->at(1).first && _path->at(0).second <= _path->at(1).second) {
				CurrentMovement(WALK_LEFT);
			}
			else if (_path->at(0).first >= _path->at(1).first && _path->at(0).second >= _path->at(1).second) {
				CurrentMovement(WALK_BACK);
			}
			else if (_path->at(0).first <= _path->at(1).first && _path->at(0).second <= _path->at(1).second) {
				CurrentMovement(WALK_FRONT);
			}
			else if (_path->at(0).first <= _path->at(1).first && _path->at(0).second >= _path->at(1).second) {
				CurrentMovement(WALK_RIGHT);
			}
			current_turn = MOVE;
			pos = App->map->WorldToMap(position.first, position.second);
			if (App->entity_manager->CalculateDistance(pos, nearposition) < current_stats.RangeAtk && (App->map->MapToWorld(pos.first, pos.second) == position)){
				current_turn = SEARCH_ATTACK;

			}

			std::pair<int, int> cancer = App->map->MapToWorld(pos.first,pos.second);
			
			LOG("posii %i , %i , cancer %i ,%i", position.first, position.second,cancer.first, cancer.second);
			//std::pair<int, int> yoooo = App->map->WorldToMap(objective_position.back().first, objective_position.back().second);
			if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
				
				if (current_movement == WALK_LEFT)
				{
					CurrentMovement(IDLE_LEFT);
				}
				else if (current_movement == WALK_RIGHT)
				{
					CurrentMovement(IDLE_RIGHT);
				}
				else if (current_movement == WALK_BACK)
				{
					CurrentMovement(IDLE_LEFT);
				}
				else if (current_movement == WALK_FRONT)
				{
					CurrentMovement(IDLE_LEFT);
				}
				inDanger = false;
				if (inRange)
					current_turn = SEARCH_ATTACK;
				else
					current_turn = END_TURN;
			}

		}
		else {
			if (App->entity_manager->CalculateDistance(pos, nearposition) <= current_stats.RangeAtk)
				current_turn = SEARCH_ATTACK;
			else
				current_turn = END_TURN;
		}


	}

	else {
		if (App->entity_manager->CalculateDistance(pos, nearposition) <= current_stats.RangeAtk)
			current_turn = SEARCH_ATTACK;
		else
			current_turn = END_TURN;

	}
	
	
}

void Pinkking::SearchAttack()
{
	objective_position.clear();
	nearposition = App->entity_manager->NearestCharacter(position);
	current_turn = ATTACK;
}

void Pinkking::Attack(const std::vector<std::pair<int, int>> *_path)
{
	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAtk, tiles_range_attk);
	std::pair<int, int> car = App->entity_manager->CharactersPrioritzationAttack(range, tiles_range_attk);
	objective_position.push_back(car);

	if (car.first > pos.first)
		CurrentMovement(ATTACK_FRONT);
	else 
		CurrentMovement(ATTACK_BACK);

	if (current_animation->isDone()) {
		App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_PINKKING, false);
		current_animation->Reset();
		if (current_movement == ATTACK_FRONT)
			CurrentMovement(IDLE_RIGHT);
		else
			CurrentMovement(IDLE_LEFT);
		current_turn = END_TURN;
	}
}

void Pinkking::SearchAbility_1()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	current_turn = ABILITY_1;
}

void Pinkking::Ability_1(const std::vector<std::pair<int, int>> *_path)
{
	objective_position.push_back(nearposition);
	App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkS*1.5, ENTITY_TYPE::ENTITY_ENEMY_PINKKING, true);
	current_turn = END_TURN;
}


std::pair<int, int> Pinkking::Next_cell(std::pair<int, int> pos)
{
	std::pair<int, int> ret = pos;

	switch (current_movement) 
	{
	case Entity::WALK_LEFT:
		ret.first -= 1;
		ret.second += 1;

		break;
	case Entity::WALK_RIGHT:
		ret.first -= 1;
		ret.second += 1;

		break;
	case Entity::WALK_FRONT:
		ret.first -= 1;
		ret.second += 1;

		break;
	case Entity::WALK_BACK:
		ret.first -= 1;
		ret.second += 1;

		break;
	default:
		break;
	}
	ret = App->map->MapToWorld(ret.first, ret.second);
	return ret;
}

void Pinkking::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT:
		current_movement = IDLE_LEFT;
		current_animation = &idle_left;
		flipX = false;
		break;
	case Entity::IDLE_RIGHT:
		current_movement = IDLE_RIGHT;
		current_animation = &idle_left;
		flipX = true;
		break;
	case Entity::IDLE_FRONT:
		current_movement = IDLE_FRONT;
		current_animation = &idle_front;
		flipX = false;
		break;
	case Entity::IDLE_BACK:
		current_movement = IDLE_BACK;
		current_animation = &idle_back;
		flipX = false;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		flipX = false;
		position.first -= 2;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_left;
		position.first += 2 ;
		flipX = true;
		break;
	case Entity::WALK_FRONT:
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		flipX = false;
		position.second += 2;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		flipX = false;
		position.second -= 2;
		break;
	case Entity::ATTACK_LEFT:
		current_movement = ATTACK_LEFT;
		current_animation = &attack_left;
		flipX = false;
		break;
	case Entity::ATTACK_RIGHT:
		current_movement = ATTACK_RIGHT;
		current_animation = &attack_left;
		flipX = true;
		break;
	case Entity::ATTACK_FRONT:
		current_movement = ATTACK_FRONT;
		current_animation = &attack_front;
		flipX = false;
		break;
	case Entity::ATTACK_BACK:
		current_movement = ATTACK_BACK;
		current_animation = &attack_back;
		flipX = false;
		break;
	case Entity::ABILITY_1_LEFT:
		current_movement = ABILITY_1_LEFT;
		current_animation = &ability_1_left;
		flipX = false;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT:
		current_movement = ABILITY_1_RIGHT;
		current_animation = &ability_1_left;
		flipX = true;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		current_animation = &ability_1_front;
		flipX = false;
		break;
	case Entity::ABILITY_1_BACK: 
		current_movement = ABILITY_1_BACK;
		current_animation = &ability_1_back;
		flipX = false;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT:
		current_movement = DEAD_LEFT;
		current_animation = &dead_left;
		flipX = false;
		if (current_animation->Finished()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_RIGHT:
		current_movement = DEAD_RIGHT;
		current_animation = &dead_left;
		flipX = true;
		if (current_animation->Finished()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_FRONT:
		current_movement = DEAD_FRONT;
		current_animation = &dead_front;
		flipX = false;
		if (current_animation->Finished()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_BACK:
		current_movement = DEAD_BACK;
		current_animation = &dead_back;
		flipX = false;
		if (current_animation->Finished()) {
			current_state = DEATH;
		}
		break;
	default:
		break;
	}
}