#include "Application.h"
#include "Entity.h"
#include "ModuleAudio.h"


Entity::Entity()
{
}

// Destructor

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

void Entity::SetPosition(const float &_x, const float &_y)
{

}