#include "Particle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleParticleSystem.h"



Particle::Particle() :life(0) { }

void Particle::Init(std::pair<float, float> pos, float startSpeed, float endSpeed, float angle, double rotSpeed, float startSize, float endSize, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode, bool vortexSensitive)
{
	// Movement properties
	pState.pLive.pos = pos;
	pState.pLive.startVel.first = startSpeed * cos(DEG_2_RAD(angle));
	pState.pLive.startVel.second = -startSpeed * sin(DEG_2_RAD(angle));
	pState.pLive.endVel.first = endSpeed * cos(DEG_2_RAD(angle));
	pState.pLive.endVel.second = -endSpeed * sin(DEG_2_RAD(angle));
	pState.pLive.startRotSpeed = rotSpeed;
	pState.pLive.currentRotSpeed = rotSpeed;

	// Life properties
	this->life = pState.pLive.startLife = life;
	pState.pLive.currentSize = pState.pLive.startSize = startSize;
	pState.pLive.endSize = endSize;
	pState.pLive.t = 0.0f;

	// Color properties
	pState.pLive.startColor = startColor;
	pState.pLive.endColor = endColor;
	pState.pLive.blendMode = blendMode;
	pState.pLive.pRect = pState.pLive.rectSize = textureRect;

	// Vortex
	pState.pLive.vortexSensitive = vortexSensitive;

	// Add vortex to the system (optional and only one is allowed)
	if (pState.pLive.vortexSensitive)
		AddVortex({ 250.0f, 200.0f }, 10.0f, 30.0f);
}

void Particle::Update(float dt)
{
	// Vortex control 
	if (!pState.pLive.vortexSensitive && vortex.scale != 0 && vortex.speed != 0)
		AddVortex({ 0.0f, 0.0f }, 0.0f, 0.0f);

	// Age ratio is used to interpolate between particle properties
	pState.pLive.ageRatio = (float)life / (float)pState.pLive.startLife;

	// Particle size interpolation
	pState.pLive.currentSize = InterpolateBetweenRange(pState.pLive.startSize, pState.pLive.t, pState.pLive.endSize);

	// Particle speed interpolation
	pState.pLive.currentVel.first = InterpolateBetweenRange(pState.pLive.startVel.first, pState.pLive.t, pState.pLive.endVel.first);
	pState.pLive.currentVel.second = InterpolateBetweenRange(pState.pLive.startVel.second, pState.pLive.t, pState.pLive.endVel.second);

	// Assign new size to particle rect
	pState.pLive.rectSize.w = pState.pLive.rectSize.h = pState.pLive.currentSize;

	// Calculates new particle position.
	CalculateParticlePos(dt);

	// Decrementing particle life
	life--;
}

bool Particle::Draw()
{
	bool ret = true;

	// Calculations to determine the current center of particle texture
	SDL_Rect tmpRect = { (int)pState.pLive.startSize, (int)pState.pLive.startSize };
	float centerX = pState.pLive.pos.first + ((tmpRect.w - pState.pLive.rectSize.w) / 2.0f);
	float centerY = pState.pLive.pos.second + ((tmpRect.h - pState.pLive.rectSize.h) / 2.0f);

	// Color interpolation, only if the particle has enough life
	SDL_Color resColor = pState.pLive.startColor;

	if (pState.pLive.startLife > MIN_LIFE_TO_INTERPOLATE)
		resColor = RgbInterpolation(pState.pLive.startColor, pState.pLive.t, pState.pLive.endColor);

	// Blitting particle on screen
	ret = App->render->BlitParticle(App->particle_system->GetParticleAtlas(), (int)centerX, (int)centerY, &pState.pLive.pRect,
		&pState.pLive.rectSize, resColor, pState.pLive.blendMode, 1.0f, pState.pLive.currentRotSpeed);

	// Calculating new rotation according to rotation speed
	pState.pLive.currentRotSpeed += pState.pLive.startRotSpeed;

	// Time step increment to interpolate colors
	pState.pLive.t += (1.0f / (float)pState.pLive.startLife);

	if (pState.pLive.t >= 1.0f)
		pState.pLive.t = 0.0f;

	return ret;
}

bool Particle::IsAlive()
{
	return life > 0;
}

Particle* Particle::GetNext()
{
	return pState.next;
}

void Particle::SetNext(Particle* next)
{
	pState.next = next;
}

SDL_Color Particle::RgbInterpolation(SDL_Color startColor, float timeStep, SDL_Color endColor)
{
	SDL_Color finalColor;

	finalColor.r = startColor.r + (endColor.r - startColor.r) * timeStep;
	finalColor.g = startColor.g + (endColor.g - startColor.g) * timeStep;
	finalColor.b = startColor.b + (endColor.b - startColor.b) * timeStep;
	finalColor.a = startColor.a + (endColor.a - startColor.a) * timeStep;

	return finalColor;
}

float Particle::InterpolateBetweenRange(float min, float timeStep, float max)
{
	return min + (max - min) * timeStep;
}

void Particle::AddVortex(std::pair<float, float> pos, float speed, float scale)
{
	vortex.pos = pos;
	vortex.speed = speed;
	vortex.scale = scale;
}

void Particle::CalculateParticlePos(float dt)
{
	float dx = pState.pLive.pos.first - vortex.pos.first;
	float dy = pState.pLive.pos.second - vortex.pos.second;
	float vx = -dy * vortex.speed;
	float vy = dx * vortex.speed;
	float factor = 1.0f / (1.0f + (dx * dx + dy * dy) / vortex.scale);

	pState.pLive.pos.first += (vx - pState.pLive.currentVel.first) * factor + pState.pLive.currentVel.first * dt;
	pState.pLive.pos.second += (vy - pState.pLive.currentVel.second) * factor + pState.pLive.currentVel.second * dt;
}