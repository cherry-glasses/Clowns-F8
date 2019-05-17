#include "Application.h"
#include "Character.h"
#include "ModuleRender.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"
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
		else if (current_turn == SEARCH_ABILITY_1)
		{
			SearchAbility_1();
		}
		else if (current_turn == SEARCH_ABILITY_2)
		{
			SearchAbility_2();
		}
		else if (current_turn == SEARCH_ABILITY_3)
		{
			SearchAbility_3();
		}
	}
	else if (current_turn == SEARCH_MOVE)
	{
		current_turn = END_TURN;
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
	else if (current_turn == SELECT_ABILITY_1)
	{
		SelectAbility_1();
	}
	else if (current_turn == SELECT_ABILITY_2)
	{
		SelectAbility_2();
	}
	else if (current_turn == SELECT_ABILITY_3)
	{
		SelectAbility_3();
	}
	else if (current_turn == MOVE)
	{
		Walk();
	}
	else if (current_turn == ATTACK)
	{
		if (critic) 
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Attack();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else 
		{
			Attack();
		}
	}
	else if (current_turn == ABILITY_1)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_1();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_1();
		}
	}
	else if (current_turn == ABILITY_2)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_2();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_2();
		}
	}
	else if (current_turn == ABILITY_3)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_3();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_3();
		}
	}
	else if (current_turn == DEFEND)
	{
		Defend();
	}

	return true;
}

bool Character::PostUpdate(float _dt) {

	if (current_turn == END_TURN)
	{
		EndTurn();
	}
	current = current_animation->frames[current_animation->GetCurrentFrameIndex()];
	if (entity_texture != nullptr)
	{
		if (critic)
		{
			App->render->Blit(debug_texture, position.first, position.second, &circle_yellow);
		}
		else
		{
			if (current_turn != NONE)
			{
				App->render->Blit(debug_texture, position.first, position.second, &circle_green);
			}
			else
			{
				App->render->Blit(debug_texture, position.first, position.second, &circle_blue);
			}

		}
		App->render->Blit(entity_texture, position.first - (current.w / 2) + position_margin.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(_dt), 1.0f, flipX);
		
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

		if (i != Cap && (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end()
			|| type == ENTITY_TYPE::ENTITY_CHARACTER_IRIS))
		{
			if (App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
				&& !App->pathfinding->IsUsed({ (*possible_mov).first , (*possible_mov).second }, this)
				&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
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
	
	if (App->input->Accept() 
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()) {
		current_turn = Entity::MOVE;
		comeback_position = { position.first, position.second };
		comeback_movement = current_movement;
	}
}

void Character::Walk()
{
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		if (i == Cap)
		{
			objective_position.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
		}
		++i;
	}

	if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_LEFT_FRONT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_RIGHT_FRONT);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_LEFT_BACK);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_RIGHT_BACK);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_FRONT);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_BACK);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_LEFT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_RIGHT);
	}
	else {
		current_turn = SELECT_ACTION;
	}

	// Ending walk and start idle animation
	if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		EndTurn();
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
		else if (current_movement == WALK_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == WALK_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == WALK_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == WALK_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
		current_turn = SELECT_ACTION;
		
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

	if (App->input->Accept() && App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second) , type))
	{
		current_turn = ATTACK;
		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline()) 
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}

}

