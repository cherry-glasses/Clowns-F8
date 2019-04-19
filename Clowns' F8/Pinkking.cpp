#include "Log.h"
#include "Application.h"
#include "Pinkking.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Pinkking::Pinkking(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{
	CurrentMovement(IDLE_LEFT_FRONT);
	current = current_animation->GetCurrentFrame();
}
Pinkking::~Pinkking()
{
}

bool Pinkking::PreUpdate()
{

	if (current_state == ALIVE) {

		objective_position.clear();
		objective_position.push_back(position);

		if (current_stats.Hp <= 0)
		{
			Die();
		}

		if (current_turn == SEARCH_MOVE)
		{
			SearchWalk();
		}
		else if (current_turn == SEARCH_ATTACK)
		{
			SearchAttack();
		}
	}
	else if (current_turn == SEARCH_MOVE)
	{
		current_turn == END_TURN;
	}

	return true;
}

bool Pinkking::Update(float dt)
{
	if (current_turn == NONE)
	{
		return true;
	}
	else if (current_turn == MOVE)
	{
		Walk(App->pathfinding->GetLastPath());
	}
	else if (current_turn == ATTACK)
	{
		Attack(App->pathfinding->GetLastPath());
	}

	return true;
}

bool Pinkking::PostUpdate()
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f);
	}

	return true;
}

// Load and Save
bool Pinkking::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Pinkking::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}

// Actions (SearchWalk, Walk, Attack, Hability 1, Hability 2, Die)
void Pinkking::SearchWalk()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);
	App->pathfinding->CreatePathBishop(App->map->WorldToMap(position.first, position.second), nearposition, current_stats.PMove);
	//App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
	current_turn = MOVE;

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
	
	if (_path->size() > 1)
	{
		objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
	}

	if (sqrt((nearposition.first - _path->at(0).first)*(nearposition.first - _path->at(0).first) + (nearposition.second - _path->at(0).second)*(nearposition.second - _path->at(0).second)) > current_stats.RangeAtk) {
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
	}
	else {
		current_turn = SEARCH_ATTACK;
		//current_turn = END_TURN;
	}

	if ((objective_position.back().first == position.first && objective_position.back().second == position.second) || (objective_position.back().first == position.first && objective_position.back().second == position.second)) {
		CurrentMovement(IDLE_LEFT_FRONT);
		current_turn = END_TURN;
		//current_turn = SEARCH_ATTACK;

	}
	//LOG("current position: x. %i y. %i  objective position: x. %i y. %i", position.first, position.second, objective_position.back().first, objective_position.back().second);
	
		
}

void Pinkking::SearchAttack()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	//App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
	current_turn = ATTACK;
	//IA Attack. Into range of position + attack. If enemy is near to dead. If enemy def.
}

void Pinkking::Attack(const std::vector<std::pair<int, int>> *_path)
{
	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAtk, tiles_range_attk);
	std::pair<int, int> car = App->entity_manager->CharactersPrioritzationAttack(range, tiles_range_attk);
	objective_position.push_back(car);
	App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
	current_movement = ATTACK_LEFT_FRONT;
	current_turn = END_TURN;
	
}

void Pinkking::Die()
{
	if (current_movement == IDLE_LEFT_FRONT)
	{
		CurrentMovement(DEAD_LEFT_FRONT);
	}
	else if (current_movement == IDLE_RIGHT_FRONT)
	{
		CurrentMovement(DEAD_RIGHT_FRONT);
	}
	else if (current_movement == IDLE_LEFT_BACK)
	{
		CurrentMovement(DEAD_LEFT_BACK);
	}
	else
	{
		CurrentMovement(DEAD_RIGHT_BACK);
	}
}

void Pinkking::CurrentMovement(MOVEMENT _movement) {

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
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		position.first--;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first++ ;
		break;
	case Entity::WALK_FRONT:
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		position.second++;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		position.second--;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQUÍ
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &ability_2_left_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &ability_2_right_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &ability_2_left_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &ability_2_right_back;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_left_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_left_back;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT_BACK:
		current_movement = DEAD_RIGHT_BACK;
		current_animation = &dead_right_back;
		current_state = DEATH;
		break;
	default:
		break;
	}
}