#include "Application.h"
#include "CharacterGeorgeB.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"


CharacterGeorgeB::CharacterGeorgeB(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT);
	current = current_animation->GetCurrentFrame();
}

CharacterGeorgeB::~CharacterGeorgeB() {

}

void CharacterGeorgeB::SearchWalk() {

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
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterGeorgeB::SearchAttack() {
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
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ATTACK;
}

void CharacterGeorgeB::CurrentMovement(MOVEMENT _movement) {

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
	case Entity::ATTACK_LEFT:
		current_movement = ATTACK_LEFT;
		current_animation = &attack_left;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT:
		current_movement = ATTACK_RIGHT;
		current_animation = &attack_right;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_FRONT:
		current_movement = ATTACK_FRONT;
		current_animation = &attack_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_BACK:
		current_movement = ATTACK_BACK;
		current_animation = &attack_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT:
		current_movement = DEFEND_LEFT;
		current_animation = &defend_left;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT:
		current_movement = DEFEND_RIGHT;
		current_animation = &defend_right;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_FRONT:
		current_movement = DEFEND_FRONT;
		current_animation = &defend_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_BACK:
		current_movement = DEFEND_BACK;
		current_animation = &defend_back;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT:
		current_movement = DEAD_LEFT;
		current_animation = &dead_left;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_RIGHT:
		current_movement = DEAD_RIGHT;
		current_animation = &dead_right;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_FRONT:
		current_movement = DEAD_FRONT;
		current_animation = &dead_front;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_BACK:
		current_movement = DEAD_BACK;
		current_animation = &dead_back;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	default:
		break;
	}
}

void CharacterGeorgeB::InputSelectMove() {

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
					if (j == Cap + (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) - 1);
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
					if (j == Cap - (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) - 1);
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
					if (j == Cap + (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) + 1);
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
					if (j == Cap - (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) + 1);
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

void CharacterGeorgeB::InputSelectAttack() {

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
					if (j == Cap + (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) - 1);
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
					if (j == Cap - (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) - 1);
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
					if (j == Cap + (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) + 1);
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
					if (j == Cap - (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) + 1);
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
