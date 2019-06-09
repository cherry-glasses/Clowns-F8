#ifndef __Language_H_
#define __Language_H_

enum LANGUAGE_TYPE {
	ENGLISH,
	SPANISH
};

class Language
{
public:

	Language();
	~Language() {
		lines.clear();
		end_lines.clear();
		range_val.clear();
		target_val.clear();
		ability_type_val.clear();
		effect_val.clear();
		amount_val.clear();
		credits_lines.clear();
	};

	void SetLanguage(LANGUAGE_TYPE language);
	void change_language();
public:
	LANGUAGE_TYPE type = ENGLISH;
	std::string new_game = "";
	std::string load_game = "";
	std::string options = "";
	std::string credits = "";
	std::string exit = "";
	std::string choose_language = "";
	std::string english = "";
	std::string spanish = "";
	std::string volume = "";
	std::string controls = "";
	std::string back = "";
	std::string accept = "";
	std::string decline = "";
	std::string keyboard = "";
	std::string gamepad = "";
	std::vector<std::string> credits_lines;
	std::string character_stats = "";
	std::string character_abilities = "";
	std::string abilities = "";
	std::string pause = "";
	std::string select = "";
	std::string resume = "";
	std::string choose_map = "";
	std::string battle_abilities = "";
	std::string defend = "";
	std::string win_accept = "";
	std::string map1 = "";
	std::string map2 = "";
	std::string map3 = "";
	std::string map4 = "";
	std::string save_game = "";
	std::string saved = "";
	std::string main_menu = "";

	//ability info
	std::string range = "";
	std::string target = "";
	std::string ability_type = "";
	std::string effect = "";
	std::string amount = "";

	std::vector<std::string> range_val;
	std::vector<std::string> target_val;
	std::vector<std::string> ability_type_val;
	std::vector<std::string> effect_val;
	std::vector<std::string> amount_val;

	//intro
	std::vector<std::string> lines;
	std::vector<std::string> end_lines;

	std::string attack_sapphire = "";
	std::string ability1_sapphire = "";
	std::string ability2_sapphire = "";
	std::string ability3_sapphire = "";

	std::string attack_iris = "";
	std::string ability1_iris = "";
	std::string ability2_iris = "";
	std::string ability3_iris = "";

	std::string attack_storm = "";
	std::string ability1_storm = "";
	std::string ability2_storm = "";
	std::string ability3_storm = "";

	std::string attack_georgeb = "";
	std::string ability1_georgeb = "";
	std::string ability2_georgeb = "";
	std::string ability3_georgeb = "";

	std::string greatings_tutorial = "";
	std::string greatings_2 = "";
	std::string greatings_3 = "";
	std::string lets_do_it_tutorial = "";
	std::string movemnent_tutorial = "";
	std::string movemnent_2 = "";
	std::string movemnent_3 = "";
	std::string movemnent_4 = "";
	std::string movemnent_5 = "";
	std::string movemnent_6 = "";
	std::string defens_tutorial = "";
	std::string defens_2 = "";
	std::string defens_3 = "";
	std::string defens_4 = "";
	std::string defens_5 = "";
	std::string defens_6 = "";
};


#endif //!__Language_H_