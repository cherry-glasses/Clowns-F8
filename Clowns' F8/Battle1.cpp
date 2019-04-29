#include "Application.h"
#include "Battle1.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleMap.h"
#include "ModuleEntityManager.h"
#include "ModuleRender.h"
#include "ModuleTransitionManager.h"
#include "Color.h"



Battle1::Battle1(SCENE_TYPE _type, pugi::xml_node& _config) : Battle(_type, _config)
{

}

// Destructor
Battle1::~Battle1()
{}


// Called before the first frame
bool Battle1::Start()
{
	battle1_background = App->textures->Load("Assets/Maps/map_level_1.png");
	battle1_grid = App->textures->Load("Assets/Maps/grid_level_1.png");
	battle_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
	return true;
}

// Called before all Updates
bool Battle1::PreUpdate()
{
	if (!App->scene_manager->music_created) {
		CreateMusic();
	}
	if (!battle1_created) {
		CreateBattle1();
	}
	return true;
}

// Called each loop iteration
bool Battle1::Update(float _dt)
{
	bool ret = true;

	if (App->input->Pause() || ingame_options_menu_created == true)
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
			App->scene_manager->music_created = false;
			App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
			
		}

		Navigate();

	}
	else
	{
		if (!App->entity_manager->ThereAreCharAlive()) {
			App->scene_manager->ChangeScene(LOSE_SCENE);
		}
		else if (App->entity_manager->enemies.empty()) {
			App->scene_manager->ChangeScene(WIN_SCENE);
		}
		else {
			UpdateCharacters();
			UpdateEnemies();
			for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
			{
				if ((*character)->current_turn == Entity::TURN::SELECT_ACTION)
				{
					ActionsMenu();
				}
			}
			
		}
		App->render->Blit(battle1_background, 0 - (screen_width / 2), 0 - (screen_height / 8));
		App->render->Blit(battle1_grid, 0 - (screen_width / 2), 0 - (screen_height / 8));
		App->map->Draw();
	}

	return ret;
}

// Called before all Updates
bool Battle1::PostUpdate()
{
	
	return true;
}

// Called before quitting
bool Battle1::CleanUp()
{
	App->textures->UnLoad(battle_menu_background);
	App->textures->UnLoad(battle1_background);
	App->textures->UnLoad(battle1_grid);
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
	buttons2.clear();
	battle1_created = false;
	App->map->CleanUp();
	App->entity_manager->CleanUp();
	return true;
}


// Load
bool Battle1::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Battle1::Save(pugi::xml_node& _data) const
{
	return true;
}

void Battle1::CreateMusic()
{
	App->scene_manager->music_created = true;
	App->audio->PlayMusic("Battle_1_Song.ogg");
}

void Battle1::CreateBattle1()
{
	battle1_created = true;
	App->map->Load("map_level1.tmx");
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_STORM);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE1);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE2);
	App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_TREE3);

	CreateUIBattle();

}




