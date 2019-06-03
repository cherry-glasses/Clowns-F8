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
	current_turn = MOVE;
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

			LOG("pos: %i.%i | path(1): %i,%i", pos.first, pos.second, _path->at(1).first, _path->at(1).second);
			LOG("position: %i,%i | objective: %i,%i", position.first, position.second, objective_position.back().first, objective_position.back().second);

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
				CurrentMovement(IDLE_LEFT);
			}
			else if (current_movement == WALK_RIGHT_FRONT)
			{
				CurrentMovement(IDLE_RIGHT);
			}
			else if (current_movement == WALK_RIGHT_BACK)
			{
				CurrentMovement(IDLE_LEFT);
			}
			else if (current_movement == WALK_LEFT_BACK)
			{
				CurrentMovement(IDLE_LEFT);
			}
			inDanger = false;

			pos = App->map->WorldToMap(position.first, position.second);

			inRange = (App->entity_manager->CalculateDistance(pos, nearposition) <= 3);	//cambiar por el rango del boss
			if (inRange)
				current_turn = SEARCH_ATTACK;
			else
				current_turn = END_TURN;
		}

	}
	else {
		if (App->entity_manager->CalculateDistance(pos, nearposition) <= 3)	//cambiar por el rango del boss
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

		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_HOTDOG, false);	//hay que cambiar esto

		current_animation->Reset();
		if (current_movement == ATTACK_FRONT)
			CurrentMovement(IDLE_RIGHT);
		else
			CurrentMovement(IDLE_LEFT);
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
	case Entity::WALK_FRONT:										//------- MOVIMIENTO
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
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT:
		current_movement = ABILITY_1_RIGHT;
		current_animation = &ability_1_right;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		current_animation = &ability_1_front;
		break;
	case Entity::ABILITY_1_BACK:
		current_movement = ABILITY_1_BACK;
		current_animation = &ability_1_back;
		current_turn = END_TURN;
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