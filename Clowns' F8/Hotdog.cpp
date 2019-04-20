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
	CurrentMovement(IDLE_LEFT_FRONT);
	current = current_animation->GetCurrentFrame();
}
Hotdog::~Hotdog()
{
}


// Actions (SearchWalk, Walk, Attack, Hability 1, Hability 2, Die)
void Hotdog::SearchWalk()
{
	std::pair<int, int> nearposition = App->entity_manager->NearestCharacter(position);
	App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
	current_turn = MOVE;
}

void Hotdog::Walk(const std::vector<std::pair<int, int>> *_path)
{
	if (App->debug) 
	{
		for (uint i = 0; i < _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);
		}
	}

	if (_path->size() > 2 && (_path->at(0).first == _path->at(2).first || _path->at(0).second == _path->at(2).second))
	{
		objective_position.push_back(App->map->MapToWorld(_path->at(2).first, _path->at(2).second));
	}
	else if (_path->size() > 1)
	{
		objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
	}

	if (_path->size() > 2) {
		if (_path->at(0).first == _path->at(1).first && _path->at(0).second < _path->at(1).second) {
			CurrentMovement(WALK_LEFT_FRONT);
		}
		else if (_path->at(0).first < _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			CurrentMovement(WALK_RIGHT_FRONT);
		}
		else if (_path->at(0).first > _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			CurrentMovement(WALK_LEFT_BACK);
		}
		else if (_path->at(0).first == _path->at(1).first && _path->at(0).second > _path->at(1).second) {
			CurrentMovement(WALK_RIGHT_BACK);
		}
		current_turn = MOVE;
	}
	else {
		current_turn = SEARCH_ATTACK;
	}

	if (objective_position.back().first == position.first || objective_position.back().second == position.second) {
		if (current_movement == WALK_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == WALK_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == WALK_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == WALK_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		current_turn = SEARCH_ATTACK;
	}
}

void Hotdog::SearchAttack()
{
	std::pair<int, int> nearposition = App->entity_manager->NearestCharacter(position);
	App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
	current_turn = ATTACK;
	//IA Attack. Into range of position + attack. If enemy is near to dead. If enemy def.
}

void Hotdog::Attack(const std::vector<std::pair<int, int>> *_path)
{
	if (App->debug) 
	{
		for (uint i = 0; i < _path->size() && 2 == _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);
		}
	}

	if (_path->size() == 2) {
		objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
		if (_path->at(0).first == _path->at(1).first && _path->at(0).second < _path->at(1).second) {
			CurrentMovement(ATTACK_LEFT_FRONT);
		}
		else if (_path->at(0).first < _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			CurrentMovement(ATTACK_RIGHT_FRONT);
		}
		else if (_path->at(0).first > _path->at(1).first && _path->at(0).second == _path->at(1).second) {
			CurrentMovement(ATTACK_LEFT_BACK);
		}
		else if (_path->at(0).first == _path->at(1).first && _path->at(0).second > _path->at(1).second) {
			CurrentMovement(ATTACK_RIGHT_BACK);
		}
	}
	else
	{
		current_turn = END_TURN;
	}
}

void Hotdog::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		current_movement = IDLE_LEFT_FRONT;
		current_animation = &idle_left_front;
		break;
	case Entity::IDLE_RIGHT_FRONT:
		current_movement = IDLE_RIGHT_FRONT;
		current_animation = &idle_right_front;
		break;
	case Entity::IDLE_LEFT_BACK:
		current_movement = IDLE_LEFT_BACK;
		current_animation = &idle_left_back;
		break;
	case Entity::IDLE_RIGHT_BACK:
		current_movement = IDLE_RIGHT_BACK;
		current_animation = &idle_right_back;
		break;
	case Entity::WALK_LEFT_FRONT:
		current_movement = WALK_LEFT_FRONT;
		current_animation = &walk_left_front;
		position.first -= 2;
		position.second++;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 2;
		position.second++;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_left_back;
		position.first -= 2;
		position.second--;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 2;
		position.second--;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQU�
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &ability_2_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &ability_2_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &ability_2_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &ability_2_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_left_front;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_left_back;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_RIGHT_BACK:
		current_movement = DEAD_RIGHT_BACK;
		current_animation = &dead_right_back;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	default:
		break;
	}
}