#include "Application.h"
#include "MainMenu.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTransitionManager.h"



MainMenu::MainMenu(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{
	
}

// Destructor
MainMenu::~MainMenu()
{}


// Called before the first frame
bool MainMenu::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
	return true;
}

// Called before all Updates
bool MainMenu::PreUpdate()
{
	if (!App->scene_manager->music_created) {
		CreateMusic();
	}
	if (!main_menu_created) {
		CreateMainMenu();
	}
	return true;
}

// Called each loop iteration
bool MainMenu::Update(float _dt)
{
	bool ret = true;


	if (new_game_button->has_been_clicked)
	{
		App->scene_manager->music_created = false;
		App->scene_manager->ChangeScene(FIRST_BATTLE);
		//App->transition_manager->CreateFadeTransition(2, true, FIRST_BATTLE, Scene_1_color);

	}
	else if (load_game_button->has_been_clicked)
	{
	}
	else if (options_button->has_been_clicked)
	{
		App->scene_manager->ChangeScene(OPTION_MENU);
	}
	else if (credits_button->has_been_clicked)
	{
		App->scene_manager->ChangeScene(CREDITS_MENU);
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

	return ret;
}

// Called before all Updates
bool MainMenu::PostUpdate()
{
	App->render->Blit(main_menu_background, 0, 0);
	return true;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->gui_manager->DeleteAllGUIElements();
	main_menu_created = false;
	buttons.clear();
	return true;
}


// Load
bool MainMenu::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool MainMenu::Save(pugi::xml_node& _data) const
{
	return true;
}


void MainMenu::CreateMusic()
{
	App->scene_manager->music_created = true;
	App->audio->PlayMusic("Main_menu_8_bits.ogg");
}

void MainMenu::CreateMainMenu()
{
	main_menu_created = true;
	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - ((button_margin * 2) + (button.h * 2) + (button.h / 2)), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(new_game_button);
	load_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), new_game_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(load_game_button);
	options_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), load_game_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(options_button);
	credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), options_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(credits_button);
	exit_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), credits_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(exit_button);
	cherry_glasses_logo_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2) + 360, exit_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(cherry_glasses_logo_button);

	if (language)
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2), new_game_button->position.second + (button.h / 2), "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2), load_game_button->position.second + (button.h / 2), "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2), options_button->position.second + (button.h / 2), "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2), credits_button->position.second + (button.h / 2), "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2), exit_button->position.second + (button.h / 2), "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2), new_game_button->position.second + (button.h / 2), "NUEVA PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2), load_game_button->position.second + (button.h / 2), "CARGAR PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2), options_button->position.second + (button.h / 2), "OPCIONES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2), credits_button->position.second + (button.h / 2), "CRÉDITOS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2), exit_button->position.second + (button.h / 2), "SALIR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, cherry_glasses_logo_button->position.first + (button.w / 2) - (cherry_glasses_logo.w / 2), cherry_glasses_logo_button->position.second + (button.h / 2) - (cherry_glasses_logo.h / 2), { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);
}




