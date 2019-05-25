#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "Entity.h"
#include "ModuleMap.h"


Entity::Entity(ENTITY_TYPE _type, pugi::xml_node _config)
{
	type = _type;

	entity_texture = App->textures->Load(_config.child("texture").attribute("value").as_string());
	debug_texture = App->textures->Load(_config.parent().child("debug_texture").attribute("value").as_string());
	debug_green = { _config.parent().child("debug_green").attribute("x").as_int(), _config.parent().child("debug_green").attribute("y").as_int(),
		_config.parent().child("debug_green").attribute("width").as_int(), _config.parent().child("debug_green").attribute("height").as_int() };
	debug_red = { _config.parent().child("debug_red").attribute("x").as_int(), _config.parent().child("debug_red").attribute("y").as_int(),
		_config.parent().child("debug_red").attribute("width").as_int(), _config.parent().child("debug_red").attribute("height").as_int() };
	debug_blue = { _config.parent().child("debug_blue").attribute("x").as_int(), _config.parent().child("debug_blue").attribute("y").as_int(),
		_config.parent().child("debug_blue").attribute("width").as_int(), _config.parent().child("debug_blue").attribute("height").as_int() };
	debug_yellow = { _config.parent().child("debug_yellow").attribute("x").as_int(), _config.parent().child("debug_yellow").attribute("y").as_int(),
		_config.parent().child("debug_yellow").attribute("width").as_int(), _config.parent().child("debug_yellow").attribute("height").as_int() };
	circle_green = { _config.parent().child("circle_green").attribute("x").as_int(), _config.parent().child("circle_green").attribute("y").as_int(),
		_config.parent().child("circle_green").attribute("width").as_int(), _config.parent().child("circle_green").attribute("height").as_int() };
	circle_red = { _config.parent().child("circle_red").attribute("x").as_int(), _config.parent().child("circle_red").attribute("y").as_int(),
		_config.parent().child("circle_red").attribute("width").as_int(), _config.parent().child("circle_red").attribute("height").as_int() };
	circle_blue = { _config.parent().child("circle_blue").attribute("x").as_int(), _config.parent().child("circle_blue").attribute("y").as_int(),
		_config.parent().child("circle_blue").attribute("width").as_int(), _config.parent().child("circle_blue").attribute("height").as_int() };
	circle_yellow = { _config.parent().child("circle_yellow").attribute("x").as_int(), _config.parent().child("circle_yellow").attribute("y").as_int(),
		_config.parent().child("circle_yellow").attribute("width").as_int(), _config.parent().child("circle_yellow").attribute("height").as_int() };

	name = _config.child("name").attribute("value").as_string("");
	position_margin = { _config.parent().child("position_margin").attribute("x").as_int(), _config.parent().child("position_margin").attribute("y").as_int() };
	position =  App->map->MapToWorld(_config.child("position").attribute("x").as_int(), _config.child("position").attribute("y").as_int());
	
	default_stats.Hp = _config.child("stats").attribute("hp").as_int();
	default_stats.Mana = _config.child("stats").attribute("mana").as_int();
	default_stats.PMove = _config.child("stats").attribute("p_move").as_int();
	default_stats.AtkF = _config.child("stats").attribute("atk_f").as_int();
	default_stats.AtkS = _config.child("stats").attribute("atk_s").as_int();
	default_stats.RangeAtk = _config.child("stats").attribute("range_atk").as_int();
	default_stats.RangeAbility_1 = _config.child("stats").attribute("range_ability_1").as_int();
	default_stats.RangeAbility_2 = _config.child("stats").attribute("range_ability_2").as_int();
	default_stats.RangeAbility_3 = _config.child("stats").attribute("range_ability_3").as_int();
	default_stats.DefF = _config.child("stats").attribute("def_f").as_int();
	default_stats.DefS = _config.child("stats").attribute("def_s").as_int();
	default_stats.Crit = _config.child("stats").attribute("crit").as_int();
	default_stats.Agi = _config.child("stats").attribute("agi").as_int();
	default_stats.Attack = _config.child("stats").attribute("attack").as_int();
	default_stats.Ability_1 = _config.child("stats").attribute("ability_1").as_int();
	default_stats.Ability_2 = _config.child("stats").attribute("ability_2").as_int();
	default_stats.Ability_3 = _config.child("stats").attribute("ability_3").as_int();

	evolution_stats.Hp = _config.child("evolution").attribute("hp").as_int();
	evolution_stats.Mana = _config.child("evolution").attribute("mana").as_int();
	evolution_stats.AtkF = _config.child("evolution").attribute("atk_f").as_int();
	evolution_stats.AtkS = _config.child("evolution").attribute("atk_s").as_int();
	evolution_stats.DefF = _config.child("evolution").attribute("def_f").as_int();
	evolution_stats.DefS = _config.child("evolution").attribute("def_s").as_int();
	evolution_stats.Crit = _config.child("evolution").attribute("crit").as_int();

	current_stats = default_stats;

	//Portraits
	portrait = { _config.child("portrait").attribute("x").as_int(), _config.child("portrait").attribute("y").as_int(),
		_config.child("portrait").attribute("w").as_int() , _config.child("portrait").attribute("h").as_int() };

	//FXSounds
	sfx.Attack_SFX = App->audio->LoadFx(_config.child("sfx").child("attack_sfx").attribute("value").as_string());
	sfx.Ability_1_SFX = App->audio->LoadFx(_config.child("sfx").child("ability_1_sfx").attribute("value").as_string());
	sfx.Ability_2_SFX = App->audio->LoadFx(_config.child("sfx").child("ability_2_sfx").attribute("value").as_string());
	sfx.Ability_3_SFX = App->audio->LoadFx(_config.child("sfx").child("ability_3_sfx").attribute("value").as_string());
	sfx.Dead_SFX = App->audio->LoadFx(_config.child("sfx").child("dead_sfx").attribute("value").as_string());


	exp = _config.child("exp").attribute("value").as_int();

	pugi::xml_node _node = _config.parent().child("levelup").child("nextlevel");
	for (; _node; _node = _node.next_sibling("nextlevel")) {
		levels.push_back(_node.attribute("value").as_int());
	}

	LoadAnim(_config);

	current_turn = NONE;

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

void Entity::LoadAnim(pugi::xml_node _config)
{
	// IDLE
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

	LoadAnimation(_config.child("animations").child("idle_left").child("frame"), idle_left);
	idle_left.speed = _config.child("animations").child("idle_left").attribute("speed").as_float();
	idle_left.loop = _config.child("animations").child("idle_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_right").child("frame"), idle_right);
	idle_right.speed = _config.child("animations").child("idle_right").attribute("speed").as_float();
	idle_right.loop = _config.child("animations").child("idle_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_front").child("frame"), idle_front);
	idle_front.speed = _config.child("animations").child("idle_front").attribute("speed").as_float();
	idle_front.loop = _config.child("animations").child("idle_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("idle_back").child("frame"), idle_back);
	idle_back.speed = _config.child("animations").child("idle_back").attribute("speed").as_float();
	idle_back.loop = _config.child("animations").child("idle_back").attribute("loop").as_bool(true);

	// WALK
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

	LoadAnimation(_config.child("animations").child("walk_left").child("frame"), walk_left);
	walk_left.speed = _config.child("animations").child("walk_left").attribute("speed").as_float();
	walk_left.loop = _config.child("animations").child("walk_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_right").child("frame"), walk_right);
	walk_right.speed = _config.child("animations").child("walk_right").attribute("speed").as_float();
	walk_right.loop = _config.child("animations").child("walk_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_right_2").child("frame"), walk_right_2);
	walk_right_2.speed = _config.child("animations").child("walk_right_2").attribute("speed").as_float();
	walk_right_2.loop = _config.child("animations").child("walk_right_2").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_front").child("frame"), walk_front);
	walk_front.speed = _config.child("animations").child("walk_front").attribute("speed").as_float();
	walk_front.loop = _config.child("animations").child("walk_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_front_2").child("frame"), walk_front_2);
	walk_front_2.speed = _config.child("animations").child("walk_front_2").attribute("speed").as_float();
	walk_front_2.loop = _config.child("animations").child("walk_front_2").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_back").child("frame"), walk_back);
	walk_back.speed = _config.child("animations").child("walk_back").attribute("speed").as_float();
	walk_back.loop = _config.child("animations").child("walk_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("walk_back_2").child("frame"), walk_back_2);
	walk_back_2.speed = _config.child("animations").child("walk_back_2").attribute("speed").as_float();
	walk_back_2.loop = _config.child("animations").child("walk_back_2").attribute("loop").as_bool(true);

	// ATTACK
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

	LoadAnimation(_config.child("animations").child("attack_left").child("frame"), attack_left);
	attack_left.speed = _config.child("animations").child("attack_left").attribute("speed").as_float();
	attack_left.loop = _config.child("animations").child("attack_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_right").child("frame"), attack_right);
	attack_right.speed = _config.child("animations").child("attack_right").attribute("speed").as_float();
	attack_right.loop = _config.child("animations").child("attack_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_front").child("frame"), attack_front);
	attack_front.speed = _config.child("animations").child("attack_front").attribute("speed").as_float();
	attack_front.loop = _config.child("animations").child("attack_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("attack_back").child("frame"), attack_back);
	attack_back.speed = _config.child("animations").child("attack_back").attribute("speed").as_float();
	attack_back.loop = _config.child("animations").child("attack_back").attribute("loop").as_bool(true);

	// Ability 1
	LoadAnimation(_config.child("animations").child("ability_1_left_front").child("frame"), ability_1_left_front);
	ability_1_left_front.speed = _config.child("animations").child("ability_1_left_front").attribute("speed").as_float();
	ability_1_left_front.loop = _config.child("animations").child("ability_1_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_right_front").child("frame"), ability_1_right_front);
	ability_1_right_front.speed = _config.child("animations").child("ability_1_right_front").attribute("speed").as_float();
	ability_1_right_front.loop = _config.child("animations").child("ability_1_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_left_back").child("frame"), ability_1_left_back);
	ability_1_left_back.speed = _config.child("animations").child("ability_1_left_back").attribute("speed").as_float();
	ability_1_left_back.loop = _config.child("animations").child("ability_1_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_right_back").child("frame"), ability_1_right_back);
	ability_1_right_back.speed = _config.child("animations").child("ability_1_right_back").attribute("speed").as_float();
	ability_1_right_back.loop = _config.child("animations").child("ability_1_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_left").child("frame"), ability_1_left);
	ability_1_left.speed = _config.child("animations").child("ability_1_left").attribute("speed").as_float();
	ability_1_left.loop = _config.child("animations").child("ability_1_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_right").child("frame"), ability_1_right);
	ability_1_right.speed = _config.child("animations").child("ability_1_right").attribute("speed").as_float();
	ability_1_right.loop = _config.child("animations").child("ability_1_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_front").child("frame"), ability_1_front);
	ability_1_front.speed = _config.child("animations").child("ability_1_front").attribute("speed").as_float();
	ability_1_front.loop = _config.child("animations").child("ability_1_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_1_back").child("frame"), ability_1_back);
	ability_1_back.speed = _config.child("animations").child("ability_1_back").attribute("speed").as_float();
	ability_1_back.loop = _config.child("animations").child("ability_1_back").attribute("loop").as_bool(true);

	// Ability 2
	LoadAnimation(_config.child("animations").child("ability_2_left_front").child("frame"), ability_2_left_front);
	ability_2_left_front.speed = _config.child("animations").child("ability_2_left_front").attribute("speed").as_float();
	ability_2_left_front.loop = _config.child("animations").child("ability_2_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_right_front").child("frame"), ability_2_right_front);
	ability_2_right_front.speed = _config.child("animations").child("ability_2_right_front").attribute("speed").as_float();
	ability_2_right_front.loop = _config.child("animations").child("ability_2_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_left_back").child("frame"), ability_2_left_back);
	ability_2_left_back.speed = _config.child("animations").child("ability_2_left_back").attribute("speed").as_float();
	ability_2_left_back.loop = _config.child("animations").child("ability_2_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_right_back").child("frame"), ability_2_right_back);
	ability_2_right_back.speed = _config.child("animations").child("ability_2_right_back").attribute("speed").as_float();
	ability_2_right_back.loop = _config.child("animations").child("ability_2_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_left").child("frame"), ability_2_left);
	ability_2_left.speed = _config.child("animations").child("ability_2_left").attribute("speed").as_float();
	ability_2_left.loop = _config.child("animations").child("ability_2_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_right").child("frame"), ability_2_right);
	ability_2_right.speed = _config.child("animations").child("ability_2_right").attribute("speed").as_float();
	ability_2_right.loop = _config.child("animations").child("ability_2_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_front").child("frame"), ability_2_front);
	ability_2_front.speed = _config.child("animations").child("ability_2_front").attribute("speed").as_float();
	ability_2_front.loop = _config.child("animations").child("ability_2_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_2_back").child("frame"), ability_2_back);
	ability_2_back.speed = _config.child("animations").child("ability_2_back").attribute("speed").as_float();
	ability_2_back.loop = _config.child("animations").child("ability_2_back").attribute("loop").as_bool(true);

	// Ability 3
	LoadAnimation(_config.child("animations").child("ability_3_left_front").child("frame"), ability_3_left_front);
	ability_3_left_front.speed = _config.child("animations").child("ability_3_left_front").attribute("speed").as_float();
	ability_3_left_front.loop = _config.child("animations").child("ability_3_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_right_front").child("frame"), ability_3_right_front);
	ability_3_right_front.speed = _config.child("animations").child("ability_3_right_front").attribute("speed").as_float();
	ability_3_right_front.loop = _config.child("animations").child("ability_3_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_left_back").child("frame"), ability_3_left_back);
	ability_3_left_back.speed = _config.child("animations").child("ability_3_left_back").attribute("speed").as_float();
	ability_3_left_back.loop = _config.child("animations").child("ability_3_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_right_back").child("frame"), ability_3_right_back);
	ability_3_right_back.speed = _config.child("animations").child("ability_3_right_back").attribute("speed").as_float();
	ability_3_right_back.loop = _config.child("animations").child("ability_3_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_left").child("frame"), ability_3_left);
	ability_3_left.speed = _config.child("animations").child("ability_3_left").attribute("speed").as_float();
	ability_3_left.loop = _config.child("animations").child("ability_3_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_right").child("frame"), ability_3_right);
	ability_3_right.speed = _config.child("animations").child("ability_3_right").attribute("speed").as_float();
	ability_3_right.loop = _config.child("animations").child("ability_3_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_front").child("frame"), ability_3_front);
	ability_3_front.speed = _config.child("animations").child("ability_3_front").attribute("speed").as_float();
	ability_3_front.loop = _config.child("animations").child("ability_3_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("ability_3_back").child("frame"), ability_3_back);
	ability_3_back.speed = _config.child("animations").child("ability_3_back").attribute("speed").as_float();
	ability_3_back.loop = _config.child("animations").child("ability_3_back").attribute("loop").as_bool(true);

	// DEFEND
	LoadAnimation(_config.child("animations").child("defend_left_front").child("frame"), defend_left_front);
	defend_left_front.speed = _config.child("animations").child("defend_left_front").attribute("speed").as_float();
	defend_left_front.loop = _config.child("animations").child("defend_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_right_front").child("frame"), defend_right_front);
	defend_right_front.speed = _config.child("animations").child("defend_right_front").attribute("speed").as_float();
	defend_right_front.loop = _config.child("animations").child("defend_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_left_back").child("frame"), defend_left_back);
	defend_left_back.speed = _config.child("animations").child("defend_left_back").attribute("speed").as_float();
	defend_left_back.loop = _config.child("animations").child("defend_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_right_back").child("frame"), defend_right_back);
	defend_right_back.speed = _config.child("animations").child("defend_right_back").attribute("speed").as_float();
	defend_right_back.loop = _config.child("animations").child("defend_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_left").child("frame"), defend_left);
	defend_left.speed = _config.child("animations").child("defend_left").attribute("speed").as_float();
	defend_left.loop = _config.child("animations").child("defend_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_right").child("frame"), defend_right);
	defend_right.speed = _config.child("animations").child("defend_right").attribute("speed").as_float();
	defend_right.loop = _config.child("animations").child("defend_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_front").child("frame"), defend_front);
	defend_front.speed = _config.child("animations").child("defend_front").attribute("speed").as_float();
	defend_front.loop = _config.child("animations").child("defend_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("defend_back").child("frame"), defend_back);
	defend_back.speed = _config.child("animations").child("defend_back").attribute("speed").as_float();
	defend_back.loop = _config.child("animations").child("defend_back").attribute("loop").as_bool(true);

	// DEAD
	LoadAnimation(_config.child("animations").child("dead_left_front").child("frame"), dead_left_front);
	dead_left_front.speed = _config.child("animations").child("dead_left_front").attribute("speed").as_float();
	dead_left_front.loop = _config.child("animations").child("dead_left_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_right_front").child("frame"), dead_right_front);
	dead_right_front.speed = _config.child("animations").child("dead_right_front").attribute("speed").as_float();
	dead_right_front.loop = _config.child("animations").child("dead_right_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_left_back").child("frame"), dead_left_back);
	dead_left_back.speed = _config.child("animations").child("dead_left_back").attribute("speed").as_float();
	dead_left_back.loop = _config.child("animations").child("dead_left_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_right_back").child("frame"), dead_right_back);
	dead_right_back.speed = _config.child("animations").child("dead_right_back").attribute("speed").as_float();
	dead_right_back.loop = _config.child("animations").child("dead_right_back").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_left").child("frame"), dead_left);
	dead_left.speed = _config.child("animations").child("dead_left").attribute("speed").as_float();
	dead_left.loop = _config.child("animations").child("dead_left").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_right").child("frame"), dead_right);
	dead_right.speed = _config.child("animations").child("dead_right").attribute("speed").as_float();
	dead_right.loop = _config.child("animations").child("dead_right").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_front").child("frame"), dead_front);
	dead_front.speed = _config.child("animations").child("dead_front").attribute("speed").as_float();
	dead_front.loop = _config.child("animations").child("dead_front").attribute("loop").as_bool(true);

	LoadAnimation(_config.child("animations").child("dead_back").child("frame"), dead_back);
	dead_back.speed = _config.child("animations").child("dead_back").attribute("speed").as_float();
	dead_back.loop = _config.child("animations").child("dead_back").attribute("loop").as_bool(true);

}