#include "Log.h"
#include "Application.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Scene.h"


Scene::Scene(SCENE_TYPE _type, pugi::xml_node& _config)
{
	type = _type;
	button = { _config.child("button").attribute("x").as_int(), _config.child("button").attribute("y").as_int(),
		_config.child("button").attribute("w").as_int() , _config.child("button").attribute("h").as_int() };
	small_button = { _config.child("small_button").attribute("x").as_int(), _config.child("small_button").attribute("y").as_int(),
		_config.child("small_button").attribute("w").as_int() , _config.child("small_button").attribute("h").as_int() };
	button_margin = _config.child("button").attribute("margin").as_int();
	cherry_glasses_logo = { _config.child("cherry_glasses_logo").attribute("x").as_int(), _config.child("cherry_glasses_logo").attribute("y").as_int(),
		_config.child("cherry_glasses_logo").attribute("w").as_int() , _config.child("cherry_glasses_logo").attribute("h").as_int() };
	option_background = { _config.child("option_background").attribute("x").as_int(), _config.child("option_background").attribute("y").as_int(),
		_config.child("option_background").attribute("w").as_int() , _config.child("option_background").attribute("h").as_int() };

	//Audios
	App->scene_manager->move_sfx = App->audio->LoadFx(_config.child("move_fx_name").attribute("source").as_string());
	App->scene_manager->accept_sfx = App->audio->LoadFx(_config.child("press_fx_name").attribute("source").as_string());
	App->scene_manager->decline_sfx = App->audio->LoadFx(_config.child("decline_fx_name").attribute("source").as_string());
	App->scene_manager->defend_sfx = App->audio->LoadFx(_config.child("defend_fx_name").attribute("source").as_string());
	App->scene_manager->critic_sfx = App->audio->LoadFx(_config.child("critic_fx_name").attribute("source").as_string());

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

}

// Destructor
Scene::~Scene()
{}


// NAVIGATION----------------------------------------

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
	App->audio->PlayFx(App->scene_manager->move_sfx);
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
	App->audio->PlayFx(App->scene_manager->move_sfx);
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
