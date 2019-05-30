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
	~Language() {};

	void SetLanguage(LANGUAGE_TYPE language);

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
	std::string character_stats = "";
	std::string character_abilities = "";
	std::string abilities = "";
	std::string pause = "";
	std::string select = "";
	std::string resume = "";
	std::string main_menu = "";
	std::string battle_abilities = "";
	std::string defend = "";
	std::string win_accept = "";
	std::string map1 = "";
	std::string map2 = "";
	std::string map3 = "";
	std::string map4 = "";

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
	std::string lets_do_it_tutorial = "";
	std::string movemnent_tutorial = "";
	std::string defens_tutorial = "";
};


#endif //!__Language_H_