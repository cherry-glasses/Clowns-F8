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
	UI_maps_size.first = _config.child("UI_maps").attribute("w").as_int();
	UI_maps_size.second = _config.child("UI_maps").attribute("h").as_int();
	margin = _config.child("UI_maps").attribute("margin").as_int();
}

// Destructor
ChooseMap::~ChooseMap()
{}


// Called before the first frame
bool ChooseMap::Start()
{
	menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");
	maps_texture = App->textures->Load("Assets/Sprites/UI/UI_sprites.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();
	alpha = { 0, 0, screen_width, screen_height };
	map1 = { 390, 0, 750, 398 };
	map2 = { 390, 404, 760, 388 };
	map3 = { 390, 792, 763, 421 };
	map4 = { 390, 1213, 760, 388 };
	App->audio->PlayMusic("Main_menu_8_bits.ogg");
	map_selected = 1;
	CreateMenu();
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

	if (App->input->Right())
	{
		App->audio->PlayFx(1, 0);
		if (map_selected < 4) ++map_selected;
		else map_selected = 1;
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();
	}
	if (App->input->Left())
	{
		App->audio->PlayFx(1, 0);
		if (map_selected > 1) --map_selected;
		else map_selected = 4;
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();
	} 
	App->render->Blit(menu_background, 0, 0);
	if (map_selected != 1) App->render->Blit(maps_texture, 0, screen_height * 0.5 - UI_maps_size.second * 0.66, &map1);
	if (map_selected != 2) App->render->Blit(maps_texture, UI_maps_size.first * 0.5 + margin, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin*3, &map2);
	if (map_selected != 3) App->render->Blit(maps_texture, UI_maps_size.first + margin * 2, screen_height * 0.5 - UI_maps_size.second * 0.66, &map3);
	if (map_selected != 4) App->render->Blit(maps_texture, UI_maps_size.first * 1.5 + margin * 3, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin*3, &map4);
	App->render->DrawQuad(alpha, 0, 0, 0, 220);
	switch (map_selected)
	{
	case 1:
		App->render->Blit(maps_texture, 0, screen_height * 0.5 - UI_maps_size.second * 0.66, &map1);
		break;
	case 2:
		App->render->Blit(maps_texture, UI_maps_size.first * 0.5 + margin, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin * 3, &map2);
		break;
	case 3:
		App->render->Blit(maps_texture, UI_maps_size.first + margin * 2, screen_height * 0.5 - UI_maps_size.second * 0.66, &map3);
		break;
	case 4:
		App->render->Blit(maps_texture, UI_maps_size.first * 1.5 + margin * 3, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin * 3, &map4);
		break;
	}

	if (App->input->Accept() && !App->scene_manager->changing)
	{
		switch (map_selected)
		{
		case 1:
			App->transition_manager->CreateSquaresTransition(2, true, FIRST_BATTLE, Scene_1_color);
			App->scene_manager->changing = true;
			break;
		case 2:
			App->transition_manager->CreateSquaresTransition(2, true, SECOND_BATTLE, Scene_2_color);
			App->scene_manager->changing = true;
			break;
		case 3:
			App->transition_manager->CreateSquaresTransition(2, true, THIRD_BATTLE, Scene_3_color);
			App->scene_manager->changing = true;
			break;
		case 4:
			App->transition_manager->CreateSquaresTransition(2, true, FOURTH_BATTLE, Scene_4_color);
			App->scene_manager->changing = true;
			break;
		}
	}
	if (App->input->Decline() && !App->scene_manager->changing)
	{
		App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
		App->scene_manager->changing = true;
	}

	return ret;
}

// Called before all Updates
bool ChooseMap::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool ChooseMap::CleanUp()
{
	App->textures->UnLoad(menu_background);
	App->textures->UnLoad(maps_texture);
	App->gui_manager->DeleteAllGUIElements();
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
	switch (map_selected)
	{
	case 1:
		map1_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, UI_maps_size.first * 0.5, screen_height * 0.5 - UI_maps_size.second * 0.66 - 30, App->scene_manager->language->map1, { 194, 213, 231, 255 }, App->gui_manager->default_font_used);
		break;
	case 2:	
		map2_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, UI_maps_size.first * 0.5 + margin + UI_maps_size.first * 0.5, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin * 3 + UI_maps_size.second + 15, App->scene_manager->language->map2, { 178, 2, 2, 255 }, App->gui_manager->default_font_used);
		break;
	case 3:
		map3_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, UI_maps_size.first + margin * 2 + UI_maps_size.first * 0.5, screen_height * 0.5 - UI_maps_size.second * 0.66 - 30, App->scene_manager->language->map3, { 226, 232, 234, 255 }, App->gui_manager->default_font_used);
		break;
	case 4:
		map4_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, UI_maps_size.first * 1.5 + margin * 3 + UI_maps_size.first * 0.5, (screen_height * 0.5 - UI_maps_size.second * 0.66) + UI_maps_size.second * 0.5 + margin * 3 + UI_maps_size.second + 15, App->scene_manager->language->map4, { 254, 0, 114, 255 }, App->gui_manager->default_font_used);
		break;
	}
}