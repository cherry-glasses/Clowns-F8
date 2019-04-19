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
#include "ModuleEntityManager.h"
#include "ModuleWindow.h"
#include <string.h>

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

	button = { _config.child("button").attribute("x").as_int(), _config.child("button").attribute("y").as_int(),
		_config.child("button").attribute("w").as_int() , _config.child("button").attribute("h").as_int() };
	small_button = { _config.child("small_button").attribute("x").as_int(), _config.child("small_button").attribute("y").as_int(),
		_config.child("small_button").attribute("w").as_int() , _config.child("small_button").attribute("h").as_int() };
	button_margin = _config.child("button").attribute("margin").as_int();
	cherry_glasses_logo = { _config.child("cherry_glasses_logo").attribute("x").as_int(), _config.child("cherry_glasses_logo").attribute("y").as_int(),
		_config.child("cherry_glasses_logo").attribute("w").as_int() , _config.child("cherry_glasses_logo").attribute("h").as_int() };
	option_background = { _config.child("option_background").attribute("x").as_int(), _config.child("option_background").attribute("y").as_int(),
		_config.child("option_background").attribute("w").as_int() , _config.child("option_background").attribute("h").as_int() };
	iris_portrait = { _config.child("portraits").child("iris").attribute("x").as_int(), _config.child("portraits").child("iris").attribute("y").as_int(),
		_config.child("portraits").child("iris").attribute("w").as_int() , _config.child("portraits").child("iris").attribute("h").as_int() };
	sapphire_portrait = { _config.child("portraits").child("sapphire").attribute("x").as_int(), _config.child("portraits").child("sapphire").attribute("y").as_int(),
		_config.child("portraits").child("sapphire").attribute("w").as_int() , _config.child("portraits").child("sapphire").attribute("h").as_int() };
	storm_portrait = { _config.child("portraits").child("storm").attribute("x").as_int(), _config.child("portraits").child("storm").attribute("y").as_int(),
		_config.child("portraits").child("storm").attribute("w").as_int() , _config.child("portraits").child("storm").attribute("h").as_int() };
	george_b_portrait = { _config.child("portraits").child("george_b").attribute("x").as_int(), _config.child("portraits").child("george_b").attribute("y").as_int(),
		_config.child("portraits").child("george_b").attribute("w").as_int() , _config.child("portraits").child("george_b").attribute("h").as_int() };
	std::pair<int, int> life_margin = { _config.child("life_position").attribute("margin_x").as_int(), _config.child("life_position").attribute("margin_y").as_int() };
	std::pair<int, int> mana_margin = { _config.child("mana_position").attribute("margin_x").as_int(), _config.child("mana_position").attribute("margin_y").as_int() };
	std::pair<int, int> actions_margin = { _config.child("actions_position").attribute("margin_x").as_int(), _config.child("actions_position").attribute("margin_y").as_int() };
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			life_position.push_back({ life_margin.first, life_margin.second});
			mana_position.push_back({ mana_margin.first, mana_margin.second });
			act_menu_position.push_back({ actions_margin.first, actions_margin.second });
			break;
		case 1:
			life_position.push_back({ App->window->GetScreenWidth() - life_margin.first,  life_margin.second });
			mana_position.push_back({ App->window->GetScreenWidth() - mana_margin.first,  mana_margin.second });
			act_menu_position.push_back({ App->window->GetScreenWidth() - actions_margin.first - 259, actions_margin.second });
			break;
		case 2:
			life_position.push_back({ life_margin.first, App->window->GetScreenHeight() - mana_margin.second });
			mana_position.push_back({ mana_margin.first, App->window->GetScreenHeight() - life_margin.second });
			act_menu_position.push_back({ actions_margin.first, App->window->GetScreenHeight() - actions_margin.second -  153});
			break;
		case 3:
			life_position.push_back({ App->window->GetScreenWidth() - life_margin.first, App->window->GetScreenHeight() - mana_margin.second });
			mana_position.push_back({ App->window->GetScreenWidth() - mana_margin.first, App->window->GetScreenHeight() - life_margin.second });
			act_menu_position.push_back({ App->window->GetScreenWidth() - actions_margin.first - 259, App->window->GetScreenHeight() - actions_margin.second - 153});
			break;
		default:
			break;
		}
	}

	//Audio
	press_fx_name = _config.child("press_fx_name").attribute("source").as_string();
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/Main_menu_art_shot.png");
	options_background = App->textures->Load("Assets/Sprites/UI/4259708641.png");
	credits_page = App->textures->Load("Assets/Sprites/UI/credits_done.png");
	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
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
		if (!music_created) {
			music_created = true;
			App->audio->PlayMusic("Main_menu_8_bits.ogg");
		}
		if (!main_menu_created) {
			main_menu_created = true;
			CreateMainMenu();
		}

		App->render->Blit(main_menu_background, 0, 0);

		if (new_game_button->has_been_clicked)
		{
			current_scene = FIRST_BATTLE;
			music_created = false;
			DeleteMenu();
		}
		else if (load_game_button->has_been_clicked)
		{
			current_scene = FIRST_BATTLE;
			music_created = false;
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
		break;

	case Scene::MM_OPTIONS:
		App->render->Blit(main_menu_background, 0, 0);
		App->render->Blit(options_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));

		if (!mm_options_created)
		{
			mm_options_created = true;
			CreateMMOptions();
		}

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
		else if (back_button->has_been_clicked || App->input->Decline())
		{
			mm_options_created = false;
			current_scene = MAIN_MENU;
			DeleteMenu();
		}
		else if(controls_button->has_been_clicked)
		{
			mm_options_created = false;
			current_scene = MM_CONTROLS;
			DeleteMenu();

		}

		Navigate();
		break;

	case Scene::MM_CREDITS:
		App->render->Blit(main_menu_background, 0, 0);
		App->render->Blit(credits_page, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	
		if (mm_credits_created == false)
		{
			back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 2, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
			buttons.push_back(back_button);
			back_button->Select(SELECTED);
			if (language)
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			else
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			mm_credits_created = true;
		}

		if (back_button->has_been_clicked || App->input->Decline())
		{
			current_scene = MAIN_MENU;
			back_button->Select(SELECTED);
			mm_credits_created = false;
			DeleteMenu();
		}

		break;
	case Scene::MM_CONTROLS:
		App->render->Blit(main_menu_background, 0, 0);
		App->render->Blit(options_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));

		if (!mm_controls_menu_created) {
			mm_controls_menu_created = true;
			CreateMMControls();
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
			mm_controls_menu_created = false;
			current_scene = MM_OPTIONS;
			DeleteMenu();
		}
		Navigate();
		break;

	case Scene::FIRST_BATTLE:
		if (!music_created) {
			music_created = true;
			App->audio->PlayMusic("Main_menu_8_bits.ogg");
		}
		if (!map_loaded) {
			map_loaded = true;
			App->map->Load("map_level1.tmx");
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
			App->render->camera.x = App->window->GetScreenWidth() / 2;
			App->render->camera.y = App->window->GetScreenHeight() / 8;

			int i = 0;
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				life_x.push_back((124 * (*character)->current_stats.Hp) / (*character)->default_stats.Hp);
				mana_x.push_back((124 * (*character)->current_stats.Mana) / (*character)->default_stats.Mana);
				life.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, life_position.at(i).first, life_position.at(i).second, { 0, 58, life_x.at(i), 29 }));
				mana.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, mana_position.at(i).first, mana_position.at(i).second, { 0, 86, mana_x.at(i), 29 }));
				++i;
			}
			i = 0;
			for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
			{
				enemies_life_position.push_back((*enemy)->GetPosition());
				enemies_life_x.push_back((64 * (*enemy)->current_stats.Hp) / (*enemy)->default_stats.Hp);
				enemies_life.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, enemies_life_position.at(i).first + screen_width * 0.5 , enemies_life_position.at(i).second + (screen_height / 8) + (*enemy)->position_margin.second, { 0, 58, enemies_life_x.at(i) , 5 }));
				++i;
			}
			port.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 30.0f, 13.0f, iris_portrait));
			port.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1650.0f, 13.0f, sapphire_portrait));
			port.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 30.0f, 904.0f, storm_portrait));
			port.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 1650.0f, 904.0f, george_b_portrait));
			portrait.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 12.0f, { 0, 134, 256, 128 }));
			portrait.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, screen_height - 178, { 0, 134, 256, 128 }));
			portrait.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, 12.0f, { 0, 134, 256, 128 }));
			portrait.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, screen_height - 178, { 0, 134, 256, 128 }));
			character_names.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 210.0f, 55.0f, "Iris", { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
			character_names.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 1832.0f, 55.0f, "Sapphire", { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
			character_names.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 210.0f, 945.0f, "Storm", { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
			character_names.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 1832.0f, 945.0f, "G. Bush", { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
		}
		if (App->input->Pause() || resume_game == false)
		{
			App->map->Draw();
			if (resume_game)
			{
				resume_game = false;
			}
			if (waiting_for_input && !ability_menu_created && !(buttons2.empty()))
			{
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
			}
			else if (ability_menu_created && !(buttons2.empty()))
			{
				App->gui_manager->DeleteGUIElement(ability1_button);
				App->gui_manager->DeleteGUIElement(ability2_button);
				App->gui_manager->DeleteGUIElement(ability3_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
			}
			if(!ingame_options_menu_created)
			{
				ingame_options_menu_created = true;
				CreateOptionsIngame();
			}
			if (english_button->has_been_clicked)
			{
				if (!language)
				{
					language = true;
					DeleteOptionsIngame();
					CreateOptionsIngame();
				}
				english_button->Select(SELECTED);
			}
			else if (spanish_button->has_been_clicked)
			{
				if (language)
				{
					language = false;
					DeleteOptionsIngame();
					CreateOptionsIngame();
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
			else if (resume_button->has_been_clicked || App->input->Decline())
			{
				ingame_options_menu_created = false;
				DeleteOptionsIngame();
				resume_game = true;
				if (waiting_for_input && !ability_menu_created)
				{
					waiting_for_input = false;
				}
				else if (waiting_for_input && ability_menu_created)
				{
					int j = 0;
					for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
					{
						if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
						{
							ability1_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
							buttons2.push_back(ability1_button);
							ability2_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
							buttons2.push_back(ability2_button);
							ability3_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 79, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
							buttons2.push_back(ability3_button);
							ability1_button->Select(SELECTED);
							if (language)
							{
								attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability1_button->position.first + (small_button.w * 0.5), ability1_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_1_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
								ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability2_button->position.first + (small_button.w * 0.5), ability2_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_2_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
								defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability3_button->position.first + (small_button.w * 0.5), ability3_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_3_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							}
							else
							{
								attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability1_button->position.first + (small_button.w * 0.5), ability1_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_1_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
								ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability2_button->position.first + (small_button.w * 0.5), ability2_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_2_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
								defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability3_button->position.first + (small_button.w * 0.5), ability3_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_3_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							}
						}
						++j;
					}
				}
			}
			else if (mm_button->has_been_clicked)
			{
				action_menu_created = false;
				map_loaded = false;
				portraits_created = false;
				resume_game = true;
				ingame_options_menu_created = false;
				current_scene = MAIN_MENU;
				DeleteOptionsIngame();
				DeleteMenu();
				waiting_for_input = false;
				port.clear();
				portrait.clear();
				life.clear();
				mana.clear();
				life_x.clear();
				mana_x.clear();
				enemies_life.clear();
				enemies_life_x.clear();
				enemies_life_position.clear();

				App->map->CleanUp();
				App->entity_manager->CleanUp();
				App->render->camera.x = 0;
				App->render->camera.y = 0;
			}
			Navigate();
			App->render->Blit(options_background, 0 - (option_background.w / 2), (screen_height / 2.7) - (option_background.h / 2));
		}
		else
		{
			int i = 0;
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if (life_x.at(i) != ((124 * (*character)->current_stats.Hp) / (*character)->default_stats.Hp)
					|| mana_x.at(i) != ((124 * (*character)->current_stats.Mana) / (*character)->default_stats.Mana))
				{
					CreatePortraits(*character, i);
				}
				++i;
			}

			i = 0;
			bool change = false;
			for (std::list<Entity*>::iterator enemies = App->entity_manager->enemies.begin(); enemies != App->entity_manager->enemies.end(); ++enemies)
			{
				if (enemies_life_x.at(i) != ((100 * (*enemies)->current_stats.Hp) / (*enemies)->default_stats.Hp)
					|| enemies_life_position.at(i).first != (*enemies)->GetPosition().first ||
				enemies_life_position.at(i).second != (*enemies)->GetPosition().second)
				{
					if(!change) change = true;
					CreateEnemyPortraits(*enemies, i);
				}
				++i;
			}
			if (change)
			{
				enemies_life_position.clear();
				for (std::list<Entity*>::iterator enemies = App->entity_manager->enemies.begin(); enemies != App->entity_manager->enemies.end(); ++enemies)
				{
					enemies_life_position.push_back((*enemies)->GetPosition());
				}
				change = false;
			}
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					ActionsMenu();
				}
			}
			App->map->Draw();
		}
		break;
	}
	
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
	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - ((button_margin * 2) + (button.h * 2) + (button.h /2)), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
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
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2) , new_game_button->position.second + (button.h / 2), "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2) , load_game_button->position.second + (button.h / 2), "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2) , options_button->position.second + (button.h / 2), "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2) , credits_button->position.second + (button.h / 2), "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2) , exit_button->position.second + (button.h / 2), "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2) , new_game_button->position.second + (button.h / 2), "NUEVA PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2) , load_game_button->position.second + (button.h / 2), "CARGAR PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2) , options_button->position.second + (button.h / 2), "OPCIONES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2) , credits_button->position.second + (button.h / 2), "CR�DITOS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2) , exit_button->position.second + (button.h / 2), "SALIR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, cherry_glasses_logo_button->position.first + (button.w / 2) - (cherry_glasses_logo.w / 2) , cherry_glasses_logo_button->position.second + (button.h / 2) - (cherry_glasses_logo.h / 2), { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);
}

