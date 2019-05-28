#include "Application.h"
#include "Battle.h"
#include "ModuleSceneManager.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"
#include "ModuleRender.h"
#include "ModuleTransitionManager.h"
#include "Language.h"


Battle::Battle(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{
	App->scene_manager->objects_texture = App->textures->Load(_config.child("texture").attribute("value").as_string());
	press_sfx = App->audio->LoadFx(_config.child("press_fx_name").attribute("source").as_string());

	life_margin = { _config.child("life_position").attribute("margin_x").as_int(), _config.child("life_position").attribute("margin_y").as_int() };
	mana_margin = { _config.child("mana_position").attribute("margin_x").as_int(), _config.child("mana_position").attribute("margin_y").as_int() };
	actions_margin = { _config.child("actions_position").attribute("margin_x").as_int(), _config.child("actions_position").attribute("margin_y").as_int() };
	port_margin = { _config.child("port_position").attribute("margin_x").as_int(), _config.child("port_position").attribute("margin_y").as_int() };
	portrait_margin = { _config.child("portrait_position").attribute("margin_x").as_int(), _config.child("portrait_position").attribute("margin_y").as_int() };
	name_margin = { _config.child("name_position").attribute("margin_x").as_int(), _config.child("name_position").attribute("margin_y").as_int() };
	level_margin = { _config.child("level_position").attribute("margin_x").as_int(), _config.child("level_position").attribute("margin_y").as_int() };

	board = { _config.child("board").attribute("x").as_int(), _config.child("board").attribute("y").as_int(),
		_config.child("board").attribute("w").as_int(), _config.child("board").attribute("h").as_int() };
	port_board = { _config.child("board").child("port").attribute("margin_x").as_int(),  _config.child("board").child("port").attribute("margin_y").as_int() };
	name_board = { _config.child("board").child("name").attribute("margin_x").as_int(),  _config.child("board").child("name").attribute("margin_y").as_int() };
	life_board = { _config.child("board").child("life").attribute("margin_x").as_int(),  _config.child("board").child("life").attribute("margin_y").as_int() };
	mana_board = { _config.child("board").child("mana").attribute("margin_x").as_int(),  _config.child("board").child("mana").attribute("margin_y").as_int() };
	attack_board = { _config.child("board").child("attack").attribute("margin_x").as_int(),  _config.child("board").child("attack").attribute("margin_y").as_int() };
	ability_1_board = { _config.child("board").child("ability_1").attribute("margin_x").as_int(),  _config.child("board").child("ability_1").attribute("margin_y").as_int() };
	ability_2_board = { _config.child("board").child("ability_2").attribute("margin_x").as_int(),  _config.child("board").child("ability_2").attribute("margin_y").as_int() };
	ability_3_board = { _config.child("board").child("ability_3").attribute("margin_x").as_int(),  _config.child("board").child("ability_3").attribute("margin_y").as_int() };

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			portrait_position.push_back({ portrait_margin.first - screen_width / 2, portrait_margin.second - screen_height / 8 });
			act_menu_position.push_back({ portrait_position.at(i).first + actions_margin.first, portrait_position.at(i).second + actions_margin.second });
			break;
		case 1:
			portrait_position.push_back({ screen_width / 2 - portrait_margin.first - 259, portrait_margin.second - screen_height / 8 });
			act_menu_position.push_back({ portrait_position.at(i).first + actions_margin.first, portrait_position.at(i).second + actions_margin.second });
			break;
		case 2:
			portrait_position.push_back({ portrait_margin.first - screen_width / 2, screen_height - 300 });
			act_menu_position.push_back({ portrait_position.at(i).first + actions_margin.first, portrait_position.at(i).second - actions_margin.second });
			break;
		case 3:
			portrait_position.push_back({ screen_width / 2 - portrait_margin.first - 259, screen_height - 300 });
			act_menu_position.push_back({ portrait_position.at(i).first + actions_margin.first, portrait_position.at(i).second - actions_margin.second });
			break;
		default:
			break;
		}
	}
}

// Destructor
Battle::~Battle()
{}

