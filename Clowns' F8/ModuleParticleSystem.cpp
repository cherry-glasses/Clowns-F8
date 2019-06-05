#include "Log.h"
#include "Application.h"
#include "ModuleParticleSystem.h"
#include "ModuleTextures.h"
#include "Emitter.h"


ModuleParticleSystem::ModuleParticleSystem() : Module()
{
	name = "particle_system";

	LOG("Loading Particle System");

}

ModuleParticleSystem::~ModuleParticleSystem()
{

}

bool ModuleParticleSystem::Awake(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_document	psystem_config;
	pugi::xml_node* node = &App->LoadEmitters(psystem_config);
	nameParticleAtlas = node->child("particle_atlas").attribute("name").as_string();

	for (pugi::xml_node emitters = node->child("particle_atlas").child("emitter"); emitters && ret; emitters = emitters.next_sibling("emitter"))
	{
		std::string emitterType = emitters.attribute("type").as_string();

		if (emitterType.compare("num") == 0)
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_NUM);
		else if (emitterType.compare("attack") == 0)
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_ATTACK);
		else if (emitterType.compare("ability3") == 0)
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_ABILITY3);
		
	}
	return ret;
}

bool ModuleParticleSystem::Start()
{
	particleAtlas = App->textures->Load(nameParticleAtlas.c_str());

	return true;
}

bool ModuleParticleSystem::PreUpdate()
{
	return true;
}

bool ModuleParticleSystem::Update(float dt)
{
	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			(*it)->Update(dt);
	}

	return true;
}

bool ModuleParticleSystem::PostUpdate(float _dt)
{
	bool ret = true;

	if (!emittersToDestroy.empty())
	{
		std::list<Emitter*>::const_iterator it;

		for (it = emittersToDestroy.begin(); it != emittersToDestroy.end(); ++it)
		{
			emittersList.remove(*it);
			delete (*it);
		}

		emittersToDestroy.clear();
	}

	ret = emittersToDestroy.size() <= 0;

	if (ret)
	{
		std::list<Emitter*>::iterator it;

		for (it = emittersList.begin(); it != emittersList.end() && ret; ++it)
			ret = (*it)->Draw(_dt);
	}

	return true;
}

bool ModuleParticleSystem::CleanUp()
{
	LOG("Freeing emitters from the system.");

	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			delete (*it);
	}

	emittersList.clear();
	App->textures->UnLoad(particleAtlas);

	return true;
}

Emitter* ModuleParticleSystem::AddEmiter(std::pair<int, int> pos, EmitterType type)
{
	Emitter* tmp_emitter = new Emitter(pos, vecEmitterData[type]);

	emittersList.push_back(tmp_emitter);

	return tmp_emitter;
}

bool ModuleParticleSystem::RemoveEmitter(Emitter* emitter)
{
	bool ret = false;

	if (emitter != nullptr && !emittersList.empty())
	{
		emittersToDestroy.push_back(emitter);
		ret = true;
	}

	return ret;
}

bool ModuleParticleSystem::RemoveAllEmitters()
{
	bool ret = false;

	if (!emittersList.empty())
	{
		std::list<Emitter*>::const_iterator it;

		for (it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			if ((*it) != nullptr)
				emittersToDestroy.push_back(*it);

			ret = true;
		}
	}

	return ret;
}

SDL_Texture* ModuleParticleSystem::GetParticleAtlas() const
{
	return particleAtlas;
}

void ModuleParticleSystem::LoadEmitterData(pugi::xml_node & emitter, EmitterType type)
{
	EmitterData tmp;

	// Angle range
	tmp.angleRange.first = emitter.child("angleRange").attribute("min").as_float();
	tmp.angleRange.second = emitter.child("angleRange").attribute("max").as_float();

	// Particle speeds
	tmp.rotSpeed = emitter.child("rotSpeed").attribute("value").as_double();
	tmp.startSpeed = emitter.child("startSpeed").attribute("value").as_float();
	tmp.endSpeed = emitter.child("endSpeed").attribute("value").as_float();

	// Random controls
	tmp.startSpeedRand.first = emitter.child("startSpeed").attribute("randMin").as_float();
	tmp.startSpeedRand.second = emitter.child("startSpeed").attribute("randMax").as_float();

	tmp.endSpeedRand.first = emitter.child("endSpeed").attribute("randMin").as_float();
	tmp.endSpeedRand.second = emitter.child("endSpeed").attribute("randMax").as_float();

	tmp.rotSpeedRand.first = emitter.child("rotSpeed").attribute("randMin").as_float();
	tmp.rotSpeedRand.second = emitter.child("rotSpeed").attribute("randMax").as_float();

	tmp.emitVarianceRand.first = emitter.child("emitVariance").attribute("randMin").as_float();
	tmp.emitVarianceRand.second = emitter.child("emitVariance").attribute("randMax").as_float();

	tmp.lifeRand.first = emitter.child("maxParticleLife").attribute("randMin").as_float();
	tmp.lifeRand.second = emitter.child("maxParticleLife").attribute("randMax").as_float();

	tmp.startSizeRand.first = emitter.child("startSize").attribute("randMin").as_float();
	tmp.startSizeRand.second = emitter.child("startSize").attribute("randMax").as_float();

	tmp.endSizeRand.first = emitter.child("endSizeRand").attribute("randMin").as_float();
	tmp.endSizeRand.second = emitter.child("endSizeRand").attribute("randMax").as_float();

	// Particle size
	tmp.startSize = emitter.child("startSize").attribute("value").as_float();
	tmp.endSize = emitter.child("endSize").attribute("value").as_float();

	// Emission properties
	tmp.emitNumber = emitter.child("emitNumber").attribute("value").as_uint();
	tmp.emitVariance = emitter.child("emitVariance").attribute("value").as_uint();

	// Particle life
	tmp.maxParticleLife = emitter.child("maxParticleLife").attribute("value").as_uint();

	// Rect from particle atlas
	tmp.textureRect.x = emitter.child("textureRect").attribute("x").as_int();
	tmp.textureRect.y = emitter.child("textureRect").attribute("y").as_int();
	tmp.textureRect.w = emitter.child("textureRect").attribute("w").as_int();
	tmp.textureRect.h = emitter.child("textureRect").attribute("h").as_int();

	// Lifetime of emitter
	tmp.lifetime = emitter.child("lifetime").attribute("value").as_double();

	// Start color
	tmp.startColor.r = emitter.child("startColor").attribute("r").as_uint();
	tmp.startColor.g = emitter.child("startColor").attribute("g").as_uint();
	tmp.startColor.b = emitter.child("startColor").attribute("b").as_uint();
	tmp.startColor.a = emitter.child("startColor").attribute("a").as_uint();

	// End color
	tmp.endColor.r = emitter.child("endColor").attribute("r").as_uint();
	tmp.endColor.g = emitter.child("endColor").attribute("g").as_uint();
	tmp.endColor.b = emitter.child("endColor").attribute("b").as_uint();
	tmp.endColor.a = emitter.child("endColor").attribute("a").as_uint();

	// Blend mode
	std::string blendModeString = emitter.child("blendMode").attribute("mode").as_string();

	if (blendModeString == "add")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
	else if (blendModeString == "blend")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
	else if (blendModeString == "mod")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
	else if (blendModeString == "none")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;

	// Vortex
	tmp.vortexSensitive = emitter.child("vortexSensitive").attribute("state").as_bool();

	vecEmitterData[type] = tmp;
}