#ifndef __ModuleFadeToBlack_H__
#define __ModuleFadeToBlack_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start();
	bool Update(float _dt);
	bool FadeToBlack(float _time = 2.0f);

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__ModuleFadeToBlack_H__