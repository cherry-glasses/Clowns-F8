#include "Application.h"
#include "Character.h"
#include "ModuleRender.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"


bool Character::PreUpdate() {

	if (current_state == ALIVE) {

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
	else
	{
		current_turn == END_TURN;
	}

	return true;
}
bool Character::Update(float _dt) {

	if (current_turn == NONE)
	{
	}
	else if (current_turn == SELECT_MOVE)
	{
		SelectWalk();
	}
	else if (current_turn == SELECT_ATTACK)
	{
		SelectAttack();
	}
	else if (current_turn == Entity::MOVE)
	{
		Walk();
	}
	else if (current_turn == ATTACK)
	{
		Attack();
	}
	else if (current_turn == DEFEND)
	{
		Defend();
	}

	return true;
}

bool Character::PostUpdate() {

	if (current_turn == END_TURN)
	{
		EndTurn();
	}

	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f);
	}

	return true;
}

bool Character::Load(pugi::xml_node& node) {
	return true;
}

bool Character::Save(pugi::xml_node& node) const {
	return true;
}

void Character::SelectWalk() {

	if (Cap == -1) {
		Cap = possible_mov_list.size() / 2;
	}
	
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
				&& !App->pathfinding->IsUsed({ (*possible_mov).first , (*possible_mov).second }, this))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}
		++i;
	}
	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	InputSelectMove();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		current_turn = Entity::MOVE;
	}
}


void Character::SelectAttack() {

	if (Cap == -1) {
		Cap = possible_mov_list.size() / 2;
	}

	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second } , type))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}
		++i;
	}
	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	InputSelectAttack();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN
		&& App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second) , type))
	{
		current_turn = ATTACK;
	}
	else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) 
	{
		current_turn = SELECT_ACTION;
	}
}

void Character::Defend()
{
	defend = true;
	switch (current_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		CurrentMovement(DEFEND_LEFT_FRONT);
		break;
	case Entity::IDLE_RIGHT_FRONT:
		CurrentMovement(DEFEND_RIGHT_FRONT);
		break;
	case Entity::IDLE_LEFT_BACK:
		CurrentMovement(DEFEND_LEFT_BACK);
		break;
	case Entity::IDLE_RIGHT_BACK:
		CurrentMovement(DEFEND_RIGHT_BACK);
		break;
	default:
		break;
	}

}

void Character::EndTurn() {
	possible_mov_list.clear();
	inrange_mov_list.clear();
	possible_map.clear();
	objective_position.clear();
	Cap = -1;
}

void Character::Die()
{
	switch (current_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		CurrentMovement(DEAD_LEFT_FRONT);
		break;
	case Entity::IDLE_RIGHT_FRONT:
		CurrentMovement(DEAD_RIGHT_FRONT);
		break;
	case Entity::IDLE_LEFT_BACK:
		CurrentMovement(DEAD_LEFT_BACK);
		break;
	case Entity::IDLE_RIGHT_BACK:
		CurrentMovement(DEAD_RIGHT_BACK);
		break;
	default:
		break;
	}
}
