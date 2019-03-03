#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

#include <stdio.h>
#include <string.h>

// Constructor
ModuleFonts::ModuleFonts() : Module()
{}

// Destructor
ModuleFonts::~ModuleFonts()
{}

// Load new texture from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		//LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		//LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		//LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}


	unsigned int width, height;
	App->textures->GetSize(tex, width, height);


	fonts[id].graphic = tex; // graphic: pointer to the texture
	fonts[id].rows = rows; // rows: rows of characters in the texture
	fonts[id].len = strlen(characters);
	fonts[id].row_chars = fonts[id].len / rows;
	fonts[id].char_w = width / fonts[id].row_chars;
	fonts[id].char_h = height / rows;
	strcpy_s(fonts[id].table, characters);


	//LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		//LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bitmap font
void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		//LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	unsigned int len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	for (unsigned int i = 0; i < len; ++i)
	{
		for (int z = 0; z < font->len; z++) {
			if (font->table[z] == text[i]) {
				int row = 0;
				int new_x = 0;
				new_x = font->char_w * z;
				if (z >= font->row_chars) {
					new_x = font->char_w * (z - font->row_chars);
					row = 1;
				}

				SDL_Rect rect = { new_x, font->char_h * row, font->char_w,font->char_h };
				App->render->Blit(font->graphic, x, y, &rect, 0.0f, false);
				x += font->char_w;
			}
		}
	}
}
