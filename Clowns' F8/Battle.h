#ifndef __Battle_H_
#define __Battle_H_

#include "Scene.h"

class Entity;
struct SDL_Texture;

class Battle : public Scene
{
public:
	Battle(SCENE_TYPE _type, pugi::xml_node& _config);
	virtual ~Battle();

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called before all Updates
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	virtual bool PostUpdate(float _dt) { return true; };

	// Called before quitting
	bool CleanUp();

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };

protected:
	
	void CreateUIBattle();
	void CreateAttackMenu();
	void CreateAbilitiesMenu();
	void CreateOptionsIngame();

	void UpdateCharacters();
	void UpdateEnemies();
	void UpdateCharacterPortraits(Entity* _character, int _i);
	void UpdateEnemyPortraits(Entity* _enemy, int _i);

	void ShowEntityInfo(Entity* _entity);
	void LevelUp(bool _up);

	void DeleteEntityInfo();
	void DeleteAttackMenu();
	void DeleteAbilitiesMenu();
	void DeleteOptionsIngame();

	void ActionsMenu();

	void ControlLanguageAndMusic();

protected:

	SDL_Texture * battle_menu_background = nullptr;
	SDL_Texture* battle_background = nullptr;
	SDL_Texture* battle_grid = nullptr;


	GUIImage* action_menu = nullptr;
	GUIButton* attack_button = nullptr;
	GUIButton* ability_button = nullptr;
	GUIButton* defend_button = nullptr;
	GUILabel* attack_label = nullptr;
	GUILabel* ability_label = nullptr;
	GUILabel* defend_label = nullptr;
	GUIButton* ability1_button = nullptr;
	GUIButton* ability2_button = nullptr;
	GUIButton* ability3_button = nullptr;


	std::pair<int, int> portrait_margin;
	std::pair<int, int> life_margin;
	std::pair<int, int> mana_margin;
	std::pair<int, int> port_margin;
	std::pair<int, int> name_margin;
	std::pair<int, int> level_margin;
	std::pair<int, int> actions_margin;

	std::vector<GUIImage*> life;
	std::vector<GUIImage*> enemies_life;
	std::vector<GUIImage*> mana;
	std::vector<GUIImage*> portrait;
	std::vector<GUIImage*> port;
	std::vector<GUILabel*> character_names;
	std::vector<GUILabel*> character_levels;
	std::vector<GUILabel*> life_numbers;
	std::vector<GUILabel*> mana_numbers;
	std::vector<int> life_x;
	std::vector<int> mana_x;
	std::vector<int> enemies_life_x;
	std::vector<std::pair<int, int>> portrait_position;
	std::vector<std::pair<int, int>> act_menu_position;

	bool waiting_for_input = false;

	std::list<GUIButton*> buttons2;
	std::vector<GUIImage*> stun_image;
	std::vector<GUIImage*> enemy_stun_image;
	std::vector<GUIImage*> defense_image;
	std::vector<bool> defense_image_created;
	std::vector<bool> stun_image_created;
	std::vector<bool> enemy_stun_image_created;
	std::vector<bool> turn_created;

	bool ability_menu_created = false;
	bool first_battle_created = false;
	bool portraits_created = false;
	bool action_menu_created = false;

	bool ingame_options_menu_created = false;

	GUIButton* english_button = nullptr;
	GUIButton* spanish_button = nullptr;
	GUIButton* volume_up_button = nullptr;
	GUIButton* volume_down_button = nullptr;
	GUIButton* resume_button = nullptr;
	GUIButton* main_menu_button = nullptr;

	GUILabel* english_label = nullptr;
	GUILabel* spanish_label = nullptr;
	GUILabel* volume_up_label = nullptr;
	GUILabel* volume_down_label = nullptr;
	GUILabel* back_label = nullptr;
	GUILabel* controls_label = nullptr;
	GUILabel* language_label = nullptr;
	GUILabel* volume_label = nullptr;
	GUILabel* resume_label = nullptr;
	GUILabel* main_menu_label = nullptr;

	bool character_ability1up = false;
	bool character_ability2up = false;
	bool character_ability3up = false;

	//Showing info entities
	std::pair<int, int> aux_target = { 0,0 };
	SDL_Rect board;
	std::pair<int, int> port_board;
	std::pair<int, int> name_board;
	std::pair<int, int> life_board;
	std::pair<int, int> mana_board;
	std::pair<int, int> attack_board;
	std::pair<int, int> ability_1_board;
	std::pair<int, int> ability_2_board;
	std::pair<int, int> ability_3_board;

	GUIImage* board_entity = nullptr;
	GUIImage* port_entity = nullptr;
	GUILabel* name_entity = nullptr;
	GUILabel* lvl_entity = nullptr;
	GUILabel* exp_entity = nullptr;
	GUILabel* life_entity = nullptr;
	GUILabel* mana_entity = nullptr;
	GUILabel* atk_f_entity = nullptr;
	GUILabel* def_f_entity = nullptr;
	GUILabel* atk_s_entity = nullptr;
	GUILabel* def_s_entity = nullptr;
	GUILabel* crit_entity = nullptr;
	GUILabel* eva_entity = nullptr;
	GUILabel* attack_entity = nullptr;
	GUILabel* ability1_entity = nullptr;
	GUILabel* ability2_entity = nullptr;
	GUILabel* ability3_entity = nullptr;

};

#endif //!__Battle_H_