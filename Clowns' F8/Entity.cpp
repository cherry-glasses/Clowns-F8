#include "Log.h"
#include "Application.h"
#include "Entity.h"
#include "ModuleAudio.h"


Entity::Entity()
{
}

Entity::Entity(ENTITY_TYPE _type) : type(_type)
{
}

// Destructor
Entity::~Entity()
{
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
	return true;
}


std::pair<float, float> Entity::GetPosition()
{
	return position;
}

void Entity::SetPosition(const float &x, const float &y)
{

}