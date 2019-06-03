#include "Application.h"
#include "CharacterIris.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include "ModuleSceneManager.h"


CharacterIris::CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT_FRONT);
	current = current_animation->GetCurrentFrame(1);
	std::pair<int, int> pos = { 12,12 };
	int x = 0;
	int y = 0;
	for (int i = 0; i < (25 * 25); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > 24) {
			x = 0;
			++y;
		}
		if (y > 24) {
			y = 0;
		}
	}

	attacks_names.Attack_name = App->scene_manager->language->attack_iris;
	attacks_names.Ability_1_name = App->scene_manager->language->ability1_iris;
	attacks_names.Ability_2_name = App->scene_manager->language->ability2_iris;
	attacks_names.Ability_3_name = App->scene_manager->language->ability3_iris;
}

CharacterIris::~CharacterIris() {

}

void CharacterIris::SearchWalk() {

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second += 2;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 2;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 2;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second -= 2;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second -= 2;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 2;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 2;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second += 2;
		inrange_mov_list.push_back(tmp);
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterIris::SearchAttack() {

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

void CharacterIris::SearchAbility_1() {

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

void CharacterIris::SearchAbility_2() {

	inrange_mov_list.push_back({ App->map->WorldToMap(position.first, position.second) });
	current_turn = Entity::SELECT_ABILITY_2;
}

void CharacterIris::SearchAbility_3() {

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

	current_turn = Entity::SELECT_ABILITY_3;

}

void CharacterIris::CurrentMovement(MOVEMENT _movement) {

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
	case Entity::WALK_LEFT_FRONT:
		current_movement = WALK_LEFT_FRONT;
		current_animation = &walk_left_front;
		position.first -= 8;
		position.second += 4;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 8;
		position.second += 4;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_left_back;
		position.first -= 8;
		position.second -= 4;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 8;
		position.second -= 4;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		position.first -= 8;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first += 8;
		break;
	case Entity::WALK_FRONT:
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		position.second += 4;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		position.second -= 4;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_BACK:
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_RIGHT:
		current_movement = ABILITY_2_RIGHT;
		current_animation = &ability_2_right_front;
		if (current_animation->isDone()) {
			objective_position.clear();
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT_FRONT:
		current_movement = ABILITY_3_LEFT_FRONT;
		current_animation = &ability_3_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT_FRONT:
		current_movement = ABILITY_3_RIGHT_FRONT;
		current_animation = &ability_3_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT_BACK:
		current_movement = ABILITY_3_LEFT_BACK;
		current_animation = &ability_3_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT_BACK:
		current_movement = ABILITY_3_RIGHT_BACK;
		current_animation = &ability_3_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS, false);
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
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_left_front;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_left_back;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
		break;
	case Entity::DEAD_RIGHT_BACK:
		current_movement = DEAD_RIGHT_BACK;
		current_animation = &dead_right_back;
		if (current_animation->isDone()) {
			current_state = DEATH;
		}
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

