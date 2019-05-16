#include "Application.h"
#include "Win.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
#include "ModuleTransitionManager.h"



Win::Win(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{

}

// Destructor
Win::~Win()
{}


// Called before the first frame
bool Win::Start()
{
	win_background = App->textures->Load("Assets/Sprites/UI/win_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->PlayMusic("Main_menu_8_bits.ogg");
	CreateWin();

	return true;
}

// Called before all Updates
bool Win::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Win::Update(float _dt)
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
bool Win::PostUpdate(float dt)
{
	App->render->Blit(win_background, 0, 0);

	return true;
}

// Called before quitting
bool Win::CleanUp()
{
	App->textures->UnLoad(win_background);
	App->gui_manager->DeleteGUIElement(win_label);
	return true;
}


// Load
bool Win::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool Win::Save(pugi::xml_node& _data) const
{
	return true;
}

void Win::CreateWin()
{
	win_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5, screen_height * 0.5 + 200, App->scene_manager->language->win_accept, { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
}


