#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"

#define MAX_MUSICS 50
#define MAX_SFX 100

struct _Mix_Music;
struct Mix_Chunk;

class ModuleAudio : public Module
{
public:
	ModuleAudio();

	// Destructor
	virtual ~ModuleAudio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* _path);

	// Load a WAV in memory
	unsigned int LoadFx(const char* _path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int _fx, int _repeat = 0);

	//Change Volume	
	void StopMusic(int _mut);
	void VolumeUp();
	void VolumeDown();
	void SliderVolumeFx(int _vol);

	//Save & Load volume
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

	_Mix_Music * music = nullptr;
	std::vector<Mix_Chunk*>	fx;

	uint volume = 0u;
	uint volume_fx = 0u;
	uint max_volume = 0u;
	float default_music_fade_time;
	uint volume_change_ratio = 0u;
	bool mute = false;
	bool mute_volume = false;
	bool mute_fx = false;

public:
	std::string			folder_music;
	std::string			folder_sfx;
};

#endif // __ModuleAudio_H__
