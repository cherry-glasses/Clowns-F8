#ifndef __Fade_H_
#define __Fade_H_

#include "Transition.h"
#include "Scene.h"

class Fade :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	SCENE_TYPE scene_to_change = SCENE_TYPE::NONE;

	SDL_Rect screen;
public:
	Fade(float transition_time, bool is_scene_change = false, SCENE_TYPE scene_to_change = SCENE_TYPE::NONE, Color color = Black);
	~Fade();

	void Entering();
	void Action();
	void Exiting();

	void DrawFadeRect(float alpha_value);
	void SetColor(Color color);
};

#endif // !__Fade_H__