bool Battle::Update(float _dt)
{
	bool ret = true;

	if (App->input->Pause() || ingame_options_menu_created)
	{
		App->render->Blit(battle_menu_background, 0 - (option_background.w / 2), (screen_height / 2.7) - (option_background.h / 2));
		if (!ingame_options_menu_created)
		{
			CreateOptionsIngame();
			if (waiting_for_input)
			{
				DeleteAttackMenu();
			}
			else if (ability_menu_created)
			{
				DeleteAbilitiesMenu();
			}
		}

		ControlLanguageAndMusic();

		if (resume_button->has_been_clicked || App->input->Decline())
		{

			DeleteOptionsIngame();
			int i = 0;
			for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
			{
				UpdateEnemyPortraits((*enemy), i);
				++i;
			}

		}
		else if (main_menu_button->has_been_clicked)
		{
			if (!App->scene_manager->changing)
			{
				App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
				App->scene_manager->changing = true;
			}
		}

		Navigate();
	}
	else
	{
		if (!App->entity_manager->ThereAreCharAlive()) {
			if (!App->scene_manager->changing)
			{
				App->transition_manager->CreateFadeTransition(2, true, LOSE_SCENE, Black);
				App->scene_manager->changing = true;
			}
		}
		else if (App->entity_manager->enemies.empty()) {
			if (!App->scene_manager->changing)
			{
				switch (type)
				{
				case FIRST_BATTLE:
					App->scene_manager->battle1_passed = true;
					App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, White);
					break;
				case SECOND_BATTLE:
					App->scene_manager->battle2_passed = true;
					App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, White);
					break;
				case THIRD_BATTLE:
					App->scene_manager->battle3_passed = true;
					App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, White);
					break;
				case FOURTH_BATTLE:
					App->transition_manager->CreateFadeTransition(2, true, WIN_SCENE, White);
					break;
				default:
					break;
				}
				App->scene_manager->changing = true;
			}
		}

		if(App->input->LevelUp())
		{
			LevelUp(true);
		}
		else if(App->input->LevelDown())
		{
			LevelUp(false);
		}

		UpdateCharacters();
		UpdateEnemies();
		
		for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
		{
			if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
			{
				ActionsMenu();
			}
			if ((*character)->current_turn != Entity::TURN::NONE)
			{
				for (std::list<Entity*>::iterator entity = App->entity_manager->entities.begin(); entity != App->entity_manager->entities.end(); ++entity)
				{
					if ((*entity)->GetPosition() == (*character)->target && aux_target != (*character)->target
						&& !(std::find(App->entity_manager->objects.begin(), App->entity_manager->objects.end(), (*entity)) != App->entity_manager->objects.end()))
					{
						aux_target = (*character)->target;
						ShowEntityInfo((*entity));
					}
					else if(aux_target != (*character)->target)
					{
						DeleteEntityInfo();
					}
				}
				aux_target = (*character)->target;
			}
			
		}
		

		App->render->Blit(battle_background, 0 - (screen_width / 2), 0 - (screen_height / 8));
		App->render->Blit(battle_grid, 0 - (screen_width / 2), 0 - (screen_height / 8));
		App->map->Draw();
	}

	return ret;
}

bool Battle::CleanUp()
{
	App->textures->UnLoad(battle_menu_background);
	App->textures->UnLoad(battle_background);
	App->textures->UnLoad(battle_grid);
	App->textures->UnLoad(App->scene_manager->objects_texture);
	App->gui_manager->DeleteAllGUIElements();

	App->map->CleanUp();
	App->audio->UnloadAllFx();
	App->entity_manager->CleanUp();
	return true;
}

