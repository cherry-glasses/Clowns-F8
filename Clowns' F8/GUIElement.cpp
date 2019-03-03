#include "Application.h"
#include "GUIElement.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

GUIElement::GUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect area, GUIElement* son) : type(type), position(x, y), area(area), son(son)
{

}

GUIElement::~GUIElement()
{

}

void GUIElement::Draw(SDL_Texture* sprites, float speed)
{
	App->render->Blit(sprites, position.x, position.y, &area, speed);

	if (App->gui->debug_gui)
	{
		DrawDebug();
	}
}

void GUIElement::Update(float dt)
{
	Move();
}

void GUIElement::Move()
{
	/*int x, y;
	App->input->GetMousePosition(x, y);

	bool is_inside_parent = x > position.x && x < position.x + area.w && y > position.y && y < position.y + area.h;

	bool is_inside_son = false;
	if (son != nullptr)
	is_inside_son = x > son->position.x && x < son->position.x + son->area.w && y > son->position.y && y < son->position.y + son->area.h;

	bool is_ctrl = false;
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
	is_ctrl = true;
	}
	else is_ctrl = false;

	if (is_inside_parent)
	{
	if (son == nullptr && !is_ctrl)
	{
	if (App->input->GetMouseButtonDown(3))
	{
	position.x = x - area.w / 2;
	position.y = y - area.h / 2;
	}
	}
	else if (son != nullptr)
	{
	if (App->input->GetMouseButtonDown(3) && !is_ctrl)
	{
	position.x = x - area.w / 2;
	position.y = y - area.h / 2;
	son->position.x = x - son->area.w / 2;
	son->position.y = y - son->area.h / 2;
	}
	}
	}

	if (is_inside_son)
	{
	if (App->input->GetMouseButtonDown(3) && is_ctrl)
	{
	son->position.x = x - son->area.w / 2;
	son->position.y = y - son->area.h / 2;
	}
	}*/
}

void GUIElement::DrawDebug()
{
	Uint8 alpha = 80;
	SDL_Rect quad = { position.x - DEBUG_DRAW_OFFSET, position.y - DEBUG_DRAW_OFFSET, area.w + DEBUG_DRAW_OFFSET * 2, area.h + DEBUG_DRAW_OFFSET * 2 };
	App->render->DrawQuad(quad, 0, 0, 0, alpha, false);
}