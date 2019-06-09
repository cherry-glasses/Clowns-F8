#include "Application.h"
#include "MainMenu.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
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
	option_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();
	

	App->audio->PlayMusic("Main_menu_8_bits.ogg");

	CreateMainMenu();
	return true;
}

// Called before all Updates
bool MainMenu::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool MainMenu::Update(float _dt)
{
	bool ret = true;

	if (main_menu_created) 
	{
		if (new_game_button->has_been_clicked)
		{
			if (!App->scene_manager->changing)
			{
				App->scene_manager->victory = false;
				App->scene_manager->start_new_game = true;
				App->transition_manager->CreateFadeTransition(1, true, INTRO, Black);
				App->scene_manager->changing = true;
			}
			
		}
		else if (load_game_button->has_been_clicked)
		{
			if (!App->scene_manager->changing)
			{
				App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, Black);
				App->scene_manager->loading = true;
				App->scene_manager->changing = true;
			}
		}
		else if (options_button->has_been_clicked)
		{
			Delete();
			CreateOptionMenu();
		}
		else if (credits_button->has_been_clicked)
		{
			Delete();
			CreateCreditsMenu();
		}
		else if (exit_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		else if (cherry_glasses_logo_button->has_been_clicked)
		{
			ShellExecuteA(NULL, "open", "https://cherry-glasses.github.io/Clowns-F8/", NULL, NULL, SW_SHOWNORMAL);
			cherry_glasses_logo_button->Select(SELECTED);
		}
	}
	else if (option_menu_created) 
	{
		ControlLanguageAndMusic();

		if (back_option_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateMainMenu();
		}
		else if (controls_button->has_been_clicked)
		{
			Delete();
			CreateControlsMenu();
		}
	}
	else if (controls_menu_created)
	{
		if (!controls_menu_created) {
			CreateControlsMenu();
		}
		if (accept_button->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.ACCEPT, accept_button, true, accept_label3);
		}
		if (decline_button->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.DECLINE, decline_button, true, decline_label3);
		}
		if (start_button->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.PAUSE, start_button, true, start_label3);
		}
		if (accept_button2->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.ACCEPT, accept_button2, false, accept_label4);
		}
		if (decline_button2->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.DECLINE, decline_button2, false, decline_label4);
		}
		if (start_button2->has_been_clicked) {
			ChangeControls(App->input->keyboard_buttons.buttons_to_use.PAUSE, start_button2, false, start_label4);
		}

		else if (back_controls_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateOptionMenu();
		}
	}
	else if (credits_menu_created)
	{
		if (back_credits_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateMainMenu();
		}
	}
	
	Navigate();

	return ret;
}

// Called before all Updates
bool MainMenu::PostUpdate(float dt)
{
	
	App->render->Blit(main_menu_background, 0, 0);

	if (option_menu_created || controls_menu_created || credits_menu_created)
	{
		App->render->Blit(option_menu_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	}	

	return true;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->textures->UnLoad(option_menu_background);
	App->audio->UnloadAllFx();
	Delete();
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

	new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2), new_game_button->position.second + (button.h / 2), App->scene_manager->language->new_game, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2), load_game_button->position.second + (button.h / 2), App->scene_manager->language->load_game, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2), options_button->position.second + (button.h / 2), App->scene_manager->language->options, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2), credits_button->position.second + (button.h / 2), App->scene_manager->language->credits, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2), exit_button->position.second + (button.h / 2), App->scene_manager->language->exit, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, cherry_glasses_logo_button->position.first + (button.w / 2) - (cherry_glasses_logo.w / 2), cherry_glasses_logo_button->position.second + (button.h / 2) - (cherry_glasses_logo.h / 2), { 352, 231, 105, 59 });
	
	new_game_button->Select(SELECTED);
}


void MainMenu::CreateOptionMenu()
{
	option_menu_created = true;
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 1.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), english_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), spanish_button->position.second + (button.h * 2.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), volume_up_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	controls_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 2.7, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(controls_button);
	back_option_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 1.5, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_option_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), App->scene_manager->language->english, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), App->scene_manager->language->spanish, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	back_option_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_option_button->position.first + (button.w / 2), back_option_button->position.second + (button.h / 2), App->scene_manager->language->back, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, controls_button->position.first + (button.w / 2), controls_button->position.second + (button.h / 2), App->scene_manager->language->controls, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), english_button->position.second - (button.h / 2), App->scene_manager->language->choose_language, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), volume_up_button->position.second - (button.h / 2), App->scene_manager->language->volume, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	
	english_button->Select(SELECTED);

}

