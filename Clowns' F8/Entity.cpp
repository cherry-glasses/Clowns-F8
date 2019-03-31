#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Entity.h"


Entity::Entity(ENTITY_TYPE _type, pugi::xml_node _config)
{
	type = _type;

	string_texture = _config.child("texture").attribute("value").as_string();
	entity_texture = App->textures->Load(string_texture.c_str());
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
bool Entity::LoadAnimation(pugi::xml_node &_node, Animation &_anim)
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

std::pair<float, float> Entity::GetPosition()
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