void Scene::CreateMMOptions()
{
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
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 1.5, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);
	
	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	if (language)
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "ENGLISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "SPANISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, controls_button->position.first + (button.w / 2), controls_button->position.second + (button.h / 2), "CONTROLS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), english_button->position.second - (button.h / 2), "CHOOSE LANGUAGE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), volume_up_button->position.second - (button.h / 2), "ADJUST VOLUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "INGL�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "ESPA�OL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, controls_button->position.first + (button.w / 2), controls_button->position.second + (button.h / 2), "CONTROLES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, english_button->position.second - (button.h / 2), "ESCOJER LENGUA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, volume_up_button->position.second - (button.h / 2), "AJUSTAR VOLUMEN", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	english_button->Select(SELECTED);

}

void Scene::CreateOptionsIngame()
{
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 1.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), english_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), spanish_button->position.second + (button.h * 2), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), volume_up_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	resume_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 3.1, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(resume_button);
	mm_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 1.7, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(mm_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	if (language)
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "ENGLISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "SPANISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		mm_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, mm_button->position.first + (button.w / 2), mm_button->position.second + (button.h / 2), "MAIN MENU", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		resume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, resume_button->position.first + (button.w / 2), resume_button->position.second + (button.h / 2), "RESUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), english_button->position.second - (button.h / 2), "CHOOSE LANGUAGE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), volume_up_button->position.second - (button.h / 2), "ADJUST VOLUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "INGL�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "ESPA�OL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		mm_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, mm_button->position.first + (button.w / 2), mm_button->position.second + (button.h / 2), "MEN� PRINCIPAL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		resume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, resume_button->position.first + (button.w / 2), resume_button->position.second + (button.h / 2), "CONTINUAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, english_button->position.second - (button.h / 2), "ESCOJER LENGUA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, volume_up_button->position.second - (button.h / 2), "AJUSTAR VOLUMEN", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	english_button->Select(SELECTED);
}

