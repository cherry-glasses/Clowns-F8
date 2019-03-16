//#include "Application.h"
//#include "ModuleGUIManager.h"
//#include "GUIElement.h"
//#include "GUILabel.h"
//#include "ModuleRender.h"
//
//#include "SDL_ttf/include/SDL_ttf.h"
//
//GUILabel::GUILabel(int x, int y, std::string text, SDL_Color color, _TTF_Font* font, GUIElement* son) : GUIElement(type, x, y, area, son)
//{
//	type = GUI_ELEMENT_TYPE::GUI_LABEL;
//
//	this->text = text;
//	this->color = color;
//	this->font = font;
//
//	tex = App->fonts->Print(this->text.GetString(), this->color, this->font);
//
//	// Create rect
//	int width = 0, height = 0;
//	App->fonts->CalcSize(this->text.GetString(), width, height, App->gui->default_font_used);
//	area.w = width;
//	area.h = height;
//	area.x = 0;
//	area.y = 0;
//
//	position.first = x;
//	position.second = y;
//}
//
//void GUILabel::Update(float dt)
//{
//	Move();
//}
//
//void GUILabel::DrawLabel()
//{
//	App->render->Blit(tex, position.first, position.second, &area, 0.0f);
//}
//
//// Note: see SDL_RenderSetClip() for clipping
//void GUILabel::SetText(std::string text)
//{
//	this->text = text;
//
//	tex = App->fonts->Print(this->text.GetString(), this->color, this->font);
//
//	// Create new rect
//	int width = 0, height = 0;
//	App->fonts->CalcSize(this->text.GetString(), width, height, App->gui->default_font_used);
//	area.w = width;
//	area.h = height;
//
//	area.x = 0;
//	area.y = 0;
//}
//
//void GUILabel::AddChar(p2SString c)
//{
//	this->text += c;
//
//	tex = App->fonts->Print(this->text.GetString(), this->color, this->font);
//
//	// Create new rect
//	int width = 0, height = 0;
//	App->fonts->CalcSize(this->text.GetString(), width, height, App->gui->default_font_used);
//	area.w = width;
//	area.h = height;
//
//	area.x = 0;
//	area.y = 0;
//}
//
//void GUILabel::DeleteLastChar()
//{
//	if (this->text.Length() > 0)
//	{
//		char* temp_text = (char*)(this->text.GetString());
//		temp_text[this->text.Length() - 1] = '\0';
//
//		this->text = temp_text;
//	}
//
//	// Create new rect
//	int width = 0, height = 0;
//	App->fonts->CalcSize(this->text.GetString(), width, height, App->gui->default_font_used);
//	area.w = width;
//	area.h = height;
//
//	area.x = 0;
//	area.y = 0;
//}