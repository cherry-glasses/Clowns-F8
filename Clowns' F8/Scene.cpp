#include "Log.h"
#include "Scene.h"



Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}


// Called before render is available
bool Scene::Awake(pugi::xml_node& _config)
{

	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	return true;
}

// Called before all Updates
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float _dt)
{
	return true;
}

// Called before all Updates
bool Scene::PostUpdate()
{

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{

	return true;
}


// Load
bool Scene::Load(pugi::xml_node& savegame)
{

	return true;
}

//Save
bool Scene::Save(pugi::xml_node& _data) const
{
	return true;
}