void Scene::DeleteOptionsIngame()
{
	App->gui_manager->DeleteGUIElement(english_button);
	App->gui_manager->DeleteGUIElement(spanish_button);
	App->gui_manager->DeleteGUIElement(volume_up_button);
	App->gui_manager->DeleteGUIElement(volume_down_button);
	App->gui_manager->DeleteGUIElement(resume_button);
	App->gui_manager->DeleteGUIElement(mm_button);
	App->gui_manager->DeleteGUIElement(volume_up_label);
	App->gui_manager->DeleteGUIElement(volume_down_label);
	App->gui_manager->DeleteGUIElement(english_label);
	App->gui_manager->DeleteGUIElement(spanish_label);
	App->gui_manager->DeleteGUIElement(mm_label);
	App->gui_manager->DeleteGUIElement(resume_label);
	App->gui_manager->DeleteGUIElement(language_label);
	App->gui_manager->DeleteGUIElement(volume_label);
	buttons.clear();
}

void Scene::CreateMMControls()
{
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
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), select_button->position.second + (button_margin + button.h*1), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
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

void Scene::CreatePortraits(Entity* _character, int _i)
{
	App->gui_manager->DeleteGUIElement(life.at(_i));
	App->gui_manager->DeleteGUIElement(mana.at(_i));
		
	life_x.at(_i) = (124 * _character->current_stats.Hp) / _character->default_stats.Hp;
	mana_x.at(_i) = (124 * _character->current_stats.Mana) / _character->default_stats.Mana;
	life.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, life_position.at(_i).first, life_position.at(_i).second, { 0, 58, life_x.at(_i), 29 });
	mana.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, mana_position.at(_i).first, mana_position.at(_i).second, { 0, 86, mana_x.at(_i), 29 });
}

