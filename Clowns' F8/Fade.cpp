#include "Log.h"
#include "Application.h"
#include "Fade.h"
#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"


Fade::Fade(float transition_time, bool is_scene_change, int scene_to_change, Color color) : Transition(transition_time)
{
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;
	this->color = color;

	uint width, height;

	App->window->GetWindowSize(width, height);
	screen = { 0, 0, (int)width, (int)height };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
}

Fade::~Fade()
{
}

void Fade::Entering()
{
	Transition::Entering();

	float normalized_alpha = floor(current_time->ReadSec()*(255.0F / transition_time));
	DrawFadeRect(normalized_alpha);
}

void Fade::Action()
{
	Transition::Action();

	DrawFadeRect(255.0F);

	if (is_scene_change)
	{
		App->scene->current_scene = App->scene->FIRST_BATTLE;
	}
}

void Fade::Exiting()
{
	Transition::Exiting();

	float normalized_alpha = floor((transition_time - current_time->ReadSec())*(255.0F / transition_time));
	DrawFadeRect(normalized_alpha);
}

void Fade::DrawFadeRect(float alpha_value)
{
	if (alpha_value > 255)alpha_value = 255;
	if (alpha_value < 0)alpha_value = 0;

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, alpha_value);
	SDL_RenderFillRect(App->render->renderer, &screen);
}

void Fade::SetColor(Color color)
{
	this->color = color;
}