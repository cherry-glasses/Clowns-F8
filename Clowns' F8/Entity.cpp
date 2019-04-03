#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Entity.h"
#include "ModuleMap.h"


Entity::Entity(ENTITY_TYPE _type, pugi::xml_node _config)
{
	type = _type;

	entity_texture = App->textures->Load(_config.child("texture").attribute("value").as_string());
	debug_texture = App->textures->Load(_config.child("debug_texture").attribute("value").as_string());
	debug_green = { _config.parent().child("debug_green").attribute("x").as_int(), _config.parent().child("debug_green").attribute("y").as_int(),
		_config.parent().child("debug_green").attribute("width").as_int(), _config.parent().child("debug_green").attribute("height").as_int() };
	debug_red = { _config.parent().child("debug_red").attribute("x").as_int(), _config.parent().child("debug_red").attribute("y").as_int(),
		_config.parent().child("debug_red").attribute("width").as_int(), _config.parent().child("debug_red").attribute("height").as_int() };
	debug_blue = { _config.parent().child("debug_blue").attribute("x").as_int(), _config.parent().child("debug_blue").attribute("y").as_int(),
		_config.parent().child("debug_blue").attribute("width").as_int(), _config.parent().child("debug_blue").attribute("height").as_int() };

	position = App->map->MapToWorld(_config.child("position").attribute("x").as_int(), _config.child("position").attribute("y").as_int());
	objective_position = position;
	Default_States.Hp = _config.child("stats").attribute("hp").as_int();
	Default_States.Mana = _config.child("stats").attribute("mana").as_int();
	Default_States.Cp = _config.child("stats").attribute("cp").as_int();
	Default_States.AtkF = _config.child("stats").attribute("atk_f").as_int();
	Default_States.AtkS = _config.child("stats").attribute("atk_s").as_int();
	Default_States.DefF = _config.child("stats").attribute("def_f").as_int();
	Default_States.DefS = _config.child("stats").attribute("def_s").as_int();
	Default_States.Crit = _config.child("stats").attribute("crit").as_int();

	LoadAnimation(_config.child("animations").child("idle_left_front").child("frame"), idle_left_front);
	idle_left_front.speed = _config.child("animations").child("idle_left_front").attribute("speed").as_float();
	idle_left_front.loop = _config.child("animations").child("idle_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_right_front").child("frame"), idle_right_front);
	idle_right_front.speed = _config.child("animations").child("idle_right_front").attribute("speed").as_float();
	idle_right_front.loop = _config.child("animations").child("idle_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_left_back").child("frame"), idle_left_back);
	idle_left_back.speed = _config.child("animations").child("idle_left_back").attribute("speed").as_float();
	idle_left_back.loop = _config.child("animations").child("idle_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_right_back").child("frame"), idle_right_back);
	idle_right_back.speed = _config.child("animations").child("idle_right_back").attribute("speed").as_float();
	idle_right_back.loop = _config.child("animations").child("idle_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_left_front").child("frame"), walk_left_front);
	walk_left_front.speed = _config.child("animations").child("walk_left_front").attribute("speed").as_float();
	walk_left_front.loop = _config.child("animations").child("walk_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_right_front").child("frame"), walk_right_front);
	walk_right_front.speed = _config.child("animations").child("walk_right_front").attribute("speed").as_float();
	walk_right_front.loop = _config.child("animations").child("walk_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_left_back").child("frame"), walk_left_back);
	walk_left_back.speed = _config.child("animations").child("walk_left_back").attribute("speed").as_float();
	walk_left_back.loop = _config.child("animations").child("walk_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_right_back").child("frame"), walk_right_back);
	walk_right_back.speed = _config.child("animations").child("walk_right_back").attribute("speed").as_float();
	walk_right_back.loop = _config.child("animations").child("walk_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_left_front").child("frame"), attack_left_front);
	attack_left_front.speed = _config.child("animations").child("attack_left_front").attribute("speed").as_float();
	attack_left_front.loop = _config.child("animations").child("attack_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_right_front").child("frame"), attack_right_front);
	attack_right_front.speed = _config.child("animations").child("attack_right_front").attribute("speed").as_float();
	attack_right_front.loop = _config.child("animations").child("attack_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_left_back").child("frame"), attack_left_back);
	attack_left_back.speed = _config.child("animations").child("attack_left_back").attribute("speed").as_float();
	attack_left_back.loop = _config.child("animations").child("attack_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_right_back").child("frame"), attack_right_back);
	attack_right_back.speed = _config.child("animations").child("attack_right_back").attribute("speed").as_float();
	attack_right_back.loop = _config.child("animations").child("attack_right_back").attribute("loop").as_bool(true);

}


bool Entity::Load(pugi::xml_node & _file)
{
	return true;
}

bool Entity::Save(pugi::xml_node & _file) const
{
	return true;
}

void Entity::AddFX(const int _channel, const int _repeat) const
{
	App->audio->PlayFx(_channel, _repeat);
}

//LoadAnimation with LoadCollider structure
bool Entity::LoadAnimation(pugi::xml_node _node, Animation &_anim)
{
	for (; _node; _node = _node.next_sibling("frame")) {
		SDL_Rect frame_rect;
		frame_rect.x = _node.attribute("x").as_int();
		frame_rect.y = _node.attribute("y").as_int();
		frame_rect.w = _node.attribute("width").as_int();
		frame_rect.h = _node.attribute("height").as_int();
		_anim.PushBack(frame_rect);
	}
	return true;

}

bool Entity::CleanUp()
{
	if (entity_texture) 
	{
		App->textures->UnLoad(entity_texture);
		entity_texture = nullptr;
	}
	if (debug_texture)
	{
		App->textures->UnLoad(debug_texture);
		debug_texture = nullptr;
	}
	return true;
}

std::pair<int, int> Entity::GetPosition()
{
	return position;
}

void Entity::SetPosition(const float &_x, const float &_y)
{
	position = { _x, _y };
}

ENTITY_TYPE Entity::GetType()
{
	return type;
}