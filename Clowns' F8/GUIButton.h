#ifndef __GUIButton_H__
#define __GUIButton_H__

#include "GUIElement.h"

enum STATE
{

	
	NORMAL,
	SELECTED,
	PRESSED


};


class GUIButton : public GUIElement
{
public:
	GUIButton(int x, int y, SDL_Rect image_area_normal, SDL_Rect image_area_hover, SDL_Rect image_area_pressed, GUIElement* son);
	~GUIButton() {};

	void Update(float dt);

	void Click();

	void Select(STATE press);

public:

	SDL_Rect initial_area;
	SDL_Rect img_area_hover;
	SDL_Rect img_area_pressed;

	bool has_been_clicked = false;

	STATE current_state = NORMAL;
};

#endif // __GUIButton_H__