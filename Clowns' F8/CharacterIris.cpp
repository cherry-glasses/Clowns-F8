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

	/*Stats::Hp = { config.child("Hp").value.as_int()};
	Stats::Mp = { config.child("Mp").value.as_int() };
	Stats::Mana = { config.child("Mana").value.as_int() };
	Stats::Cp = { config.child("Cp").value.as_int() };
	Stats::AtkF = { config.child("AtkF").value.as_int() };
	Stats::AtkS = { config.child("AtkS").value.as_int() };
	Stats::DefF = { config.child("DefF").value.as_int() };
	Stats::DefS = { config.child("DefS").value.as_int() };
	Stats::Crit_hit = { config.child("Crit_hit").value.as_int() };*/

	/*LoadAnimation(config.child("animations").child("idle").child("frame"), idle);
	idle.speed = config.child("animations").child("idle").attribute("speed").as_float();
	idle.loop = config.child("animations").child("idle").attribute("loop").as_bool(true);*/

	/*LoadAnimation(config.child("animations").child("walk").child("frame"), walk);
	walk.speed = config.child("animations").child("walk").attribute("speed").as_float();
	walk.loop = config.child("animations").child("walk").attribute("loop").as_bool(true);

	LoadAnimation(config.child("animations").child("die").child("frame"), dead);
	dead.speed = config.child("animations").child("die").attribute("speed").as_float();
	dead.speed = config.child("animations").child("die").attribute("loop").as_bool(false);*/

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
		switch (a) {
		case 0:
			mov_x_first = 0;
			mov_y_first = 1;
			mov_x_last = 1;
			mov_y_last = 0;
			break;
		case 1:
			mov_x_first = 1;
			mov_y_first = 0;
			mov_x_last = 0;
			mov_y_last = 1;
			break;
		case 2:
			mov_x_first = 1;
			mov_y_first = 0;
			mov_x_last = 0;
			mov_y_last = -1;
			break;
		case 3:
			mov_x_first = 0;
			mov_y_first = -1;
			mov_x_last = 1;
			mov_y_last = 0;
			break;
		case 4:
			mov_x_first = 0;
			mov_y_first = -1;
			mov_x_last = -1;
			mov_y_last = 0;
			break;
		case 5:
			mov_x_first = -1;
			mov_y_first = 0;
			mov_x_last = 0;
			mov_y_last = -1;
			break;
		case 6:
			mov_x_first = -1;
			mov_y_first = 0;
			mov_x_last = 0;
			mov_y_last = 1;
			break;
		case 7:
			mov_x_first = 0;
			mov_y_first = 1;
			mov_x_last = -1;
			mov_y_last = 0;
			break;
		}
	}
	
	return true;
}

bool CharacterIris::PostUpdate() {

	App->render->Blit(Iris_tex, position.first, position.second, &current, 1.0f); //Bug: Problem with the blit a close the game. Render
	
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



