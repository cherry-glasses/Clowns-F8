#include "Application.h"
#include "Lose.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
#include "ModuleTransitionManager.h"



Lose::Lose(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
Lose::~Lose()
{}


// Called before the first frame
bool Lose::Start()
{
	lose_background = App->textures->Load("Assets/Sprites/UI/lose_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Main_menu_8_bits.ogg");
	CreateLose();

	return true;
}

// Called before all Updates
bool Lose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Lose::Update(float _dt)
{
	bool ret = true;

	if (App->input->Accept())
	{
		if (!App->scene_manager->changing)
		{
			App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
			App->scene_manager->changing = true;
		}
	}

	return ret;
}

// Called before all Updates
bool Lose::PostUpdate(float dt)
{
	App->render->Blit(lose_background, 0, 0);

	return true;
}

// Called before quitting
bool Lose::CleanUp()
{
	App->textures->UnLoad(lose_background);
	App->gui_manager->DeleteGUIElement(lose_label);
	return true;
}


// Load
bool Lose::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool Lose::Save(pugi::xml_node& _data) const
{
	return true;
}

void Lose::CreateLose()
{
	lose_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5, screen_height * 0.5 + 200, App->scene_manager->language->win_accept, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
}


