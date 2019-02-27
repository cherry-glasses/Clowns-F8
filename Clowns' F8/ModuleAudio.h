#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MUSICS 50
#define MAX_SFX 100

struct _Mix_Music;
struct Mix_Chunk;

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	_Mix_Music* const LoadMusic(const char* path);
	Mix_Chunk* const LoadFx(const char* path);

	bool UnloadMusic(_Mix_Music * music);
	bool UnloadSFX(Mix_Chunk * sfx);

public:
	_Mix_Music * musics[MAX_MUSICS];
	Mix_Chunk* sfx[MAX_SFX];
};

#endif // __ModuleAudio_H__
