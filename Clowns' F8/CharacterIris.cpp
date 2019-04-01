#include "Log.h"
#include "Application.h"
#include "CharacterIris.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include <time.h>




CharacterIris::CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
}

CharacterIris::~CharacterIris() {

}

bool CharacterIris::PreUpdate() {

	if (!flag) {
		//entity_texture = App->textures->Load("Assets/Sprites/Main_Characters/Iris_Spritesheet.png");
		SDL_Rect rect;
		rect.h = 65;
		rect.w = 40;
		rect.x = 0;
		rect.y = 0;
		idle.PushBack(rect);
		idle.speed = 0;
		position = App->map->MapToWorld(30,3);
		std::pair<int, int> coso = App->map->WorldToMap((int)position.first, (int)position.second);
		position = App->map->MapToWorld(coso.first, coso.second);
		coso = App->map->WorldToMap((int)position.first, (int)position.second);
		current_animation = &idle;
		current = current_animation->GetCurrentFrame();
		flag = true;
		srand(time(NULL));
		debug_texture = App->textures->Load("Assets/Maps/meta.png");
		debug_1.x = 0;
		debug_1.y = 0;
		debug_1.w = 64;
		debug_1.h = 64;
		debug_2.x = 64;
		debug_2.y = 0;
		debug_2.w = 64;
		debug_2.h = 64;
		
	}
	
	
	return true;
}
bool CharacterIris::Update(float _dt) {
	
	//if we press start the cycle iniciate
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
				Cap--;
				if (Cap < 0)
					Cap = 7;
			}
			else if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
				Cap++;
				if (Cap > 7)
					Cap = 0;
			}

			for (int i = 0; i < 8; i++) {
				int j = 0;
				possible_mov_map[i] = App->map->MapToWorld((possible_mov[i].first), possible_mov[i].second);
				if (i != Cap) 
					App->render->Blit(debug_texture, possible_mov_map[i].first, possible_mov_map[i].second, &debug_1);
				
			}
			App->render->Blit(debug_texture, possible_mov_map[Cap].first, possible_mov_map[Cap].second, &debug_2);
			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
				Def = Move_Steps::MOVE;
			break;

		//Now we need to move the player to that position
		case Move_Steps::MOVE:
			position = App->map->MapToWorld((possible_mov[Cap].first), possible_mov[Cap].second);
			Def = Move_Steps::IDLE;
			break;
		}


	
		
		
	
	
	return true;
}

bool CharacterIris::PostUpdate() {
	if (entity_texture != nullptr) {
		App->render->Blit(entity_texture, position.first, position.second, &current, 1.0f); //Bug: Problem with the blit a close the game. Render
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
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)  //then i check if this var is in the map (and we will need to do the same if it's walkeable or not)
		possible_mov[0] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second += 1;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[1] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second -= 1;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width) 
		possible_mov[2] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second -= 2;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[3] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second -= 2;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[4] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second -= 1;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[5] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second += 1;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[6] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 2;
	//if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[7] = tmp;


	for (int i = 0; i < 7; i++) {
		if (possible_mov[i].first == 0 && possible_mov[i].second == 0) {
			possible_mov[i] = App->map->WorldToMap((int)position.first, (int)position.second);
		}
	}

	tmp.first = NULL;
	tmp.second = NULL;
}




