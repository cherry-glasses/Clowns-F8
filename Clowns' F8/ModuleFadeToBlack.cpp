#include "Log.h"
#include "Application.h"
#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

ModuleFadeToBlack::ModuleFadeToBlack()
{
	name = "scenemanager";
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{}

// Load assets
bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	screen = { 0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight() };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool ModuleFadeToBlack::Update(float _dt)
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			App->scene->CleanUp();
			

			total_time += total_time * _dt;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		App->scene->Start();
		normalized = 1.0F - normalized;
		
		if (now >= total_time)
			
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 255, (Uint8)(normalized * 255.0F));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool ModuleFadeToBlack::FadeToBlack(float _time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(_time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}