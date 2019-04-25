#include "Application.h"
#include "CharacterStorm.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"


CharacterStorm::CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT_FRONT);
	current = current_animation->GetCurrentFrame();
}

CharacterStorm::~CharacterStorm() {

}

void CharacterStorm::SearchWalk() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	int x = pos.first - current_stats.PMove;
	int y = pos.second - current_stats.PMove;
	for (int i = 0; i < ((current_stats.PMove * 2) + 1) * ((current_stats.PMove * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.PMove) {
			x = pos.first - current_stats.PMove;
			++y;
		}
		if (y > pos.second + current_stats.PMove) {
			y = pos.second - current_stats.PMove;
		}
	}

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterStorm::SearchAttack() {
	objective_position.clear();
	inrange_mov_list.clear();
	possible_mov_list.clear();
	possible_map.clear();
	Cap = -1;

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	int x = pos.first - current_stats.RangeAtk;
	int y = pos.second - current_stats.RangeAtk;
	for (int i = 0; i < ((current_stats.RangeAtk * 2) + 1) * ((current_stats.RangeAtk * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.RangeAtk) {
			x = pos.first - current_stats.RangeAtk;
			++y;
		}
		if (y > pos.second + current_stats.RangeAtk) {
			y = pos.second - current_stats.RangeAtk;
		}
	}

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ATTACK;
}

void CharacterStorm::SearchAbility_1() {
	objective_position.clear();
	inrange_mov_list.clear();
	possible_mov_list.clear();
	possible_map.clear();
	Cap = -1;

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_1, tiles_range_attk);
	int x = pos.first - current_stats.RangeAbility_1;
	int y = pos.second - current_stats.RangeAbility_1;
	for (int i = 0; i < ((current_stats.RangeAbility_1 * 2) + 1) * ((current_stats.RangeAbility_1 * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.RangeAbility_1) {
			x = pos.first - current_stats.RangeAbility_1;
			++y;
		}
		if (y > pos.second + current_stats.RangeAbility_1) {
			y = pos.second - current_stats.RangeAbility_1;
		}
	}
	
	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_1;

}

void CharacterStorm::CurrentMovement(MOVEMENT _movement) {

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
		position.first -= 4;
		position.second += 2;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 4;
		position.second += 2;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_left_back;
		position.first -= 4;
		position.second -= 2;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 4;
		position.second -= 2;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_BACK:
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			current_turn = END_TURN;
		}
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

void CharacterStorm::InputSelectMove() {

	if (App->input->Left()) {

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
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= 1;
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
	else if (App->input->Right()) {

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
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += 1;
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
	else if (App->input->Down()) {

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
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += sqrt(possible_mov_list.size());
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
	else if (App->input->Up()) {

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
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= sqrt(possible_mov_list.size());
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

void CharacterStorm::InputSelectAttack() {

	if (App->input->Left()) {

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
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= 1;
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
	else if (App->input->Right()) {

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
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += 1;
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
	else if (App->input->Down()) {

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
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += sqrt(possible_mov_list.size());
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
	else if (App->input->Up()) {

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
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= sqrt(possible_mov_list.size());
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

