#include "Application.h"
#include "Hotdog.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleRender.h"
#include "ModuleMap.h"
#include "Log.h"

Hotdog::Hotdog(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config) {
	CurrentMovement(IDLE_LEFT);
	current = current_animation->GetCurrentFrame(1);
}
Hotdog::~Hotdog()
{
}


void Hotdog::SearchWalk() {
	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);
	App->pathfinding->CreatePathKnight(App->map->WorldToMap(position.first, position.second), nearposition);
	first = false;
	second = false;
	if (timer_skill_1 >= 2) {
		current_turn = SEARCH_ABILITY_1;
		timer_skill_1 = 0;
	}
	else
		current_turn = MOVE;

	timer_skill_1 += 1;
}

void Hotdog::Walk(const std::vector<std::pair<int, int>>* _path) {

	if (App->debug)
	{
		for (uint i = 0; i < _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_blue);
		}
	}

	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second); //posicion en mapa del enemigo mas cercano a Hotdog


	if (!App->pathfinding->IsUsed(_path->at(1), this)) //si no esta en una casilla adyacente al enemigo
	{
		if (!inRange) {
			objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
		}

		pos = App->map->WorldToMap(position.first, position.second);
		inRange = false;


		if (_path->at(0).first == (_path->at(1).first + 2)) {


			//por que no entra en el if de abajo del todo, de cuando llega a la posicion exacta de pixeles

			if (position != App->map->MapToWorld(_path->at(0).first - 2, _path->at(0).second) && !first && !second) //que este en el mismo eje x que la posicion final
				CurrentMovement(WALK_LEFT_FRONT);
			else if ((pos.second < _path->at(1).second) || first) //pa'rriba
			{
				first = true;
				CurrentMovement(WALK_LEFT_BACK);
			}

			else if ((pos.second > _path->at(1).second) || second) //pa'bajo
			{
				second = true;
				CurrentMovement(WALK_RIGHT_FRONT);
			}
			//else
				//current_turn = END_TURN;
		}
		else
			if (_path->at(0).first == (_path->at(1).first - 2)) {
				if (position != App->map->MapToWorld(_path->at(0).first + 2, _path->at(0).second) && !first && !second) //que este en el mismo eje x que la posicion final
					CurrentMovement(WALK_RIGHT_BACK);
				else if ((pos.second < _path->at(1).second) || first) //pa'rriba
				{
					first = true;
					CurrentMovement(WALK_LEFT_BACK);
				}

				else if ((pos.second > _path->at(1).second) || second) //pa'bajo
				{
					second = true;
					CurrentMovement(WALK_RIGHT_FRONT);
				}
			}
			else
				if (_path->at(0).second == (_path->at(1).second - 2)) {		//este va bien

					if (position != App->map->MapToWorld(_path->at(0).first, _path->at(0).second + 2) && !first && !second) //que este en el mismo eje x que la posicion final
						CurrentMovement(WALK_LEFT_BACK);
					else if ((pos.first < _path->at(1).first) || first) //pa'rriba
					{
						first = true;
						CurrentMovement(WALK_RIGHT_BACK);
					}

					else if ((pos.first > _path->at(1).first) || second) //pa'bajo
					{
						second = true;
						CurrentMovement(WALK_LEFT_FRONT);
					}
				}
				else
					if (_path->at(0).second == (_path->at(1).second + 2)) {

						if (position != App->map->MapToWorld(_path->at(0).first, _path->at(0).second - 2) && !first && !second) //que este en el mismo eje x que la posicion final
							CurrentMovement(WALK_RIGHT_FRONT);
						else if ((pos.first < _path->at(1).first) || first) //pa'rriba
						{
							first = true;
							CurrentMovement(WALK_RIGHT_BACK);
						}

						else if ((pos.first > _path->at(1).first) || second) //pa'bajo
						{
							second = true;
							CurrentMovement(WALK_LEFT_FRONT);
						}
					}

		current_turn = MOVE;

		if (objective_position.back().first == position.first && objective_position.back().second == position.second) {

			if (current_movement == WALK_LEFT_FRONT)
			{
				CurrentMovement(IDLE_FRONT);
			}
			else if (current_movement == WALK_RIGHT_FRONT)
			{
				CurrentMovement(IDLE_BACK);
			}
			else if (current_movement == WALK_RIGHT_BACK)
			{
				CurrentMovement(IDLE_BACK);
			}
			else if (current_movement == WALK_LEFT_BACK)
			{
				CurrentMovement(IDLE_FRONT);
			}
			inDanger = false;

			pos = App->map->WorldToMap(position.first, position.second);

			inRange = (App->entity_manager->CalculateDistance(pos, nearposition) <= current_stats.RangeAtk);	//cambiar por el rango del boss
			if (inRange)
				current_turn = SEARCH_ATTACK;
			else
				current_turn = END_TURN;
		}

	}
	else {
		pos = App->map->WorldToMap(position.first, position.second);
		inRange = (App->entity_manager->CalculateDistance(pos, nearposition) <= current_stats.RangeAtk);	//cambiar por el rango del boss
		if (inRange)
			current_turn = SEARCH_ATTACK;
		else
			current_turn = END_TURN;
	}
}


