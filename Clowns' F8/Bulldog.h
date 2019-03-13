#ifndef _Bulldog_H__
#define _Bulldog_H__

#include "Enemy.h"
#include "Animation.h"

class Bulldog : public Enemy
{
public:
	Bulldog(ENTITY_TYPE _type);

	// Destructor
	~Bulldog();

	// Called before render is available
	bool	Awake(pugi::xml_node & _config);

	// Called before the first frame
	bool	Start(uint _i);

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();


	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);
private:

	bool CleanUp();

private:
	SDL_Texture * bulldog_tex = nullptr;

	Animation*	current_animation = nullptr;
	Animation idle;
	Animation walk;
	Animation dead;
};

#endif // __Bulldog_H__