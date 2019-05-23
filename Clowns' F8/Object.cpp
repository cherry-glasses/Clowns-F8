#include "Application.h"
#include "Object.h"
#include "ModuleEntityManager.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleMap.h"

Object::Object(ENTITY_TYPE _type, pugi::xml_node _config, int _copy) : Entity(_type, _config)
{
	int i = 0;
	pugi::xml_node _node = _config.child("position");
	for (; _node; _node = _node.next_sibling("position")) {
		if (_copy == i) {
			position = App->map->MapToWorld(_node.attribute("x").as_int(), _node.attribute("y").as_int());
		}
		i++;
	}
	current = { 0 , 0, _config.child("size").attribute("width").as_int(), _config.child("size").attribute("height").as_int() };
}
Object::~Object()
{
}

bool Object::PreUpdate()
{
	if (current_turn == SEARCH_MOVE)
	{
		current_turn = END_TURN;
	}

	return true;
}

bool Object::Update(float dt)
{
	return true;
}

bool Object::PostUpdate(float _dt)
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first - (current.w / 2) + position_margin.first, position.second - current.h + position_margin.second);
	}

	return true;
}

// Load and Save
bool Object::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Object::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}




