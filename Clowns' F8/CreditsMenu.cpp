#include "Application.h"
#include "CreditsMenu.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"



CreditsMenu::CreditsMenu(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
CreditsMenu::~CreditsMenu()
{}


// Called before the first frame
bool CreditsMenu::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");
	credits_menu_background = App->textures->Load("Assets/Sprites/UI/credits_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
	return true;
}

// Called before all Updates
bool CreditsMenu::PreUpdate()
{
	if (!credits_menu_created) {
		CreateCreditsMenu();
	}
	return true;
}

// Called each loop iteration
bool CreditsMenu::Update(float _dt)
{
	bool ret = true;

	if (back_button->has_been_clicked || App->input->Decline())
	{
		App->scene_manager->ChangeScene(MAIN_MENU);
	}

	return ret;
}

// Called before all Updates
bool CreditsMenu::PostUpdate()
{
	App->render->Blit(main_menu_background, 0, 0);
	App->render->Blit(credits_menu_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	return true;
}

// Called before quitting
bool CreditsMenu::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->textures->UnLoad(credits_menu_background);
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
	credits_menu_created = false;
	return true;
}

// Load
bool CreditsMenu::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool CreditsMenu::Save(pugi::xml_node& _data) const
{
	return true;
}

void CreditsMenu::CreateCreditsMenu()
{
	credits_menu_created = true;
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 2, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);
	back_button->Select(SELECTED);
	if (language)
	{
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATRÁS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
}




