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
#include "ModuleAudio.h"
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
	credits_page = App->textures->Load("Assets/Sprites/UI/credits_done.png");

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
		App->render->Blit(main_menu_background, -200, 0);
		
		if (new_game_button->has_been_clicked)
		{
			current_scene = GLOBAL_MAP;
			DeleteMenu();
		}
		else if (load_game_button->has_been_clicked)
		{
			current_scene = GLOBAL_MAP;
			DeleteMenu();
		}
		else if (options_button->has_been_clicked)
		{
			current_scene = MM_OPTIONS;
			DeleteMenu();
		}
		else if (credits_button->has_been_clicked )
		{
			current_scene = MM_CREDITS;
			DeleteMenu();
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
		Navigate();
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
		App->render->Blit(main_menu_background, -200, 0);
		App->render->Blit(options_background, 365, 130);
		if (!mm_options_created)
		{
			mm_options_created = true;
			CreateMMOptions();
		}
		main_menu_created = false;
		if (english_button->has_been_clicked)
		{
			if (!language)
			{
				language = true;
				DeleteMenu();
				CreateMMOptions();
			}
			english_button->Select(SELECTED);
		}
		else if (spanish_button->has_been_clicked)
		{
			if (language) 
			{
				language = false;
				DeleteMenu();
				CreateMMOptions();
				spanish_button->Select(NORMAL);
			}
			else spanish_button->Select(SELECTED);
		}
		else if (volume_up_button->has_been_clicked)
		{
			App->audio->VolumeUp();
			volume_up_button->Select(SELECTED);
		}
		else if (volume_down_button->has_been_clicked)
		{
			App->audio->VolumeDown();
			volume_down_button->Select(SELECTED);
		}
		else if (back_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			mm_options_created = false;
			current_scene = MAIN_MENU;
			DeleteMenu();
		}

		Navigate();
		break;

	case Scene::MM_CREDITS:
		App->render->Blit(main_menu_background, -200, 0);
		App->render->Blit(credits_page, 365, 130);
		main_menu_created = false;
		if (mm_credits_created == false)
		{
			back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 710.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
			buttons.push_back(back_button);
			back_button->Select(SELECTED);
			if (language)
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 605.0f, 720.0f, "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			else
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 600.0f, 720.0f, "ATRÁS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			mm_credits_created = true;
		}
		if (back_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			current_scene = MAIN_MENU;
			back_button->Select(SELECTED);
			mm_credits_created = false;
			DeleteMenu();

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
	App->textures->UnLoad(mana_bar_tex);
	App->textures->UnLoad(health_bar_tex);
	App->textures->UnLoad(portrait_tex);
	App->textures->UnLoad(options_background);
	App->textures->UnLoad(credits_page);
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

	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 300.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(new_game_button);
	load_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 400.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(load_game_button);
	options_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 500.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(options_button);
	credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 600.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(credits_button);
	exit_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 700.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(exit_button);
	cherry_glasses_logo_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 952.0f, 800.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(cherry_glasses_logo_button);

	if (language)
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 560.0f, 310.0f, "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 555.0f, 410.0f, "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 573.0f, 510.0f, "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 580.0f, 610.0f, "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 610.0f, 710.0f, "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 525.0f, 310.0f, "NUEVA PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 520.0f, 410.0f, "CARGAR PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 568.0f, 510.0f, "OPCIONES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 572.0f, 610.0f, "CREDIT0S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 603.0f, 710.0f, "SALIR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1042.0f, 803.0f, { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);

}

void Scene::CreateMMOptions()
{
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 270.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 340.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 500.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 570.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 497.0f, 710.0f, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 632.0f, 510.0f, "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 635.0f, 580.0f, "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	if (language)
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 575.0f, 280.0f, "ENGLISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 580.0f, 350.0f, "SPANISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 605.0f, 720.0f, "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 495.0f, 205.0f, "CHOOSE LANGUAGE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 520.0f, 440.0f, "ADJUST VOLUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 588.0f, 280.0f, "INGLÉS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 577.0f, 350.0f, "ESPAÑOL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 600.0f, 720.0f, "ATRÁS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 515.0f, 205.0f, "ESCOJER LENGUA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 505.0f, 440.0f, "AJUSTAR VOLUMEN", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	english_button->Select(SELECTED);
}

void Scene::DeleteMenu()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
}

void Scene::Navigate()
{
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN)
	{
		NavigateDown();
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN)
	{
		NavigateUp();
	}
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