// CREATES -----------------------------------------------------------------------
void Battle::CreateUIBattle()
{
	App->entity_manager->OrderEntitiesByAgility();
	int i = 0;
	for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
	{
		life_x.push_back((124 * (*character)->current_stats.Hp) / (*character)->default_stats.Hp);
		mana_x.push_back((124 * (*character)->current_stats.Mana) / (*character)->default_stats.Mana);
		life.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first + life_margin.first, portrait_position.at(i).second + life_margin.second, { 0, 58, life_x.at(i), 29 }));
		mana.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first + mana_margin.first, portrait_position.at(i).second + mana_margin.second, { 0, 86, mana_x.at(i), 29 }));
		portrait.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first, portrait_position.at(i).second, { 0, 134, 256, 128 }));
		life_numbers.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, life.at(i)->position.first + 60, life.at(i)->position.second + 11, "0", { 100, 255, 100, 255 }, App->gui_manager->default_font_used, (*character)->current_stats.Hp, (*character)->default_stats.Hp));
		mana_numbers.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, mana.at(i)->position.first + 60, mana.at(i)->position.second + 13, "0", { 255, 200, 255, 255 }, App->gui_manager->default_font_used, (*character)->current_stats.Mana, (*character)->default_stats.Mana));
		stun_image.push_back(nullptr);
		defense_image.push_back(nullptr);
		stun_image_created.push_back(false);
		defense_image_created.push_back(false);
		turn_created.push_back(false);

		character_levels.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, portrait_position.at(i).first + level_margin.first, portrait_position.at(i).second + level_margin.second, "Lvl: " + std::to_string((*character)->level), { 255, 255, 0, 255 }, App->gui_manager->default_font_used));
		character_names.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, portrait_position.at(i).first + name_margin.first - (*character)->name.size(), portrait_position.at(i).second + name_margin.second, (*character)->name.c_str(), { 0, 0, 0, 255 }, App->gui_manager->default_font_used));
		port.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first + port_margin.first, portrait_position.at(i).second + port_margin.second, (*character)->portrait));
		
		++i;
	}
	i = 0;
	for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
	{
		enemy_stun_image.push_back(nullptr);
		enemy_stun_image_created.push_back(false);
		enemies_life_x.push_back((64 * (*enemy)->current_stats.Hp) / (*enemy)->default_stats.Hp);
		enemies_life.push_back((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (*enemy)->GetPosition().first, (*enemy)->GetPosition().second + (*enemy)->position_margin.second - (*enemy)->current.h, { 0, 58, enemies_life_x.at(i) , 5 }));
		++i;
	}
}

void Battle::CreateAttackMenu()
{
	waiting_for_input = true;
	int i = 0;
	for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
	{
		if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
		{
			attack_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
			buttons2.push_back(attack_button);
			if ((*character)->level >= 2)
			{
				character_ability1up = true;
				ability_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons2.push_back(ability_button);
				defend_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second + 78, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				
			}
			else
			{
				defend_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(i).first, act_menu_position.at(i).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				character_ability1up = false;
			}
			buttons2.push_back(defend_button);
			attack_button->Select(SELECTED);
			if ((*character)->level >= 2)
				ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability_button->position.first + (small_button.w * 0.5), ability_button->position.second + (small_button.h * 0.5), App->scene_manager->language->battle_abilities, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, defend_button->position.first + (small_button.w * 0.5), defend_button->position.second + (small_button.h * 0.5), App->scene_manager->language->defend, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, attack_button->position.first + (small_button.w * 0.5), attack_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Attack_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			
		}
		++i;
	}
}

void Battle::CreateAbilitiesMenu()
{
	ability_menu_created = true;
	int j = 0;
	for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
	{
		if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
		{
			if ((*character)->level >= 2)
			{
				character_ability1up = true;
				ability1_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons2.push_back(ability1_button);
			}
			else
				character_ability1up = false;
			if ((*character)->level >= 4)
			{
				character_ability2up = true;
				ability2_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 39, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons2.push_back(ability2_button);
			}
			else
				character_ability2up = false;
			if ((*character)->level >= 7)
			{
				character_ability3up = true;
				ability3_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, act_menu_position.at(j).first, act_menu_position.at(j).second + 79, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons2.push_back(ability3_button);
			}
			else
				character_ability3up = false;

			ability1_button->Select(SELECTED);
			
			attack_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability1_button->position.first + (small_button.w * 0.5), ability1_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_1_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			if ((*character)->level >= 4)
				ability_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability2_button->position.first + (small_button.w * 0.5), ability2_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_2_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			if ((*character)->level >= 7)
				defend_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, ability3_button->position.first + (small_button.w * 0.5), ability3_button->position.second + (small_button.h * 0.5), (*character)->attacks_names.Ability_3_name, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

			
		}
		++j;
	}
}

