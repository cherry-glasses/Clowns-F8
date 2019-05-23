#include "Log.h"
#include "Application.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
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
	iris_portrait = { _config.child("portraits").child("iris").attribute("x").as_int(), _config.child("portraits").child("iris").attribute("y").as_int(),
		_config.child("portraits").child("iris").attribute("w").as_int() , _config.child("portraits").child("iris").attribute("h").as_int() };
	sapphire_portrait = { _config.child("portraits").child("sapphire").attribute("x").as_int(), _config.child("portraits").child("sapphire").attribute("y").as_int(),
		_config.child("portraits").child("sapphire").attribute("w").as_int() , _config.child("portraits").child("sapphire").attribute("h").as_int() };
	storm_portrait = { _config.child("portraits").child("storm").attribute("x").as_int(), _config.child("portraits").child("storm").attribute("y").as_int(),
		_config.child("portraits").child("storm").attribute("w").as_int() , _config.child("portraits").child("storm").attribute("h").as_int() };
	george_b_portrait = { _config.child("portraits").child("george_b").attribute("x").as_int(), _config.child("portraits").child("george_b").attribute("y").as_int(),
		_config.child("portraits").child("george_b").attribute("w").as_int() , _config.child("portraits").child("george_b").attribute("h").as_int() };
	life_margin = { _config.child("life_position").attribute("margin_x").as_int(), _config.child("life_position").attribute("margin_y").as_int() };
	mana_margin = { _config.child("mana_position").attribute("margin_x").as_int(), _config.child("mana_position").attribute("margin_y").as_int() };
	actions_margin = { _config.child("actions_position").attribute("margin_x").as_int(), _config.child("actions_position").attribute("margin_y").as_int() };
	port_margin = { _config.child("port_position").attribute("margin_x").as_int(), _config.child("port_position").attribute("margin_y").as_int() };
	portrait_margin = { _config.child("portrait_position").attribute("margin_x").as_int(), _config.child("portrait_position").attribute("margin_y").as_int() };
	name_margin = { _config.child("name_position").attribute("margin_x").as_int(), _config.child("name_position").attribute("margin_y").as_int() };
	

	//Audio
	press_fx_name = _config.child("press_fx_name").attribute("source").as_string();

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
