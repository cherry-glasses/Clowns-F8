#ifndef __j1GUIManager_H__
#define __j1GUIManager_H__

#include "Module.h"
//#include "p2DynArray.h"

#include "SDL/include/SDL.h"
//#include "SDL_ttf/include/SDL_ttf.h"

class GUIElement;

enum class GUI_ELEMENT_TYPE
{
	UNKNOWN,
	GUI_IMAGE,
	GUI_BUTTON,
	GUI_LABEL,
	GUI_PANEL,
	GUI_INPUTBOX,
	GUI_THUMB,
	GUI_SLIDER
};

class GUIManager : public Module
{
public:

	GUIManager();
	virtual ~GUIManager();

	//bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	/*bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;*/

public:
	GUIElement* CreateGUIImage(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son = nullptr);
	GUIElement* CreateGUIButton(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1 = { NULL }, SDL_Rect a_2 = { NULL }, GUIElement* son = nullptr);
	GUIElement* CreateGUILabel(GUI_ELEMENT_TYPE type, int x, int y, p2SString text, SDL_Color color, _TTF_Font* font, GUIElement* son = nullptr);
	GUIElement* CreateGUIPanel(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son = nullptr);
	/*GUIElement* CreateGUIInputBox(GUI_ELEMENT_TYPE type, int x, int y, SDL_Color color, _TTF_Font* font, SDL_Rect a, GUIElement* son = nullptr);
	GUIElement* CreateGUIThumb(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1 = { NULL }, SDL_Rect a_2 = { NULL }, GUIElement* son = nullptr);
	GUIElement* CreateGUISlider(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect thumb_1, SDL_Rect thumb_2, SDL_Rect thumb_3, GUIElement* son = nullptr);*/

	void DeleteGUIElement(GUIElement* e);
	void DeleteAllGUIElements();

public:
	//_TTF_Font* default_font_used = nullptr;
	bool debug_gui = false;

private:
	p2DynArray<GUIElement*> gui_elements = NULL;

	SDL_Texture* image_textures = nullptr;
	SDL_Texture* button_textures = nullptr;
	/*SDL_Texture* inputbox_textures = nullptr;
	SDL_Texture* thumb_textures = nullptr;
	SDL_Texture* slider_textures = nullptr;*/
};

#endif // __j1GUIManager_H__