void Hotdog::SearchAttack() {
	objective_position.clear();
	nearposition = App->entity_manager->NearestCharacter(position);
	current_turn = ATTACK;

}

void Hotdog::Attack(const std::vector<std::pair<int, int>>* _path)
{
	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, 3, tiles_range_attk);	//cambiar por el rango del boss
	std::pair<int, int> car = App->entity_manager->CharactersPrioritzationAttack(range, tiles_range_attk);
	objective_position.push_back(car);
	car = App->map->WorldToMap(car.first, car.second);

	if (car.first > pos.first)
		CurrentMovement(ATTACK_FRONT);
	else
		CurrentMovement(ATTACK_BACK);

	if (current_animation->isDone()) {

		App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_HOTDOG, false);	//hay que cambiar esto

		current_animation->Reset();
		if (current_movement == ATTACK_FRONT)
			CurrentMovement(IDLE_FRONT);
		else
			CurrentMovement(IDLE_BACK);
		current_turn = END_TURN;
	}
}

void Hotdog::SearchAbility_1()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);
	App->pathfinding->CreatePathKnight(App->map->WorldToMap(position.first, position.second), nearposition);
	pos = App->map->WorldToMap(position.first, position.second);
	current_turn = ABILITY_1;
	is_fucking_walkeable = false;
	they_see_me_rolling = App->map->WorldToMap(position.first, position.second);
	const std::vector<std::pair<int, int>>* _path = App->pathfinding->GetLastPath();
	int axis_x = nearposition.first - pos.first;
	int axis_y = nearposition.second - pos.second;
	if (axis_x < 0) {
		axis_x = axis_x * -1;
	}
	if (axis_y < 0) {
		axis_y = axis_y * -1;
	}
	if (axis_x > axis_y) 
	{
		if (nearposition.first < pos.first) {
			they_see_me_rolling.first -= ability_1_range;
			
		}
		else
		{
			they_see_me_rolling.first += ability_1_range;
			
		}
	}
	else
	{
		if (nearposition.second > pos.second) {
			they_see_me_rolling.second += ability_1_range;
		
		}
		else
		{
			they_see_me_rolling.second -= ability_1_range;
			
		}
	}


}

