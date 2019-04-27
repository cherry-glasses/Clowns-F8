#include "Application.h"
#include "ControlsMenu.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"


ControlsMenu::ControlsMenu(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
ControlsMenu::~ControlsMenu()
{}


// Called before the first frame
bool ControlsMenu::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");
	controls_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
	return true;
}

// Called before all Updates
bool ControlsMenu::PreUpdate()
{
	if (!controls_menu_created) {
		CreateControlsMenu();
	}
	return true;
}

// Called each loop iteration
bool ControlsMenu::Update(float _dt)
{
	bool ret = true;

	if (!controls_menu_created) {
		CreateControlsMenu();
	}
	if (accept_button->has_been_clicked) {

	}
	if (decline_button->has_been_clicked) {

	}
	if (character_stats_button->has_been_clicked) {

	}
	if (characeter_abilities_button->has_been_clicked) {

	}
	if (abilities_button->has_been_clicked) {

	}
	if (start_button->has_been_clicked) {

	}
	if (select_button->has_been_clicked) {

	}

	else if (back_button->has_been_clicked || App->input->Decline())
	{
		App->scene_manager->ChangeScene(OPTION_MENU);
	}

	Navigate();

	return ret;
}

// Called before all Updates
bool ControlsMenu::PostUpdate()
{
	App->render->Blit(main_menu_background, 0, 0);
	App->render->Blit(controls_menu_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	return true;
}

// Called before quitting
bool ControlsMenu::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->textures->UnLoad(controls_menu_background);
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
	controls_menu_created = false;
	return true;
}


// Load
bool ControlsMenu::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool ControlsMenu::Save(pugi::xml_node& _data) const
{
	return true;
}

void ControlsMenu::CreateControlsMenu()
{
	controls_menu_created = true;
	accept_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(accept_button);
	decline_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), accept_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(decline_button);
	character_stats_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), decline_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(character_stats_button);
	characeter_abilities_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), character_stats_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(characeter_abilities_button);
	abilities_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), characeter_abilities_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(abilities_button);
	start_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), abilities_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(start_button);
	select_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), start_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(select_button);
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), select_button->position.second + (button_margin + button.h * 1), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);

	if (language)
	{
		accept_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first + (button.w / 2), accept_button->position.second + (button.h / 2), "ACCEPT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		decline_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first + (button.w / 2), decline_button->position.second + (button.h / 2), "DECLINE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_stats_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, character_stats_button->position.first + (button.w / 2), character_stats_button->position.second + (button.h / 2), "CHARACTER STATS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_abilites_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, characeter_abilities_button->position.first + (button.w / 2), characeter_abilities_button->position.second + (button.h / 2), "CHARACTER ABILITIES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		abilities_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, abilities_button->position.first + (button.w / 2), abilities_button->position.second + (button.h / 2), "ABILITIES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		start_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first + (button.w / 2), start_button->position.second + (button.h / 2), "PAUSE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		select_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, select_button->position.first + (button.w / 2), select_button->position.second + (button.h / 2), "SELECT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	}
	else
	{
		accept_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first + (button.w / 2), accept_button->position.second + (button.h / 2), "ACEPTAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		decline_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first + (button.w / 2), decline_button->position.second + (button.h / 2), "DECLINAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_stats_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, character_stats_button->position.first + (button.w / 2), character_stats_button->position.second + (button.h / 2), "STATS PERSONAJES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_abilites_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, characeter_abilities_button->position.first + (button.w / 2), characeter_abilities_button->position.second + (button.h / 2), "HAB PERSONAJES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		abilities_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, abilities_button->position.first + (button.w / 2), abilities_button->position.second + (button.h / 2), "HABILIDADES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		start_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first + (button.w / 2), start_button->position.second + (button.h / 2), "PAUSA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		select_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, select_button->position.first + (button.w / 2), select_button->position.second + (button.h / 2), "SELECT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATRAS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	}
	accept_button->Select(SELECTED);
}



