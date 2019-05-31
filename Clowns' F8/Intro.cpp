#include "Application.h"
#include "Intro.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
#include "ModuleTransitionManager.h"



Intro::Intro(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
Intro::~Intro()
{}


// Called before the first frame
bool Intro::Start()
{

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Main_menu_8_bits.ogg");

	return true;
}

// Called before all Updates
bool Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Intro::Update(float _dt)
{
	bool ret = true;

	if (which_text == 0)
	{
		if (!lines_created)
		{
			for (int i = 0; i < 8; ++i)
			{
				labels.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5, 300 + 40 * i, App->scene_manager->language->lines.at(i), { 255, 255, 255, 255 }, App->gui_manager->default_font_used));
			}

			lines_created = true;
		}
		if (App->input->Accept())
		{
			which_text = 1;
			lines_created = false;
			App->gui_manager->DeleteAllGUIElements();
			labels.clear();
		}
	}
	else if (which_text == 1)
	{
		if (!lines_created)
		{
			for (int i = 8; i < 22; ++i)
			{
				labels.push_back((GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5, 300 + 40 * (i - 8), App->scene_manager->language->lines.at(i), { 255, 255, 255, 255 }, App->gui_manager->default_font_used));
			}

			lines_created = true;
		}
		if (App->input->Accept())
		{
			lines_created = false;
			App->gui_manager->DeleteAllGUIElements();
			labels.clear();
			if (!App->scene_manager->changing)
			{
				App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, Black);
				App->scene_manager->changing = true;
			}
		}
	}

	return ret;
}

// Called before all Updates
bool Intro::PostUpdate(float dt)
{

	return true;
}

// Called before quitting
bool Intro::CleanUp()
{
	App->gui_manager->DeleteAllGUIElements();
	labels.clear();
	return true;
}


// Load
bool Intro::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool Intro::Save(pugi::xml_node& _data) const
{
	return true;
}


