#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGUIManager.h"
#include "GUIElement.h"
#include "GUIButton.h"
#include "ModuleAudio.h"

GUIButton::GUIButton(int x, int y, SDL_Rect image_area, SDL_Rect image_area_hover, SDL_Rect image_area_pressed, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_BUTTON;
	initial_area = image_area;
	area = initial_area;

	img_area_hover = image_area_hover;
	img_area_pressed = image_area_pressed;
	
}

void GUIButton::Update(float dt)
{
	Click();
	Move();
}

void GUIButton::Click()
{
	if (App->debug)
	{
		int x, y;
		App->input->GetMousePosition(x, y);

		if (x > position.first && x < position.first + initial_area.w && y > position.second && y < position.second + initial_area.h)
		{
			if (App->input->GetMouseButtonDown(1) == SDL_PRESSED)
			{
				Select(PRESSED);
			}
			else if (App->input->GetMouseButtonDown(1) == SDL_RELEASED)
			{
				Select(SELECTED);
			}
		}
		else
		{
			area = initial_area;
		}
	}

	if (current_state == SELECTED && (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->gamepad.A == BUTTON_DOWN))
	{
		Select(PRESSED);
	}

}

void GUIButton::Select(STATE press) 
{
	switch (press)
	{
	case NORMAL:
		area = initial_area;
		has_been_clicked = false;
		current_state = STATE::NORMAL;
		break;
	case PRESSED:
		area = img_area_pressed;
		has_been_clicked = true;
		current_state = STATE::PRESSED;
		break;
	case SELECTED:
		area = img_area_hover;
		has_been_clicked = false;
		current_state = STATE::SELECTED;
		break;
	}
}