void Scene::CreateEnemyPortraits(Entity* _enemy, int _i)
{
	App->gui_manager->DeleteGUIElement(enemies_life.at(_i));
	enemies_life_x.at(_i) = (64 * _enemy->current_stats.Hp) / _enemy->default_stats.Hp;
	enemies_life.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, _enemy->GetPosition().first + screen_width * 0.5, _enemy->GetPosition().second + (screen_height / 8) + _enemy->position_margin.second, { 0, 58, enemies_life_x.at(_i) , 5 });
}

void Scene::ActionsMenu()
{
	if (resume_game)
	{
		if (!waiting_for_input)
		{
			int i = 0;
			waiting_for_input = true;
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					attack_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
					buttons2.push_back(attack_button);
					ability_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
					buttons2.push_back(ability_button);
					defend_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second + 78, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
					buttons2.push_back(defend_button);
					attack_button->Select(SELECTED);
					if (language)
					{
						attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, attack_button->position.first + (small_button.w * 0.5), attack_button->position.second + (small_button.h * 0.5), "ATTACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
						ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability_button->position.first + (small_button.w * 0.5), ability_button->position.second + (small_button.h * 0.5), "ABILITY", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
						defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, defend_button->position.first + (small_button.w * 0.5), defend_button->position.second + (small_button.h * 0.5), "DEFEND", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

					}
					else
					{
						attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, attack_button->position.first + (small_button.w * 0.5), attack_button->position.second + (small_button.h * 0.5), "ATACAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
						ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability_button->position.first + (small_button.w * 0.5), ability_button->position.second + (small_button.h * 0.5), "HABILIDAD", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
						defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, defend_button->position.first + (small_button.w * 0.5), defend_button->position.second + (small_button.h * 0.5), "DEFENSA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
					}
				}
				++i;
			}
		}
		//navigate for buttons2
		if (App->input->Down())
		{
			for (std::list<GUIButton*>::const_iterator it_vector = buttons2.begin(); it_vector != buttons2.end(); ++it_vector)
			{
				if ((*it_vector)->current_state == SELECTED) {
					if ((*it_vector) != buttons2.back()) {
						(*it_vector)->Select(NORMAL);
						it_vector++;
						(*it_vector)->Select(SELECTED);
						break;
					}
					else
					{
						(*it_vector)->Select(NORMAL);
						it_vector = buttons2.begin();
						(*it_vector)->Select(SELECTED);
					}
				}
			}
		}
		if (App->input->Up())
		{
			for (std::list<GUIButton*>::const_iterator it_vector = buttons2.begin(); it_vector != buttons2.end(); ++it_vector)
			{
				if ((*it_vector)->current_state == SELECTED) {
					if ((*it_vector) != buttons2.front()) {
						(*it_vector)->Select(NORMAL);
						it_vector--;
						(*it_vector)->Select(SELECTED);

						break;
					}
					else
					{
						(*it_vector)->Select(NORMAL);
						it_vector = buttons2.end();
						it_vector--;
						(*it_vector)->Select(SELECTED);
					}
				}
			}
		}
		if (!ability_menu_created)
		{
			if (attack_button->has_been_clicked)
			{
				//Put attack function
				waiting_for_input = false;
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						(*character)->current_turn = Entity::TURN::SEARCH_ATTACK;
					}
				}

			}
			else if (ability_button->has_been_clicked)
			{
				//Put ability function
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				int j = 0;
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						ability1_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
						buttons2.push_back(ability1_button);
						ability2_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
						buttons2.push_back(ability2_button);
						ability3_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 79, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
						buttons2.push_back(ability3_button);
						ability1_button->Select(SELECTED);
						if (language)
						{
							attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability1_button->position.first + (small_button.w * 0.5), ability1_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_1_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability2_button->position.first + (small_button.w * 0.5), ability2_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_2_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability3_button->position.first + (small_button.w * 0.5), ability3_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_3_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

						}
						else
						{
							attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability1_button->position.first + (small_button.w * 0.5), ability1_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_1_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability2_button->position.first + (small_button.w * 0.5), ability2_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_2_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
							defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability3_button->position.first + (small_button.w * 0.5), ability3_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Habilidad_3_nombre, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
						}
					}
					++j;
				}
				ability_menu_created = true;
			}
			else if (defend_button->has_been_clicked)
			{
				//Put defend function
				waiting_for_input = false;
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						(*character)->current_turn = Entity::TURN::DEFEND;
					}
				}
			}
		}
		else if (ability_menu_created)
		{
			if (ability1_button->has_been_clicked)
			{
				//Put attack function
				waiting_for_input = false;
				ability_menu_created = false;
				App->gui_manager->DeleteGUIElement(ability1_button);
				App->gui_manager->DeleteGUIElement(ability2_button);
				App->gui_manager->DeleteGUIElement(ability3_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_1;
					}
				}
			}
			else if (ability2_button->has_been_clicked)
			{
				//Put attack function
				waiting_for_input = false;
				ability_menu_created = false;
				App->gui_manager->DeleteGUIElement(ability1_button);
				App->gui_manager->DeleteGUIElement(ability2_button);
				App->gui_manager->DeleteGUIElement(ability3_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_1;
					}
				}
			}
			else if (ability3_button->has_been_clicked)
			{
				//Put attack function
				waiting_for_input = false;
				ability_menu_created = false;
				App->gui_manager->DeleteGUIElement(ability1_button);
				App->gui_manager->DeleteGUIElement(ability2_button);
				App->gui_manager->DeleteGUIElement(ability3_button);
				App->gui_manager->DeleteGUIElement(attack_label);
				App->gui_manager->DeleteGUIElement(ability_label);
				App->gui_manager->DeleteGUIElement(defend_label);
				buttons2.clear();
				for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
				{
					if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
					{
						(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_1;
					}
				}
			}
		}
	}
}

void Scene::DeleteMenu()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
	main_menu_created = false;
}

void Scene::Navigate()
{
	if (App->input->Down())
	{
		NavigateDown();
	}
	if (App->input->Up())
	{
		NavigateUp();
	}
}
void Scene::NavigateDown() 
{
	App->audio->PlayFx(1, 0);
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
	App->audio->PlayFx(1, 0);
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


