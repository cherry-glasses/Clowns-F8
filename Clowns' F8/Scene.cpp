#include "Log.h"
#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
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
	main_menu_background = App->textures->Load("Assets/Sprites/UI/Main_menu_art_shot.png");
	health_bar_tex = App->textures->Load("Assets/Sprites/UI/UI_Health_Bar.png");
	mana_bar_tex = App->textures->Load("Assets/Sprites/UI/UI_Mana_Bar.png");
	portrait_tex = App->textures->Load("Assets/Sprites/UI/UI_Portrait_Box.png");
	options_background = App->textures->Load("Assets/Sprites/UI/4259708641.png");

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

		if (!main_menu_created) {
			main_menu_created = true;
			CreateMainMenu();
		}
		App->render->Blit(main_menu_background, -160, 0);
		
		if (new_game_button->has_been_clicked)
		{
			current_scene = GLOBAL_MAP;
			DeleteMainMenu();
		}
		else if (load_game_button->has_been_clicked)
		{
			current_scene = GLOBAL_MAP;
			DeleteMainMenu();
		}
		else if (options_button->has_been_clicked)
		{
			current_scene = MM_OPTIONS;
			DeleteMainMenu();
		}
		else if (credits_button->has_been_clicked )
		{
			current_scene = GLOBAL_MAP;
			DeleteMainMenu();
		}
		else if (exit_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		else if (cherry_glasses_logo_button->has_been_clicked)
		{
			ShellExecuteA(NULL, "open", "https://github.com/cherry-glasses/Clowns-F8/wiki", NULL, NULL, SW_SHOWNORMAL);
			cherry_glasses_logo_button->Select(SELECTED);
		}
		
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN)
		{
			NavigateDown();
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN)
		{
			NavigateUp();
		}
		

		break;
	case Scene::GLOBAL_MAP:
		App->render->Blit(portrait_tex, 870, 25);
		App->render->Blit(health_bar_tex, 1000, 30);
		App->render->Blit(mana_bar_tex, 1000, 90);

		App->render->Blit(portrait_tex, 20, 25);
		App->render->Blit(health_bar_tex, 150, 30);
		App->render->Blit(mana_bar_tex, 150, 90);

		App->render->Blit(portrait_tex, 20, 805);
		App->render->Blit(health_bar_tex, 150, 810);
		App->render->Blit(mana_bar_tex, 150, 870);

		App->render->Blit(portrait_tex, 870, 805);
		App->render->Blit(health_bar_tex, 1000, 810);
		App->render->Blit(mana_bar_tex, 1000, 870);
		if (!map_loaded) {
			map_loaded = true;
			main_menu_created = false;
			App->map->Load("iso_walk.tmx");
		}

		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}

		break;
	case Scene::MM_OPTIONS:
		App->render->Blit(main_menu_background, -160, 0);
		App->render->Blit(options_background, 350, 130);
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

	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 570.0f, 300.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(new_game_button);
	load_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 570.0f, 400.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(load_game_button);
	options_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 570.0f, 500.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(options_button);
	credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 570.0f, 600.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(credits_button);
	exit_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 570.0f, 700.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(exit_button);
	cherry_glasses_logo_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 1000.0f, 800.0f, { 0, 0, 192, 64 }, { 0, 64, 192, 64 }, { 0, 128, 192, 64 });
	buttons.push_back(cherry_glasses_logo_button);

	new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 585.0f, 310.0f, "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 580.0f, 410.0f, "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 598.0f, 510.0f, "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 605.0f, 610.0f, "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 635.0f, 710.0f, "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1042.0f, 803.0f, { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);

}

void Scene::DeleteMainMenu()
{
	/*if (cherry_glasses_logo_button != nullptr)
	{
		App->gui_manager->DeleteGUIElement(cherry_glasses_logo_button);
		cherry_glasses_logo_button = nullptr;
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
	}*/
	App->gui_manager->DeleteAllGUIElements();
}

void Scene::NavigateDown() 
{
	for (std::list<GUIButton*>::const_iterator it_vector = buttons.begin(); it_vector != buttons.end(); ++it_vector)
	{
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != buttons.back()) {
				(*it_vector)->Select(NORMAL);
				it_vector++;
				(*it_vector)->Select(SELECTED);
				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = buttons.begin();
				(*it_vector)->Select(SELECTED);
			}
		}
	}
}

void Scene::NavigateUp() 
{
	for (std::list<GUIButton*>::const_iterator it_vector = buttons.begin(); it_vector != buttons.end(); ++it_vector)
	{
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != buttons.front()) {
				(*it_vector)->Select(NORMAL);
				it_vector--;
				(*it_vector)->Select(SELECTED);

				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = buttons.end();
				it_vector--;
				(*it_vector)->Select(SELECTED);
			}
		}
	}
}


