#include "Application.h"
#include "ChooseMap.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
#include "ModuleTransitionManager.h"



ChooseMap::ChooseMap(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
ChooseMap::~ChooseMap()
{}


// Called before the first frame
bool ChooseMap::Start()
{
	menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Main_menu_8_bits.ogg");

	return true;
}

// Called before all Updates
bool ChooseMap::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ChooseMap::Update(float _dt)
{
	bool ret = true;
	if (!menu_created)
	{
		menu_created = true;
		CreateMenu();
	}
	Navigate();
	if (App->input->Down() || App->input->Up())
	{
		ChangeMapImage();
	}

	if (back_button->has_been_clicked && !App->scene_manager->changing)
	{
		App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
		App->scene_manager->changing = true;
	}
	else if (map1_button->has_been_clicked && !App->scene_manager->changing)
	{
		App->transition_manager->CreateFadeTransition(2, true, FIRST_BATTLE, Scene_1_color);
		App->scene_manager->changing = true;
	}
	else if (map2_button->has_been_clicked && !App->scene_manager->changing)
	{
		App->transition_manager->CreateFadeTransition(2, true, SECOND_BATTLE, Scene_2_color);
		App->scene_manager->changing = true;
	}
	else if (map3_button->has_been_clicked && !App->scene_manager->changing)
	{
		App->transition_manager->CreateSquaresTransition(4, true, FIRST_BATTLE, Scene_1_color);//change when third battle is implemented
		App->scene_manager->changing = true;
	}


	return ret;
}

// Called before all Updates
bool ChooseMap::PostUpdate()
{
	App->render->Blit(menu_background, 0, 0);

	return true;
}

// Called before quitting
bool ChooseMap::CleanUp()
{
	App->textures->UnLoad(menu_background);
	Delete();
	return true;
}


// Load
bool ChooseMap::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool ChooseMap::Save(pugi::xml_node& _data) const
{
	return true;
}

void ChooseMap::CreateMenu()
{
	map1_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(map1_button);
	map2_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), map1_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(map2_button);
	map3_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), map2_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(map3_button);
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), map3_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);
	map1_button->Select(SELECTED);

	map1_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, map1_button->position.first + (button.w / 2), map1_button->position.second + (button.h / 2), App->scene_manager->language->map1, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	map2_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, map2_button->position.first + (button.w / 2), map2_button->position.second + (button.h / 2), App->scene_manager->language->map2, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	map3_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, map3_button->position.first + (button.w / 2), map3_button->position.second + (button.h / 2), App->scene_manager->language->map3, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), App->scene_manager->language->back, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	map_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 0, 718, 406 });
	frame = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 406, 718, 406 });

}

void ChooseMap::ChangeMapImage()
{
	App->gui_manager->DeleteGUIElement(map_image);
	App->gui_manager->DeleteGUIElement(frame);
	if (map1_button->current_state == SELECTED)
	{
		map_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 0, 718, 406 });
		frame = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 406, 718, 406 });
	}
	else if (map2_button->current_state == SELECTED)
	{
		map_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 812, 718, 406 });
		frame = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 406, 718, 406 });
	}
	else if (map3_button->current_state == SELECTED)
	{
		map_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 1218, 718, 406 });
		frame = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (screen_width / 2) - 718 * 0.5, 100, { 390, 406, 718, 406 });
	}
}

void ChooseMap::Delete()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();

	menu_created = false;
}