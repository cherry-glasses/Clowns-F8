#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "ModuleParticleSystem.h"
#include "ParticlePool.h"
#include "PerfTimer.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
class ParticlePool;
struct EmitterData;

class Emitter
{
private:

	// Particles size and movement
	std::pair<int, int> pos = { 0.0f, 0.0f };
	std::pair<int, int> angleRange = { 0.0f, 360.0f };
	float startSpeed = 0.0f;
	float endSpeed = 0.0f;
	float startSize = 0.0f;
	float endSize = 0.0f;
	double rotSpeed = 0;

	// Particles emission
	int emissionRate = 0;
	uint emitNumber = 0u;
	uint emitVariance = 0u;
	uint maxParticleLife = 0u;
	uint maxParticlesPerFrame = 0u;

	// Pool
	ParticlePool* emitterPool = nullptr;
	int poolSize = 0;

	// Emission time
	PerfTimer stopTimer;
	PerfTimer emissionTimer;
	PerfTimer lifeTimer;
	double stopTime = 0.0f;
	double emissionTime = 0.0f;
	double lifetime = -1.0f;
	bool active = false;

	// Color and render properties
	SDL_Color startColor = { 0, 0, 0, 0 };
	SDL_Color endColor = { 0, 0, 0, 0 };
	SDL_Rect textureRect;
	SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	float timeStep = 0.0f;

	// Random control parameters
	std::pair<int, int> rotSpeedRand = { 0.0f, 0.0f };
	std::pair<int, int> startSpeedRand = { 0.0f, 0.0f };
	std::pair<int, int> endSpeedRand = { 0.0f, 0.0f };
	std::pair<int, int> emitVarianceRand = { 0.0f, 0.0f };
	std::pair<int, int> lifeRand = { 0.0f, 0.0f };
	std::pair<int, int> startSizeRand = { 0.0f, 0.0f };
	std::pair<int, int> endSizeRand = { 0.0f, 0.0f };

	// Vortex control parameters
	bool vortexSensitive = false;

public:

	Emitter(std::pair<float, float> pos, EmitterData data);
	// fPoint pos, uint emitNumber, uint emitVariance, uint maxParticleLife, fPoint angleRange, double rotSpeed, float maxSpeed, float startSize, float endSize, SDL_Rect textureRect, SDL_Color startColor = { 0, 0, 0, 0 }, SDL_Color endColor = { 0, 0, 0, 0 }, SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE, double lifetime = -1.0f
	virtual ~Emitter();

	void Update(float dt);
	bool Draw(float dt);

	// Generates random number between given range
	float RangeRandomNum(float min = -1.0f, float max = 1.0f);

	// Returns emitter pool size
	int GetPoolSize() const;

	// Starts emission specified by timer, if not emission time is infinite
	void StartEmission(double timer = -1.0f);

	// Stops emission specified by timer, if not emission stop time is infinite
	void StopEmission(double timer = 0.0f);

	// Emitter move methods
	void MoveEmitter(std::pair<int, int> newPos);
	std::pair<int, int> GetEmitterPos() const;

	// Setters
	void SetTextureRect(SDL_Rect _texture_rect);
	void SetColor(SDL_Color _start_color = { 0, 0, 0, 0 },SDL_Color _end_color = { 0, 0, 0, 0 }, SDL_BlendMode _blendmode = SDL_BlendMode::SDL_BLENDMODE_ADD);
	void SetPosition(std::pair<int, int> _pos);
	void SetSize(int _start, int _end);

	void VortexSensitive(bool sensitive);
};

#endif
