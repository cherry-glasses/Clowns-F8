#include "Application.h"
#include "CharacterSapphire.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"


CharacterSapphire::CharacterSapphire(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT_FRONT);
	current = current_animation->GetCurrentFrame(1);
}

CharacterSapphire::~CharacterSapphire() {

}

void CharacterSapphire::SearchWalk() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.PMove, tiles_range_attk);
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

	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;

}

void CharacterSapphire::SearchAttack() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAtk, tiles_range_attk);
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
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ATTACK;

}

void CharacterSapphire::SearchAbility_1() {

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
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_1;

}

void CharacterSapphire::SearchAbility_2() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_2, tiles_range_attk);
	int x = pos.first - current_stats.RangeAbility_2;
	int y = pos.second - current_stats.RangeAbility_2;
	for (int i = 0; i < ((current_stats.RangeAbility_2 * 2) + 1) * ((current_stats.RangeAbility_2 * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.RangeAbility_2) {
			x = pos.first - current_stats.RangeAbility_2;
			++y;
		}
		if (y > pos.second + current_stats.RangeAbility_2) {
			y = pos.second - current_stats.RangeAbility_2;
		}
	}
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_2;

}

void CharacterSapphire::SearchAbility_3() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_3, tiles_range_attk);
	int x = pos.first - current_stats.RangeAbility_3;
	int y = pos.second - current_stats.RangeAbility_3;
	for (int i = 0; i < ((current_stats.RangeAbility_3 * 2) + 1) * ((current_stats.RangeAbility_3 * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.RangeAbility_3) {
			x = pos.first - current_stats.RangeAbility_3;
			++y;
		}
		if (y > pos.second + current_stats.RangeAbility_3) {
			y = pos.second - current_stats.RangeAbility_3;
		}
	}
	for (int i = 0; i < tiles_range_attk; i++) {
		inrange_mov_list.push_back({ range[i].first, range[i].second });
	}

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_3;

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
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_LEFT:
		current_movement = ATTACK_LEFT;
		current_animation = &attack_left;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_RIGHT:
		current_movement = ATTACK_RIGHT;
		current_animation = &attack_right;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_FRONT:
		current_movement = ATTACK_FRONT;
		current_animation = &attack_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ATTACK_BACK:
		current_movement = ATTACK_BACK;
		current_animation = &attack_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, true);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT_BACK:
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_LEFT:
		current_movement = ABILITY_1_LEFT;
		current_animation = &ability_1_left;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_RIGHT:
		current_movement = ABILITY_1_RIGHT;
		current_animation = &ability_1_right;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		current_animation = &ability_1_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_1_BACK:
		current_movement = ABILITY_1_BACK;
		current_animation = &ability_1_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_1 + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &ability_2_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &ability_2_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &ability_2_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &ability_2_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_LEFT:
		current_movement = ABILITY_2_LEFT;
		current_animation = &ability_2_left;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_RIGHT:
		current_movement = ABILITY_2_RIGHT;
		current_animation = &ability_2_right;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_FRONT:
		current_movement = ABILITY_2_FRONT;
		current_animation = &ability_2_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_2_BACK:
		current_movement = ABILITY_2_BACK;
		current_animation = &ability_2_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_2 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT_FRONT:
		current_movement = ABILITY_3_LEFT_FRONT;
		current_animation = &ability_3_left_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT_FRONT:
		current_movement = ABILITY_3_RIGHT_FRONT;
		current_animation = &ability_3_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT_BACK:
		current_movement = ABILITY_3_LEFT_BACK;
		current_animation = &ability_3_left_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT_BACK:
		current_movement = ABILITY_3_RIGHT_BACK;
		current_animation = &ability_3_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_LEFT:
		current_movement = ABILITY_3_LEFT;
		current_animation = &ability_3_left;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT:
		current_movement = ABILITY_3_RIGHT;
		current_animation = &ability_3_right;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_FRONT:
		current_movement = ABILITY_3_FRONT;
		current_animation = &ability_3_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_BACK:
		current_movement = ABILITY_3_BACK;
		current_animation = &ability_3_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Ability_3 + current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE, false);
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
	case Entity::DEAD_LEFT:
		current_movement = DEAD_LEFT;
		current_animation = &dead_left;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT:
		current_movement = DEAD_RIGHT;
		current_animation = &dead_right;
		current_state = DEATH;
		break;
	case Entity::DEAD_FRONT:
		current_movement = DEAD_FRONT;
		current_animation = &dead_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_BACK:
		current_movement = DEAD_BACK;
		current_animation = &dead_back;
		current_state = DEATH;
		break;
	default:
		break;
	}
}

void CharacterSapphire::InputSelectMove() {

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
						if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
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
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)
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
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second > (*possible_mov_2).second)
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

void CharacterSapphire::InputSelectAttack() {
	
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