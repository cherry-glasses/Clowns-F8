#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleGUIManager.h"
#include "ModuleTextures.h"
/*#include "p2Defs.h"
#include "p2Log.h"*/
#include "ModuleWindow.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
//#include "GUIPanel.h"
#include "ModuleAudio.h"

ModuleGUIManager::ModuleGUIManager()
{
	//name.create("gui");
}

// Destructor
ModuleGUIManager::~ModuleGUIManager()
{

}

/*bool j1GUIManager::Awake(pugi::xml_node& config)
{
return true;
}*/

bool ModuleGUIManager::Start()
{
	return true;
}

// Called before render is available
bool ModuleGUIManager::Update(float dt)
{
	/*
	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr) gui_elements[i]->Update(dt);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_IMAGE)
	gui_elements[i]->Draw(image_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_PANEL)
	gui_elements[i]->Draw(image_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_BUTTON)
	gui_elements[i]->Draw(button_textures);

	/*for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_SLIDER)
	gui_elements[i]->Draw(slider_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_THUMB)
	gui_elements[i]->Draw(thumb_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_INPUTBOX)
	{
	gui_elements[i]->Draw(inputbox_textures);
	gui_elements[i]->DrawInputBox();
	}

	for (uint i = 0; i < gui_elements.Count(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_LABEL)
	gui_elements[i]->DrawLabel();*/

	return true;
}

// Called before quitting
bool ModuleGUIManager::CleanUp()
{
	//LOG("Freeing all gui elements");
	/*
	for (uint i = 0; i < gui_elements.Count(); ++i)
	{
	if (gui_elements[i] != nullptr)
	{
	delete gui_elements[i];
	gui_elements[i] = nullptr;
	}
	}*/

	return true;
}

GUIElement* ModuleGUIManager::CreateGUIImage(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	/*GUIElement* ret = nullptr;

	ret = new GUIImage(x, y, a, son);

	if (ret != nullptr)
	gui_elements.PushBack(ret);

	return ret;*/
}

GUIElement* ModuleGUIManager::CreateGUIButton(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1, SDL_Rect a_2, GUIElement* son)
{
	/*GUIElement* ret = nullptr;

	ret = new GUIButton(x, y, a, a_1, a_2, son);

	if (ret != nullptr)
	gui_elements.PushBack(ret);

	return ret;*/
}

GUIElement* ModuleGUIManager::CreateGUILabel(GUI_ELEMENT_TYPE type, int x, int y, p2SString text, SDL_Color color, _TTF_Font* font, GUIElement* son)
{
	/*GUIElement* ret = nullptr;

	ret = new GUILabel(x, y, text, color, font, son);


	if (ret != nullptr)
	gui_elements.PushBack(ret);

	return ret;*/
}

GUIElement* ModuleGUIManager::CreateGUIPanel(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	/*GUIElement* ret = nullptr;

	ret = new GUIPanel(x, y, a, son);

	if (ret != nullptr)
	gui_elements.PushBack(ret);

	return ret;*/
}

/*GUIElement* j1GUIManager::CreateGUIInputBox(GUI_ELEMENT_TYPE type, int x, int y, SDL_Color color, _TTF_Font* font, SDL_Rect a, GUIElement* son)
{
GUIElement* ret = nullptr;

ret = new GUIInputBox(x, y, color, font, a, son);

if (ret != nullptr)
gui_elements.PushBack(ret);

return ret;
}

GUIElement* j1GUIManager::CreateGUIThumb(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1, SDL_Rect a_2, GUIElement* son)
{
GUIElement* ret = nullptr;

ret = new GUIThumb(x, y, a, a_1, a_2, son);

if (ret != nullptr)
gui_elements.PushBack(ret);

return ret;
}

GUIElement* j1GUIManager::CreateGUISlider(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a,
SDL_Rect thumb_1, SDL_Rect thumb_2, SDL_Rect thumb_3, GUIElement* son)
{
GUIElement* ret = nullptr;

ret = new GUISlider(x, y, a, thumb_1, thumb_2, thumb_3, son);

if (ret != nullptr)
gui_elements.PushBack(ret);

return ret;
}*/

/*bool GUIManager::Load(pugi::xml_node& save)
{
return true;
}*/

/*bool GUIManager::Save(pugi::xml_node& save) const
{
return true;
}*/

void ModuleGUIManager::DeleteGUIElement(GUIElement* e)
{
	for (int i = 0; i < gui_elements.Count(); ++i)
	{
		if (gui_elements[i] == e)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
			break;
		}
	}
}

void ModuleGUIManager::DeleteAllGUIElements()
{
	for (unsigned int i = 0; i < gui_elements.Count(); ++i)
	{
		if (gui_elements[i] != nullptr)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
		}
	}
}