void MainMenu::CreateControlsMenu()
{
	controls_menu_created = true;

	// Keyboard
	accept_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), (screen_height / 2) - (option_background.h / 2) + (button.h * 1.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(accept_button);
	decline_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), accept_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(decline_button);
	start_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), decline_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(start_button);

	control_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), (screen_height / 2) - (option_background.h / 2) + (button.h), App->scene_manager->language->keyboard, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	accept_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first - (button.w / 5), accept_button->position.second + (button.h / 2), App->scene_manager->language->accept, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	decline_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first - (button.w / 5), decline_button->position.second + (button.h / 2), App->scene_manager->language->decline, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	start_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first - (button.w / 5), start_button->position.second + (button.h / 2), App->scene_manager->language->pause, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	
	accept_label3 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first + (button.w / 2), accept_button->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->keyboard_buttons.buttons_to_use.ACCEPT), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	decline_label3 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first + (button.w / 2), decline_button->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->keyboard_buttons.buttons_to_use.DECLINE), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	start_label3 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first + (button.w / 2), start_button->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->keyboard_buttons.buttons_to_use.PAUSE), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);


	// Gamepad
	accept_button2 = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), start_button->position.second + (button_margin + button.h * 1.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(accept_button2);
	decline_button2 = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), accept_button2->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(decline_button2);
	start_button2 = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 4), decline_button2->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(start_button2);

	control_label2 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), start_button->position.second + (button_margin + button.h), App->scene_manager->language->gamepad, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	accept_label2 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button2->position.first - (button.w / 5), accept_button2->position.second + (button.h / 2), App->scene_manager->language->accept, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	decline_label2 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button2->position.first - (button.w / 5), decline_button2->position.second + (button.h / 2), App->scene_manager->language->decline, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	start_label2 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button2->position.first - (button.w / 5), start_button2->position.second + (button.h / 2), App->scene_manager->language->pause, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	
	accept_label4 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button2->position.first + (button.w / 2), accept_button2->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->gamepad.A), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	decline_label4 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button2->position.first + (button.w / 2), decline_button2->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->gamepad.B), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	start_label4 = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button2->position.first + (button.w / 2), start_button2->position.second + (button.h / 2), SDL_GetScancodeName((SDL_Scancode)App->input->gamepad.START), { 0, 0, 0, 255 }, App->gui_manager->default_font_used);


	back_controls_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), start_button2->position.second + (button_margin + button.h * 1), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_controls_button);
	back_controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_controls_button->position.first + (button.w / 2), back_controls_button->position.second + (button.h / 2), App->scene_manager->language->back, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	accept_button->Select(SELECTED);
}

void MainMenu::CreateCreditsMenu()
{
	credits_menu_created = true;
	for (int i = 0; i < 14; ++i)
	{
		if(i == 0)
			credits_text.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width / 2, (screen_height / 2) - (option_background.h / 2) + 40, App->scene_manager->language->credits_lines.at(i), { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
		else if (i >= 12)
			credits_text.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width / 2, (screen_height / 2) - (option_background.h / 2) + (i * 40) + 70, App->scene_manager->language->credits_lines.at(i), { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
		else
			credits_text.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width / 2 - (screen_width / 9), (screen_height / 2) - (option_background.h / 2) + (i * 40) + 50, App->scene_manager->language->credits_lines.at(i), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false));

	}
	back_credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 1.5, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_credits_button);
	back_credits_button->Select(SELECTED);
	back_credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_credits_button->position.first + (button.w / 2), back_credits_button->position.second + (button.h / 2), App->scene_manager->language->back, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
}

void MainMenu::Delete()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();

	main_menu_created = false;
	option_menu_created = false;
	controls_menu_created = false;
	credits_menu_created = false;
}


void MainMenu::ControlLanguageAndMusic()
{
	if (english_button->has_been_clicked)
	{
		App->scene_manager->language->change_language();
		App->scene_manager->language->SetLanguage(LANGUAGE_TYPE::ENGLISH);
		Delete();
		CreateOptionMenu();
	}
	else if (spanish_button->has_been_clicked)
	{
		App->scene_manager->language->change_language();
		App->scene_manager->language->SetLanguage(LANGUAGE_TYPE::SPANISH);
		Delete();
		CreateOptionMenu();
	}

	if (volume_up_button->has_been_clicked)
	{
		App->audio->VolumeUp();
		volume_up_button->Select(SELECTED);
	}
	else if (volume_down_button->has_been_clicked)
	{
		App->audio->VolumeDown();
		volume_down_button->Select(SELECTED);
	}
}

void MainMenu::ChangeControls(int &control, GUIButton* button, bool keyboard, GUILabel* label) {
	
	if (App->input->Decline())
	{
		isWaittingButt = false;
		button->has_been_clicked = false;
		button->Select(SELECTED);
	}
	else if (keyboard)
	{
		if (isWaittingButt && App->input->scancode != SDL_SCANCODE_UNKNOWN
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.ACCEPT
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.DECLINE
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.PAUSE
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.UP
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.DOWN
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.LEFT
			&& App->input->scancode != App->input->keyboard_buttons.buttons_to_use.RIGHT)
		{
			control = App->input->scancode;
			button->has_been_clicked = false;
			button->Select(SELECTED);
			isWaittingButt = false;
			label->SetText(SDL_GetScancodeName(App->input->scancode));
		}
		else
			isWaittingButt = true;
	}
	else 
	{
		if (isWaittingButt && App->input->scancode != SDL_SCANCODE_UNKNOWN
			&& App->input->scancode != App->input->gamepad.A
			&& App->input->scancode != App->input->gamepad.B
			&& App->input->scancode != App->input->gamepad.START)
		{
			control = App->input->scancode;
			button->has_been_clicked = false;
			button->Select(SELECTED);
			isWaittingButt = false;
			label->SetText(SDL_GetScancodeName(App->input->scancode));
		}
		else
			isWaittingButt = true;
	}
	

	
}

	