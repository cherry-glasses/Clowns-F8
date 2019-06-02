#include "Application.h"
#include "CharacterGeorgeB.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include "ModuleSceneManager.h"



CharacterGeorgeB::CharacterGeorgeB(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT);
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

	attacks_names.Attack_name = App->scene_manager->language->attack_georgeb;
	attacks_names.Ability_1_name = App->scene_manager->language->ability1_georgeb;
	attacks_names.Ability_2_name = App->scene_manager->language->ability2_georgeb;
	attacks_names.Ability_3_name = App->scene_manager->language->ability3_georgeb;
}

CharacterGeorgeB::~CharacterGeorgeB() {

}

void CharacterGeorgeB::SearchWalk() {

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second -= 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			break;
		}
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			break;
		}
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			break;
		}
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			break;
		}
		inrange_mov_list.push_back(tmp);
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterGeorgeB::SearchAttack() {

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

void CharacterGeorgeB::SearchAbility_1() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_1, tiles_range_attk);
	
	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first += 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_1;

}

void CharacterGeorgeB::SearchAbility_2() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_2, tiles_range_attk);

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_2; i++)
	{
		tmp.first += 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_2; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_2; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_2; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_2;

}

void CharacterGeorgeB::SearchAbility_3() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_3, tiles_range_attk);
	
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_3;

}

void CharacterGeorgeB::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT:
		current_movement = IDLE_LEFT;
		current_animation = &idle_right;
		flipX = true;
		break;
	case Entity::IDLE_RIGHT:
		current_movement = IDLE_RIGHT;
		current_animation = &idle_right;
		flipX = false;
		break;
	case Entity::IDLE_FRONT:
		current_movement = IDLE_FRONT;
		current_animation = &idle_front;
		flipX = false;
		break;
	case Entity::IDLE_BACK:
		current_movement = IDLE_BACK;
		current_animation = &idle_back;
		flipX = false;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_right;
		if ((current_animation == &walk_right && current_animation->isDone()) || current_animation == &walk_right_2) {
			current_animation = &walk_right_2;
			position.first -= 4;
		}
		flipX = true;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		if ((current_animation == &walk_right && current_animation->isDone()) || current_animation == &walk_right_2) {
			current_animation = &walk_right_2;
			position.first += 4;
		}
		flipX = false;
		break;
	case Entity::WALK_FRONT:
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		if ((current_animation == &walk_front && current_animation->isDone()) || current_animation == &walk_front_2) {
			current_animation = &walk_front_2;
			position.second += 2;
		}
		flipX = false;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		if ((current_animation == &walk_back && current_animation->isDone()) || current_animation == &walk_back_2) {
			current_animation = &walk_back_2;
			position.second -= 2;
		}
		flipX = false;
		break;
	case Entity::ATTACK_LEFT:
		current_movement = ATTACK_LEFT;
		current_animation = &attack_right;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT:
		current_movement = ATTACK_RIGHT;
		current_animation = &attack_right;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_FRONT:
		current_movement = ATTACK_FRONT;
		current_animation = &attack_front;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_BACK:
		current_movement = ATTACK_BACK;
		current_animation = &attack_back;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT:
		current_movement = ABILITY_1_LEFT;
		current_animation = &ability_1_right;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT:
		current_movement = ABILITY_1_RIGHT;
		current_animation = &ability_1_right;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		current_animation = &ability_1_front;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_BACK:
		current_movement = ABILITY_1_BACK;
		current_animation = &ability_1_back;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_LEFT:
		current_movement = ABILITY_2_LEFT;
		current_animation = &ability_2_right;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_RIGHT:
		current_movement = ABILITY_2_RIGHT;
		current_animation = &ability_2_right;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_FRONT:
		current_movement = ABILITY_2_FRONT;
		current_animation = &ability_2_front;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_BACK:
		current_movement = ABILITY_2_BACK;
		current_animation = &ability_2_back;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT:
		current_movement = ABILITY_3_LEFT;
		current_animation = &ability_3_right;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT:
		current_movement = ABILITY_3_RIGHT;
		current_animation = &ability_3_right;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_FRONT:
		current_movement = ABILITY_3_FRONT;
		current_animation = &ability_3_front;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_BACK:
		current_movement = ABILITY_3_BACK;
		current_animation = &ability_3_back;
		flipX = false;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::DEFEND_LEFT:
		current_movement = DEFEND_LEFT;
		current_animation = &defend_right;
		flipX = true;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT:
		current_movement = DEFEND_RIGHT;
		current_animation = &defend_right;
		flipX = false;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_FRONT:
		current_movement = DEFEND_FRONT;
		current_animation = &defend_front;
		flipX = false;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_BACK:
		current_movement = DEFEND_BACK;
		current_animation = &defend_back;
		flipX = false;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT:
		current_movement = DEAD_LEFT;
		current_animation = &dead_right;
		flipX = true;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT:
		current_movement = DEAD_RIGHT;
		current_animation = &dead_right;
		flipX = false;
		current_state = DEATH;
		break;
	case Entity::DEAD_FRONT:
		current_movement = DEAD_FRONT;
		current_animation = &dead_front;
		flipX = false;
		current_state = DEATH;
		break;
	case Entity::DEAD_BACK:
		current_movement = DEAD_BACK;
		current_animation = &dead_back;
		flipX = false;
		current_state = DEATH;
		break;
	default:
		break;
	}
}


