#ifndef _CharacterIris_H__
#define _CharacterIris_H__

#include "Character.h"
#include "Animation.h"

enum Move_Steps {
	SEARCH,
	SELECT,
	MOVE,
	IDLE
};

class CharacterIris : public Character
{
public:
	CharacterIris(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterIris();

	// Called each loop iteration
	bool	PreUpdate();
	bool	Update(float _dt);
	bool	PostUpdate();


	//Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	void Wheremove();

	//bool LoadAnimation(pugi::xml_node &_node, Animation &_anim);

private:
	int Cap = 0;
	SDL_Rect debug_2;
	SDL_Rect debug_1;
	bool flag = false;
	Move_Steps Def = Move_Steps::IDLE;
	Animation*	current_animation = nullptr;
	int mov_x_first;
	int mov_y_first;
	int mov_x_last;
	int mov_y_last;
	Animation idle;
	Animation walk;
	Animation dead;

	std::pair<int, int> possible_mov[8];
	std::pair<int, int> possible_mov_map[8];
};

#endif // !__CharacterIris_H__
