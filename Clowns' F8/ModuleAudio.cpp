#include "Log.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() : Module()
{
	music = NULL;
	name = "audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before audio is available
bool ModuleAudio::Awake(pugi::xml_node& _config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;

	volume = _config.child("volume").attribute("value").as_uint();
	volume_fx = _config.child("volume").attribute("value").as_uint();
	max_volume = _config.child("volume").attribute("value").as_uint();
	default_music_fade_time = _config.child("default_music_fade_time").attribute("value").as_float();
	volume_change_ratio = _config.child("volume_change_ratio").attribute("value").as_uint();
	mute = _config.child("mute").attribute("value").as_bool(false);

	folder_music = _config.child("music").child_value("folder");
	folder_sfx = _config.child("sfx").child_value("folder");

	if (mute)
	{
		Mix_VolumeMusic(0);
	}
	else
	{
		Mix_VolumeMusic(volume);

	}

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (int i = 0; i < fx.size(); i++)
	{
		Mix_FreeChunk(fx[i]);
	}

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* _path)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (default_music_fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(default_music_fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	std::string tmp  = folder_music + _path;

	music = Mix_LoadMUS(tmp.c_str());

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", _path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (default_music_fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(default_music_fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", _path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", _path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", _path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* _path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	std::string tmp = folder_sfx + _path;

	Mix_Chunk* chunk = Mix_LoadWAV(tmp.c_str());

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", _path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int _id, int _repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (_id > 0 && _id <= fx.size())
	{
		Mix_PlayChannel(-1, fx[_id - 1], _repeat);
	}

	LOG("VOLUME %d", volume_fx);
	return ret;
}

void ModuleAudio::StopMusic(int _mut)
{
	switch (_mut)
	{
	case -1:
		mute = !mute;
		mute_volume = mute;
		mute_fx = mute;
		if (mute == true)
		{
			Mix_VolumeMusic(0);
			for (int id = 1; id <= fx.size(); id++)
			{
				Mix_VolumeChunk(fx[id - 1], 0);
			}
		}
		else
		{
			Mix_VolumeMusic(volume);
			for (int id = 1; id <= fx.size(); id++)
			{
				Mix_VolumeChunk(fx[id - 1], volume_fx);
			}
		}
		break;
	case -2:
		mute_volume = !mute_volume;
		if (mute_volume == true)
		{
			Mix_VolumeMusic(0);
		}
		else
		{
			Mix_VolumeMusic(volume);
		}
		break;
	case -3:
		mute_fx = !mute_fx;
		if (mute_fx == true)
		{
			for (int id = 1; id <= fx.size(); id++)
			{
				Mix_VolumeChunk(fx[id - 1], 0);
			}
		}
		else
		{
			for (int id = 1; id <= fx.size(); id++)
			{
				Mix_VolumeChunk(fx[id - 1], volume_fx);
			}
		}
		break;
	}

}

void ModuleAudio::VolumeUp(int _vol)
{
	
}

void ModuleAudio::VolumeDown(int _vol)
{
	
}

void ModuleAudio::SliderVolumeFx(int _vol)
{
	for (int id = 0; id < fx.size(); id++)
	{
		Mix_VolumeChunk(fx[id], _vol);
	}
	volume_fx = _vol;
}

bool ModuleAudio::Load(pugi::xml_node& _data)
{
	volume = _data.child("volume").attribute("value").as_uint();
	volume_fx = _data.child("volume_fx").attribute("value").as_uint();
	mute = _data.child("mute").attribute("value").as_bool();

	return true;
}

bool ModuleAudio::Save(pugi::xml_node& _data) const
{
	pugi::xml_node vol = _data.append_child("volume");
	vol.append_attribute("value") = volume;

	pugi::xml_node vol_fx = _data.append_child("volume_fx");
	vol_fx.append_attribute("value") = volume_fx;

	pugi::xml_node mut = _data.append_child("mute");
	mut.append_attribute("value") = mute;

	return true;
}