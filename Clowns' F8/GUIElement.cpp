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
	App->render->Blit(sprites, position.first, position.second, &area, speed);

	if (App->gui_manager->debug_gui)
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
	int x, y;
	App->input->GetMousePosition(x, y);

	bool is_inside_parent = x > position.first && x < position.first + area.w && y > position.second && y < position.second + area.h;

	bool is_inside_son = false;
	if (son != nullptr)
	is_inside_son = x > son->position.first && x < son->position.first + son->area.w && y > son->position.second && y < son->position.second + son->area.h;

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
	position.first = x - area.w / 2;
	position.second = y - area.h / 2;
	}
	}
	else if (son != nullptr)
	{
	if (App->input->GetMouseButtonDown(3) && !is_ctrl)
	{
	position.first = x - area.w / 2;
	position.second = y - area.h / 2;
	son->position.first = x - son->area.w / 2;
	son->position.second = y - son->area.h / 2;
	}
	}
	}

	if (is_inside_son)
	{
	if (App->input->GetMouseButtonDown(3) && is_ctrl)
	{
	son->position.first = x - son->area.w / 2;
	son->position.second = y - son->area.h / 2;
	}
	}
}

void GUIElement::DrawDebug()
{
	Uint8 alpha = 80;
	SDL_Rect quad = { position.first - DEBUG_DRAW_OFFSET, position.second - DEBUG_DRAW_OFFSET, area.w + DEBUG_DRAW_OFFSET * 2, area.h + DEBUG_DRAW_OFFSET * 2 };
	App->render->DrawQuad(quad, 0, 0, 0, alpha, false);
}