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

	text = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5, 30, App->scene_manager->language->intro_text, { 255, 255, 255, 255 }, App->gui_manager->default_font_used);


	if (App->input->Accept())
	{
		if (!App->scene_manager->changing)
		{
			App->transition_manager->CreateFadeTransition(1, true, CHOOSE_MAP, Black);
			App->scene_manager->changing = true;
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
	App->gui_manager->DeleteGUIElement(text);
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