void Battle::CreateOptionsIngame()
{
	ingame_options_menu_created = true;
	App->entity_manager->paused = true;
	int i = 0;
	for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
	{
		UpdateEnemyPortraits(nullptr, i);
		++i;
	}
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2) , (screen_height / 2) - (option_background.h / 2) + (button.h * 1.5) - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2) , english_button->position.second + (button_margin * 1.5 + button.h * 2.5) - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2) , spanish_button->position.second + (button_margin + button.h * 3.5) - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2), volume_up_button->position.second + (button_margin * 1.5 + button.h * 2.5) - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	resume_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2) , (screen_height / 2) + (option_background.h / 2) - button.h * 3.1 - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(resume_button);
	main_menu_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 0 - (button.w / 2) , (screen_height / 2) + (option_background.h / 2) - button.h * 1.7 - screen_height / 8, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(main_menu_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), App->scene_manager->language->english, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), App->scene_manager->language->spanish, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	main_menu_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, main_menu_button->position.first + (button.w / 2), main_menu_button->position.second + (button.h / 2), App->scene_manager->language->main_menu, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	resume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, resume_button->position.first + (button.w / 2), resume_button->position.second + (button.h / 2), App->scene_manager->language->resume, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 0, english_button->position.second + (button.h * 1.5) - screen_height / 8, App->scene_manager->language->choose_language, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 0, volume_up_button->position.second + (button.h * 1.5) - screen_height / 8, App->scene_manager->language->volume, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	
	english_button->Select(SELECTED);
}

// UPDATES ---------------------------------------------------------------------------------------------

void Battle::UpdateCharacters()
{
	int i = 0;
	for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
	{
		if (life_x.at(i) != ((124 * (*character)->current_stats.Hp) / (*character)->default_stats.Hp)
			|| mana_x.at(i) != ((124 * (*character)->current_stats.Mana) / (*character)->default_stats.Mana))
		{
			UpdateCharacterPortraits(*character, i);
		}

		if ((*character)->current_turn != Entity::TURN::NONE && !turn_created.at(i)) {
			App->gui_manager->DeleteGUIElement(portrait.at(i));
			portrait.at(i) = ((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first, portrait_position.at(i).second, { 0, 264, 256, 128 }));
			turn_created.at(i) = true;
		}
		else if ((*character)->current_turn == Entity::TURN::NONE && turn_created.at(i)) {
			App->gui_manager->DeleteGUIElement(portrait.at(i));
			portrait.at(i) = ((GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first, portrait_position.at(i).second, { 0, 134, 256, 128 }));
			turn_created.at(i) = false;
		}

		if ((*character)->stunned && !stun_image_created.at(i))
		{
			stun_image.at(i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first + 100, portrait_position.at(i).second + 70, { 0, 115, 16, 16 });
			stun_image_created.at(i) = true;
		}
		else if (!(*character)->stunned && stun_image_created.at(i))
		{
			App->gui_manager->DeleteGUIElement(stun_image.at(i));
			stun_image_created.at(i) = false;
		}
		if ((*character)->defend && !defense_image_created.at(i))
		{
			defense_image.at(i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(i).first + 100, portrait_position.at(i).second + 100, { 16, 115, 16, 16 });
			defense_image_created.at(i) = true;
		}
		else if (!(*character)->defend && defense_image_created.at(i))
		{
			App->gui_manager->DeleteGUIElement(defense_image.at(i));
			defense_image_created.at(i) = false;
		}
		++i;
	}
}

void Battle::UpdateEnemies()
{
	int i = 0;
	for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
	{
		if (enemies_life_x.at(i) != ((64 * (*enemy)->current_stats.Hp) / (*enemy)->default_stats.Hp)
			|| enemies_life.at(i)->position.first != (*enemy)->GetPosition().first ||
			enemies_life.at(i)->position.second != (*enemy)->GetPosition().second + (*enemy)->position_margin.second - (*enemy)->current.h)
		{
			UpdateEnemyPortraits(*enemy, i);
		}
		if ((*enemy)->stunned && !enemy_stun_image_created.at(i))
		{
			enemy_stun_image.at(i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, (*enemy)->GetPosition().first + 70, (*enemy)->GetPosition().second + (*enemy)->position_margin.second - (*enemy)->current.h, { 0, 115, 16, 16 });
			enemy_stun_image_created.at(i) = true;
		}
		else if (!(*enemy)->stunned && enemy_stun_image_created.at(i))
		{
			App->gui_manager->DeleteGUIElement(enemy_stun_image.at(i));
			enemy_stun_image_created.at(i) = false;
		}
		++i;
	}
	
	if (enemies_life.size() > App->entity_manager->enemies.size())
	{
		App->gui_manager->DeleteGUIElement(enemies_life.back());
		enemies_life.pop_back();
	}
}

