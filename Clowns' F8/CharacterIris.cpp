#include "Log.h"
#include "Application.h"
#include "CharacterIris.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include <time.h>




CharacterIris::CharacterIris() {

}

CharacterIris::~CharacterIris() {

}

bool CharacterIris::Awake(pugi::xml_node & _config) {

	

	return true;
	
}

bool CharacterIris::Start() {
	
	return true;
}

bool CharacterIris::PreUpdate() {

	if (!flag) {
		Iris_tex = App->textures->Load("Assets/Sprites/Main_Characters/Iris_Spritesheet.png");
		SDL_Rect rect;
		rect.h = 65;
		rect.w = 40;
		rect.x = 0;
		rect.y = 0;
		idle.PushBack(rect);
		idle.speed = 0;
		position = App->map->MapToWorld(3,3);
		std::pair<int, int> coso = App->map->WorldToMap((int)position.first, (int)position.second);
		position = App->map->MapToWorld(coso.first, coso.second);
		coso = App->map->WorldToMap((int)position.first, (int)position.second);
		current_animation = &idle;
		current = current_animation->GetCurrentFrame();
		flag = true;
		srand(time(NULL));
	}
	
	
	return true;
}
bool CharacterIris::Update(float _dt) {
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Wheremove();
		int a = (rand() % 8);
		position = App->map->MapToWorld((possible_mov[a].first), possible_mov[a].second);
		
		std::pair<int, int> coso = App->map->WorldToMap((int)position.first, (int)position.second);
		LOG("AHHHHH, %i %i", coso.first, coso.second);
		LOG("AHHHHH, %i", a);
	}
	
	return true;
}

bool CharacterIris::PostUpdate() {

	App->render->Blit(Iris_tex, position.first, position.second, &current, 1.0f);
	
	return true;
}

bool CharacterIris::Load(pugi::xml_node& node) {
	return true;
}

bool CharacterIris::Save(pugi::xml_node& node) const{
	return true;
}

void CharacterIris::move() {

}

void CharacterIris::Wheremove() {

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second += 2;  //first i get where the hell i can be
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)  //then i check if this var is in the map (and we will need to do the same if it's walkeable or not)
		possible_mov[0] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second += 1;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[1] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 2;
	tmp.second -= 1;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width) 
		possible_mov[2] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	tmp.second -= 2;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[3] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second -= 2;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[4] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second -= 1;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[5] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 2;
	tmp.second += 1;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[6] = tmp;

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	tmp.second += 2;
	if (tmp.first >= 0 && tmp.first <= App->map->data.height && tmp.second >= 0 && tmp.second <= App->map->data.width)
		possible_mov[7] = tmp;


	for (int i = 0; i < 7; i++) {
		if (possible_mov[i].first == 0 && possible_mov[i].second == 0) {
			possible_mov[i] = App->map->WorldToMap((int)position.first, (int)position.second);
		}
	}
}

bool CharacterIris::CleanUp() {

	App->textures->UnLoad(Iris_tex);
	Iris_tex = nullptr;

	return true;
}



