#include "Log.h"
#include "Application.h"
#include "CharacterStorm.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include <time.h>


CharacterStorm::CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	current_movement = IDLE_RIGHT_FRONT;
	current_animation = &idle_right_front;
	current = current_animation->GetCurrentFrame();

}

CharacterStorm::~CharacterStorm() {

}

bool CharacterStorm::PreUpdate() {

	bool ret = true;

	if (current_state == ALIVE) {

		if (current_stats.Hp <= 0)
		{
			Die();
		}

		if (current_turn == SEARCH_MOVE)
		{
			SearchWalk();
		}
		

	}
	else
	{
		current_turn == END_TURN;
	}

	return ret;

}
bool CharacterStorm::Update(float _dt) {


	switch (current_turn)
	{
	case Entity::SELECT_MOVE:
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
			Cap_2 += 1;
			if (Cap_2 > 4) {
				Cap_2 = 1;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
			Cap_2 -= 1;
			if (Cap_2 < 1) {
				Cap_2 = 4;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

			Cap--;
			if (Cap < 0)
				Cap = (current_stats.Agi/2) - 1;

		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

			Cap++;
			if (Cap > ((current_stats.Agi/2) - 1))
				Cap = 0;
		}

		for (int i = 0; i < current_stats.Agi / 2; i++) {
			App->render->Blit(debug_texture, movment_1[i].first, movment_1[i].second, &debug_blue);
			App->render->Blit(debug_texture, movment_2[i].first, movment_2[i].second, &debug_blue);
			App->render->Blit(debug_texture, movment_3[i].first, movment_3[i].second, &debug_blue);
			App->render->Blit(debug_texture, movment_4[i].first, movment_4[i].second, &debug_blue);
		}

		switch (Cap_2) {
		case 1:
			App->render->Blit(debug_texture, movment_1[Cap].first, movment_1[Cap].second, &debug_green);
			break;
		case 2:
			App->render->Blit(debug_texture, movment_2[Cap].first, movment_2[Cap].second, &debug_green);
			break;
		case 3:
			App->render->Blit(debug_texture, movment_3[Cap].first, movment_3[Cap].second, &debug_green);
			break;
		case 4:
			App->render->Blit(debug_texture, movment_4[Cap].first, movment_4[Cap].second, &debug_green);
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			current_turn = Entity::MOVE;
		}


		break;

	case Entity::SEARCH_ATTACK:
		switch (Attk) {
		case Attack_Steps::IDLE_A:
			break;
		case Attack_Steps::SEARCH_A:
			SearchAttack();
			Attk = Attack_Steps::SELECT_A;
			Cap = 0;
			break;
		case Attack_Steps::SELECT_A:
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				Cap--;
				Cap_2 = -1;
				if (Cap < 0)
					Cap = 3;
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				Cap++;
				Cap_2 = 1;
				if (Cap > 3)
					Cap = 0;
			}
			for (int i = 0; i < 4; i++) {
				possible_att_map[i] = App->map->MapToWorld((possible_att[i].first), possible_att[i].second);

				if (i != Cap) {

					App->render->Blit(debug_texture, possible_att_map[i].first, possible_att_map[i].second, &debug_green);

				}

			}
			App->render->Blit(debug_texture, possible_att_map[Cap].first, possible_att_map[Cap].second, &debug_red);
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				objective_position.push_back(possible_att_map[Cap]);
				Attk = Attack_Steps::ATTACK_A;
			}

			break;
		case Attack_Steps::ATTACK_A:
			App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
			current_turn = Entity::END_TURN;
			Attk = Attack_Steps::IDLE_A;
		}
		

		break;
	
	case Entity::MOVE:
		switch (Cap_2) {
		case 1:
			position = movment_1[Cap];
			break;
		case 2:
			position = movment_2[Cap];
			break;
		case 3:
			position = movment_3[Cap];
			break;
		case 4:
			position = movment_4[Cap];
			break;
		}

		current_turn = SELECT_ACTION;
		Attk = Attack_Steps::SEARCH_A;

		break;
	}
	

	return true;
}

bool CharacterStorm::PostUpdate() {
	if (entity_texture != nullptr && current_state != DEATH)
	{
		App->render->Blit(entity_texture, position.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f);
	}

	return true;
}

bool CharacterStorm::Load(pugi::xml_node& node) {
	return true;
}

bool CharacterStorm::Save(pugi::xml_node& node) const {
	return true;
}

void CharacterStorm::SearchWalk() {

	// Tower movment.
	movment_1 = new std::pair<int, int>[current_stats.Agi/2];
	movment_2 = new std::pair<int, int>[current_stats.Agi/2];
	movment_3 = new std::pair<int, int>[current_stats.Agi/2];
	movment_4 = new std::pair<int, int>[current_stats.Agi/2];


	std::pair<int,int> tmp = App->map->WorldToMap(position.first, position.second);
	// x axis
	for (int i = 0; i < current_stats.Agi / 2;i++){
		movment_1[i].first = tmp.first + i + 1;
		movment_1[i].second = tmp.second;
		movment_1[i] = App->map->MapToWorld(movment_1[i].first, movment_1[i].second);
	}

	for (int i = 0; i < current_stats.Agi / 2; i++) {
		movment_3[i].first = tmp.first - i - 1;
		movment_3[i].second = tmp.second;
		movment_3[i] = App->map->MapToWorld(movment_3[i].first, movment_3[i].second);
	}

	// y axis
	for (int i = 0; i < current_stats.Agi / 2; i++) {
		movment_2[i].first = tmp.first;
		movment_2[i].second = tmp.second + i + 1;
		movment_2[i] = App->map->MapToWorld(movment_2[i].first, movment_2[i].second);
	}

	for (int i = 0; i < current_stats.Agi / 2; i++) {
		movment_4[i].first = tmp.first;
		movment_4[i].second = tmp.second - i - 1;
		movment_4[i] = App->map->MapToWorld(movment_4[i].first, movment_4[i].second);
	}

	tmp.first = NULL;
	tmp.second = NULL;
	current_turn = Entity::SELECT_MOVE;
}

void CharacterStorm::SearchAttack() {
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
}

void CharacterStorm::Die() {
	current_state = DEATH;
}


