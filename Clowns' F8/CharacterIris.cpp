#include "Log.h"
#include "Application.h"
#include "CharacterIris.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include <time.h>




CharacterIris::CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	current_movement = IDLE_RIGHT_FRONT;
	current_animation = &idle_right_front;
	current = current_animation->GetCurrentFrame();

}

CharacterIris::~CharacterIris() {

}

bool CharacterIris::PreUpdate() {

	if (!flag) {
		
		flag = true;
		srand(time(NULL));
		
	}
	
	
	return true;
}
bool CharacterIris::Update(float _dt) {
	
	//if we press start the cycle iniciate
	/*if (current_turn == SEARCH_MOVE && Def == Move_Steps::IDLE)
		Def = Move_Steps::SEARCH;*/



	switch (current_turn)
	{
	case Entity::SEARCH_MOVE:
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			Def = Move_Steps::SEARCH;
			current_turn = Entity::MOVE;
		}
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			Attk = Attack_Steps::SEARCH_A;
			current_turn = Entity::ATTACK;
		}
		break;
	case Entity::MOVE:

		switch (Def) {
			//first we need to know where to move and save that position in a array
		case Move_Steps::SEARCH:
			Wheremove();
			Def = Move_Steps::SELECT;
			break;

			//Now we need to 1. Highlight the possibles options 2. Highlight the current option 3. Let the player chose the option.
		case Move_Steps::SELECT:
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

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				Def = Move_Steps::MOVE;
			break;

			//Now we need to move the player to that position
		case Move_Steps::MOVE:
			position = App->map->MapToWorld((possible_mov[Cap].first), possible_mov[Cap].second);
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				current_turn = END_TURN;
				Def = Move_Steps::IDLE;
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
				current_turn = Entity::ATTACK;
				Attk = Attack_Steps::SEARCH_A;
				Def = Move_Steps::IDLE;
			}
			break;
		}
		break;

	case Entity::SEARCH_ATTACK:

		break;
	case Entity::ATTACK:
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
			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
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
	case Entity::END_TURN:
		break;
	case Entity::NONE:
		break;
	default:
		break;
	}
	//this switch will have the different steps of the cycle
	


	// Aattack
	
	
	

	return true;
}

bool CharacterIris::PostUpdate() {
	if (entity_texture != nullptr) 
	{
		App->render->Blit(entity_texture, position.first , position.second  - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f); 
	}
	
	return true;
}

bool CharacterIris::Load(pugi::xml_node& node) {
	return true;
}

bool CharacterIris::Save(pugi::xml_node& node) const{
	return true;
}

void CharacterIris::Wheremove() {

	std::pair<int, int> tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second += 2;  //first i get where the hell i can be
	
	possible_mov[0] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second += 1;
	
		possible_mov[1] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second -= 1;
	
	possible_mov[2] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second -= 2;
	
	possible_mov[3] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second -= 2;
	
	possible_mov[4] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second -= 1;
	
	possible_mov[5] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second += 1;
	
	possible_mov[6] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 2;
	
	possible_mov[7] = tmp;


	tmp.first = NULL;
	tmp.second = NULL;
}

void CharacterIris::SearchAttack() {

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


