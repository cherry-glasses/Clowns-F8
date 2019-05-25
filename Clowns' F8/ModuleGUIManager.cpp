#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleGUIManager.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "ModuleAudio.h"

ModuleGUIManager::ModuleGUIManager()
{
	name = "gui";
}

// Destructor
ModuleGUIManager::~ModuleGUIManager()
{

}

bool ModuleGUIManager::Awake(pugi::xml_node& config)
{
return true;
}

bool ModuleGUIManager::Start()
{
	image_textures = App->textures->Load("Assets/Sprites/UI/UI_sprites.png");
	button_textures = App->textures->Load("Assets/Sprites/UI/buttons.png");
	return true;
}

// Called before render is available
bool ModuleGUIManager::Update(float dt)
{
	
	for (uint i = 0; i < gui_elements.size(); ++i)
	if (gui_elements[i] != nullptr) gui_elements[i]->Update(dt);

	return true;
}

bool ModuleGUIManager::PostUpdate(float _dt)
{
	for (uint i = 0; i < gui_elements.size(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_BUTTON)
			gui_elements[i]->Draw(button_textures);

	for (uint i = 0; i < gui_elements.size(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_IMAGE)
			gui_elements[i]->Draw(image_textures);

	for (uint i = 0; i < gui_elements.size(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_LABEL)
			gui_elements[i]->DrawLabel();

	/*for (uint i = 0; i < gui_elements.size(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_PANEL)
	gui_elements[i]->Draw(image_textures);*/


	/*for (uint i = 0; i < gui_elements.size(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_SLIDER)
	gui_elements[i]->Draw(slider_textures);

	for (uint i = 0; i < gui_elements.size(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_THUMB)
	gui_elements[i]->Draw(thumb_textures);

	for (uint i = 0; i < gui_elements.size(); ++i)
	if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_INPUTBOX)
	{
	gui_elements[i]->Draw(inputbox_textures);
	gui_elements[i]->DrawInputBox();
	}*/

	return true;
}

// Called before quitting
bool ModuleGUIManager::CleanUp()
{
	//LOG("Freeing all gui elements");
	App->textures->UnLoad(image_textures);
	App->textures->UnLoad(button_textures);
	for (uint i = 0; i < gui_elements.size(); ++i)
	{
		if (gui_elements[i] != nullptr)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
		}
	}
	gui_elements.clear();

	return true;
}

GUIElement* ModuleGUIManager::CreateGUIImage(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIImage(x, y, a, son);

	if (ret != nullptr)
	gui_elements.push_back(ret);

	return ret;
}

GUIElement* ModuleGUIManager::CreateGUIButton(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1, SDL_Rect a_2, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIButton(x, y, a, a_1, a_2, son);

	if (ret != nullptr)
	gui_elements.push_back(ret);

	return ret;
}

GUIElement* ModuleGUIManager::CreateGUILabel(GUI_ELEMENT_TYPE type, int x, int y, std::string text, SDL_Color color, _TTF_Font* font, int curr, int def, GUIElement* son, bool centrated)
{
	GUIElement* ret = nullptr;

	ret = new GUILabel(x, y, text, color, font, curr, def, son, centrated);


	if (ret != nullptr)
	gui_elements.push_back(ret);

	return ret;
}

/*GUIElement* ModuleGUIManager::CreateGUIPanel(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIPanel(x, y, a, son);

	if (ret != nullptr)
	gui_elements.PushBack(ret);

	return ret;
}*/

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

bool ModuleGUIManager::Load(pugi::xml_node& save)
{
return true;
}

bool ModuleGUIManager::Save(pugi::xml_node& save) const
{
return true;
}

void ModuleGUIManager::DeleteGUIElement(GUIElement* e)
{
	int i = 0;
	for (std::vector<GUIElement*>::iterator element = gui_elements.begin(); element != gui_elements.end(); ++element)
	{
		if ((*element) == e)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
			gui_elements.erase(element);
			break;
		}
		++i;
	}
}

void ModuleGUIManager::DeleteAllGUIElements()
{
	for (unsigned int i = 0; i < gui_elements.size(); ++i)
	{
		if (gui_elements[i] != nullptr)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
		}
	}
	gui_elements.clear();
}