void Battle::UpdateCharacterPortraits(Entity* _character, int _i)
{
	App->gui_manager->DeleteGUIElement(life.at(_i));
	App->gui_manager->DeleteGUIElement(mana.at(_i));
	App->gui_manager->DeleteGUIElement(life_numbers.at(_i));
	App->gui_manager->DeleteGUIElement(mana_numbers.at(_i));
	App->gui_manager->DeleteGUIElement(character_levels.at(_i));
	life_x.at(_i) = (124 * _character->current_stats.Hp) / _character->default_stats.Hp;
	mana_x.at(_i) = (124 * _character->current_stats.Mana) / _character->default_stats.Mana;
	life.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(_i).first + life_margin.first, portrait_position.at(_i).second + life_margin.second, { 0, 58, life_x.at(_i), 29 });
	mana.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, portrait_position.at(_i).first + mana_margin.first, portrait_position.at(_i).second + mana_margin.second, { 0, 86, mana_x.at(_i), 29 });
	life_numbers.at(_i) = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, life.at(_i)->position.first + 60, life.at(_i)->position.second + 11, "0", { 100, 255, 100, 255 }, App->gui_manager->default_font_used, (_character)->current_stats.Hp, (_character)->default_stats.Hp);
	mana_numbers.at(_i) = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, mana.at(_i)->position.first + 60, mana.at(_i)->position.second + 13, "0", { 255, 200, 255, 255 }, App->gui_manager->default_font_used, (_character)->current_stats.Mana, (_character)->default_stats.Mana);
	character_levels.at(_i) = ((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, portrait_position.at(_i).first + level_margin.first, portrait_position.at(_i).second + level_margin.second, "Lvl: " + std::to_string((_character)->level), { 255, 255, 0, 255 }, App->gui_manager->default_font_used));
}

void Battle::UpdateEnemyPortraits(Entity* _enemy, int _i)
{
	App->gui_manager->DeleteGUIElement(enemies_life.at(_i));
	enemies_life_x.at(_i) = 0;
	if (_enemy != nullptr) 
	{
		enemies_life_x.at(_i) = (64 * _enemy->current_stats.Hp) / _enemy->default_stats.Hp;
		enemies_life.at(_i) = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, _enemy->GetPosition().first, _enemy->GetPosition().second + _enemy->position_margin.second - _enemy->current.h, { 0, 58, enemies_life_x.at(_i) , 5 });
	}
}

