#ifndef __Entity_H__
#define __Entity_H__

#include "Animation.h"
#include "ModuleTextures.h"

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER,
	ENTITY_ENEMY,
	ENTITY_BOSS,
	NO_TYPE
};

class Entity
{
public:
	Entity();
	Entity(ENTITY_TYPE _type);
	~Entity();
	virtual bool Awake(pugi::xml_node & _config) { return true; };
	virtual bool Start(uint _i) { return true; };
	virtual bool Restart(uint _i) { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float _dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	virtual bool Save(pugi::xml_node& _file) const;
	virtual bool Load(pugi::xml_node& _file);

	void		AddFX(const int _channel, const int _repeat) const;
	bool		LoadAnimation(pugi::xml_node &_node, Animation &_anim);

	virtual std::pair<float, float> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);

public:
	ENTITY_TYPE type;
	std::pair<float, float>  position;
	Animation* current_animation = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect	current = { 0,0,0,0 };

protected:
	enum MOVEMENT { IDLE, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN };
	enum STATE { ALIVE, DEATH };

	STATE current_state = ALIVE;
	MOVEMENT last_movement;
	MOVEMENT current_movement = IDLE;

};

#endif // __Entity_H_
