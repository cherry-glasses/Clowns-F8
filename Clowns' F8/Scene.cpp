#include "Log.h"
#include "Scene.h"
#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIImage.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"


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
	if (!game_start)
	{
		main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_bg.png");
		CreateMainMenu();
	}
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
	if (!game_start)
	{
		App->render->Blit(main_menu_background, 160, 0);
	}
	if (cherry_glasses_logo != nullptr && cherry_glasses_logo->has_been_clicked)
	{
		ShellExecuteA(NULL, "open", "https://github.com/cherry-glasses/Clowns-F8/wiki", NULL, NULL, SW_SHOWNORMAL);
	}
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
	App->textures->UnLoad(main_menu_background);
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

void Scene::CreateMainMenu()
{
	cherry_glasses_logo = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1050.0f, 800.0f, { 0, 0, 178, 101 });
}

void Scene::DeleteMainMenu()
{
	if (cherry_glasses_logo != nullptr)
	{
		App->gui_manager->DeleteGUIElement(cherry_glasses_logo);
		cherry_glasses_logo = nullptr;
	}
}