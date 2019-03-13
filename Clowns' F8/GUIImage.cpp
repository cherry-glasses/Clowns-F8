#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "ModuleInput.h"

GUIImage::GUIImage(int x, int y, SDL_Rect image_area, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_IMAGE;
	area = image_area;

	position.first = x;
	position.second = y;
}

void GUIImage::Update(float dt)
{
	Move();

	int x, y;
	App->input->GetMousePosition(x, y);
	bool is_inside = x > position.first && x < position.first + area.w && y > position.second && y < position.second + area.h;
	if (is_inside)
	{
		if (App->input->GetMouseButtonDown(1) == SDL_PRESSED && !has_been_clicked)
		{
			has_been_clicked = true;
		}
		else
		{
			has_been_clicked = false;
		}
	}
	else
	{
		has_been_clicked = false;
	}
}
