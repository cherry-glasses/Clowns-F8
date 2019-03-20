#include "Log.h"
#include "Application.h"
#include "Bulldog.h"
#include "ModulePathfinding.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Bulldog::Bulldog() : Enemy()
{

}
Bulldog::~Bulldog()
{
}

bool Bulldog::Awake(pugi::xml_node & _config)
{
	pugi::xml_node config = _config.child("Bulldog");

	string_texture = config.child("texture").child_value();

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

bool Bulldog::Start(uint i)
{
	entity_texture = App->textures->Load(string_texture.c_str());
	position.first = 300;
	position.second = 100;

	return true;
}

bool Bulldog::PreUpdate()
{
	bool ret = true;
	if (current_movement == IDLE) {
		if (moving)
		{
			std::pair<int, int> pos = { (int)position.first, (int)position.second };
			App->pathfinding->CreatePath(App->map->WorldToMap(pos.first, pos.second), App->map->WorldToMap(pos.first + 1, pos.second));
			App->pathfinding->CreatePath(App->map->WorldToMap(pos.first, pos.second), App->map->WorldToMap(pos.first - 1, pos.second));
			App->pathfinding->CreatePath(App->map->WorldToMap(pos.first, pos.second), App->map->WorldToMap(pos.first, pos.second + 1));
			App->pathfinding->CreatePath(App->map->WorldToMap(pos.first, pos.second), App->map->WorldToMap(pos.first, pos.second - 1));
			//Walk(App->pathfinding->GetLastPath());
		}
		else {
			current_movement = IDLE;
			current_animation = &idle;
		}
	}

	return ret;
}

bool Bulldog::Update(float dt)
{
	bool ret = true;

	if (current_movement == IDLE)
	{
		current_animation = &idle;
	}
	else if (current_movement == LEFTUP)
	{
		current_animation = &walk;
	}
	else if (current_movement == LEFTDOWN)
	{
		current_animation = &walk;
	}
	else if (current_movement == RIGHTUP)
	{
		current_animation = &walk;
	}
	else if (current_movement == RIGHTDOWN)
	{
		current_animation = &walk;
	}

	return true;
}

bool Bulldog::PostUpdate()
{

	App->render->Blit(entity_texture, position.first, position.second, &current, 1.0f);

	return true;
}

bool Bulldog::CleanUp()
{
	App->textures->UnLoad(entity_texture);
	entity_texture = nullptr;
	return true;
}

bool Bulldog::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Bulldog::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;


	return ret;
}


bool Bulldog::LoadAnimation(pugi::xml_node &node, Animation &anim) {

	for (; node; node = node.next_sibling("frame")) {
		SDL_Rect frame_rect;
		frame_rect.x = node.attribute("x").as_int();
		frame_rect.y = node.attribute("y").as_int();
		frame_rect.w = node.attribute("width").as_int();
		frame_rect.h = node.attribute("height").as_int();
		anim.PushBack(frame_rect);
	}
	return true;
}


void Bulldog::Walk(const std::list<std::pair<int, int>> *_path)
{
	if (_path->size() > 0)
	{
		/*App->render->Blit(App->scene->debug_tex, pos.x, pos.y);*/



	}
}