// UTILITIES---------------------------------------------------------------------------------------
void Battle::ShowEntityInfo(Entity* _entity)
{
	App->gui_manager->DeleteGUIElement(board_entity);
	App->gui_manager->DeleteGUIElement(port_entity);
	App->gui_manager->DeleteGUIElement(name_entity);
	App->gui_manager->DeleteGUIElement(lvl_entity);
	App->gui_manager->DeleteGUIElement(exp_entity);
	App->gui_manager->DeleteGUIElement(life_entity);
	App->gui_manager->DeleteGUIElement(mana_entity);
	App->gui_manager->DeleteGUIElement(atk_f_entity);
	App->gui_manager->DeleteGUIElement(def_f_entity);
	App->gui_manager->DeleteGUIElement(atk_s_entity);
	App->gui_manager->DeleteGUIElement(def_s_entity);
	App->gui_manager->DeleteGUIElement(crit_entity);
	App->gui_manager->DeleteGUIElement(eva_entity);
	App->gui_manager->DeleteGUIElement(attack_entity);
	App->gui_manager->DeleteGUIElement(ability1_entity);
	App->gui_manager->DeleteGUIElement(ability2_entity);
	App->gui_manager->DeleteGUIElement(ability3_entity);
	if (_entity != nullptr) 
	{
		board_entity = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 0 - (board.w/2), screen_height - screen_height/7 - portrait_margin.second - board.h, board);
		port_entity = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, board_entity->position.first + port_board.first , board_entity->position.second + port_board.second, _entity->portrait);
		name_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + name_board.first, board_entity->position.second + name_board.second, _entity->name.c_str(), { 255, 255, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		lvl_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, port_entity->position.first, board_entity->position.second + (life_board.second * 24), "Lvl: " + std::to_string(_entity->level), { 0, 100, 200, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		exp_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, lvl_entity->position.first + lvl_entity->area.w, board_entity->position.second + (life_board.second * 24), "Exp: " + std::to_string(_entity->exp), { 0, 100, 200, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		life_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + life_board.first, board_entity->position.second + life_board.second, "Hp: " + std::to_string(_entity->current_stats.Hp), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		mana_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + mana_board.first, board_entity->position.second + mana_board.second, "Mana: " + std::to_string(_entity->current_stats.Mana), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		atk_f_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + life_board.first, board_entity->position.second + (life_board.second * 7), "Atk_F: " + std::to_string(_entity->current_stats.AtkF), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		def_f_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + mana_board.first, board_entity->position.second + (mana_board.second * 7), "Def_F: " + std::to_string(_entity->current_stats.DefF), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		atk_s_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + life_board.first, board_entity->position.second + (life_board.second * 13), "Atk_S: " + std::to_string(_entity->current_stats.AtkS), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		def_s_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + mana_board.first, board_entity->position.second + (mana_board.second * 13), "Def_S: " + std::to_string(_entity->current_stats.DefS), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		crit_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + life_board.first, board_entity->position.second + (life_board.second * 19), "Crit: " + std::to_string(_entity->current_stats.Crit), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		eva_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + mana_board.first, board_entity->position.second + (mana_board.second * 19), "Eva: " + std::to_string(_entity->current_stats.Eva), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		attack_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + attack_board.first, board_entity->position.second + attack_board.second, ("Attack: " + _entity->attacks_names.Attack_name).c_str(), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		ability1_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + ability_1_board.first, board_entity->position.second + ability_1_board.second, ("Ability 1: " + _entity->attacks_names.Ability_1_name).c_str(), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		if (_entity->attacks_names.Ability_2_name != "")
		{
			ability2_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + ability_2_board.first, board_entity->position.second + ability_2_board.second, ("Ability 2: " + _entity->attacks_names.Ability_2_name).c_str(), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
			ability3_entity = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, board_entity->position.first + ability_3_board.first, board_entity->position.second + ability_3_board.second, ("Ability 3: " + _entity->attacks_names.Ability_3_name).c_str(), { 0, 0, 0, 255 }, App->gui_manager->default_font_used, -1000, -1000, nullptr, false);
		}
		
	}
	
}

void Battle::LevelUp(bool _up)
{
	int i = 0;
	for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
	{
		App->entity_manager->exp_sapphire = 0;
		App->entity_manager->exp_iris = 0;
		App->entity_manager->exp_storm = 0;
		App->entity_manager->exp_georgeb = 0;
		if (i == 0 && _up && (*character)->level < 10)
		{
			App->entity_manager->LevelUP((*character)->levels.at((*character)->level - 1));
		}
		else if (i == 0 && !_up && (*character)->level > 2)
		{
			App->entity_manager->LevelUP((*character)->levels.at((*character)->level - 3));
		}
		else if (i == 0 && !_up)
		{
			App->entity_manager->LevelUP(0);
		}
		App->gui_manager->DeleteGUIElement(character_levels.at(i));
		character_levels.at(i) = ((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, portrait_position.at(i).first + level_margin.first, portrait_position.at(i).second + level_margin.second, "Lvl: " + std::to_string((*character)->level), { 255, 255, 0, 255 }, App->gui_manager->default_font_used));
		++i;
	}
	aux_target = { 0,0 };

}

// DELETES -------------------------------------------------------------------------------------------

void Battle::DeleteEntityInfo()
{
	App->gui_manager->DeleteGUIElement(board_entity);
	App->gui_manager->DeleteGUIElement(port_entity);
	App->gui_manager->DeleteGUIElement(name_entity);
	App->gui_manager->DeleteGUIElement(lvl_entity);
	App->gui_manager->DeleteGUIElement(exp_entity);
	App->gui_manager->DeleteGUIElement(life_entity);
	App->gui_manager->DeleteGUIElement(mana_entity);
	App->gui_manager->DeleteGUIElement(atk_f_entity);
	App->gui_manager->DeleteGUIElement(def_f_entity);
	App->gui_manager->DeleteGUIElement(atk_s_entity);
	App->gui_manager->DeleteGUIElement(def_s_entity);
	App->gui_manager->DeleteGUIElement(crit_entity);
	App->gui_manager->DeleteGUIElement(eva_entity);
	App->gui_manager->DeleteGUIElement(attack_entity);
	App->gui_manager->DeleteGUIElement(ability1_entity);
	App->gui_manager->DeleteGUIElement(ability2_entity);
	App->gui_manager->DeleteGUIElement(ability3_entity);
}

