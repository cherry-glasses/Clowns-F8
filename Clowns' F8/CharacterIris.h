#ifndef _CharacterIris_H__
#define _CharacterIris_H__

#include "Character.h"
#include "Animation.h"

class CharacterIris : public Character
{
public:
	CharacterIris();

	// Destructor
	~CharacterIris();

	// Called before render is available
	bool	Awake(pugi::xml_node & _config);

	// Called before the first frame
	bool	Start();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();


	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void move();
	void Wheremove();

	//bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);
private:

	bool CleanUp();

private:
	SDL_Texture * Iris_tex = nullptr;
	bool flag = false;
	Animation*	current_animation = nullptr;
	Animation idle;
	Animation walk;
	Animation dead;

	std::pair<int, int> possible_mov[7];
};

#endif // !__CharacterIris_H__
