#include "Log.h"
#include "Scene.h"
#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

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
	main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_bg.png");
	CreateMainMenu();
	
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
	bool ret = true;

	switch (current_scene)
	{
	case Scene::MAIN_MENU:

		App->render->Blit(main_menu_background, 160, 0);
		
		if (new_game_button->has_been_clicked)
		{
			current_scene = GLOBAL_MAP;
			DeleteMainMenu();
		}else if (credits_button->has_been_clicked )
		{

		}else if (exit_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}else if (cherry_glasses_logo != nullptr && cherry_glasses_logo->has_been_clicked)
		{
			ShellExecuteA(NULL, "open", "https://github.com/cherry-glasses/Clowns-F8/wiki", NULL, NULL, SW_SHOWNORMAL);
		}
		
		if (exitpressed == true)
		{
			ret = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN )
		{
			NavigateDown(buttons);
			

		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN )
		{
			NavigateUp(buttons);
			
		}
		

		break;
	case Scene::GLOBAL_MAP:
		if (!map_loaded) {
			map_loaded = true;
			App->map->Load("iso_walk.tmx");
		}
		break;
	case Scene::FIRST_BATTLE:
		break;
	default:
		break;
	}
	
	App->map->Draw();
	return ret;
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

	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 550.0f, 400.0f, { 0, 62, 178, 62 }, { 0, 186, 178, 62 }, { 0, 310, 178, 62 });
	buttons.push_back(new_game_button);
	load_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 550.0f, 500.0f, { 356, 0, 178, 62 }, { 356, 124, 178, 62 }, { 356, 248, 178, 62 });
	buttons.push_back(load_game_button);
	options_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 550.0f, 600.0f, { 178, 62, 178, 62 }, { 178, 186, 178, 62 }, { 178, 310, 178, 62 });
	buttons.push_back(options_button);
	credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 550.0f, 700.0f, { 178, 0, 178, 62 }, { 178, 124, 178, 62 }, { 178, 248, 178, 62 });
	buttons.push_back(credits_button);
	exit_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 550.0f, 800.0f, { 0, 0, 178, 62 }, { 0, 124, 178, 62 }, { 0, 248, 178, 62 });
	buttons.push_back(exit_button);
	cherry_glasses_logo = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1050.0f, 800.0f, { 0, 0, 178, 101 });
	
	new_game_button->Select(SELECTED);

}

void Scene::DeleteMainMenu()
{
	if (cherry_glasses_logo != nullptr)
	{
		App->gui_manager->DeleteGUIElement(cherry_glasses_logo);
		cherry_glasses_logo = nullptr;
	}
	if (new_game_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(new_game_button);
		new_game_button = nullptr;
	}
	if (load_game_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(load_game_button);
		load_game_button = nullptr;
	}
	if (options_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(options_button);
		options_button = nullptr;
	}
	if (credits_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(credits_button);
		credits_button = nullptr;
	}
	if (exit_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(exit_button);
		exit_button = nullptr;
	}
}

void Scene::NavigateDown(std::vector<GUIButton*> &current_vector) {
	std::vector<GUIButton*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != current_vector.back()) {
				(*it_vector)->Select(NORMAL);
				it_vector++;
				(*it_vector)->Select(SELECTED);
				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = current_vector.begin();
				(*it_vector)->Select(SELECTED);
				
			}
		}
		it_vector++;
	}
}

void Scene::NavigateUp(std::vector<GUIButton*> &current_vector) {
	std::vector<GUIButton*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != current_vector.front()) {
				(*it_vector)->Select(NORMAL);
				it_vector--;
				(*it_vector)->Select(SELECTED);
				
				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = current_vector.end() - 1;
				(*it_vector)->Select(SELECTED);
			
			}
		}
		it_vector++;
	}
}


