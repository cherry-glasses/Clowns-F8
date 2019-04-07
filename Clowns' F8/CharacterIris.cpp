#include "Log.h"
#include "Application.h"
#include "CharacterIris.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
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
	if (current_turn == SEARCH_MOVE && Def == Move_Steps::IDLE)
		Def = Move_Steps::SEARCH;
	//this switch will have the different steps of the cycle
	switch (Def) {
		//first we need to know where to move and save that position in a array
		case Move_Steps::SEARCH:
			Wheremove();
			Def = Move_Steps::SELECT;
			break;

		//Now we need to 1. Highlight the possibles options 2. Highlight the current option 3. Let the player chose the option.
		case Move_Steps::SELECT:
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				Cap--;
				Cap_2 = -1;
				if (Cap < 0)
					Cap = 7;
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				Cap++;
				Cap_2 = 1;
				if (Cap > 7)
					Cap = 0;
			}

			for (int i = 0; i < 8; i++) {
				possible_mov_map[i] = App->map->MapToWorld((possible_mov[i].first), possible_mov[i].second);
				
				if (i != Cap) {
					if (App->pathfinding->IsWalkable(possible_mov[i]))
						App->render->Blit(debug_texture, possible_mov_map[i].first, possible_mov_map[i].second, &debug_blue);
					else
						App->render->Blit(debug_texture, possible_mov_map[i].first, possible_mov_map[i].second, &debug_red);
				}
				else if (i == Cap){
					if (!App->pathfinding->IsWalkable(possible_mov[i])) {
						if (Cap_2 == 1) {
							Cap++;
							if (Cap > 7)
								Cap = 0;
						}
						else {
							Cap--;
							if (Cap < 0)
								Cap = 7;
						}
						
					}
				}
				
			}
			App->render->Blit(debug_texture, possible_mov_map[Cap].first, possible_mov_map[Cap].second, &debug_green);
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				Def = Move_Steps::MOVE;
			break;

		//Now we need to move the player to that position
		case Move_Steps::MOVE:
			position = App->map->MapToWorld((possible_mov[Cap].first), possible_mov[Cap].second);
			Def = Move_Steps::IDLE;
			current_turn = END_TURN;
			break;
		}

	return true;
}

bool CharacterIris::PostUpdate() {
	if (entity_texture != nullptr) 
	{
		App->render->Blit(entity_texture, position.first , position.second  - (current.h / 3), &current_animation->GetCurrentFrame(), 1.0f); 
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