void Character::Attack()
{
	if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
		CurrentMovement(ATTACK_LEFT_FRONT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
		CurrentMovement(ATTACK_RIGHT_FRONT);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
		CurrentMovement(ATTACK_LEFT_BACK);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
		CurrentMovement(ATTACK_RIGHT_BACK);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
		CurrentMovement(ATTACK_FRONT);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
		CurrentMovement(ATTACK_BACK);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
		CurrentMovement(ATTACK_LEFT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
		CurrentMovement(ATTACK_RIGHT);
	}
	else {
		CurrentMovement(ATTACK_RIGHT_FRONT);
	}

	// Ending attack and start idle animation
	if (current_animation->isDone()) {
		EndTurn();
		if (current_movement == ATTACK_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ATTACK_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ATTACK_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ATTACK_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ATTACK_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ATTACK_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ATTACK_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ATTACK_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
	}
}

void Character::SelectAbility_1() {

	if (Cap == -1) {
		Cap = possible_mov_list.size() / 2;
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type) 
				|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
					&& App->pathfinding->IsTrapped({ (*possible_mov).first , (*possible_mov).second })))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}

		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if ((i == Cap + 1) && ((Cap + 1) % mod != 0) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				App->render->Blit(debug_texture, possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second, &debug_green);
			}
			else if ((i == Cap - 1) && (Cap % mod != 0) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				App->render->Blit(debug_texture, possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second, &debug_green);
			}
			else if ((i == Cap + mod) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				App->render->Blit(debug_texture, possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second, &debug_green);
			}
			else if ((i == Cap - mod) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				App->render->Blit(debug_texture, possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second, &debug_green);
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			if (i == Cap) {
				if (Cap == mod || Cap - 2 == mod) {
					App->render->Blit(debug_texture, possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second, &debug_green);
				}
				else if (Cap == (mod / 2) || Cap + 1 == possible_mov_list.size() - (mod / 2)) {
					App->render->Blit(debug_texture, possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second, &debug_green);
				}
			}
			break;
		default:
			break;
		}
		++i;
	}

	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);
	
	// Input Select Tiled
	InputSelectAttack();

	// Input Accept and Decline
	if (App->input->Accept() 
		&& (App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
		|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second))
			&& App->pathfinding->IsTrapped(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)))))
	{
		current_turn = ABILITY_1;

		i = 0;
		mod = sqrt(possible_mov_list.size());
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if ((i == Cap + 1) && ((Cap + 1) % mod != 0) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					objective_position.push_back({ possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second });
				}
				else if ((i == Cap - 1) && (Cap % mod != 0) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					objective_position.push_back({ possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second });
				}
				else if ((i == Cap + mod) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					objective_position.push_back({ possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second });
				}
				else if ((i == Cap - mod) && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					objective_position.push_back({ possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second });
				}
				++i;
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if (i == Cap) {
					if (Cap == mod || Cap - 2 == mod) {
						objective_position.push_back({ possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second });
						objective_position.push_back({ possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second });
					}
					else if (Cap == (mod / 2) || Cap + 1 == possible_mov_list.size() - (mod / 2)) {
						objective_position.push_back({ possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second });
						objective_position.push_back({ possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second });
					}
				}
				++i;
			}
			break;
		default:
			break;
		}

		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}
}

void Character::Ability_1()
{
	if (current_stats.Mana >= 25) {
		if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_1_LEFT_FRONT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_1_RIGHT_FRONT);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_1_LEFT_BACK);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_1_RIGHT_BACK);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_1_FRONT);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_1_BACK);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_1_LEFT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_1_RIGHT);
		}
		else {
			CurrentMovement(ABILITY_1_LEFT_FRONT);
		}
	}
	else {
		current_turn = SELECT_ACTION;
	}
	

	// Ending attack and start idle animation
	if (current_animation->isDone()) {
		current_stats.Mana -= 25;
		EndTurn();
		if (current_movement == ABILITY_1_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_1_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_1_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_1_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_1_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_1_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_1_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_1_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}

	}
}

void Character::SelectAbility_2() {

	if (Cap == -1) {
		Cap = possible_mov_list.size() / 2;
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type)
				|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
					&& App->pathfinding->IsTrapped({ (*possible_mov).first , (*possible_mov).second })))
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

	// Input Select Tiled
	InputSelectAttack();

	// Input Accept and Decline
	if (App->input->Accept()
		&& (App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
			|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second))
				&& App->pathfinding->IsTrapped(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)))))
	{
		current_turn = ABILITY_2;

		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}
}

void Character::Ability_2()
{
	if (current_stats.Mana >= 50) {
		if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_2_LEFT_FRONT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_2_RIGHT_FRONT);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_2_LEFT_BACK);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_2_RIGHT_BACK);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_2_FRONT);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_2_BACK);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_2_LEFT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_2_RIGHT);
		}
		else {
			CurrentMovement(ABILITY_2_LEFT_FRONT);
		}
	}
	else {
		current_turn = SELECT_ACTION;
	}


	// Ending attack and start idle animation
	if (current_animation->isDone()) {
		current_stats.Mana -= 50;
		EndTurn();
		if (current_movement == ABILITY_2_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_2_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_2_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_2_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_2_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_2_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_2_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_2_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
	}
}

