#include "Globals.h"
#include "ModuleInput.h"
#include "Scene.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"



Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//for (pugi::xml_node map_node = config.child("maps"); map_node != nullptr; map_node = map_node.next_sibling("maps"))
	//{

	//	const char* name = map_node.attribute("map").as_string();

	//	MapsList_String.add(name);
	//}

	//CurrentMap = MapsList_String.start;

	return ret;
}

// Called before render is available
bool Awake(pugi::xml_node& config) {

	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Start() {

	return true;
}

// Called before all Updates
bool PreUpdate() {

	return true;
}

// Called each loop iteration
bool Update(float dt) {

	return true;
}

// Called before all Updates
bool PostUpdate() {

	return true;
}

// Called before quitting
bool CleanUp() {

	return true;
}

// Load
bool Load(pugi::xml_node& savegame) {

	return true;
}

bool LoadScene() { //introduce the map

	return true;
}

//Save
bool Save(pugi::xml_node& data) const {

	return  true;
}