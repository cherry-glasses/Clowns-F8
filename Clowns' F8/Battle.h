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
	virtual bool Update(float dt) { return true; };

	// Called before all Updates
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual void CreateMusic() {};

protected:
	
	void CreateUIBattle();
	void CreateAttackMenu();
	void CreateAbilitiesMenu();
	void CreateOptionsIngame();

	void UpdateCharacters();
	void UpdateEnemies();
	void UpdateCharacterPortraits(Entity* _character, int _i);
	void UpdateEnemyPortraits(Entity* _enemy, int _i);

	void DeleteAttackMenu();
	void DeleteAbilitiesMenu();
	void DeleteOptionsIngame();

	void ActionsMenu();

	void ControlLanguageAndMusic();

protected:

	SDL_Texture * battle_menu_background = nullptr;


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
	std::vector<GUIImage*> life;
	std::vector<GUIImage*> enemies_life;
	std::vector<GUIImage*> mana;
	std::vector<GUIImage*> portrait;
	std::vector<GUIImage*> port;
	std::vector<GUILabel*> character_names;
	std::vector<GUILabel*> life_numbers;
	std::vector<GUILabel*> mana_numbers;
	std::vector<int> life_x;
	std::vector<int> mana_x;
	std::vector<int> enemies_life_x;
	std::vector<std::pair<int, int>> life_position;
	std::vector<std::pair<int, int>> enemies_life_position;
	std::vector<std::pair<int, int>> mana_position;
	std::vector<std::pair<int, int>> port_position;
	std::vector<std::pair<int, int>> portrait_position;
	std::vector<std::pair<int, int>> name_position;
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

};

#endif //!__Battle_H_