void Character::SelectAbility_3() {

	if (Cap == -1) {
		Cap = possible_mov_list.size() / 2;
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (type == ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end()) {
			App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_green);
		}
		else if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type)
				|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
					&& App->pathfinding->IsTrapped({ (*possible_mov).first , (*possible_mov).second })))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}

		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			if (i == Cap) {
				if (Cap == mod || Cap - 2 == mod) {
					App->render->Blit(debug_texture, possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second, &debug_green);
				}
				else if (Cap == (mod / 2) || Cap + 1 == possible_mov_list.size() - (mod / 2)) {
					App->render->Blit(debug_texture, possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second, &debug_green);
				}
			}
			break;
		default:
			break;
		}

		++i;
	}

	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	InputSelectAttack();

	if (App->input->Accept()
		&& (App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
			|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second))
				&& App->pathfinding->IsTrapped(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)))))
	{
		current_turn = ABILITY_3;

		i = 0;
		mod = sqrt(possible_mov_list.size());
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				objective_position.push_back({ possible_map.at(i).first, possible_map.at(i).second });
				++i;
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if (i == Cap) {
					if (Cap == mod || Cap - 2 == mod) {
						objective_position.push_back({ possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second });
						objective_position.push_back({ possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second });
					}
					else if (Cap == (mod / 2) || Cap + 1 == possible_mov_list.size() - (mod / 2)) {
						objective_position.push_back({ possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second });
						objective_position.push_back({ possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second });
					}
				}
				++i;
			}
			break;
		default:
			break;
		}

		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}
}

void Character::Ability_3()
{
	if (current_stats.Mana >= 100) {
		if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_3_LEFT_FRONT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_3_RIGHT_FRONT);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_3_LEFT_BACK);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_3_RIGHT_BACK);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
			CurrentMovement(ABILITY_3_FRONT);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
			CurrentMovement(ABILITY_3_BACK);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_3_LEFT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
			CurrentMovement(ABILITY_3_RIGHT);
		}
		else {
			CurrentMovement(ABILITY_3_LEFT_FRONT);
		}
	}
	else {
		current_turn = SELECT_ACTION;
	}


	// Ending attack and start idle animation
	if (current_animation->isDone()) {
		current_stats.Mana -= 100;
		EndTurn();
		if (current_movement == ABILITY_3_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_3_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_3_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_3_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_3_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_3_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_3_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_3_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
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
	case Entity::IDLE_LEFT:
		CurrentMovement(DEFEND_LEFT);
		break;
	case Entity::IDLE_RIGHT:
		CurrentMovement(DEFEND_RIGHT);
		break;
	case Entity::IDLE_FRONT:
		CurrentMovement(DEFEND_FRONT);
		break;
	case Entity::IDLE_BACK:
		CurrentMovement(DEFEND_BACK);
		break;
	case Entity::DEFEND_LEFT_FRONT:
		CurrentMovement(DEFEND_LEFT_FRONT);
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		CurrentMovement(DEFEND_RIGHT_FRONT);
		break;
	case Entity::DEFEND_LEFT_BACK:
		CurrentMovement(DEFEND_LEFT_BACK);
		break;
	case Entity::DEFEND_RIGHT_BACK:
		CurrentMovement(DEFEND_RIGHT_BACK);
		break;
	case Entity::DEFEND_LEFT:
		CurrentMovement(DEFEND_LEFT);
		break;
	case Entity::DEFEND_RIGHT:
		CurrentMovement(DEFEND_RIGHT);
		break;
	case Entity::DEFEND_FRONT:
		CurrentMovement(DEFEND_FRONT);
		break;
	case Entity::DEFEND_BACK:
		CurrentMovement(DEFEND_BACK);
		break;
	default:
		break;
	}
}

void Character::ComeBack()
{
	position = comeback_position;
	CurrentMovement(comeback_movement);
	possible_mov_list.clear();
	inrange_mov_list.clear();
	possible_map.clear();
	objective_position.clear();
	Cap = -1;
	current_turn = SEARCH_MOVE;
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
	case Entity::IDLE_LEFT:
		CurrentMovement(DEAD_LEFT);
		break;
	case Entity::IDLE_RIGHT:
		CurrentMovement(DEAD_RIGHT);
		break;
	case Entity::IDLE_FRONT:
		CurrentMovement(DEAD_FRONT);
		break;
	case Entity::IDLE_BACK:
		CurrentMovement(DEAD_BACK);
		break;
	case Entity::DEFEND_LEFT_FRONT:
		CurrentMovement(DEAD_LEFT_FRONT);
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		CurrentMovement(DEAD_RIGHT_FRONT);
		break;
	case Entity::DEFEND_LEFT_BACK:
		CurrentMovement(DEAD_LEFT_BACK);
		break;
	case Entity::DEFEND_RIGHT_BACK:
		CurrentMovement(DEAD_RIGHT_BACK);
		break;
	case Entity::DEFEND_LEFT:
		CurrentMovement(DEAD_LEFT);
		break;
	case Entity::DEFEND_RIGHT:
		CurrentMovement(DEAD_RIGHT);
		break;
	case Entity::DEFEND_FRONT:
		CurrentMovement(DEAD_FRONT);
		break;
	case Entity::DEFEND_BACK:
		CurrentMovement(DEAD_BACK);
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
	current_animation->Reset();
	critic = false;
}
