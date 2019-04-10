#include "Log.h"
#include "Application.h"
#include "CharacterSapphire.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
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

bool CharacterSapphire::PreUpdate() {
	
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
	else
	{
		current_turn == END_TURN;
	}

	return true;
}
bool CharacterSapphire::Update(float _dt) {

	if (current_turn == NONE)
	{
	}
	else if (current_turn == Entity::SELECT)
	{
		SelectWalk();
	}
	else if (current_turn == Entity::MOVE)
	{
		Walk(App->pathfinding->GetLastPath());
	}
	else if (current_turn == ATTACK)
	{
		Attack(App->pathfinding->GetLastPath());
	}

	return true;
}

bool CharacterSapphire::PostUpdate() {
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f);
	}

	return true;
}

bool CharacterSapphire::Load(pugi::xml_node& node) {
	return true;
}

bool CharacterSapphire::Save(pugi::xml_node& node) const {
	return true;
}

void CharacterSapphire::SearchWalk() {

	std::pair<int, int> tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	possible_mov[0] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second += 1;
	possible_mov[1] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second += 1;
	possible_mov[2] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 1;
	possible_mov[3] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	possible_mov[4] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second -= 1;
	possible_mov[5] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second -= 1;
	possible_mov[6] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second -= 1;
	possible_mov[7] = tmp;

	tmp.first = NULL;
	tmp.second = NULL;

	current_turn = Entity::SELECT;

}

void CharacterSapphire::SelectWalk() {

	while (!App->pathfinding->IsWalkable(possible_mov[Cap])) {
		Cap--;
		if (Cap < 0)
			Cap = 7;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		do
		{
			Cap--;
			if (Cap < 0)
				Cap = 7;
		} while (!App->pathfinding->IsWalkable(possible_mov[Cap]));
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		do
		{
			Cap++;
			if (Cap > 7)
				Cap = 0;
		} while (!App->pathfinding->IsWalkable(possible_mov[Cap]));

	}

	for (int i = 0; i < 8; i++) {
		possible_mov_map[i] = App->map->MapToWorld((possible_mov[i].first), possible_mov[i].second);

		if (i != Cap) {
			if (App->pathfinding->IsWalkable(possible_mov[i]))
				App->render->Blit(debug_texture, possible_mov_map[i].first, possible_mov_map[i].second, &debug_blue);
			else
				App->render->Blit(debug_texture, possible_mov_map[i].first, possible_mov_map[i].second, &debug_red);
		}
	}
	App->render->Blit(debug_texture, possible_mov_map[Cap].first, possible_mov_map[Cap].second, &debug_green);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		current_turn = Entity::MOVE;
	}
}

void CharacterSapphire::Walk(const std::vector<std::pair<int, int>> *_path)
{
	objective_position.push_back(App->map->MapToWorld((possible_mov[Cap].first), possible_mov[Cap].second));
	
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
	else if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		current_turn = SEARCH_ATTACK;
	}
	else {
		current_turn = SEARCH_ATTACK;
	}

	if (objective_position.back().first == position.first || objective_position.back().second == position.second) {
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
		current_turn = SEARCH_ATTACK;
	}
}

void CharacterSapphire::SearchAttack() {

	std::pair<int, int> tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	possible_att[0].first = tmp.first + 1;
	possible_att[0].second = tmp.second;

	possible_att[1].first = tmp.first;
	possible_att[1].second = tmp.second + 1;

	possible_att[2].first = tmp.first - 1;
	possible_att[2].second = tmp.second;

	possible_att[3].first = tmp.first;
	possible_att[3].second = tmp.second - 1;

	tmp.first = NULL;
	tmp.second = NULL;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		current_turn = ATTACK;
	}
}

void CharacterSapphire::Attack(const std::vector<std::pair<int, int>> *_path)
{
	current_turn = END_TURN;
}

void CharacterSapphire::Die()
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
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &hability_1_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &hability_1_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &hability_1_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQU�
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &hability_1_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &hability_2_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &hability_2_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &hability_2_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &hability_2_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
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
