#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() : Module()
{
	// Initialize all music pointers to nullptr
	for (int i = 0; i < MAX_MUSICS; i++) {
		musics[i] = nullptr;
	}
	// Initialize all fx pointers to nullptr
	for (int i = 0; i < MAX_SFX; i++) {
		sfx[i] = nullptr;
	}
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before audio is available
bool ModuleAudio::Init()
{
	LOG("Init Audios library");

	bool ret = true;

	// load support for the PNG image format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(16);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 6);

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing music and fx library");

	// Free all music and fx
	for (int i = 0; i < MAX_MUSICS; i++) {
		Mix_FreeMusic(musics[i]);
	}
	for (int i = 0; i < MAX_SFX; i++) {
		Mix_FreeChunk(sfx[i]);
	}

	Mix_CloseAudio();
	return true;
}

// Load new music from file path
Mix_Music * const ModuleAudio::LoadMusic(const char* path)
{
	// Load a music from a path (must be a ogg)
	// and check for errors
	Mix_Music* music = Mix_LoadMUS(path);
	if (!music) {
		LOG("Mix_LoadMUS: %s\n", Mix_GetError());
		// handle error
	}
	else {

		bool room = false;
		for (int i = 0; i < MAX_MUSICS; ++i)
		{
			if (musics[i] == nullptr)
			{
				musics[i] = music;
				room = true;
				break;
			}
		}
		if (room == false)
			LOG("Music buffer overflow");
	}
	return music;
}

// Load new fx from file path
Mix_Chunk * const ModuleAudio::LoadFx(const char* path)
{

	// Load a music from a path (must be a ogg)
	// and check for errors
	Mix_Chunk* fx = Mix_LoadWAV(path);

	if (!fx) {
		LOG("Mix_LoadWAV error: %s\n", Mix_GetError());
		// handle error
	}
	else {

		bool room = false;
		for (int i = 0; i < MAX_SFX; ++i)
		{
			if (sfx[i] == nullptr)
			{
				sfx[i] = fx;
				room = true;
				break;
			}
		}
		if (room == false)
			LOG("SFX buffer overflow");
	}

	return fx;
}

bool ModuleAudio::UnloadMusic(_Mix_Music* music) {

	bool ret = false;

	if (music != nullptr)
	{
		for (int i = 0; i < MAX_MUSICS; ++i)
		{
			if (musics[i] == music)
			{
				Mix_FreeMusic(musics[i]);
				musics[i] = nullptr;
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool ModuleAudio::UnloadSFX(Mix_Chunk* _sfx) {
	bool ret = false;

	if (_sfx != nullptr)
	{
		for (int i = 0; i < MAX_SFX; ++i)
		{
			if (sfx[i] == _sfx)
			{
				Mix_FreeChunk(sfx[i]);
				sfx[i] = nullptr;
				ret = true;
				break;
			}
		}
	}

	return ret;
}