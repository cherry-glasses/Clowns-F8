#include "Log.h"
#include "Application.h"
#include "ModuleTransitionManager.h"
#include "Fade.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"


ModuleTransitionManager::ModuleTransitionManager()
{

}


ModuleTransitionManager::~ModuleTransitionManager()
{

}

bool ModuleTransitionManager::Awake(pugi::xml_node &)
{
	LOG("Awakening Transition Manager");
	return true;
}

bool ModuleTransitionManager::Start()
{
	LOG("Starting Transition Manager");

	return true;
}

bool ModuleTransitionManager::PreUpdate()
{
	//TODO
	return true;
}

bool ModuleTransitionManager::Update(float dt)
{
	for (std::list<Transition*>::iterator transition = active_transitions.begin(); transition != active_transitions.end(); ++transition)
	{
		(*transition)->Update();
	}

	return true;
}

bool ModuleTransitionManager::PostUpdate()
{
	for (std::list<Transition*>::iterator transition = active_transitions.begin(); transition != active_transitions.end(); ++transition)
	{
		(*transition)->PostUpdate();
	}

	return true;
}

bool ModuleTransitionManager::CleanUp()
{
	//TODO
	return true;
}

void ModuleTransitionManager::CreateFadeTransition(float transition_time, bool is_scene_change, SCENE_TYPE scene_to_transition, Color color)
{
	active_transitions.push_back(new Fade(transition_time, is_scene_change, scene_to_transition, color));
}

void ModuleTransitionManager::DestroyTransition(Transition * transition_to_destroy)
{
	active_transitions.remove(transition_to_destroy);
	delete transition_to_destroy;
}