void Hotdog::Ability_1(const std::vector<std::pair<int, int>>* _path)
{
	
	std::pair<int, int> meta;
	
	meta = App->map->MapToWorld(they_see_me_rolling.first, they_see_me_rolling.second);
	
	
	if ((!App->pathfinding->IsWalkable(they_see_me_rolling) || App->pathfinding->IsUsed(they_see_me_rolling, this)) && !is_fucking_walkeable) {
		current_turn = END_TURN;
	}
	else {
		is_fucking_walkeable = true;
		int axis_x = nearposition.first - pos.first;
		int axis_y = nearposition.second - pos.second;
		if (axis_x < 0) {
			axis_x = axis_x * -1;
		}
		if (axis_y < 0) {
			axis_y = axis_y * -1;
		}
		if (axis_x > axis_y)
		{
			if (nearposition.first <= pos.first) {
				CurrentMovement(ABILITY_1_FRONT);
				for (int i = 1; i <= ability_1_range; i++) {
					objective_position.push_back(App->map->MapToWorld(pos.first - i, pos.second));
				}
			}
			else
			{
				CurrentMovement(ABILITY_1_BACK);
				for (int i = 1; i <= ability_1_range; i++) {
					objective_position.push_back(App->map->MapToWorld(pos.first + i, pos.second));
				}
			}
		}
		else
		{
			if (nearposition.second >= pos.second) {
				CurrentMovement(ABILITY_1_LEFT);
				for (int i = 1; i <= ability_1_range; i++) {
					objective_position.push_back(App->map->MapToWorld(pos.first, pos.second + i));
				}
			}
			else
			{
				CurrentMovement(ABILITY_1_RIGHT);
				for (int i = 1; i <= ability_1_range; i++) {
					objective_position.push_back(App->map->MapToWorld(pos.first, pos.second - i));
				}
			}
		}
	}
	
		
	
	if (position.first == meta.first && position.second == meta.second) {
		App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_HOTDOG, false);
		if (current_movement == ABILITY_1_RIGHT) {
			CurrentMovement(IDLE_BACK);
		}
		else if (current_movement == ABILITY_1_LEFT) {
			CurrentMovement(IDLE_BACK);
		}
		else if (current_movement == ABILITY_1_FRONT) {
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_1_BACK) {
			CurrentMovement(IDLE_FRONT);
		}
				

		current_turn = END_TURN;
	}

	
	

}


void Hotdog::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT:
		current_movement = IDLE_LEFT;
		current_animation = &idle_left;
		break;
	case Entity::IDLE_RIGHT:
		current_movement = IDLE_RIGHT;
		current_animation = &idle_right;
		break;
	case Entity::IDLE_FRONT:
		current_movement = IDLE_FRONT;
		current_animation = &idle_front;
		break;
	case Entity::IDLE_BACK:
		current_movement = IDLE_BACK;
		current_animation = &idle_back;
		break;
	case Entity::WALK_FRONT:										
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		position.second += 2;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first += 2;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		position.second -= 2;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		position.first -= 2;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 2;
		position.second -= 1;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 2;
		position.second += 1;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_left_back;
		position.first -= 2;
		position.second += 1;
		break;
	case Entity::WALK_LEFT_FRONT:								
		current_movement = WALK_LEFT_FRONT;
		current_animation = &walk_left_front;
		position.first -= 2;
		position.second -= 1;
		break;
	case Entity::ATTACK_LEFT:
		current_movement = ATTACK_LEFT;
		current_animation = &attack_left;
		break;
	case Entity::ATTACK_RIGHT:
		current_movement = ATTACK_RIGHT;
		current_animation = &attack_right;
		break;
	case Entity::ATTACK_FRONT:
		current_movement = ATTACK_FRONT;
		current_animation = &attack_front;
		break;
	case Entity::ATTACK_BACK:
		current_movement = ATTACK_BACK;
		current_animation = &attack_back;
		break;
	case Entity::ABILITY_1_LEFT:
		current_movement = ABILITY_1_LEFT;
		current_animation = &ability_1_left;
		position.first -= 2;
		position.second += 1;
		break;
	case Entity::ABILITY_1_RIGHT:
		current_movement = ABILITY_1_RIGHT;
		current_animation = &ability_1_right;
		position.first += 2;
		position.second -= 1;
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		position.first -= 2;
		position.second -= 1;
		current_animation = &ability_1_front;
		break;
	case Entity::ABILITY_1_BACK:
		current_movement = ABILITY_1_BACK;
		current_animation = &ability_1_back;
		position.first += 2;
		position.second += 1;
		break;
	case Entity::DEAD_LEFT:
		current_movement = DEAD_LEFT;
		current_animation = &dead_left;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_RIGHT:
		current_movement = DEAD_RIGHT;
		current_animation = &dead_right;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_FRONT:
		current_movement = DEAD_FRONT;
		current_animation = &dead_front;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_BACK:
		current_movement = DEAD_BACK;
		current_animation = &dead_back;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	default:
		break;
	}
}