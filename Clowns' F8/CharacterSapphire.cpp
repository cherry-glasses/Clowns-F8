#include "Application.h"
#include "CharacterSapphire.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"


CharacterSapphire::CharacterSapphire(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT_FRONT);
	current = current_animation->GetCurrentFrame();

}

CharacterSapphire::~CharacterSapphire() {

}


bool CompareByX(std::pair<int, int> first, std::pair<int, int> second) {
	return (first.first < second.first);
}
bool CompareByY(std::pair<int, int> first, std::pair<int, int> second) {
	return (first.second < second.second);
}

void CharacterSapphire::SearchWalk() {

	std::pair<int, int> tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second += 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second += 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second -= 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second -= 1;
	possible_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second -= 1;
	possible_mov_list.push_back(tmp);

	tmp.first = NULL;
	tmp.second = NULL;

	possible_mov_list.sort(CompareByX);
	possible_mov_list.sort(CompareByY);

	current_turn = Entity::SELECT_MOVE;

}


void CharacterSapphire::Walk()
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
	else if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		current_turn = SEARCH_ATTACK;
	}
	else {
		current_turn = SELECT_ACTION;
	}

	if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		if (current_movement == WALK_LEFT_FRONT || current_movement == WALK_LEFT)
		{
		CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == WALK_RIGHT_FRONT || current_movement == WALK_RIGHT || current_movement == WALK_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == WALK_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == WALK_RIGHT_BACK || current_movement == WALK_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		current_turn = SELECT_ACTION;
	}
}

void CharacterSapphire::SearchAttack() {
	objective_position.clear();
	possible_mov_list.clear();
	possible_map.clear();
	Cap = 0;

	std::pair<int, int> cancer = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(cancer, current_stats.RangeAtk, tiles_range_attk);
	int x = cancer.first - current_stats.RangeAtk;
	int y = cancer.second - current_stats.RangeAtk;
	for (int i = 0; i < ((current_stats.RangeAtk * 2) + 1) * ((current_stats.RangeAtk * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > cancer.first + current_stats.RangeAtk) {
			x = cancer.first - current_stats.RangeAtk;
			++y;
		}
		if (y > cancer.second + current_stats.RangeAtk) {
			y = cancer.second - current_stats.RangeAtk;
		}
	}
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort(CompareByX);
	inrange_mov_list.sort(CompareByY);

	current_turn = Entity::SELECT_ATTACK;

}

void CharacterSapphire::Attack()
{
	objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	if ((position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
		|| (position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
			&& position.second < App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second))
	{
		CurrentMovement(ATTACK_LEFT_FRONT);
	}
	else if (position.first < App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
			&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_RIGHT_FRONT);
	}
	else if (position.first > App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_LEFT_BACK);
	}
	else if (position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second > App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_RIGHT_BACK);
	}
	else {
		CurrentMovement(ATTACK_LEFT_FRONT);
	}
	
}

void CharacterSapphire::CurrentMovement(MOVEMENT _movement) {

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
		position.first -= 2;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first += 2;
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
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &hability_1_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &hability_1_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &hability_1_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQU�
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &hability_1_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &hability_2_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &hability_2_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &hability_2_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &hability_2_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT_FRONT:
		current_movement = DEFEND_LEFT_FRONT;
		current_animation = &defend_left_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		current_movement = DEFEND_RIGHT_FRONT;
		current_animation = &defend_right_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT_BACK:
		current_movement = DEFEND_LEFT_BACK;
		current_animation = &defend_left_back;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_BACK:
		current_movement = DEFEND_RIGHT_BACK;
		current_animation = &defend_right_back;
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


void CharacterSapphire::InputSelectMove() {
	// FOR CHARACTERS LIKE ALFIL

	/*if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
	if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second >(*possible_mov_2).second)
					
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
	if ((*possible_mov).first > (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)
			
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
	if ((*possible_mov).first > (*possible_mov_2).first && (*possible_mov).second > (*possible_mov_2).second)
					
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
	if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)		
	}*/

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN ) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1) {
						if ((*possible_mov).first > (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second }) 
								&& !App->pathfinding->IsAttackable({ (*possible_mov_2).first , (*possible_mov_2).second }))
							{
								Cap -= 1;
								if (Cap < 0)
								{
									Cap = possible_mov_list.size() - 1;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1) {
						if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second }) 
								&& !App->pathfinding->IsAttackable({ (*possible_mov_2).first , (*possible_mov_2).second }))
							{
								Cap += 1;
								if (Cap >= possible_mov_list.size())
								{
									Cap = 0;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN ) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second }) 
								&& !App->pathfinding->IsAttackable({ (*possible_mov_2).first , (*possible_mov_2).second }))
							{
								Cap += sqrt(possible_mov_list.size());
								if (Cap >= possible_mov_list.size())
								{
									Cap = 0;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second > (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second }) 
								&& !App->pathfinding->IsAttackable({ (*possible_mov_2).first , (*possible_mov_2).second }))
							{
								Cap -= sqrt(possible_mov_list.size());
								if (Cap < 0)
								{
									Cap = possible_mov_list.size() - 1;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
}

void CharacterSapphire::InputSelectAttack() {
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1) {
						if ((*possible_mov).first > (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= 1;
								if (Cap < 0)
								{
									Cap = possible_mov_list.size() - 1;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1) {
						if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += 1;
								if (Cap >= possible_mov_list.size())
								{
									Cap = 0;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += sqrt(possible_mov_list.size());
								if (Cap >= possible_mov_list.size())
								{
									Cap = 0;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second > (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= sqrt(possible_mov_list.size());
								if (Cap < 0)
								{
									Cap = possible_mov_list.size() - 1;
								}
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
}