void Battle::DeleteOptionsIngame()
{
	ingame_options_menu_created = false;
	App->entity_manager->paused = false;
	App->gui_manager->DeleteGUIElement(english_button);
	App->gui_manager->DeleteGUIElement(spanish_button);
	App->gui_manager->DeleteGUIElement(volume_up_button);
	App->gui_manager->DeleteGUIElement(volume_down_button);
	App->gui_manager->DeleteGUIElement(resume_button);
	App->gui_manager->DeleteGUIElement(main_menu_button);
	App->gui_manager->DeleteGUIElement(volume_up_label);
	App->gui_manager->DeleteGUIElement(volume_down_label);
	App->gui_manager->DeleteGUIElement(english_label);
	App->gui_manager->DeleteGUIElement(spanish_label);
	App->gui_manager->DeleteGUIElement(main_menu_label);
	App->gui_manager->DeleteGUIElement(resume_label);
	App->gui_manager->DeleteGUIElement(language_label);
	App->gui_manager->DeleteGUIElement(volume_label);
	buttons.clear();
}

void Battle::DeleteAttackMenu()
{
	waiting_for_input = false;
	App->gui_manager->DeleteGUIElement(attack_button);
	App->gui_manager->DeleteGUIElement(ability_button);
	App->gui_manager->DeleteGUIElement(defend_button);
	App->gui_manager->DeleteGUIElement(attack_label);
	App->gui_manager->DeleteGUIElement(ability_label);
	App->gui_manager->DeleteGUIElement(defend_label);
	buttons2.clear();
}

void Battle::DeleteAbilitiesMenu()
{
	ability_menu_created = false;
	App->gui_manager->DeleteGUIElement(ability1_button);
	App->gui_manager->DeleteGUIElement(ability2_button);
	App->gui_manager->DeleteGUIElement(ability3_button);
	App->gui_manager->DeleteGUIElement(attack_label);
	App->gui_manager->DeleteGUIElement(ability_label);
	App->gui_manager->DeleteGUIElement(defend_label);
	buttons2.clear();
}

void Battle::ActionsMenu()
{

	if (!waiting_for_input && !ability_menu_created)
	{
		CreateAttackMenu();
	}

	if (!ability_menu_created)
	{
		if (attack_button->has_been_clicked)
		{
			DeleteAttackMenu();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->current_turn = Entity::TURN::SEARCH_ATTACK;
				}
			}

		}
		else if (character_ability1up && ability_button->has_been_clicked)
		{
			DeleteAttackMenu();
			CreateAbilitiesMenu();
		}
		else if (defend_button->has_been_clicked)
		{
			DeleteAttackMenu();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->current_turn = Entity::TURN::DEFEND;
				}
			}
		}
		else if (App->input->Decline()) {
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->ComeBack();
					DeleteAttackMenu();
				}
			}
		}
	}
	else if (ability_menu_created)
	{
		if (App->input->Decline()) {
			DeleteAbilitiesMenu();
		}
		if (character_ability1up && ability1_button->has_been_clicked)
		{
			DeleteAbilitiesMenu();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_1;
				}
			}
		}
		else if (character_ability2up && ability2_button->has_been_clicked)
		{
			DeleteAbilitiesMenu();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_2;
				}
			}
		}
		else if (character_ability3up && ability3_button->has_been_clicked)
		{
			DeleteAbilitiesMenu();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					(*character)->current_turn = Entity::TURN::SEARCH_ABILITY_3;
				}
			}
		}
	}

	//navigate for buttons2
	if (App->input->Down())
	{
		App->audio->PlayFx(press_sfx);
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
		App->audio->PlayFx(press_sfx);
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
}

void Battle::ControlLanguageAndMusic()
{
	if (english_button->has_been_clicked)
	{
		App->scene_manager->language->SetLanguage(LANGUAGE_TYPE::ENGLISH);
		DeleteOptionsIngame();
		CreateOptionsIngame();
	}
	else if (spanish_button->has_been_clicked)
	{
		App->scene_manager->language->SetLanguage(LANGUAGE_TYPE::SPANISH);
		DeleteOptionsIngame();
		CreateOptionsIngame();
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